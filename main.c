/********************************************************
 Name          : main.c
 Author        : Patrick Bellasi
 Copyright     : Copyright 2010 @ Patrick Bellasi
 Description   : EVK1101 template
 **********************************************************/

// Include Files 
#include "board.h"
#include "pm.h"
#include "flashc.h"
#include "intc.h"
#include "gpio.h"
#include "navigation.h"
#include "cycle_counter.h"
#include "power_clocks_lib.h"
#include "print_funcs.h"
#include "pwm_dac.h"
#include "adc.h"

// Connection of the light sensor
#define ADC_LIGHT_CHANNEL           6
#define ADC_LIGHT_PIN               AVR32_ADC_AD_6_PIN
#define ADC_LIGHT_FUNCTION          AVR32_ADC_AD_6_FUNCTION

char dbg_buff[64];

// From helix_player.c
extern void abort_playback();
extern long play_selected_file();

// From ds_reader.c
extern void sd_reader_init();

volatile static Bool skip_direction;

unsigned short presence = 0;


/* ADC */
/* GPIO pin/adc-function map. */
static const gpio_map_t ADC_GPIO_MAP =
{
	{ADC_LIGHT_PIN, ADC_LIGHT_FUNCTION},
};
/* ADC IP registers address */
volatile avr32_adc_t *adc = &AVR32_ADC;




/*! \brief Initializes the MCU system clocks.
 */
static void init_sys_clocks(void)
{
	// Initialize system clocks.
	pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);
	pm_pll_setup(&AVR32_PM,
				0,  // pll.
				7,  // mul.
				1,   // div.
				0,   // osc.
				16); // lockcount.
	pm_pll_set_option(&AVR32_PM,
				0, // pll.
				1,  // pll_freq.
				1,  // pll_div2.
				0); // pll_wbwdisable.
	pm_pll_enable(&AVR32_PM, 0);
	pm_wait_for_pll0_locked(&AVR32_PM);
	pm_cksel(&AVR32_PM,
				0,   // pbadiv.
				0,   // pbasel.
				0,   // pbbdiv.
				0,   // pbbsel.
				0,   // hsbdiv.
				0);  // hsbsel.

	/* 0 wait state is actually up to 30MHz only (in worst case conditions),
	 * but it seems to work fine at 48MHz, so we use that since it's faster. */
	flashc_set_wait_state(0);
	pm_switch_to_clock(&AVR32_PM, AVR32_PM_MCCTRL_MCSEL_PLL0);
}

static unsigned short check_presence(void)
{
	unsigned short readings = 32;
	unsigned short light_level;
	unsigned short max_level = 0x0000;
	unsigned short min_level = 0xFFFF;

	// Enable the ADC channels
	adc_enable(adc, ADC_LIGHT_CHANNEL);

	while (readings--) {
		// launch conversion on all enabled channels
		adc_start(adc);
		// get value for the light adc channel
		light_level = adc_get_value(adc, ADC_LIGHT_CHANNEL);
		max_level = max_level > light_level ? max_level : light_level;
		min_level = min_level < light_level ? min_level : light_level;
		cpu_delay_ms(100, FOSC0);
	}

	print_dbg(CL_WHITE"Max Light: 0x");
	print_dbg_short_hex(max_level);

	// Computing delta light
	light_level = max_level-min_level;

	print_dbg(CL_WHITE", Delta Light: 0x");
	print_dbg_short_hex(light_level);
	print_dbg("\n");

	// Setting threshold being half the max light
	max_level >>= 3 ;
	if (light_level < max_level)
		light_level = 0;

	// Disable the ADC channels.
	adc_disable(adc, ADC_LIGHT_CHANNEL);

	return light_level;

}

