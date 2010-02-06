# Hey Emacs, this is a -*- makefile -*-

# The purpose of this file is to define the build configuration variables used
# by the generic Makefile. See Makefile header for further information.

# Copyright (c) 2009 Atmel Corporation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# 3. The name of Atmel may not be used to endorse or promote products derived
# from this software without specific prior written permission.
#
# 4. This software may only be redistributed and used in connection with an Atmel
# AVR product.
#
# THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
# EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
#

# Base paths
PRJ_PATH  = ../..
APPS_PATH = $(PRJ_PATH)
FMW_PATH  = $(PRJ_PATH)/FRAMEWORK
BRDS_PATH = $(FMW_PATH)/BOARDS
COMP_PATH = $(FMW_PATH)/COMPONENTS
DRVR_PATH = $(FMW_PATH)/DRIVERS
SERV_PATH = $(FMW_PATH)/SERVICES
UTIL_PATH = $(FMW_PATH)/UTILS

# CPU architecture: {ap|ucr1|ucr2}
ARCH = ucr1

# Part: {none|ap7xxx|uc3xxxxx}
PART = uc3b0256

# Flash memories: [{cfi|internal}@address,size]...
FLASH = internal@0x80000000,256Kb

# Clock source to use when programming: [{xtal|extclk|int}]
PROG_CLOCK = xtal

# Device/Platform/Board include path
PLATFORM_INC_PATH = \
  $(BRDS_PATH)/

# Target name: {*.a|*.elf}
TARGET = $(PART)-adplayer.elf

# Definitions: [-D name[=definition]...] [-U name...]
# Things that might be added to DEFS:
#   BOARD             Board used: see $(BRDS_PATH)/board.h
#   EXT_BOARD         Extension board used (if any): see $(BRDS_PATH)/board.h
#   FREERTOS_USED     Whether FreeRTOS is used
DEFS = -D BOARD=EVK1101

# Include path
INC_PATH = \
  $(BRDS_PATH)/ \
  $(BRDS_PATH)/EVK1101/ \
  $(COMP_PATH)/MEMORY/SD_MMC/SD_MMC_SPI/ \
  $(DRVR_PATH)/CPU/CYCLE_COUNTER/ \
  $(DRVR_PATH)/SPI/ \
  $(DRVR_PATH)/PDCA/ \
  $(DRVR_PATH)/GPIO/ \
  $(DRVR_PATH)/PM/ \
  $(DRVR_PATH)/FLASHC/ \
  $(DRVR_PATH)/INTC/ \
  $(DRVR_PATH)/USART/ \
  $(SERV_PATH)/MEMORY/CTRL_ACCESS/ \
  $(SERV_PATH)/FAT/ \
  $(UTIL_PATH)/DEBUG/ \
  $(UTIL_PATH)/PREPROCESSOR/ \
  $(UTIL_PATH)/LIBS/NEWLIB_ADDONS/INCLUDE/ \
  $(UTIL_PATH)/ \
  $(APPS_PATH)/CONF/ \
  $(APPS_PATH)/HELIX/pub/ \
  $(APPS_PATH)/PWM_DAC/ \
  $(APPS_PATH)/

#  $(COMP_PATH)/AUDIO/ \
#  $(COMP_PATH)/AUDIO/DAC/PWM_DAC/ \

