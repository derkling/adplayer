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


char dbg_buff[64];


// From helix_player.c
extern void abort_playback();
extern long play_selected_file();

// From ds_reader.c
extern void sd_reader_init();

volatile static Bool skip_direction;

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
		gpio_clear_pin_interrupt_flag(GPIO_PUSH_BUTTON_0);
	}
	if(gpio_get_pin_interrupt_flag(AVR32_PIN_PB03)) {
		// Door moved
		print_dbg(CL_BLUE"Door moving\n");
		LED_Toggle(LED2);
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

			print_dbg(CL_RED"Entering standby... ");
			gpio_enable_pin_interrupt(AVR32_PIN_PB03, GPIO_FALLING_EDGE);
			SLEEP(AVR32_PM_SMODE_IDLE);
			print_dbg("wakeup!\n");

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