__attribute__((__interrupt__))
static void joystick_int_handler(void)
{
	if(gpio_get_pin_interrupt_flag(GPIO_JOYSTICK_PUSH)) {
		gpio_clear_pin_interrupt_flag(GPIO_JOYSTICK_PUSH);
	}
	if(gpio_get_pin_interrupt_flag(GPIO_JOYSTICK_LEFT)) {
		// Go the previous song.
		abort_playback();
		skip_direction = FS_FIND_PREV;
		gpio_clear_pin_interrupt_flag(GPIO_JOYSTICK_LEFT);
		LED_Toggle(LED0);
	}
	if(gpio_get_pin_interrupt_flag(GPIO_JOYSTICK_RIGHT)) {
		// Skip to next song.
		abort_playback();
		skip_direction = FS_FIND_NEXT;
		gpio_clear_pin_interrupt_flag(GPIO_JOYSTICK_RIGHT);
		LED_Toggle(LED0);
	}
	if(gpio_get_pin_interrupt_flag(GPIO_JOYSTICK_UP)) {
		// Increase volume.
		pwm_dac_increase_volume();
		gpio_clear_pin_interrupt_flag(GPIO_JOYSTICK_UP);
	}
	if(gpio_get_pin_interrupt_flag(GPIO_JOYSTICK_DOWN)) {
		// Decrease volume.
		pwm_dac_decrease_volume();
		gpio_clear_pin_interrupt_flag(GPIO_JOYSTICK_DOWN);
	}

	if(gpio_get_pin_interrupt_flag(GPIO_PUSH_BUTTON_0)) {
		// Door closed
		print_dbg(CL_BLUE"Door closed\n");
		LED_Toggle(LED2);

		presence = check_presence();
		if (presence) {
			print_dbg("People inside\n");
		}

		gpio_clear_pin_interrupt_flag(GPIO_PUSH_BUTTON_0);
	}
	if(gpio_get_pin_interrupt_flag(AVR32_PIN_PB03)) {
		// Door moved
		print_dbg(CL_BLUE"Door moving\n");
		LED_Toggle(LED2);

		presence = 1;

		gpio_clear_pin_interrupt_flag(AVR32_PIN_PB03);
		gpio_disable_pin_interrupt(AVR32_PIN_PB03);
	}
}