# C source files
CSRCS = \
  $(BRDS_PATH)/EVK1101/led.c \
  $(COMP_PATH)/MEMORY/SD_MMC/SD_MMC_SPI/sd_mmc_spi.c \
  $(COMP_PATH)/MEMORY/SD_MMC/SD_MMC_SPI/sd_mmc_spi_mem.c \
  $(DRVR_PATH)/INTC/intc.c \
  $(DRVR_PATH)/USART/usart.c \
  $(DRVR_PATH)/PM/pm.c \
  $(DRVR_PATH)/PM/pm_conf_clocks.c \
  $(DRVR_PATH)/PM/power_clocks_lib.c \
  $(DRVR_PATH)/SPI/spi.c \
  $(DRVR_PATH)/FLASHC/flashc.c \
  $(DRVR_PATH)/PDCA/pdca.c \
  $(DRVR_PATH)/GPIO/gpio.c \
  $(SERV_PATH)/MEMORY/CTRL_ACCESS/ctrl_access.c \
  $(SERV_PATH)/FAT/fat_unusual.c \
  $(SERV_PATH)/FAT/navigation.c \
  $(SERV_PATH)/FAT/fat.c \
  $(SERV_PATH)/FAT/file.c \
  $(SERV_PATH)/FAT/fsaccess.c \
  $(UTIL_PATH)/DEBUG/print_funcs.c \
  $(APPS_PATH)/main.c \
  $(APPS_PATH)/PWM_DAC/pwm_dac.c \
  $(APPS_PATH)/HELIX/real/dequant.c \
  $(APPS_PATH)/HELIX/real/dct32.c \
  $(APPS_PATH)/HELIX/real/imdct.c \
  $(APPS_PATH)/HELIX/real/subband.c \
  $(APPS_PATH)/HELIX/real/trigtabs.c \
  $(APPS_PATH)/HELIX/real/buffers.c \
  $(APPS_PATH)/HELIX/mp3dec.c \
  $(APPS_PATH)/HELIX/real/bitstream.c \
  $(APPS_PATH)/HELIX/real/hufftabs.c \
  $(APPS_PATH)/HELIX/real/huffman.c \
  $(APPS_PATH)/HELIX/real/dqchan.c \
  $(APPS_PATH)/HELIX/real/polyphase.c \
  $(APPS_PATH)/HELIX/mp3tabs.c \
  $(APPS_PATH)/HELIX/real/scalfact.c \
  $(APPS_PATH)/HELIX/real/stproc.c \
  $(APPS_PATH)/helix_player.c \
  $(APPS_PATH)/sd_reader.c

#  $(COMP_PATH)/AUDIO/DAC/PWM_DAC/pwm_dac.c \

# Assembler source files
ASSRCS = \
  $(DRVR_PATH)/INTC/exception.S \
  $(SERV_PATH)/USB/CLASS/DFU/EXAMPLES/ISP/BOOT/trampoline.S \
  $(UTIL_PATH)/STARTUP_FILES/GCC/crt0.S

# Library path
LIB_PATH = \
  $(UTIL_PATH)/LIBS/NEWLIB_ADDONS/AT32UCR1/

# Libraries to link with the project
LIBS = \
  newlib_addons-at32ucr1-size_opt

# Linker script file if any
LINKER_SCRIPT = $(APPS_PATH)/link_uc3b0256.lds

# Options to request or suppress warnings: [-fsyntax-only] [-pedantic[-errors]] [-w] [-Wwarning...]
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
WARNINGS = -Wall

# Options for debugging: [-g]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
DEBUG =

# Options that control optimization: [-O[0|1|2|3|s]]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
#OPTIMIZATION = -Os -fno-strict-aliasing -mrelax -fsection-anchors -ffunction-sections -fdata-sections
#OPTIMIZATION = -Os -ffunction-sections -mrelax -fno-common -fsection-anchors
OPTIMIZATION = -Os -ffunction-sections -fdata-sections -fmessage-length=0 -masm-addr-pseudos


# Extra flags to use when preprocessing
CPP_EXTRA_FLAGS =

# Extra flags to use when compiling
C_EXTRA_FLAGS =

# Extra flags to use when assembling
AS_EXTRA_FLAGS =

# Extra flags to use when linking
LD_EXTRA_FLAGS = -nostartfiles -Wl,--gc-sections -Wl,-e,_trampoline -Wl,--gc-sections --rodata-writable -Wl,--direct-data

# Documentation path
DOC_PATH = \
  ../../../DOC/

# Documentation configuration file
DOC_CFG = \
  ../../DOCSRC/doxyfile.doxygen
