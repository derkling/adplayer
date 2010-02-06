/* This header file is part of the ATMEL AVR-UC3-SoftwareFramework-1.6.1 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief DACIFB header for AVR32 UC3.
 *
 * This file defines a useful set of functions for DACIFB on AVR32 devices.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with DACIFB can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 *****************************************************************************/

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 */

#ifndef _DACIFB_H_
#define _DACIFB_H_

#include <avr32/io.h>
#include "compiler.h"

#if ( defined (__GNUC__) && ( defined (__AVR32_UC3C064C__) || defined (__AVR32_UC3C0128C__) || defined (__AVR32_UC3C0256C__) || defined (__AVR32_UC3C0512CREVC__) || defined (__AVR32_UC3C164C__) || defined (__AVR32_UC3C1128C__) || defined (__AVR32_UC3C1256C__) || defined (__AVR32_UC3C1512CREVC__) || defined (__AVR32_UC3C264C__) || defined (__AVR32_UC3C2128C__) || defined (__AVR32_UC3C2256C__) || defined (__AVR32_UC3C2512CREVC__))) \
  ||( defined (__ICCAVR32__) && ( defined (__AT32UC3C064C__) || defined (__AT32UC3C0128C__) || defined (__AT32UC3C0256C__) || defined (__AT32UC3C0512C__) || defined (__AT32UC3C164C__) || defined (__AT32UC3C1128C__) || defined (__AT32UC3C1256C__) || defined (__AT32UC3C1512C__) || defined (__AT32UC3C264C__) || defined (__AT32UC3C2128C__) || defined (__AT32UC3C2256C__) || defined (__AT32UC3C2512C__)))
#define AVR32_FLASHC_CALIBRATION_THIRD_WORD_ADDRESS    0x80800208
#endif

//! Local Definition for Channel Selection used by the DAC
//! 
#define  DACIFB_CHANNEL_SELECTION_NONE              0x0
#define  DACIFB_CHANNEL_SELECTION_A                 0x1
#define  DACIFB_CHANNEL_SELECTION_B                 0x2
#define  DACIFB_CHANNEL_SELECTION_AB                0x3

//! Local Definition for Reference used by the DAC
//! 
#define  DACIFB_REFERENCE_EXT                       0x0
#define  DACIFB_REFERENCE_VDDANA                    0x1

//! Local Definition for Trigger Mode used by the DAC
//! 
#define  DACIFB_TRIGGER_MODE_MANUAL                 0x0
#define  DACIFB_TRIGGER_MODE_TIMER                  0x1
#define  DACIFB_TRIGGER_MODE_EVENT                  0x2

//! Status
// ----------
//! This constant is used as return value for "dacifb_configure_core" and "adcifa_configure_sequencer" functions.
#define  DACIFB_CONFIGURATION_REFUSED                0x0
// ----------
//! This constant is used as return value for "adcifa_configure_core" and "adcifa_configure_sequencer" functions.
#define  DACIFB_CONFIGURATION_ACCEPTED               0x1
// ----------
//! This constant is used as return value for "adcifa_get_values_seq" function.
#define  DACIFB_STATUS_COMPLETED                     0x2
// ----------
//! This constant is used as return value for "adcifa_get_values_seq" function.
#define  DACIFB_STATUS_NOT_COMPLETED                 0x3

//! Parameters for the DACIFB.
typedef struct
{
  //! Reference for DAC Conversion
  Bool reference;

  //! Channel Selection
  U8  channel_selection;

  //! Sampling Rate 
  U8 interval_us;

  //! Gain Calibration Value  
  U8 gain_calibration_value;
  
  //! Offset Calibration Value  
  U8 offset_calibration_value;
  
  //! Low Power Mode
  Bool low_power;
  
  //! Dual Mode
  Bool dual;
  
} dacifb_opt_t;

//! Parameters for the configuration of the channel.
typedef struct
{
  //! Refresh Time value in us.
  U32 refresh_time_us;

  //! Auto Refresh Mode.
  Bool auto_refresh_mode;

  //! Trigger Mode: Manual/Timer/Event
  U8 trigger_mode;

  //! Left or Right Adjustment.
  Bool left_adjustment;
  
  //! Data Shift Value.
  U8 data_shift;

  //! Data Round.
  Bool data_round_enable;
  
} dacifb_channel_opt_t;

