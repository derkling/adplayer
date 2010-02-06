#!/bin/sh

# set -x

################################################################################
# This script list all sub-directories in the "library path" (LIB_PATH) and build the libraries of the targets from this file.
#
# Configuration
#
# Library path
LIB_PATH='./'
# Global optimizations
GCC_OPTIONS="-fno-strict-aliasing -ffunction-sections"
# Optimizations to perform
# The optimizations have to be separated by the character ':'
#       <void>/default  : no optimization
#       speed     : speed optimization
#       size      : size optimization
#       debug     : debugging information
#       balanced  : speed/size optimization
#       dsp_speed : speed optimization (for the DSPLib only)
#       dsp_size  : size optimization (for the DSPLib only)
#       dsp_acc   : accuracy optimization (for the DSPLib only)
#       dsp_acc_size  : accuracy/size optimization (for the DSPLib only)
OPTIM_LIST='dsp_speed:dsp_size:dsp_acc:dsp_acc_size:'
# The name of the library
LIB_NAME='dsp'
# The regular expression (sed syntax) for path the exclude
EXCLUDE_PATH='RELEASES\/'
# Headers that need to be merged to form a $LIB_NAME.h file
# The headers have to be separated by the character ':'
DSPLIB_INC_DIR="../../../SERVICES/DSPLIB/INCLUDE"
MERGED_HEADERS="$DSPLIB_INC_DIR/documentation.h:$DSPLIB_INC_DIR/dsp_types.h:$DSPLIB_INC_DIR/dsp_vectors.h:$DSPLIB_INC_DIR/dsp_filtering.h:$DSPLIB_INC_DIR/dsp_operators.h:$DSPLIB_INC_DIR/dsp_signal_generation.h:$DSPLIB_INC_DIR/dsp_transforms.h:$DSPLIB_INC_DIR/dsp_windowing.h:$DSPLIB_INC_DIR/dsp_adpcm.h:$DSPLIB_INC_DIR/dsp_resampling.h"
# Copy header files
COPY_HEADERS=
################################################################################

is_regtool=`which regtool`
# IAR compiler path
if [ ! "$is_regtool" == "" ];
then
  IAR_COMP="`regtool get '\HKEY_LOCAL_MACHINE\SOFTWARE\IAR Systems\Embedded Workbench\3.1\EWAVR32\InstallPath'`/common/bin/iarbuild.exe"
else
  IAR_COMP=`reg query "HKEY_LOCAL_MACHINE\SOFTWARE\IAR Systems\Embedded Workbench\3.1\EWAVR32" | grep "InstallPath" | sed 's/.*REG_SZ.\s*//' | sed 's/\\\\/\\//g'`'/common/bin/iarbuild.exe'
fi

# Supported compiler list
COMPILER_LIST="GCC:IAR"
# Default values of the arguements
VERBOSE=""
# Default rebuild
REBUILD="1"
# Default value for header files
ONLY_HEADERS=""

# Display the usage of the script
function usage () {
  cat <<EOF
  usage: build [-v] [-h] [-m] [--optim <otim1:optim2:..>] [--only <compiler>]
    -v  : Mode verbose
    -h, --help  : Display this help
    -m  : Do not clean the project before building the library
    --optim <otim1:optim2:..> : Set the list of the optimization needed. The optimizations have to be separated by the character ':'
                                The following optimization options are supported:
                                speed         : speed optimization
                                size          : size optimization
                                debug         : debugging information
                                balanced      : speed/size optimization
                                dsp_speed     : speed optimization (for the DSPLib only)
                                dsp_size      : size optimization (for the DSPLib only)
                                dsp_acc       : accuracy optimization (for the DSPLib only)
                                dsp_acc_size  : accuracy/size optimization (for the DSPLib only)
    --only <compiler> : Builds only <compiler> libraries. i.e. IAR or GCC libraries
    --header : Rebuild only header files.
EOF
}

