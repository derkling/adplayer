/* This source file is part of the ATMEL AVR-UC3-SoftwareFramework-1.6.1 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief FAT 12/16/32 Services.
 *
 * This plug-in allow a navigation in filter mode
 * This plug-in is connected on the navigation module.
 * The filter mode includes in the file list directories and only files corresponding at a extension
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
#include "nav_filterlist.h"
#include "file.h"
#include LIB_CTRLACCESS


//_____ D E C L A R A T I O N S ____________________________________________


//************************************************************************
//****************** Filter list navigation functions ********************


//! This function affiliates an extension filter at current navigator
//!
//! @param     sz_filterext   extension filter (ASCII format, e.g.: "txt" or "txt,d*,wk" )
//!
void  nav_filterlist_setfilter( const FS_STRING sz_filterext  )
{
   fs_g_nav.sz_filterext = sz_filterext;
   nav_filterlist_reset();
}


//! This function resets the pointer of selection, so "no file selected" in filtered file list
//!
//! @return  FALSE in case of an error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
Bool  nav_filterlist_reset( void )
{
   fs_g_nav.u16_pos_filterlist = FS_NO_SEL;
   return nav_filelist_reset();
}


//! This function goes to the next position in the filtered file list
//!
//! @return    FALSE in case of error or no next file, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_filterlist_next( void )
{
   U16 u16_current_pos;
   u16_current_pos = nav_filelist_get();
   while( nav_filelist_set( 0, FS_FIND_NEXT ) )
   {
      if( nav_file_isdir()
      ||  nav_file_checkext( fs_g_nav.sz_filterext ) )
      {
         if( FS_NO_SEL == u16_current_pos )
         {
            fs_g_nav.u16_pos_filterlist = 0;
         }else{
            fs_g_nav.u16_pos_filterlist++;
         }
         return TRUE;
      }
   }
   nav_filelist_goto( u16_current_pos );
   return FALSE;
}


//! This function goes to the previous position in filtered file list
//!
//! @return    FALSE in case of an error or no next file, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_filterlist_previous( void )
{
   U16 u16_current_pos;
   u16_current_pos = nav_filelist_get();
   while( nav_filelist_set( 0, FS_FIND_PREV ) )
   {
      if( nav_file_isdir()
      ||  nav_file_checkext( fs_g_nav.sz_filterext ) )
      {
         fs_g_nav.u16_pos_filterlist--;
         return TRUE;
      }
   }
   nav_filelist_goto( u16_current_pos );
   return FALSE;
}


//! This function returns the position of selected file in filtered file list
//!
//! @return    position of selected file in directory (0 is the first position)
//! @return    FS_NO_SEL, in case of no file selected
//!
U16   nav_filterlist_get( void )
{
   return fs_g_nav.u16_pos_filterlist;
}


//! This function goes to a position in filtered file list
//!
//! @param     u16_newpos     new position to select (0 is the first position)
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_filterlist_goto( U16 u16_newpos )
{
   if (u16_newpos == FS_NO_SEL)
     return nav_filterlist_reset();
  
   if( u16_newpos < (fs_g_nav.u16_pos_filterlist/2) )
   {
      // Restart at the beginning of list to accelerate search
      if( !nav_filterlist_reset() )
         return FALSE;
   }
   if( FS_NO_SEL == fs_g_nav.u16_pos_filterlist )
      if( !nav_filterlist_next() )
         return FALSE;  // No file available
   while( u16_newpos > fs_g_nav.u16_pos_filterlist )
   {
      if( !nav_filterlist_next() )
         break;
   }
   while( u16_newpos < fs_g_nav.u16_pos_filterlist )
   {
      if( !nav_filterlist_previous() )
         break;
   }
   return (u16_newpos == fs_g_nav.u16_pos_filterlist);
}


//! This function searches a file name in file list filtered
//!
//! @param     sz_name        name to search (UNICODE or ASCII) <br>
//!                           The name must end with NULL or '*' value
//! @param     b_match_case   FALSE to ignore the case
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! This function starts a search at the next position of the current in filtered file list
//! @endverbatim
//!
Bool  nav_filterlist_findname( const FS_STRING sz_name , Bool b_match_case )
{
   while( 1 )
   {
      if ( !nav_filterlist_next())
         return FALSE;
      if ( nav_file_name( sz_name , 0 , FS_NAME_CHECK , b_match_case ))
         return TRUE;
   }
}


//! This function computes the number of files and/or directories in filtered file list
//! This function is blocking. It will only return when the final count is computed,
//! which may take some time.
//!
//! @param      fl_type       FL_ALL  To check all types (files and directories). <br>
//!                           FL_DIR  To check the directory presence. <br>
//!                           FL_FILE To check the file presence.
//!
//! @param      sz_filterext  Extension filter (ASCII format, e.g.: "txt" or "txt,d*,wk" ). <br>
//!                           If this argument in NULL, then it uses the defautl filter used by the filterlist. <br>
//!                           This argument is also ignored for b_type == FS_DIR.
//!
//! @return     number of files and/or directories present in filtered file list.
//!
U16   nav_filterlist_nb( fl_type_t fl_type, const FS_STRING sz_filterext )
{
   U16 total=(U16)-1; // Initialization is mandatory
   while( !nav_filterlist_nb_ex( fl_type, sz_filterext, &total, 0 ) );
   return total;
}

//! Update and test the counter
//! @param  counter the counter
//! @return FALSE if the counter reaches the end
//!         TRUE otherwise
static Bool update_counter(U8 *counter)
{
  if (*counter)
    (*counter)--;
  if (!*counter)
    return FALSE;
  return TRUE;
}

//! This function computes the number of files and/or directories in filtered file list
//! This function is not blocking. It will return after \<retry\> iteration if the result
//! is not yet computed.
//!
//! @param      fl_type       FL_ALL  To check all types (files and directories). <br>
//!                           FL_DIR  To check the directory presence. <br>
//!                           FL_FILE To check the file presence.
//!
//! @param      sz_filterext  Extension filter (ASCII format, e.g.: "txt" or "txt,d*,wk" ). <br>
//!                           If this argument in NULL, then it uses the defautl filter used by the filterlist. <br>
//!                           This argument is also ignored for b_type == FS_DIR.
//! @param      p_total       Pointer to the result which will be updated by the function (added).
//!                           Initialization shall be done before untering into this  function.
//! @param      retry         Number of files seen before leaving the function. 0 means that the
//!                           function is blocking untill the result is known.
//!
//! @return     number of files and/or directories present in filtered file list.
//!
Bool   nav_filterlist_nb_ex( fl_type_t fl_type, const FS_STRING sz_filterext, U16* p_total, U8 retry )
{
   static U16   u16_save_position;
   Bool   b_is_dir;

   // save current position
   if( *p_total==(U16) -1 )
   {
      u16_save_position = fs_g_nav.u16_pos_filterlist;

      // Reset position
      if ( !nav_filterlist_reset())
         return TRUE;

      if (fl_type == FL_DIR)
        nav_filelist_single_enable(FS_DIR);
      else if (fl_type == FL_FILE)
        nav_filelist_single_enable(FS_FILE);

      *p_total = 0;
   }

   // Scan all
   while( nav_filelist_set( 0, FS_FIND_NEXT ) )
   {
      b_is_dir = nav_file_isdir();

      // Check if its a file for FL_FILE mode
      if (fl_type == FL_FILE && b_is_dir)
      {
        if (!update_counter(&retry))
          return FALSE;
        continue;
      }
      // Check if its a directory for FL_DIR mode
      if (fl_type == FL_DIR && !b_is_dir)
      {
        if (!update_counter(&retry))
          return FALSE;
        continue;
      }
      // If the selection is on a file
      if (!b_is_dir)
      {
        // Check the extension of the file
        if (!((sz_filterext)?nav_file_checkext(sz_filterext):nav_file_checkext(fs_g_nav.sz_filterext)))
          if (!update_counter(&retry))
            return FALSE;
      }

      (*p_total)++;
      if (!update_counter(&retry))
         return FALSE;
   }

   nav_filelist_single_disable();

   // Restore previous position
   if ( u16_save_position != FS_NO_SEL )
   {
      nav_filterlist_reset();
   }else{
      // After operation, there are a file selected
      nav_filterlist_goto( u16_save_position );
   }
   return TRUE;
}


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
Bool  nav_filterlist_mount( void )
{
   fs_g_nav.u16_pos_filterlist = FS_NO_SEL;
   return nav_partition_mount();
}


//! This function initializes the file filtered list on the root directory
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
Bool  nav_filterlist_root( void )
{
   fs_g_nav.u16_pos_filterlist = FS_NO_SEL;
   return nav_dir_root();
}


//! This function enters in the selected directory
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! After this routine the file list changes and contains the files and directories of the new directory.
//! By default no file is selected.
//! @endverbatim
//!
Bool  nav_filterlist_cd( void )
{
   fs_g_nav.u16_pos_filterlist = FS_NO_SEL;
   return nav_dir_cd();
}


//! This function goes to at the parent directory
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! After, the filtered file list changes and contains the files and directories of the new directory.
//! By default, the file selected in filtered file list is the previous (children) directory.
//! @endverbatim
//!
Bool  nav_filterlist_gotoparent( void )
{
   U32 u32_cluster_old_dir;

   if (!fat_check_mount_noopen())
      return FALSE;

   if (0 == fs_g_nav.u32_cluster_sel_dir)
   {
      fs_g_status = FS_ERR_IS_ROOT;        // There aren't parent
      return FALSE;
   }

   // Select and read information about directory ".."
   fs_g_nav_fast.u16_entry_pos_sel_file = 1;
   if ( !fat_read_dir())
      return FALSE;
   fat_get_entry_info();
   // Save the children directory cluster
   u32_cluster_old_dir = fs_g_nav.u32_cluster_sel_dir;

   // Select the parent directory via information present in the current directory ".."
   fs_g_nav.u32_cluster_sel_dir = fs_g_nav_entry.u32_cluster;

   // Select the children directory in new directory (=parent directory)
   if( FALSE == nav_filterlist_reset())
      return FALSE;
   while( nav_filterlist_next() )
   {
      if (fs_g_nav_entry.u32_cluster == u32_cluster_old_dir)
         return TRUE;   // It is the children directory
   }
   fs_g_status = FS_ERR_FS;
   return FALSE;
}


//! This function selects a file in the navigator via a file index
//!
//! @param     index       structure with information about file to select (disk, partition, dir, file/dir selected )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! This routine allow to reinit a navigator quickly via a file index (disk, partition, dir, file/dir selected )
//! To get a file index, you shall used the routine nav_getindex().
//! @endverbatim
//!
Bool  nav_filterlist_gotoindex( const Fs_index _MEM_TYPE_SLOW_ *index )
{
   if( !nav_drive_set( index->u8_lun ))
      return FALSE;
#if (FS_MULTI_PARTITION  ==  ENABLED)
   if( !nav_partition_set(index->u8_partition))
      return FALSE;
#endif
   if( !nav_partition_mount())
      return FALSE;

    // Initialization of the current entry file with index information
   fs_g_nav.u32_cluster_sel_dir   = index->u32_cluster_sel_dir;

   // Reset position
   if ( !nav_filterlist_reset() )
      return FALSE;

   // Research the index in directory
   while( fs_g_nav_fast.u16_entry_pos_sel_file != index->u16_entry_pos_sel_file )
   {
      if( !nav_filterlist_next() )
      {
         nav_filterlist_reset();
         return FALSE;
      }
   }
   return TRUE;
}