/*! \brief Get DACIFB Calibration Data. Mandatory to call if factory calibration data are wanted to be used.
 * If not called, Calibration Data should be set by the application.
 * \param *dacifb       Base address of the DACIFB
 * \param *p_dacifb_opt Structure for the DACIFB core configuration 
 * \param instance      DACIFB core instance 0 for DACIFB0 or 1 for DACIFB1
 */
extern void dacifb_get_calibration_data(  volatile avr32_dacifb_t * dacifb,
                                          dacifb_opt_t * p_dacifb_opt,
                                          U8 instance);
                                  
/*! \brief Configure DACIFB. Mandatory to call.
 * If not called, DACIFB channels will have side effects
 *
 * \param *dacifb      Base address of the DACIFB
 * \param *p_dacifb_opt Structure for the DACIFB core configuration 
 * \param pb_hz        Periphal Bus frequency
 * \return U8          DACIFB_CONFIGURATION_REFUSED or DACIFB_CONFIGURATION_ACCEPTED
 */
extern U8 dacifb_configure(volatile avr32_dacifb_t * dacifb,
                           dacifb_opt_t * p_dacifb_opt,
                           U32 pb_hz);

/*! \brief Configure DACIFB specific channel. 
 *         - Adjustment, Refresh_time, Trigger Mode
 * \param  *dacifb                Base address of the ADCIFA
 * \param  channel                DACIFB_CHANNEL_SELECTION_NONE / DACIFB_CHANNEL_SELECTION_A / DACIFB_CHANNEL_SELECTION_B / DACIFB_CHANNEL_SELECTION_AB
 * \param  p_dacifb_channel_opt   Structure for the sequencer configuration 
 * \param  pb_hz                  Periphal Bus frequency
 * \return U8                     DACIFB_CONFIGURATION_REFUSED or DACIFB_CONFIGURATION_ACCEPTED
 */
extern U8 dacifb_configure_channel(volatile avr32_dacifb_t * dacifb,
                                   U8 channel,
                                   dacifb_channel_opt_t * p_dacifb_channel_opt,
                                   U32 pb_hz);

/*! \brief Start analog to digital conversion
 * \param *dacifb Base address of the DACIFB
 * \param  channel                DACIFB_CHANNEL_SELECTION_NONE / DACIFB_CHANNEL_SELECTION_A / DACIFB_CHANNEL_SELECTION_B / DACIFB_CHANNEL_SELECTION_AB
 * \param  cpu_hz                 CPU Clock frequency
*/
extern void dacifb_start_channel(volatile avr32_dacifb_t * dacifb,
                                 U8 channel,
                                 U32 cpu_hz);

/*! \brief Check channel conversion status
 *
 * \param *dacifb Base address of the DACIFB
 * \param  channel   channel to check (0 to 1)
 * \return Bool      HIGH if conversion not running
 *                   LOW if conversion running
 */
extern Bool dacifb_check_eoc(volatile avr32_dacifb_t * dacifb, 
                             U8 channel);
/*! \brief Set channel value
 *
 * \param *dacifb    Base address of the DACIFB
 * \param  channel   channel to handle (0 to 1)
 * \param  dual      Dual Mode Selection
 * \param  value     Value to be converted
 */
extern void dacifb_set_value(volatile avr32_dacifb_t * dacifb,
                            U8 channel, 
                            Bool dual,
                            U32 value);

/*! \brief Reload Timer for Automatic Trigger on DAC
 *  \param *dacifb  Base address of the DACIFB
 *  \param channel  DACIFB_CHANNEL_SELECTION_NONE / DACIFB_CHANNEL_SELECTION_A / DACIFB_CHANNEL_SELECTION_B / DACIFB_CHANNEL_SELECTION_AB
 * \param  timer    Timer Value
*/
extern void dacifb_reload_timer(volatile avr32_dacifb_t * dacifb,
                                 U8 channel,
                                 U8 timer);

#endif  // _DACIFB_H_
