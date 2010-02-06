/* This header file is part of the ATMEL AVR-UC3-SoftwareFramework-1.6.1 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief Play list Services.
 *
 * This plug-in allow to open and manage a play list file (*.txt,*.m3u,*.m3u8,...) included in text file
 * This plug-in is connected on the reader_txt plug-in and the navigation module.
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

#ifndef _PLAY_LIST_H_
#define _PLAY_LIST_H_

#include "fs_com.h"
#include "conf_explorer.h"

//! \name Options for "pl_add()"
//! @{
#define PL_ADD_FILE        0x00
#define PL_ADD_DIR         0x01
#define PL_ADD_SUBDIR      0x02
//! @}

//! \name List of supported file extensions
#define PL_FILE_EXT        "m3u,pls,smp"

//********************* MAIN functions ********************* 

//! This function checks if a play list file is already opened
//!
//! @return TRUE     a play list file is opened
//!
Bool   pl_main_isopen( void );

//! This function checks if the play list file is modified
//!
//! @return TRUE     the play list file is modified
//!
Bool  pl_main_ismodify( void );

//! This function creates a new empty play list file
//!
//! @param     sz_name        contains the file name (ASCII or UNICODE )
//! @param     b_undo         TRUE to autorize the undo feature
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  pl_main_new( const FS_STRING sz_name , Bool b_undo );

//! This function opens the current selected file in the current navigator
//!
//! @param     b_undo         TRUE to autorize the undo feature
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool   pl_main_open( Bool b_undo );

//! This function saves the play list file
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool   pl_main_save( void );

//! This function closes the play list and selects this one in current navigator
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool   pl_main_close( void );

//! This function creates a copy of the play list in case of a restore action
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool   pl_main_modify( void );


//********************* Play list navigation functions ********************* 

//! Return the number of files in play list
//!
//! @return number of files in play list
//!
U16 pl_nav_getnbfile( void );

//! Return the position in the play list
//!
//! @return position in the play list
//!
U16 pl_nav_getpos( void );

//! This function checks the end of the play list
//!
//! @return TRUE if the position is the last file of the play list
//!
Bool pl_nav_eol( void );

//! This function goes to a position in the play list
//!
//! @param u16_position position in play list to go
//!
Bool  pl_nav_setpos( U16 u16_position );


//! This function read the playliost's entry at a specific position
//!
//! @param u16_position Position in play list.
//! @param sz_path      The address of the string where is stored the information.
//! @param u16_size_line  The length of the path.
//!
Bool pl_nav_readentry(U16 u16_position, FS_STRING *sz_pat, U16 *u16_size_line );

//! This function reads and checks the next line
//!
//! @param b_gotopath   PL_MAIN_READLINE_OPT_CHECKLINE to check the next line in the file.\n
//!                     PL_MAIN_READLINE_OPT_READSTRING to read the string in the text file.\n
//!                     PL_MAIN_READLINE_OPT_GOTOPATH to read, chack and goto the path.
//! @param id_nav       ID navigator to update with the selected file (ignore if b_gotopatch == FALSE)
//! @param sz_path      Address of the string returned. It is used only if PL_MAIN_READLINE_OPT_READSTRING or
//!                     PL_MAIN_READLINE_OPT_GOTOPATH options are specified.
//! @param u16_size_line  The length of the path.
//!
//! @return    TRUE  if a line with a correct path
//!            FALSE line ignored or end of file
//!
typedef enum
{
  PL_MAIN_READLINE_OPT_CHECKLINE,
  PL_MAIN_READLINE_OPT_READSTRING,
  PL_MAIN_READLINE_OPT_GOTOPATH
} readline_opt_t;
Bool  pl_main_readline( readline_opt_t opt, U8 id_nav, FS_STRING *sz_path, U16 *u16_size_line );


//********************* Play list control position ********************* 

//! This function tests the valid path in list
//!
//! @return FALSE = path is not valid in list - DEAD
//!
Bool  pl_sel_isvalid( void );


//********************* Play list additional functions ********************* 

//! This function adds files in play list
//!
//! @param sz_filterext add file only corresponding to the  extension filter
//! @param u8_mode      PL_ADD_FILE, PL_ADD_DIR, PL_ADD_SUBDIR
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! It is possible to select a file or all files in a directory
//! @endverbatim
//!
Bool   pl_add( const FS_STRING sz_filterext , U8 u8_mode );


//********************* Play list remove functions ********************* 

//! This function removes all files in the list
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool   pl_rem_sel_all( void );

//! This function removes the selected file in the list
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool   pl_rem_sel( void );

//! This function remove the dead file in the list
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool pl_rem_dead( void );


#if defined(PLAYLIST_USE_NON_BLOCKING_FUNCTION_SET) && \
    PLAYLIST_USE_NON_BLOCKING_FUNCTION_SET == ENABLED
//********************* Play list non-blocking function set ********************

//! This function opens the current selected file in the current navigator in
//! read only mode.\n
//! It is a non blocking-function that returns every n-file counted.
//!
//! @param     u16_n         The number of files to look for before returning.\n
//!                          If it contains the special value of 0, then, it
//!                          will not process the count of link inside the
//!                          playlist.
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more
//!            details.
//! @return    TRUE otherwise.
//!
Bool pl_main_open_non_blocking(U16 u16_n);

//! This function read the playlist's entry at a specific position.\n
//! It is implemented as a non-blocking function.
//!
//! @param data         This data has to be set to 0 if non-caching options are
//!                     used.
//! @param u16_position Position in play list.
//! @param sz_path      The address of the string where is stored the
//!                     information.
//! @param u16_size_line  The length of the path.
//! @param u16_n        The number of line to read at one time
//!
typedef struct
{
  U16 u16_current_pos;
  U32 u32_file_pos_saved;
  U8 nav_id_saved;
  Fs_management_entry fs_entry;
} pl_nav_readentry_context_t;

Bool pl_nav_readentry_non_blocking(pl_nav_readentry_context_t *data, U16 u16_position, FS_STRING *sz_path, U16 *u16_size_line, U16 u16_n);

#endif

#endif  // _PLAY_LIST_H_
