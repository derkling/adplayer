/* This source file is part of the ATMEL AVR-UC3-SoftwareFramework-1.6.1 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief FAT 12/16/32 Services.
 *
 * This plug-in allow a navigation in FLAT mode
 * This plug-in is connected on the navigation module.
 * The flat mode includes in the file list all directories and all files of the current directory AND the sub directories
 * Before use the flat mode, you must call nav_partition_mount() to reset navigation
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
//_____  I N C L U D E S ___________________________________________________
#include "conf_explorer.h"
#include "navigation.h"
#include "nav_flat.h"
#include "file.h"
#include LIB_CTRLACCESS


//_____ D E C L A R A T I O N S ____________________________________________

//**********************************************************************
//****************** File list navigation FLAT functions ***************


//! This function mounts the selected partition 
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! If the FS_MULTI_PARTITION option is disabled
//! then the mount routine selects the first partition supported by file system. <br>
//! After mount, the file list contains files and directories of ROOT directory 
//! @endverbatim
//!
Bool  nav_flat_mount( void )
{
   fs_g_nav.u8_flat_dir_level = 0;
   fs_g_nav.u16_flat_pos_offset = 0;   
   return nav_partition_mount();
}


//! This function initializes the file filtered list on the root directory
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
Bool  nav_flat_root( void )
{
   fs_g_nav.u8_flat_dir_level = 0;
   fs_g_nav.u16_flat_pos_offset = 0;   
   return nav_dir_root();
}

//! This function resets the selection pointer, so "no file is selected" in file list FLAT
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
Bool  nav_flat_reset( void )
{
   while( 0 != fs_g_nav.u8_flat_dir_level ) 
   {
      fs_g_nav.u8_flat_dir_level--;
      nav_dir_gotoparent();
   }
   fs_g_nav.u16_flat_pos_offset = 0;
   return nav_filelist_reset();
}


//! This function checks if a file is selected
//!
//! @return    TRUE  if a file is selected
//! @return    FALSE if no file is selected
//!
Bool  nav_flat_validpos( void )
{
   return nav_filelist_validpos();
}


//! This function checks if no file is opened
//!
//! @return    TRUE  if no file is opened
//! @return    FALSE if a file is opened
//!
Bool  nav_flat_fileisnotopen( void )
{
   return nav_filelist_fileisnotopen();
}


//! Go to next file or directory in file list FLAT
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
Bool  nav_flat_next( void )
{
   U16 u16_save_current_pos;
   
   u16_save_current_pos = nav_filelist_get();
   if( nav_file_isdir() ) 
   {
      // The current file is a dir then enter in this
      if( !nav_dir_cd() )
         return FALSE;
      if( nav_filelist_set( 0 , FS_FIND_NEXT ) )
      {
         // A file is present in this dir then valid the new position
         fs_g_nav.u8_flat_dir_level++;
         fs_g_nav.u16_flat_pos_offset += u16_save_current_pos+1;
         return TRUE;
      }else{
         // No file then return in parent dir
         if( !nav_dir_gotoparent() )
            return FALSE;  // Error
      }
   }
   // Find next file in the current dir or the parent dir
   while( !nav_filelist_set( 0 , FS_FIND_NEXT ) )
   {
      // End of the directory then goes to parent
      if( 0 == fs_g_nav.u8_flat_dir_level )
         return FALSE;  // End of list FLAT
      if( !nav_dir_gotoparent() )
         return FALSE;  // Error
      fs_g_nav.u8_flat_dir_level--;
   }
   fs_g_nav.u16_flat_pos_offset = (fs_g_nav.u16_flat_pos_offset +u16_save_current_pos +1) -nav_filelist_get();
   return TRUE;
}


//! Go to previous file or directory in file list FLAT
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
Bool  nav_flat_previous( void )
{
   U16 u16_save_current_pos;
   
   u16_save_current_pos = nav_filelist_get();
   if( nav_filelist_set( 0 , FS_FIND_PREV ) )
   {
      while( 1 )
      {
         if( !nav_file_isdir() ) 
         {
            fs_g_nav.u16_flat_pos_offset = ((fs_g_nav.u16_flat_pos_offset +u16_save_current_pos) -nav_filelist_get()) -1;
            return TRUE;   // New valid position
         }
         // The selection is a dir then enter in this
         if( !nav_dir_cd() )
            return FALSE;
         if( !nav_filelist_set( 0 , FS_FIND_NEXT ) )
         {
            // The dir is empty then goes to the parent
            nav_dir_gotoparent();
            // The parent dir is the new selection
            fs_g_nav.u16_flat_pos_offset = ((fs_g_nav.u16_flat_pos_offset +u16_save_current_pos) -nav_filelist_get()) -1;
            return TRUE;
         }
         fs_g_nav.u8_flat_dir_level++;
         // Go to the end of dir 
         while( nav_filelist_set( 0 , FS_FIND_NEXT ) );
      }
   }
   // Beginning of the directory then goes to parent directory
   if( 0 == fs_g_nav.u8_flat_dir_level )
      return FALSE;  // beginning of the file list FLAT
   if( !nav_dir_gotoparent() )
      return FALSE;
   fs_g_nav.u8_flat_dir_level--;
   fs_g_nav.u16_flat_pos_offset -= (nav_filelist_get()+1);
   return TRUE;
}


//! This function returns the position of selected file in file list FLAT
//!
//! @return    position of selected file (0 is the first position)
//! @return    FS_NO_SEL, in case of no file selected
//!
U16   nav_flat_get( void )
{
   return (fs_g_nav.u16_flat_pos_offset + nav_filelist_get());
}


//! This function goes to a position in file list FLAT
//!
//! @param     u16_newpos     new position to select (0 is the first position)
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_flat_goto( U16 u16_newpos )
{
   if (u16_newpos < (nav_flat_get()/2))
   {
      // Restart at the beginning of list to accelerate the search
      if (!nav_flat_reset())
         return FALSE;      
   }
   if (!nav_flat_validpos())
   {
      // Go to first position
      if (!nav_flat_next())
         return FALSE;  // Empty list
   }
   
   while(u16_newpos < nav_flat_get())
   {
      if( !nav_flat_previous())
         return FALSE; // Position error
   }
   while(u16_newpos > nav_flat_get())
   {
      if( !nav_flat_next())
         return FALSE; // End of the list, then the position is bad
   }
   return TRUE;
}


//! This function searchs a file name in file list FLAT
//!
//! @param     sz_name        name to search (UNICODE or ASCII) <br>
//!                           It must be terminated by NULL or '*' value
//! @param     b_match_case   FALSE to ignore the case
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! This function starts a search at the next position of the current in file list 
//! @endverbatim
//!
Bool  nav_flat_findname( const FS_STRING sz_name , Bool b_match_case )
{
   while( 1 )
   {
      if ( !nav_flat_next() )
         return FALSE;
      if ( nav_file_name( sz_name , 0 , FS_NAME_CHECK , b_match_case ))
         return TRUE;
   }
}


//! This function computes the number of files and directories present in the file list FLAT
//!
//! @return    number of files and directories present in the file list FLAT
//!
U16   nav_flat_nb( void )
{
   U16   u16_save_position;
   U16   u16_number;
   
   // Save current position
   u16_save_position = nav_flat_get();

   // Reset position
   if ( !nav_flat_reset())
      return 0;

   // Scan all directories
   u16_number  = 0;
   while( nav_flat_next() )
   {
      u16_number++;
   }
   // Restore previous position
   nav_flat_goto( u16_save_position );

   return u16_number;
}


//! This function enters in the selected directory in file list FLAT
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
Bool  nav_flat_cd( void )
{
   if( !nav_dir_cd() )
      return FALSE;
   fs_g_nav.u8_flat_dir_level   = 0;
   fs_g_nav.u16_flat_pos_offset = 0;
   return TRUE;
}


//! This function goes to the parent directory
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! After the selected file is the first entry of the new file list FLAT
//! @endverbatim
//!
Bool  nav_flat_gotoparent( void )
{
   _MEM_TYPE_SLOW_ Fs_index index;
   index = nav_getindex();
   while( 0 != fs_g_nav.u8_flat_dir_level ) 
   {
      fs_g_nav.u8_flat_dir_level--;
      nav_dir_gotoparent();
   }
   if( !nav_dir_gotoparent() )
   {
      nav_gotoindex( &index );
      return FALSE;
   }
   // Go to at the beginning of FLAT list
   fs_g_nav.u8_flat_dir_level   = 0;
   fs_g_nav.u16_flat_pos_offset = 0;
   nav_filelist_reset();
   nav_flat_next();
   return TRUE;
}

