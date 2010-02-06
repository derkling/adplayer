#!/bin/sh

#uncomment on to turn on debug mode
#set -x

LIB_PATH=`pwd`
OPTIM="-ffunction-sections"
BUILD="";
CHECK="";
SILENT=1
HELP="";
EXITFLAG=0;
GCC_PROJECT_PATH=`find . -name '.svn' -prune -o -name GCC -print`
IAR_PROJECT_PATH=`find . -name '.svn' -prune -o -name IAR -print`
IAR_BUILD=`reg query "HKEY_LOCAL_MACHINE\SOFTWARE\IAR Systems\Embedded Workbench\3.1\EWAVR32" | grep "InstallPath" | sed 's/.*REG_SZ.\s*//' | sed 's/\\\\/\\//g'`'/common/bin/iarbuild.exe'

#
# Display the usage of the script
# -------------------------------
function usage() {
   cat <<EOF
   `basename $0` v0.0.4 sept. 2007
   Manage the Driver Lib. This script, depending the options, can create all the libs (for
   all targets) for both GCC and IAR compilers, or check if the current version is up to
   date (according to the current sources of the drivers).
   usage: [-v] [-b]|[-c]
   default  build libs in silent mode
   -b       build the driver libs
   -c       check the driver libs versus current driver sources
   -v       verbose mode
EOF
}

