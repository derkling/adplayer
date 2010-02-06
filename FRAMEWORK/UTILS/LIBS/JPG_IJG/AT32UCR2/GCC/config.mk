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
PRJ_PATH = ../../../../..
APPS_PATH = $(PRJ_PATH)/APPLICATIONS
BRDS_PATH = $(PRJ_PATH)/BOARDS
COMP_PATH = $(PRJ_PATH)/COMPONENTS
DRVR_PATH = $(PRJ_PATH)/DRIVERS
SERV_PATH = $(PRJ_PATH)/SERVICES
UTIL_PATH = $(PRJ_PATH)/UTILS

# CPU architecture: {ap|ucr1|ucr2}
ARCH = ucr2

# Part: {none|ap7xxx|uc3xxxxx}
PART = uc3a0512

# Flash memories: [{cfi|internal}@address,size]...
FLASH = internal@0x80000000,512Kb

# Clock source to use when programming: [{xtal|extclk|int}]
PROG_CLOCK = xtal

# Device/Platform/Board include path
PLATFORM_INC_PATH =

# Target name: {*.a|*.elf}
TARGET = libjpeg_opt.a

# Definitions: [-D name[=definition]...] [-U name...]
# Things that might be added to DEFS:
#   BOARD             Board used: see $(BRDS_PATH)/board.h
#   EXT_BOARD         Extension board used (if any): see $(BRDS_PATH)/board.h
DEFS =

# Include path
INC_PATH = \
	../../

JLIB_PATH = $(SERV_PATH)/PICTURES/JPG/IJG

# Encoder sources
CLIBSRC = \
  $(JLIB_PATH)/jcapimin.c \
  $(JLIB_PATH)/jcapistd.c \
  $(JLIB_PATH)/jctrans.c  \
  $(JLIB_PATH)/jcparam.c \
  $(JLIB_PATH)/jcinit.c   \
  $(JLIB_PATH)/jcmaster.c \
  $(JLIB_PATH)/jcmarker.c \
  $(JLIB_PATH)/jcmainct.c \
  $(JLIB_PATH)/jcprepct.c \
  $(JLIB_PATH)/jccoefct.c \
  $(JLIB_PATH)/jccolor.c  \
  $(JLIB_PATH)/jcsample.c \
  $(JLIB_PATH)/jchuff.c   \
  $(JLIB_PATH)/jcphuff.c  \
  $(JLIB_PATH)/jcdctmgr.c \
  $(JLIB_PATH)/jfdctfst.c \
  $(JLIB_PATH)/jfdctflt.c \
  $(JLIB_PATH)/jfdctint.c

# Decoder sources
DLIBSRC = \
  $(JLIB_PATH)/jdapimin.c \
  $(JLIB_PATH)/jdapistd.c \
  $(JLIB_PATH)/jdtrans.c  \
  $(JLIB_PATH)/jdmaster.c \
  $(JLIB_PATH)/jdinput.c  \
  $(JLIB_PATH)/jdmarker.c \
  $(JLIB_PATH)/jdhuff.c   \
  $(JLIB_PATH)/jdphuff.c  \
  $(JLIB_PATH)/jdmainct.c \
  $(JLIB_PATH)/jdcoefct.c \
  $(JLIB_PATH)/jdpostct.c \
  $(JLIB_PATH)/jddctmgr.c \
  $(JLIB_PATH)/jidctfst.c \
  $(JLIB_PATH)/jidctflt.c \
  $(JLIB_PATH)/jidctint.c \
  $(JLIB_PATH)/jidctred.c \
  $(JLIB_PATH)/jdsample.c \
  $(JLIB_PATH)/jdcolor.c  \
  $(JLIB_PATH)/jquant1.c  \
  $(JLIB_PATH)/jquant2.c  \
  $(JLIB_PATH)/jdmerge.c

SYSDEPSRC = \
  $(JLIB_PATH)/jmemnobs.c

# common library files
COMLIBSRC = \
  $(JLIB_PATH)/jcomapi.c \
  $(JLIB_PATH)/jutils.c \
  $(JLIB_PATH)/jerror.c \
  $(JLIB_PATH)/jmemmgr.c

# C source files
CSRCS = \
  $(COMLIBSRC) \
  $(SYSDEPSRC) \
  $(DLIBSRC)

# Assembler source files
ASSRCS =

# Library path
LIB_PATH =

# Libraries to link with the project
LIBS =

# Linker script file if any
LINKER_SCRIPT =

# Options to request or suppress warnings: [-fsyntax-only] [-pedantic[-errors]] [-w] [-Wwarning...]
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
WARNINGS = -Wall

# Options for debugging: [-g]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
DEBUG =

# Options that control optimization: [-O[0|1|2|3|s]]...
# For further details, refer to the chapter "GCC Command Options" of the GCC manual.
OPTIMIZATION = -O3 -ffunction-sections

# Extra flags to use when preprocessing
CPP_EXTRA_FLAGS =

# Extra flags to use when compiling
C_EXTRA_FLAGS =

# Extra flags to use when assembling
AS_EXTRA_FLAGS =

# Extra flags to use when linking
LD_EXTRA_FLAGS =

# Documentation path
DOC_PATH = \
  ../../DOC/

# Documentation configuration file
DOC_CFG = \
  ../doxyfile.doxygen