int main(void) {

	// Switch to PLL operation.
	init_sys_clocks();

	// Initialize USART in RS232 mode.
	pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);

	// Initialize the debug USART module.
	init_dbg_rs232(FOSC0);

	// Let's say hello
	print_dbg(CLEARSCR);
	print_dbg(CL_YELLOW"\n\tAdPlayer\n\n");

	// Set default volume.
	pwm_dac.volume_shift = 0; // 0 is the highest, higher shift is lower volume.
	pwm_dac_stop();

	LED_On(LED0);

	// Initialize interrupt controller.
	INTC_init_interrupts();
	Enable_global_interrupt();

	// Initialize joystick. Interrupt on release.
	gpio_enable_pin_pull_up(GPIO_JOYSTICK_PUSH);
	gpio_enable_pin_pull_up(GPIO_JOYSTICK_LEFT);
	gpio_enable_pin_pull_up(GPIO_JOYSTICK_RIGHT);
	gpio_enable_pin_pull_up(GPIO_JOYSTICK_UP);
	gpio_enable_pin_pull_up(GPIO_JOYSTICK_DOWN);
	gpio_enable_pin_glitch_filter(GPIO_JOYSTICK_PUSH);
	gpio_enable_pin_glitch_filter(GPIO_JOYSTICK_LEFT);
	gpio_enable_pin_glitch_filter(GPIO_JOYSTICK_RIGHT);
	gpio_enable_pin_glitch_filter(GPIO_JOYSTICK_UP);
	gpio_enable_pin_glitch_filter(GPIO_JOYSTICK_DOWN);
	INTC_register_interrupt(joystick_int_handler, AVR32_GPIO_IRQ_0, AVR32_INTC_INT1);
	INTC_register_interrupt(joystick_int_handler, AVR32_GPIO_IRQ_1, AVR32_INTC_INT1);
	INTC_register_interrupt(joystick_int_handler, AVR32_GPIO_IRQ_2, AVR32_INTC_INT1);
	INTC_register_interrupt(joystick_int_handler, AVR32_GPIO_IRQ_3, AVR32_INTC_INT1);
	INTC_register_interrupt(joystick_int_handler, AVR32_GPIO_IRQ_4, AVR32_INTC_INT1);
	INTC_register_interrupt(joystick_int_handler, AVR32_GPIO_IRQ_5, AVR32_INTC_INT1);
	gpio_enable_pin_interrupt(GPIO_JOYSTICK_PUSH, GPIO_RISING_EDGE);
	gpio_enable_pin_interrupt(GPIO_JOYSTICK_LEFT, GPIO_RISING_EDGE);
	gpio_enable_pin_interrupt(GPIO_JOYSTICK_RIGHT, GPIO_RISING_EDGE);
	gpio_enable_pin_interrupt(GPIO_JOYSTICK_UP, GPIO_RISING_EDGE);
	gpio_enable_pin_interrupt(GPIO_JOYSTICK_DOWN, GPIO_RISING_EDGE);


	// Door sensor
	gpio_enable_pin_glitch_filter(GPIO_PUSH_BUTTON_0);
	gpio_enable_pin_interrupt(GPIO_PUSH_BUTTON_0, GPIO_FALLING_EDGE);

	// Movement sensor
	gpio_enable_gpio_pin(AVR32_PIN_PB03);
	gpio_enable_pin_glitch_filter(AVR32_PIN_PA03);
	gpio_enable_pin_interrupt(AVR32_PIN_PB03, GPIO_FALLING_EDGE);


	// Configure ADC
	// Assign and enable GPIO pins to the ADC function.
	gpio_enable_module(ADC_GPIO_MAP, sizeof(ADC_GPIO_MAP) / sizeof(ADC_GPIO_MAP[0]));
	// Lower the ADC clock to match the ADC characteristics (because we configured
	// the CPU clock to 12MHz, and the ADC clock characteristics are usually lower;
	// cf. the ADC Characteristic section in the datasheet).
	AVR32_ADC.mr |= 0x1 << AVR32_ADC_MR_PRESCAL_OFFSET;
	adc_configure(adc);


	LED_Off(LED0);

	// Initialize SD-reader.
	sd_reader_init();

	// Mount file system.
	nav_reset();
	nav_drive_set(0);
	while(!nav_partition_mount()); // If we're unable to mount the partition, halt here.

	nav_checkdisk_disable(); // Skip fat_check_device() for every read.

	long ret;
	Bool replay = FALSE;
	skip_direction = FS_FIND_NEXT;

	//LED_On(LED0);
	//cpu_delay_ms(1000, FOSC0);
	//LED_Off(LED0);
	//cpu_delay_ms(1000, FOSC0);
	LED_On(LED1);

	// Go through all the files and directories in the file system.
	while(1) {
		//cpu_delay_ms(1000, FOSC0);
		LED_Toggle(LED0);

		while(replay || nav_filelist_set(0, skip_direction) || (nav_dir_gotoparent() && nav_filelist_set(0, skip_direction))) {

			LED_Toggle(LED1);



			replay = FALSE;
			skip_direction = FS_FIND_NEXT;
			if(nav_file_isdir()) {
				// There's limited support for directories, really.
				nav_dir_cd(); // Enter directory.
				continue;
			}

			if (nav_file_getname(dbg_buff, 64) == FALSE) {
				LED_On(LED2);
			} else {
				print_dbg(CL_BLUE"Next track: "); print_dbg(dbg_buff); print_dbg("\n");
			}

			do {
				print_dbg(CL_RED"Entering standby... ");
				gpio_enable_pin_interrupt(AVR32_PIN_PB03, GPIO_FALLING_EDGE);
				SLEEP(AVR32_PM_SMODE_IDLE);
				print_dbg("wakeup!\n");
			} while(!presence);
			presence = 0;

			// Play the file. We don't check the file extension/format here, we assume the
			// file is an MP3-file. Otherwise play_selected_file() should return (0) quickly,
			// skipping to the next file.
			ret = play_selected_file();

			if(ret >= 115 && skip_direction == FS_FIND_PREV) {
				// The operator pressed left on the joystick in the middle of the song,
				// so we just restart it from the beginning.
				replay = TRUE;
				skip_direction = FS_FIND_NEXT;
			}
		}

		nav_filelist_reset(); // We've played all the songs. Restart from the beginning.
		nav_dir_root();
	}

	return 0;
}