#
# Build the headers for each target
# ---------------------------------
function build_headers() {
   # Make sure we are at the top dir
   cd "$LIB_PATH"

   # Create the directory if it does not exist yet
   for cur_gcc_project in $GCC_PROJECT_PATH
   do
      if [ ! -d "$cur_gcc_project"/../INCLUDE ] ; then
         echo Making INCLUDE folder
         mkdir -p "$cur_gcc_project"/../INCLUDE
      fi
   done

   # Filtering depending Product
   if [ -e AT32UC3A ] ; then
      echo Creating AT32UC3A header files
      cp ../../../DRIVERS/ADC/adc.h                         AT32UC3A/INCLUDE
      cp ../../../DRIVERS/CPU/CYCLE_COUNTER/cycle_counter.h AT32UC3A/INCLUDE
      cp ../../../DRIVERS/EIC/eic.h                         AT32UC3A/INCLUDE
      cp ../../../DRIVERS/FLASHC/flashc.h                   AT32UC3A/INCLUDE
      cp ../../../DRIVERS/GPIO/gpio.h                       AT32UC3A/INCLUDE
      cp ../../../DRIVERS/INTC/intc.h                       AT32UC3A/INCLUDE
      cp ../../../DRIVERS/PDCA/pdca.h                       AT32UC3A/INCLUDE
      cp ../../../DRIVERS/PM/pm.h                           AT32UC3A/INCLUDE
      cp ../../../DRIVERS/PM/power_clocks_lib.h             AT32UC3A/INCLUDE
      cp ../../../DRIVERS/PWM/pwm.h                         AT32UC3A/INCLUDE
      cp ../../../DRIVERS/RTC/rtc.h                         AT32UC3A/INCLUDE
      cp ../../../DRIVERS/SPI/spi.h                         AT32UC3A/INCLUDE
      cp ../../../DRIVERS/SSC/I2S/ssc_i2s.h                 AT32UC3A/INCLUDE
      cp ../../../DRIVERS/TC/tc.h                           AT32UC3A/INCLUDE
      cp ../../../DRIVERS/TWI/twi.h                         AT32UC3A/INCLUDE
      cp ../../../DRIVERS/USART/usart.h                     AT32UC3A/INCLUDE
      cp ../../../DRIVERS/WDT/wdt.h                         AT32UC3A/INCLUDE
   fi

   if [ -e AT32UC3B ] ; then
      echo Creating AT32UC3B header files
      cp ../../../DRIVERS/ADC/adc.h                         AT32UC3B/INCLUDE
      cp ../../../DRIVERS/CPU/CYCLE_COUNTER/cycle_counter.h AT32UC3B/INCLUDE
      cp ../../../DRIVERS/EIC/eic.h                         AT32UC3B/INCLUDE
      cp ../../../DRIVERS/FLASHC/flashc.h                   AT32UC3B/INCLUDE
      cp ../../../DRIVERS/GPIO/gpio.h                       AT32UC3B/INCLUDE
      cp ../../../DRIVERS/INTC/intc.h                       AT32UC3B/INCLUDE
      cp ../../../DRIVERS/PDCA/pdca.h                       AT32UC3B/INCLUDE
      cp ../../../DRIVERS/PM/pm.h                           AT32UC3B/INCLUDE
      cp ../../../DRIVERS/PM/power_clocks_lib.h             AT32UC3B/INCLUDE
      cp ../../../DRIVERS/PWM/pwm.h                         AT32UC3B/INCLUDE
      cp ../../../DRIVERS/RTC/rtc.h                         AT32UC3B/INCLUDE
      cp ../../../DRIVERS/SPI/spi.h                         AT32UC3B/INCLUDE
      cp ../../../DRIVERS/SSC/I2S/ssc_i2s.h                 AT32UC3B/INCLUDE
      cp ../../../DRIVERS/TC/tc.h                           AT32UC3B/INCLUDE
      cp ../../../DRIVERS/TWI/twi.h                         AT32UC3B/INCLUDE
      cp ../../../DRIVERS/USART/usart.h                     AT32UC3B/INCLUDE
      cp ../../../DRIVERS/WDT/wdt.h                         AT32UC3B/INCLUDE
   fi

   if [ -e AT32UC3A3 ] ; then
      echo Creating AT32UC3A3 header files
      cp ../../../DRIVERS/ADC/adc.h                         AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/CPU/CYCLE_COUNTER/cycle_counter.h AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/EIC/eic.h                         AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/FLASHC/flashc.h                   AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/GPIO/gpio.h                       AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/INTC/intc.h                       AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/PDCA/pdca.h                       AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/PM/pm.h                           AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/PM/power_clocks_lib.h             AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/RTC/rtc.h                         AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/SPI/spi.h                         AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/SSC/I2S/ssc_i2s.h                 AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/TC/tc.h                           AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/TWIM/twim.h                       AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/TWIS/twis.h                       AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/USART/usart.h                     AT32UC3A3/INCLUDE
      cp ../../../DRIVERS/WDT/wdt.h                         AT32UC3A3/INCLUDE
   fi

   if [ -e AT32UC3L ] ; then
      echo Creating AT32UC3L header files
      cp ../../../DRIVERS/ACIFB/acifb.h                     AT32UC3L/INCLUDE
      cp ../../../DRIVERS/ADCIFB/adcifb.h                   AT32UC3L/INCLUDE
      cp ../../../DRIVERS/CPU/CYCLE_COUNTER/cycle_counter.h AT32UC3L/INCLUDE
      cp ../../../DRIVERS/CPU/MPU/mpu.h                     AT32UC3L/INCLUDE
      cp ../../../DRIVERS/FLASHCDW/flashcdw.h               AT32UC3L/INCLUDE
      cp ../../../DRIVERS/GPIO/gpio.h                       AT32UC3L/INCLUDE
      cp ../../../DRIVERS/INTC/intc.h                       AT32UC3L/INCLUDE
      cp ../../../DRIVERS/PDCA/pdca.h                       AT32UC3L/INCLUDE
      cp ../../../DRIVERS/PM/pm_uc3l.h                      AT32UC3L/INCLUDE
      cp ../../../DRIVERS/PM/power_clocks_lib.h             AT32UC3L/INCLUDE
      cp ../../../DRIVERS/PWMA/pwma.h                       AT32UC3L/INCLUDE
      cp ../../../DRIVERS/SCIF/scif_uc3l.h                  AT32UC3L/INCLUDE
      cp ../../../DRIVERS/SPI/spi.h                         AT32UC3L/INCLUDE
      cp ../../../DRIVERS/TC/tc.h                           AT32UC3L/INCLUDE
      cp ../../../DRIVERS/TWIM/twim.h                       AT32UC3L/INCLUDE
      cp ../../../DRIVERS/TWIS/twis.h                       AT32UC3L/INCLUDE
      cp ../../../DRIVERS/USART/usart.h                     AT32UC3L/INCLUDE
   fi
   
   if [ -e AT32UC3C ] ; then
      echo Creating AT32UC3C header files
      cp ../../../DRIVERS/ACIFA/acifa.h                     AT32UC3C/INCLUDE
      cp ../../../DRIVERS/ADCIFA/adcifa.h                   AT32UC3C/INCLUDE
      cp ../../../DRIVERS/AST/ast.h                         AT32UC3C/INCLUDE
      cp ../../../DRIVERS/CPU/CYCLE_COUNTER/cycle_counter.h AT32UC3C/INCLUDE
      cp ../../../DRIVERS/DACIFB/dacifb.h                   AT32UC3C/INCLUDE
      cp ../../../DRIVERS/EIC/eic.h                         AT32UC3A3/INCLUDE      
      cp ../../../DRIVERS/FLASHC/flashc.h                   AT32UC3C/INCLUDE
      cp ../../../DRIVERS/GPIO/gpio.h                       AT32UC3C/INCLUDE
      cp ../../../DRIVERS/INTC/intc.h                       AT32UC3C/INCLUDE
      cp ../../../DRIVERS/MDMA/mdma.h                       AT32UC3C/INCLUDE
      cp ../../../DRIVERS/PDCA/pdca.h                       AT32UC3C/INCLUDE
      cp ../../../DRIVERS/PEVC/pevc.h                       AT32UC3C/INCLUDE
      cp ../../../DRIVERS/PM/pm_uc3c.h                      AT32UC3C/INCLUDE
      cp ../../../DRIVERS/PM/power_clocks_lib.h             AT32UC3C/INCLUDE
      cp ../../../DRIVERS/PWM/pwm4.h                        AT32UC3C/INCLUDE  
      cp ../../../DRIVERS/QDEC/qdec.h                       AT32UC3C/INCLUDE     
      cp ../../../DRIVERS/SCIF/scif_uc3c.h                  AT32UC3C/INCLUDE
      cp ../../../DRIVERS/SPI/spi.h                         AT32UC3C/INCLUDE
      cp ../../../DRIVERS/SSC/ssc.h                         AT32UC3C/INCLUDE
      cp ../../../DRIVERS/TC/tc.h                           AT32UC3C/INCLUDE
      cp ../../../DRIVERS/TWIM/twim.h                       AT32UC3C/INCLUDE
      cp ../../../DRIVERS/USART/usart.h                     AT32UC3C/INCLUDE
   fi   
}