# Set library name
# $1 : compiler name in upper case
# $2 : the name of the library.
# $3, $4, $5, ... : other arguments
# lib<name>-<$2>-<$3>.a
function lib_name () {
  case $1 in
    "GCC")  EXT="a" ;;
    "IAR")  EXT="r82" ;;
        *)  EXT="lib" ;;
  esac
  shift
  LIB_NAME="lib"$1
  while [ $# -ne 1 ]
  do
    shift
    if [ "$1" ]; then
  	  LIB_NAME=$LIB_NAME-$1
    fi
  done
  echo $LIB_NAME"."$EXT
}

# This function returns the optimization argument to pass to the Makefile
# $1 : The optimization.
#       <void>/default  : no optimization
#       speed     : speed optimization
#       size      : size optimization
#       debug     : debugging information
#       balanced  : speed/size optimization
#       dsp_speed : speed optimization (for the DSPLib only)
#       dsp_size  : size optimization (for the DSPLib only)
#       dsp_acc   : accuracy optimization (for the DSPLib only)
#       dsp_acc_size  : accuracy/size optimization (for the DSPLib only)
# $2 : The compiler name in upper case
#       i.e. GCC or IAR
function optimization_arg () {
  case $2 in

    "GCC")
      case $1 in
        # Global optimizations
        "speed")      echo "-O3" ;;
        "size")       echo "-Os" ;;
        "balanced")   echo "-O2" ;;
        "debug")      echo "-O0 -g" ;;
        # DSPLib specific
        "dsp_speed")  echo "`optimization_arg speed GCC` -D DSP_OPTIMIZATION=DSP_OPTI_SPEED" ;;
        "dsp_size")   echo "`optimization_arg size GCC` -D DSP_OPTIMIZATION=DSP_OPTI_SIZE" ;;
        "dsp_acc")    echo "-O3 -D DSP_OPTIMIZATION=DSP_OPTI_ACCURACY" ;;
        "dsp_acc_size") echo "-Os -D DSP_OPTIMIZATION=DSP_OPTI_ACC_AND_SIZE" ;;
        # Default
        *)  echo "-O0" ;;
      esac
      ;;

    "IAR")
      case $1 in
        # Global optimizations
        "speed")      echo "speed_opt" ;;
        "size")       echo "size_opt" ;;
        "balanced")   echo "balanced_opt" ;;
        "debug")      echo "debug_opt" ;;
        # DSPLib specific
        "dsp_speed")  echo "dspspeed_opt" ;;
        "dsp_size")   echo "dspsize_opt" ;;
        "dsp_acc")    echo "dspaccuracy_opt" ;;
        "dsp_acc_size") echo "dspaccandsize_opt" ;;
        # Default
        *)  echo "Release" ;;
      esac
      ;;

    *)  echo "" ;;

  esac
}

# This function returns the name of the optimization
# $1 : the optimization.
#       none      : no optimization
#       speed     : speed optimization
#       size      : size optimization
#       debug     : debugging information
#       balanced  : speed/size optimization
#       dsp_speed : speed optimization (for the DSPLib only)
#       dsp_size  : size optimization (for the DSPLib only)
#       dsp_acc   : accuracy optimization (for the DSPLib only)
#       dsp_acc_size  : accuracy/size optimization (for the DSPLib only)
function optimization_name () {
  case $1 in
    # Global optimizations
    "speed")      echo "speed_opt" ;;
    "size")       echo "size_opt" ;;
    "balanced")   echo "balanced_opt" ;;
    "debug")      echo "debug_opt" ;;
    # DSPLib specific
    "dsp_speed")  echo "dspspeed_opt" ;;
    "dsp_size")   echo "dspsize_opt" ;;
    "dsp_acc")    echo "dspaccuracy_opt" ;;
    "dsp_acc_size") echo "dspaccandsize_opt" ;;
    # Default
    *)  echo "" ;;
  esac 
}

