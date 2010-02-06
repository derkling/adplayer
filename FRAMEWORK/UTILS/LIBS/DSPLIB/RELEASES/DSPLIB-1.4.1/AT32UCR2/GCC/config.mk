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
PRJ_PATH = ../../../../../../..
APPS_PATH = $(PRJ_PATH)/APPLICATIONS
BRDS_PATH = $(PRJ_PATH)/BOARDS
COMP_PATH = $(PRJ_PATH)/COMPONENTS
DRVR_PATH = $(PRJ_PATH)/DRIVERS
SERV_PATH = $(PRJ_PATH)/SERVICES
UTIL_PATH = $(PRJ_PATH)/UTILS

# DSP library path
DSPLIB_PATH = $(SERV_PATH)/DSPLIB
DSPLIB_GENERIC_PATH = $(DSPLIB_PATH)/GENERIC
DSPLIB_OPTIM_PATH = $(DSPLIB_PATH)/AT32UC
DSPLIB_BASIC_PATH = $(DSPLIB_OPTIM_PATH)/BASIC

# include generic configuration file
include $(DSPLIB_GENERIC_PATH)/BUILD/GCC/config.mk

# CPU architecture: {ap|ucr1|ucr2}
ARCH = ucr2

# Part: {none|ap7xxx|uc3xxxxx}
PART = uc3a0512

# Device/Platform/Board include path
PLATFORM_INC_PATH = 

# Definitions: [-D name[=definition]...] [-U name...]
# Things that might be added to DEFS:
#   BOARD               Board used: see $(BRDS_PATH)/board.h
#   EXT_BOARD           Extension board used (if any): see $(BRDS_PATH)/board.h
#   FORCE_ALL_GENERICS  To use generic C coded version of the functions
#   DSP_LMS_BETA        Beta coefficient for the LMS filter
#   DSP_NLMS_BETA       Beta coefficient for the NLMS filter
DEFS =

# Target name: {*.a|*.elf}
TARGET = $(DSPLIB_PATH)/LIB/libdsp_generic_$(PART)_speed.a

# Include path
INC_PATH := \
  $(INC_PATH) \
  $(DSPLIB_BASIC_PATH)/FILTERING/ \
  $(DSPLIB_BASIC_PATH)/OPERATORS/ \
  $(DSPLIB_BASIC_PATH)/SIGNAL_GENERATION/ \
  $(DSPLIB_BASIC_PATH)/TRANSFORMS/ \
  $(DSPLIB_BASIC_PATH)/VECTORS/ \
  $(DSPLIB_BASIC_PATH)/WINDOWING/

# Target specific sources for the filtering module
FILTERING_SRC = \
  $(DSPLIB_BASIC_PATH)/FILTERING/filt_dsp16_iir_at32uc.c \
  $(DSPLIB_BASIC_PATH)/FILTERING/filt_dsp16_iirpart_at32uc.c \
  $(DSPLIB_BASIC_PATH)/FILTERING/filt_dsp32_lms_at32uc.c \
  $(DSPLIB_BASIC_PATH)/FILTERING/filt_dsp32_nlms_at32uc.c

# Target specific sources for the operators module
OPERATORS_SRC = \
  $(DSPLIB_BASIC_PATH)/OPERATORS/op_dsp16_ln_at32uc.c \
  $(DSPLIB_BASIC_PATH)/OPERATORS/op_dsp16_sqrt_at32uc.c

# Target specific sources for the signal generation module
SIGNAL_GENERATION_SRC =

# Target specific sources for the transforms module
TRANSFORMS_SRC = \
  $(DSPLIB_BASIC_PATH)/TRANSFORMS/trans_dsp16_complex_fft_at32uc.c \
  $(DSPLIB_BASIC_PATH)/TRANSFORMS/trans_dsp16_complex_ifft_at32uc.c \
  $(DSPLIB_BASIC_PATH)/TRANSFORMS/trans_dsp16_real_complex_fft_at32uc.c

# Target specific sources for the vectors module
VECTORS_SRC = \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp16_add_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp16_convpart_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp16_dotdiv_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp16_dotmul_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp16_neg_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp16_realdiv_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp16_realmul_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp16_sub_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp32_add_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp32_convpart_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp32_max_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp32_min_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp32_neg_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp32_realmul_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp32_sub_at32uc.c \
  $(DSPLIB_BASIC_PATH)/VECTORS/vect_dsp16_complex_conj_at32uc.c

# Target specific sources for the windowing module
WINDOWING_SRC = \
  $(DSPLIB_BASIC_PATH)/WINDOWING/win_dsp16_rect_at32uc.c \
  $(DSPLIB_BASIC_PATH)/WINDOWING/win_dsp32_rect_at32uc.c

# C source files
CSRCS := \
  $(CSRCS) \
  $(FILTERING_SRC) \
  $(OPERATORS_SRC) \
  $(SIGNAL_GENERATION_SRC) \
  $(TRANSFORMS_SRC) \
  $(VECTORS_SRC) \
  $(WINDOWING_SRC)

# Assembler source files
ASSRCS = \
  $(DSPLIB_BASIC_PATH)/FILTERING/filt_dsp16_lms_at32uc.S \
  $(DSPLIB_BASIC_PATH)/FILTERING/filt_dsp16_nlms_at32uc.S \
  $(DSPLIB_BASIC_PATH)/FILTERING/filt_dsp32_lms_fir_at32uc.S \
  $(DSPLIB_BASIC_PATH)/FILTERING/filt_dsp32_nlms_fir_at32uc.S