#
# Build the GCC libs for each target
# ----------------------------------
function build_GCC_libs() {
   for cur_gcc_project in $GCC_PROJECT_PATH
   do
      # go to the GCC directory
      cd "$LIB_PATH"
      cd "$cur_gcc_project"
      # Target name is the name of the 1st sub directory
      TARGET=`echo $cur_gcc_project | tr -s '/' '\n' | head --lines=2 | tail --lines=1 | tr " [A-Z] " " [a-z] "`

      if [ -e "Makefile" ] ; then
         make rebuild TARGET=libdrivers-$TARGET-debug.a        "OPTIMIZATION= -O0 $OPTIM" "DEBUG= -g"
         make rebuild TARGET=libdrivers-$TARGET.a              "OPTIMIZATION= -O0 $OPTIM" "DEBUG="
         make rebuild TARGET=libdrivers-$TARGET-balanced_opt.a "OPTIMIZATION= -O2 $OPTIM" "DEBUG="
         make rebuild TARGET=libdrivers-$TARGET-speed_opt.a    "OPTIMIZATION= -O3 $OPTIM" "DEBUG="
         make rebuild TARGET=libdrivers-$TARGET-size_opt.a     "OPTIMIZATION= -Os $OPTIM" "DEBUG="
      fi
   done

   cd "$LIB_PATH"
}

#
# Build the IAR libs for each target
# ----------------------------------
function build_IAR_libs() {
   for cur_iar_project in $IAR_PROJECT_PATH
   do
      # go to the GCC directory
      cd "$LIB_PATH"
      cd "$cur_iar_project"
      # Target name is the name of the 1st sub directory
      TARGET=`echo $cur_iar_project | tr -s '/' '\n' | head --lines=2 | tail --lines=1 | tr " [A-Z] " " [a-z] "`

      project_file_name=`find ./ -name *.ewp | head -n 1`
      if [ -e "$project_file_name" ] ; then
       	"$IAR_BUILD" "$project_file_name" -build Release      -log warnings
       	"$IAR_BUILD" "$project_file_name" -build speed_opt    -log warnings
       	"$IAR_BUILD" "$project_file_name" -build size_opt     -log warnings
       	"$IAR_BUILD" "$project_file_name" -build balanced_opt -log warnings
      fi
   done

   cd "$LIB_PATH"
}

#function comp_header() {
#}