# Regroup all the header files in one
function build_headers() {
  cd "$LIB_PATH"

  MERGED_HEADERS=`echo $MERGED_HEADERS | tr -s ':' '\n'`

  # Create the directory if it does not exist yet
  if [ ! -d "./INCLUDE" ] ; then
    mkdir "./INCLUDE"
  fi

  # Clean the file
  echo "" > "./INCLUDE/$LIB_NAME.h"

  echo -e "// This header file is a compilation of multiple $LIB_NAME header files\r\n" >> "./INCLUDE/$LIB_NAME.h"

  # Build the header file
  for header in $MERGED_HEADERS
  do
    cat "$header" >> "./INCLUDE/$LIB_NAME.h"
#    sed '/\/\*[*!].*\\file/,/\*\//{ /\*\//i d }' $header >> "./INCLUDE/$LIB_NAME.h"
  done;

  unix2dos.exe "./INCLUDE/$LIB_NAME.h" 2> /dev/null > /dev/null
}

warning_list=""
error_list=""
warning_count=0
error_count=0

# Function to check errors
# $1 : The name of the file
# $2 : The compiler name in upper case
#       i.e. GCC or IAR
function check_ew() {
  warning_count=0
  error_count=0

  case $2 in

  "GCC")
    warning_list="`echo "$1" | grep [Ww]arning`"
    error_list="`echo "$1" | grep '[Ee]rror'`"
    error_list="$error_list""`echo "$1" | grep 'Stop'`"
    ;;

  "IAR")
    warning_list=`echo "$1" | grep 'Warning'`
    error_list=`echo "$1" | grep 'Error|Usage'`
    ;;

  *)
    warning_list=""
    error_list=""
    ;;

  esac

  if [ "$error_list" ] ; then
    error_count=`echo "$error_list" | wc -l`
  fi
  if [ "$warning_list" ] ; then
    warning_count=`echo "$warning_list" | wc -l`
  fi
}

# Print the number of errors and warnings
function print_errors() {
  printf "$error_count error(s), $warning_count warning(s)\n"

  if [ $warning_count -ne 0 ] ; then
    printf "$warning_list"
    printf "\n"
  fi
  if [ $error_count -ne 0 ] ; then
    if [ $warning_count -ne 0 ] ; then
      printf "\n"
    fi
    printf "$error_list\n"
  fi
}

# Compile a project inside specifics directories.
# i.e. inside GCC or IAR directories
# $1 : The name of the direcory
# $2 : The compiler command line.
#      Two patterns are available:
#        %[TARGET] replaced by the target name
#        %[OPTIM] replaced by the optimization options
#        %[PROJ] replaced by the project name
# $3 : Message
# $4 : Command line to find the project file name inside the project directory
function compile_ex() {

  MATCHING_DIR_NAME="$1"
  COMPILER_CMD_LINE="$2"
  COMPILER_MSG="$3"
  PROJ_CMD_LINE="$4"

  # Find all GCC sub-directories
  project_path=`find ./ -name "$MATCHING_DIR_NAME" -type d`
  # For each GCC directory, do ...
  for cur_project in $project_path
  do
    if [ "`echo "$cur_project" | sed -n '/'"$EXCLUDE_PATH"'/p'`" ]; then
      continue
    fi
  
    # go to the GCC directory
    cd "$LIB_PATH/$cur_project"

    # Target name is the name of the 1st sub directory
    TARGET=`echo $cur_project | sed 's/.*\/\([^\/]*\)\/[^\/]*$/\1/' | tr " [A-Z] " " [a-z] "`

    # For each optimization ...
    for cur_optim in $OPTIM_LIST
    do
      # Get the name of the optimization
      mylib_optim_name=`optimization_name "$cur_optim"`
      # Get the name of the library 
      mylib_name=`lib_name "$MATCHING_DIR_NAME" "$LIB_NAME" "$TARGET" "$mylib_optim_name"`
      # Get the optimization of the library
      mylib_optim=`optimization_arg "$cur_optim" "$MATCHING_DIR_NAME"`
      # Get the project file name
      mylib_project_name=`sh -c "$PROJ_CMD_LINE"`

      # Build the library
      # Set the arguments
      BUILD=`echo "$COMPILER_CMD_LINE" | sed "s/%\[OPTIM\]/$mylib_optim/g"`
      BUILD=`echo "$BUILD" | sed "s/%\[TARGET\]/\"$mylib_name\"/g"`
      BUILD=`echo "$BUILD" | sed "s/%\[PROJ\]/\"$mylib_project_name\"/g"`

      # Delete last library
      rm -f "$mylib_name"

      # Execute the command
      echo "$COMPILER_MSG '$mylib_name' to '$cur_project/$mylib_name'"
      if [ "$VERBOSE" ] ; then
        echo "$BUILD"
        sh -c "$BUILD" 2>&1
      else
        # Build
        output=`sh -c "$BUILD" 2>&1`

	# Check errors
        check_ew "$output" "$MATCHING_DIR_NAME"
        print_errors
      fi
    done

  done;
}

