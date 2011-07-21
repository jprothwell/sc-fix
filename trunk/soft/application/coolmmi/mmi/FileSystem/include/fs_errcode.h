/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *	fs_errcode.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *    Include this file for using the error codes of file system only
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _FS_ERRNO_H
#define _FS_ERRNO_H
//User:     It means the task owner should fix this error at development stage
//Normal:   It means this maybe a normal case at run time. Task owner should handle such case.
//System:   It is the file system internal error code.
//Fatal:    It means this is file system fatal error. 
//          To let target keep working, we don't issue a fatal error.
#define FS_NO_ERROR					    0
#define FS_ERROR_RESERVED			   -1
#define FS_PARAM_ERROR				   -2    /* User */
#define FS_INVALID_FILENAME		       -3    /* User */
#define FS_DRIVE_NOT_FOUND 		       -4    /* User or Fatal */
#define FS_TOO_MANY_FILES			   -5     
#define FS_NO_MORE_FILES         	   -6    /* Normal */
#define FS_WRONG_MEDIA          	   -7    /* Fatal */
#define FS_INVALID_FILE_SYSTEM   	   -8    /* Fatal */
#define FS_FILE_NOT_FOUND        	   -9    /* User or Normal */
#define FS_INVALID_FILE_HANDLE   	   -10   /* User or Normal */
#define FS_UNSUPPORTED_DEVICE   	   -11   /* User */
#define FS_UNSUPPORTED_DRIVER_FUNCTION -12   /* User or Fatal */
#define FS_CORRUPTED_PARTITION_TABLE   -13   /* fatal */
#define FS_TOO_MANY_DRIVES      	   -14   /* not use so far */
#define FS_INVALID_FILE_POS      	   -15   /* User */
#define FS_ACCESS_DENIED         	   -16   /* User or Normal */
#define FS_STRING_BUFFER_TOO_SMALL     -17   /* User */
#define FS_GENERAL_FAILURE       	   -18   /* Normal */
#define FS_PATH_NOT_FOUND       	   -19   /* User */
#define FS_FAT_ALLOC_ERROR     		   -20   /* Fatal: disk crash */
#define FS_ROOT_DIR_FULL         	   -21   /* Normal */
#define FS_DISK_FULL             	   -22   /* Normal */
#define FS_TIMEOUT               	   -23   /* Normal: FS_CloseMSDC with nonblock */
#define FS_BAD_SECTOR           	   -24   /* Normal: NAND flash bad block */
#define FS_DATA_ERROR            	   -25   /* Normal: NAND flash bad block */
#define FS_MEDIA_CHANGED        	   -26   /* Normal */
#define FS_SECTOR_NOT_FOUND    	       -27   /* Fatal */
#define FS_ADDRESS_MARK_NOT_FOUND      -28   /* not use so far */
#define FS_DRIVE_NOT_READY       	   -29   /* Normal */
#define FS_WRITE_PROTECTION      	   -30   /* Normal: only for MSDC */
#define FS_DMA_OVERRUN           	   -31   /* not use so far */
#define FS_CRC_ERROR				   -32   /* not use so far */
#define FS_DEVICE_RESOURCE_ERROR       -33   /* Fatal: Device crash */
#define FS_INVALID_SECTOR_SIZE         -34   /* Fatal */
#define FS_OUT_OF_BUFFERS      	       -35   /* Fatal */
#define FS_FILE_EXISTS           	   -36   /* User or Normal */
#define FS_LONG_FILE_POS        	   -37   /* User : FS_Seek new pos over sizeof int */
#define FS_FILE_TOO_LARGE       	   -38   /* User: filesize + pos over sizeof int */
#define FS_BAD_DIR_ENTRY        	   -39   /* Fatal */
#define FS_ATTR_CONFLICT               -40   /* User: Can't specify FS_PROTECTION_MODE and FS_NONBLOCK_MOD */
#define FS_CHECKDISK_RETRY             -41   /* System: don't care */
#define FS_LACK_OF_PROTECTION_SPACE    -42   /* Fatal: Device crash */
#define FS_SYSTEM_CRASH                -43   /* Normal */
#define FS_FAIL_GET_MEM                -44   /* Normal */
#define FS_READ_ONLY_ERROR             -45   /* User or Normal */
#define FS_DEVICE_BUSY                 -46   /* Normal */
#define FS_ABORTED_ERROR               -47   /* Normal */
#define FS_FLASH_MOUNT_ERROR           -48   /* Normal */
#define FS_PATH_OVER_LEN_ERROR         -49   /* Normal */
#define FS_APP_QUOTA_FULL              -50   /* Normal */
#define FS_VF_MAP_ERROR                -51   /* User or Normal */
#define FS_DEVICE_EXPORTED_ERROR       -52   /* User or Normal */

#define FS_MSDC_MOUNT_ERROR			   -100  /* Normal */
#define FS_MSDC_READ_SECTOR_ERROR	   -101  /* Normal */
#define FS_MSDC_WRITE_SECTOR_ERROR	   -102  /* Normal */
#define FS_MSDC_DISCARD_SECTOR_ERROR   -103  /* Normal */
#define FS_MSDC_PRESNET_NOT_READY      -104  /* System */
#define FS_MSDC_NOT_PRESENT            -105  /* Normal */

#define FS_FLASH_ERASE_BUSY			   -120   /* Normal: only for nonblock mode */
#define FS_LOCK_MUTEX_FAIL			   -121   /* System: don't care */
#define FS_NO_NONBLOCKMODE             -122  /* User: try to call nonblock mode other than NOR flash */
#define FS_NO_PROTECTIONMODE           -123  /* User: try to call protection mode other than NOR flash */
#define FS_FILE_OPENED                 -124
#define FS_REN_SAME_NAME           -125
#define FS_FILENAME_TOO_LONG     -126
 
#define FS_FOLDER_HANDLE     	       0xFF

#endif //_FS_ERRNO_H


