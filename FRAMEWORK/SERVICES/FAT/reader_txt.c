/* This source file is part of the ATMEL AVR-UC3-SoftwareFramework-1.6.1 Release */

/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief FAT 12/16/32 Text reader service.
 *
 * This file defines a useful set of functions for the text file accesses on
 * AVR32 devices.
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
#include LIB_MEM
#include "file.h"
#include "navigation.h"
#include "reader_txt.h"
#include "unicode.h"


//_____ D E F I N I T I O N S ______________________________________________


//! Open a selected text file in the current navigator
//!
//! @param     b_readonly     TRUE to open the text file in read only access
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  reader_txt_open( Bool b_readonly )
{
   if( !file_open( (b_readonly)? FOPEN_MODE_R : FOPEN_MODE_R_PLUS ) )
      return FALSE; // Open error
   reader_txt_beg();
   return TRUE;
}


//! Create a new text file
//!
//! @param     sz_name        contains the file name (ASCII or UNICODE )
//! @param     u8_txt_format  text format to use (UNI_TYPE_UTF8, UNI_TYPE_UTF16BE, UNI_TYPE_UTF16LE, UNI_TYPE_ASCII)
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  reader_txt_new( const FS_STRING sz_name , U8 u8_txt_format )
{
   // Table to store the header of text file
   U8    string_header[UNI_MAX_HEADER_SIZE];
   U8    u8_header_size;

   // Create empty file
   if ( !nav_file_create( sz_name ))
      return FALSE;

   fs_g_nav_entry.u8_txt_format = u8_txt_format;

   // Write at the beginning of file the text header
   u8_header_size = unicode_header_get( string_header, fs_g_nav_entry.u8_txt_format );
   file_write_buf( string_header , u8_header_size );
   return TRUE;
}


//! This function goes at the beginning of file and read the text header
//!
void  reader_txt_beg( void )
{
   // Table to store the header of text file
   U8    string_header[UNI_MAX_HEADER_SIZE];

   file_seek( 0, FS_SEEK_SET );  // go to at the beginning of file

   // Read the header to detect the text format (ASCII, UNICODE LE, UNICODE BE, UTF8)
   memset( string_header, 0, UNI_MAX_HEADER_SIZE );     // By default ASCII
   file_read_buf( string_header , UNI_MAX_HEADER_SIZE );
   fs_g_nav_entry.u8_txt_format = unicode_header_scan( string_header );
   // Set cursor after header
   file_seek( unicode_header_get( string_header, fs_g_nav_entry.u8_txt_format ), FS_SEEK_SET );
}


//! This function allow to select the text format to decode
//!
//! @param     u8_txt_format  text format selected (UNI_TYPE_UTF8, UNI_TYPE_UTF16BE, UNI_TYPE_UTF16LE, UNI_TYPE_ASCII)
//!
void  reader_txt_select_format( U8 u8_txt_format )
{
   fs_g_nav_entry.u8_txt_format = u8_txt_format;
}


//! This function fills an ASCII buffer with the current text line of opened file
//!
//! @param     b_unicode      TRUE to return a unicode string (UTF16), ASCII otherwise over there
//! @param     string         string used to store text line
//! @param     u16_str_size   maximum size of string (unit character, 1B for ASCII, 2B for unicode)
//!
//! @return    size of the line (may be < or > at u16_str_size)
//! @return    0, in case of error
//!
//! @verbatim
//! This routine remove the character '\n' '\r'
//! @endverbatim
//!
U16   reader_txt_get_line( Bool b_unicode, FS_STRING string , U16 u16_str_size )
{
   U8    utf8[UNI_MAX_UTF8_SIZE];
   U8    size_utf8_buf=0;
   U8    size_utf8_dec=0;
   U16   u16_size_line = 1;   // 1 to compute null terminator
   U16   u16_unicode;
   Bool  b_error = FALSE;

   nav_checkdisk_disable();   // To optimize speed
   
   while( 0 == file_eof() && !b_error )
   {
      // Get a unicode value
      switch( fs_g_nav_entry.u8_txt_format )
      {
         case UNI_TYPE_UTF8:
         // Remove UTF8 codes used
         if( 0 != size_utf8_dec )
         {
            U8 u8_i;
            for( u8_i=0; u8_i<(UNI_MAX_UTF8_SIZE-size_utf8_dec); u8_i++ )
            {
               utf8[u8_i]=utf8[u8_i+size_utf8_dec];
            }
            size_utf8_buf -= size_utf8_dec;
            size_utf8_dec =0;
         }
         // Complet UTF8 array
         size_utf8_buf += file_read_buf( &utf8[size_utf8_buf], (UNI_MAX_UTF8_SIZE-size_utf8_buf) );
         // Decode UTF8 to unicode
         size_utf8_dec = utf8_to_unicode( &utf8[0], &u16_unicode );
         break;
      
         case UNI_TYPE_UTF16BE:
         MSB(u16_unicode) = file_getc();
         LSB(u16_unicode) = file_getc();
         if (LSB(u16_unicode) == (FS_EOF & 0xFF) && fs_g_status != FS_ERR_EOF)
           b_error = TRUE;
         break;

         case UNI_TYPE_UTF16LE:
         LSB(u16_unicode) = file_getc();
         MSB(u16_unicode) = file_getc();
         if (MSB(u16_unicode) == (FS_EOF & 0xFF) && fs_g_status != FS_ERR_EOF)
           b_error = TRUE;
         break;

         default: // ASCII or other
         u16_unicode = file_getc();
         if (u16_unicode == FS_EOF && fs_g_status != FS_ERR_EOF)
           b_error = TRUE;
         break;
      }

      // Check character to remove
      if( '\r' == u16_unicode )
         continue;   // Ignore character
      
      // Check end of line
      if((  0  == u16_unicode )
      || ('\n' == u16_unicode ) )
         break;      // End of line

      u16_size_line++;
      // Fill string
      if( 1 < u16_str_size )
      {
         if( b_unicode )
         {
            ((FS_STR_UNICODE)string)[0] = u16_unicode;
         }else{
            string[0] = u16_unicode;
         }
         string += (b_unicode? 2 : 1 );
         u16_str_size--;
      }
   }
   // HERE, the line is readed
   
   if( UNI_TYPE_UTF8 == fs_g_nav_entry.u8_txt_format )
   {
      // Re position cursor on next UTF8
      file_seek( (size_utf8_buf-size_utf8_dec) , FS_SEEK_CUR_RE );
   }

   // Handle error cases
   if (b_error)
   {
     nav_checkdisk_enable();
     return 0;
   }

   // Add Null terminate
   if( 0 != u16_str_size )
   {
      if( b_unicode )
      {
         ((FS_STR_UNICODE)string)[0] = 0;
      }else{
         string[0] = 0;
      }
   }
   nav_checkdisk_enable();
   return u16_size_line;
}


//! This function jumps text lines
//!
//! @param     nb_line        number of lines to jump
//!
//! @return    FALSE in case of error
//!
Bool  reader_txt_jump_line( U16 nb_line )
{
   while( 0 != nb_line )
   {
      if( 0 != file_eof() )
         return FALSE;
      reader_txt_get_line( FALSE, NULL, 0 );
      nb_line--;
   }
   return TRUE;
}


//! Close text file
//!
void  reader_txt_close( void )
{
   file_close();
}