# Build libraries
# $1 : the name of the compiler in upper case
#   i.e. IAR or GCC
function compile() {

  COMP_NAME="$1"

  # Compiler message
  if [ "$REBUILD" ] ; then
    COMPILER_MSG="Rebuild library"
  else
    COMPILER_MSG="Build library"
  fi

  case "$COMP_NAME" in
    "GCC")
      if [ "$REBUILD" ] ; then
        COMPILER_CMD_LINE="make clean && make a TARGET=\"%[TARGET]\" OPTIMIZATION=\"$GCC_OPTIONS %[OPTIM]\""
      else
        COMPILER_CMD_LINE="make a TARGET=\"%[TARGET]\" OPTIMIZATION=\"$GCC_OPTIONS %[OPTIM]\""
      fi
      PROJ_CMD_LINE=""
      ;;

    "IAR")
      if [ "$REBUILD" ] ; then
        COMPILER_CMD_LINE="\"$IAR_COMP\" %[PROJ] -clean %[OPTIM] && \"$IAR_COMP\" %[PROJ] -build %[OPTIM] -log warnings"
      else
        COMPILER_CMD_LINE="\"$IAR_COMP\" %[PROJ] -build %[OPTIM] -log warnings"
      fi
      PROJ_CMD_LINE="find ./ -name \"*.ewp\" | head -n 1 | sed \"s/.\///g\""
      ;;

    *)  echo "Unknown compiler $COMP_NAME" && return ;;
  esac

  # Compile
  compile_ex "$COMP_NAME" "$COMPILER_CMD_LINE" "$COMPILER_MSG" "$PROJ_CMD_LINE"

}





# Read the arguments
while [ $# -ne 0 ]
do
	if [ $1 == "-v" ];
  then
    VERBOSE="1"
  elif [ $1 == "-m" ]; then
    REBUILD=""
  elif [ $1 == "--optim" ]; then
    shift
    OPTIM_LIST=$1
  elif [ $1 == "--only" ]; then
    shift
    COMPILER_LIST=$1
  elif [ $1 == "--header" ]; then
    ONLY_HEADERS="1"
  else
    usage
    exit
  fi
  shift
done


# Make a list
OPTIM_LIST=`echo "$OPTIM_LIST" | tr -s ':' '\n'`
COMPILER_LIST=`echo "$COMPILER_LIST" | tr -s ':' '\n'`
# Go to the library directory
cd "$LIB_PATH"
# Save the absolute path
LIB_PATH=`pwd`

if [ ! "$ONLY_HEADERS" ]; then
  for cur_compiler in $COMPILER_LIST
  do
    cd "$LIB_PATH"
    echo "Building $cur_compiler libraries"
    compile "$cur_compiler"
  done;
fi

# Update headers
if [ ! "$MERGED_HEADERS" == "" ]; then
echo "Update header files"
build_headers
fi
# Copy headers
if [ ! "$COPY_HEADERS" == "" ]; then
  echo "Copying header files"
  cd "$LIB_PATH"
  # Create the directory if it does not exist yet
  if [ ! -d "./INCLUDE" ] ; then
    mkdir "./INCLUDE"
  fi
  cp "$COPY_HEADERS" "./INCLUDE"
fi