#
# Compare the built target vs the reference
# -----------------------------------------
function comp_file() {
   cmp $FILE ref-$FILE > /dev/null
   if [ $? != 0 ]
   then
      echo $FILE: done | sed 's/\.lss/.a/g'
   else
      echo $FILE: idle done | sed 's/\.lss/.a/g'
   fi
}



#
# Here starts the script
# ----------------------
while [ $# != 0 ]
do
    case $1 in
   -b) BUILD=1 ;;
   -c) CHECK=1 ;;
   -v) SILENT="" ;;
   -*) echo unknown flag $1 >&2 ;HELP=1;EXITFLAG=1 ;;
    esac
    shift
done

[ $HELP ] && {
   usage
   exit $EXITFLAG
}

[ $BUILD ] && [ $CHECK ] && {
   echo Cannot build and check at the same time. Please choose one option only.
   exit 1
}

[ $BUILD ] || [ $CHECK ] || {
   BUILD=1
}

# Build the Drivers Libs and headers
# ----------------------------------
[ $BUILD ] && {
   echo "Creating Driver Headers"
   if [ $SILENT ]
   then
      build_headers >> /dev/null 2>> /dev/null
   else
      build_headers
   fi

   echo "Creating GCC Driver Libs (this may take some time...)"
   if [ $SILENT ]
   then
      build_GCC_libs >> /dev/null 2>> /dev/null
   else
      build_GCC_libs
   fi

   # Cleaning unwanted objects
   rm -f */GCC/*.lss */GCC/*.sym

   echo "Creating IAR Driver Libs (this may take some time...)"
   if [ $SILENT ]
   then
      build_IAR_libs >> /dev/null 2>> /dev/null
   else
      build_IAR_libs
   fi


   exit $EXITFLAG
}

# Check the Drivers Libs and headers
# ----------------------------------
[ $CHECK ] && {
   # lss creation is done *from* $TMP_FILES/GCC, otherwise avr32-objdump does not add source lines
   echo Backuping driver libs in order to use them as reference.
   for cur_gcc_project in $GCC_PROJECT_PATH
   do
      # go to the GCC directory
      cd "$LIB_PATH"
      cd "$cur_gcc_project"
      # Target name is the name of the 1st sub directory
      TARGET=`echo $cur_gcc_project | tr -s '/' '\n' | head --lines=2 | tail --lines=1 | tr " [A-Z] " " [a-z] "`

      avr32-objdump -h -S libdrivers-$TARGET-debug.a        > ref-libdrivers-$TARGET-debug.lss
      avr32-objdump -h -S libdrivers-$TARGET.a              > ref-libdrivers-$TARGET.lss
      avr32-objdump -h -S libdrivers-$TARGET-balanced_opt.a > ref-libdrivers-$TARGET-balanced_opt.lss
      avr32-objdump -h -S libdrivers-$TARGET-speed_opt.a    > ref-libdrivers-$TARGET-speed_opt.lss
      avr32-objdump -h -S libdrivers-$TARGET-size_opt.a     > ref-libdrivers-$TARGET-size_opt.lss
   done

   cd "$LIB_PATH"

   echo "Creating Driver Libs (this may take some time...)"
   if [ $SILENT ]
   then
      build_GCC_libs >> /dev/null 2>> /dev/null
   else
      build_GCC_libs
   fi

   echo Comparing Driver libs.
   for cur_gcc_project in $GCC_PROJECT_PATH
   do
      # go to the GCC directory
      cd "$LIB_PATH"
      cd "$cur_gcc_project"
      # Target name is the name of the 1st sub directory
      TARGET=`echo $cur_gcc_project | tr -s '/' '\n' | head --lines=2 | tail --lines=1 | tr " [A-Z] " " [a-z] "`

      if [ -e "Makefile" ] ; then
         FILE=libdrivers-$TARGET-debug.lss        ; comp_file
         FILE=libdrivers-$TARGET.lss              ; comp_file
         FILE=libdrivers-$TARGET-balanced_opt.lss ; comp_file
         FILE=libdrivers-$TARGET-speed_opt.lss    ; comp_file
         FILE=libdrivers-$TARGET-size_opt.lss     ; comp_file
      fi
   done

   echo "Legend:"
   echo "  done     : the target is rebuilt and NEW compared to the previous built."
   echo "  idle done: the target is rebuilt and identical to the previous built."
   cd "$LIB_PATH"

   # Cleaning unwanted objects
   rm -f */GCC/*.lss */GCC/*.sym
}
