#ifdef DSM_SHELL_COMMAND

#ifdef _MS_VC_VER_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#else
#include <cswtype.h>
#include <errorcode.h>
#include <sul.h>
#include <ts.h>
#include <ds.h>/*for DS_ASSERT*/
#include <tm.h>
#include <rfm.h>
#include <base_prv.h>
#include <hal.h>
#endif

//#include <errorcode.h>

#include "fat_base.h"
#include "fs.h"
#include "fs_base.h"
#include "fat_local.h"

#define FS_SHELL_BUFFER_LEN_MAX 1024
#define FFS_SLEEP_VALUE   10
#define LEN_FOR_OEM_UNI_BUFFER      (FS_FILE_NAME_UNICODE_LEN + 2)

char g_lwrite_buf[ VDS_SHELL_WRITE_BUFFER_LEN_MAX ];
UINT8 g_Oem_Uni_Buf[LEN_FOR_OEM_UNI_BUFFER];
UINT8 g_Oem_Uni_Buf2[LEN_FOR_OEM_UNI_BUFFER];

UINT16 g_ULen;

extern char g_shell_buffer[ ];
extern char g_lwrite_buf[];

INT32 g_fs_has_initiated = FALSE;

#ifdef _MS_VC_VER_
extern INT32 g_dsm_errno;
#endif


//init [YES| NO] [root_dev_name] {[normal | quick]}
//YES means force format option is selected;
//NO  means force format option is not selected.
static BOOL shell_fs_init( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;
    INT32 iFormatType = FS_FORMAT_QUICK;
    
    if ( g_fs_has_initiated )
    {
        dsm_Printf( BASE_VDS_TS_ID, "Init command is not allowed to call from shell\n" );
        return FALSE;
    }

    if ( argc < 3 )
    {
        goto show_init_cmd;
    }

    if ( 4 == argc && ( 'n' == argv[ 3 ][ 0 ] || 'N' == argv[ 3 ][ 0 ] ) )
    {
        iFormatType = FS_FORMAT_NORMAL;
    }

    if ( 0 == DSM_StrCaselessCmp( "yes", argv[ 1 ] ) )
    {
        iRet = FS_Format( (UINT8*)argv[ 2 ], FS_TYPE_FAT, (UINT8)iFormatType );
        if(ERR_SUCCESS != iRet)
        {
            dsm_Printf( BASE_DSM_TS_ID, "format [%s] failed: %d\n", argv[ 2 ], iRet ); 
            return FALSE;
        }
        else
        {
            dsm_Printf( BASE_DSM_TS_ID, "Format %s OK\n", argv[ 2 ] ); 
        }
    }
    else
    {
        iRet = FS_HasFormatted( (UINT8*)argv[ 2 ], FS_TYPE_FAT );
        if ( ERR_FS_HAS_FORMATED != iRet )
        {
            iRet = FS_Format( (UINT8*)argv[ 2 ], FS_TYPE_FAT, (UINT8)iFormatType );
            if(ERR_SUCCESS != iRet)
            {
                dsm_Printf( BASE_DSM_TS_ID, "format [%s] failed: %d\n", argv[ 2 ], iRet ); 
                return FALSE;
            }
            else
            {
                dsm_Printf( BASE_DSM_TS_ID, "Format %s OK\n", argv[ 2 ] ); 
            }
        }
        else
        {
            dsm_Printf( BASE_DSM_TS_ID, "%s has formated before\n", argv[ 2 ] ); 
        }
    }  
    
    iRet = FS_MountRoot( (UINT8*)argv[ 2 ] );
    if(ERR_SUCCESS != iRet)
    {
        dsm_Printf( BASE_DSM_TS_ID, "Mount Root FS on %s failed: %d", argv[ 2 ], iRet ); 
        return FALSE;
    }
    else
    {
        dsm_Printf( BASE_DSM_TS_ID, "Mount Root FS on %s OK\n", argv[ 2 ] ); 
    }
        
    g_fs_has_initiated = TRUE;
    return TRUE;

show_init_cmd:
    PRINT_CMD_USAGE( ent );
    return FALSE;
}

static BOOL shell_FS_Format( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 iRet;
  UINT8 iFsType;
  UINT8 iFormatType;
  
  if ( 4 == argc )
  {
    iFsType = (UINT8)DSM_StrAToI(argv[2]);
    iFormatType = (UINT8)DSM_StrAToI(argv[3]);
    iRet = FS_Format( (UINT8*)argv[ 1 ],iFsType,iFormatType );
    if ( ERR_SUCCESS == iRet )
    {
      dsm_Printf( BASE_FAT_TS_ID, "format succeed!\n");
      return TRUE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "format failed! ErrorCode: [%d]\n",iRet);
      return FALSE;
    }
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}



/*mount [FAT|FFS|MinixFS] <dev_name> <mount_point>*/
static BOOL shell_mount( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;
    
    if ( 4 <= argc )
    {
        DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
        fat_OEM2Unicode((UINT8*)argv[ 3 ], (UINT16)DSM_StrLen((UINT8*)argv[ 3 ]), g_Oem_Uni_Buf, &g_ULen);
        iRet = FS_Mount( (UINT8*)argv[ 2 ], g_Oem_Uni_Buf, 0, FS_TYPE_FAT );
        if ( ERR_SUCCESS != iRet )
            dsm_Printf( BASE_FAT_TS_ID, "mount failed: %d!\n", iRet );
    }
    else
    {
        PRINT_CMD_USAGE( ent );
    }

    return TRUE;
}


/*mountr <dev_name>*/
static BOOL shell_mount_root( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;
    
    if ( 2 == argc )
    {
        iRet = FS_MountRoot( (UINT8*)argv[ 1 ] );
        if ( ERR_SUCCESS != iRet )
            dsm_Printf( BASE_FAT_TS_ID, "mount root fs on %s failed: %d!\n", argv[ 1 ], iRet );
    }
    else
    {
        PRINT_CMD_USAGE( ent );
    }

    return TRUE;
}


/*unmount <mount_point>*/
static BOOL shell_unmount( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;
    BOOL bForce = FALSE;

    if ( argc < 2 )
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }

    if ( argc >= 3 )
    {
        bForce = DSM_StrAToI( argv[ 2 ] );
    }

    DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
    fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
    iRet = FS_Unmount( g_Oem_Uni_Buf, bForce );
    if ( ERR_SUCCESS != iRet )
    {
        dsm_Printf( BASE_FAT_TS_ID, "Unmount failed: [%d]!\n", iRet );
    }
    return TRUE;
}

static BOOL shell_FS_Find( struct cmd_entry *ent, int argc, char **argv )
{
#define FS_SHELL_DIR_FILENAME_LEN 256
    FS_FIND_DATA FindFileData;
    INT32 fd = -1;
    long errCode;
    char filename[ FS_SHELL_DIR_FILENAME_LEN ] = { 0, };
    UINT8 filenameForUni[ LEN_FOR_OEM_UNI_BUFFER] = { 0, };
    long filename_len;

    if( 0 == DSM_StrCaselessCmp( "find", argv[0] ) && argc < 2)
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }

    if ( 0 == DSM_StrCaselessCmp( "ls", argv[0] ) || 0 == DSM_StrCaselessCmp( "dir", argv[0] ) )
    {
        if ( 1 == argc )
        {
            strcpy( filename,  "./*" );
        }
        else
        {
            filename_len = DSM_StrLen( argv[ 1 ] );
            if ( filename_len > FS_SHELL_DIR_FILENAME_LEN - 2 ) // 2 is for the length of "/*" 
            {
                dsm_Printf( BASE_FAT_TS_ID, "Invalid pathname: too long\n" );
                return FALSE;
            }

            strcpy( filename, argv[1] );
            DSM_StrCat( filename, "/*" );
        }
        DSM_MemZero(filenameForUni, LEN_FOR_OEM_UNI_BUFFER);
        fat_OEM2Unicode((UINT8*)filename, (UINT16)DSM_StrLen(filename), filenameForUni, &g_ULen);
    }

    if (0 == DSM_StrCaselessCmp( "find", argv[0] ))
    {
        DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
        fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
    }
    
    memset( FindFileData.st_name, NULL_CHAR, sizeof( FindFileData.st_name ) );
    
    fd = FS_FindFirstFile( (0 == DSM_StrCaselessCmp( "find", argv[0] )) ? g_Oem_Uni_Buf : filenameForUni, &FindFileData );//使用FindFirstFile函数来开始文件查找
    if ( fd < 0 ) 
    {
        if ( ERR_FS_NO_MORE_FILES == fd ) 
        {
            dsm_Printf( BASE_FAT_TS_ID, "No Matching entry is found\n" );
            return TRUE;
        }
        else
        {
            dsm_Printf( BASE_FAT_TS_ID, "Find failed: [%d]\n", fd );
            return FALSE;
        }
    }
    if ( !( FS_INODE_IS_DIR( FindFileData.st_mode ) && 
         ( 0 == DSM_StrCmp( ".", FindFileData.st_name ) || 0 == DSM_StrCmp( "..", FindFileData.st_name ))))
    {
        DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
        fat_Unicode2OEM(FindFileData.st_name, fat_UnicodeLen(FindFileData.st_name), g_Oem_Uni_Buf, &g_ULen);
        dsm_Printf( BASE_FAT_TS_ID, "\t%s%s\n", g_Oem_Uni_Buf, FS_INODE_IS_DIR( FindFileData.st_mode ) ? "/" : "" );
    }

    while ( ERR_SUCCESS == ( errCode = FS_FindNextFile( fd, &FindFileData ) ) ) 
    {
        if ( !( FS_INODE_IS_DIR( FindFileData.st_mode ) && 
             ( 0 == DSM_StrCmp( ".", FindFileData.st_name ) || 0 == DSM_StrCmp( "..", FindFileData.st_name ))))
        {
            DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
            fat_Unicode2OEM(FindFileData.st_name, fat_UnicodeLen(FindFileData.st_name), g_Oem_Uni_Buf, &g_ULen);
            dsm_Printf( BASE_FAT_TS_ID, "\t%s%s\n", g_Oem_Uni_Buf, FS_INODE_IS_DIR( FindFileData.st_mode ) ? "/" : "" );
        }
    }

    if ( ERR_FS_NO_MORE_FILES != errCode ) 
    {
        dsm_Printf( BASE_FAT_TS_ID, "ERROR[%d] in FindNextFile\n", errCode );
        FS_FindClose( fd );
        return FALSE;
    }

    dsm_Printf( BASE_FAT_TS_ID, "\n" );
    FS_FindClose( fd );
    return TRUE;
}



static BOOL shell_FS_SearchFirst( struct cmd_entry *ent, int argc, char **argv )
{
    FS_FIND_DATA FindFileData;
    INT32 fd = -1;

    if( argc < 2 )
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }

    memset( FindFileData.st_name, NULL_CHAR, sizeof( FindFileData.st_name ) );
    DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
    fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
    fd = FS_FindFirstFile( g_Oem_Uni_Buf , &FindFileData );//使用FindFirstFile函数来开始文件查找

    if ( fd >= 0) 
    {
        dsm_Printf( BASE_FAT_TS_ID, "search descriptor[%d]\n", fd );
        DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
        fat_Unicode2OEM(FindFileData.st_name, fat_UnicodeLen(FindFileData.st_name), g_Oem_Uni_Buf, &g_ULen);
        dsm_Printf( BASE_FAT_TS_ID, "\t%s%s\n", g_Oem_Uni_Buf, FS_INODE_IS_DIR( FindFileData.st_mode ) ? "/" : "" );
    }
    else
        dsm_Printf( BASE_FAT_TS_ID, "search file failed[%d]\n", fd );

    return TRUE;
}


static BOOL shell_FS_SearchNext( struct cmd_entry *ent, int argc, char **argv )
{
    FS_FIND_DATA FindFileData;
    INT32 fd = -1;
    long errCode;

    if( argc < 2)
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }

    fd = DSM_StrAToI( argv[1] );
    
    while ( ERR_SUCCESS == ( errCode = FS_FindNextFile( fd, &FindFileData ) ) ) 
    {
        DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
        fat_Unicode2OEM(FindFileData.st_name, fat_UnicodeLen(FindFileData.st_name), g_Oem_Uni_Buf, &g_ULen);
        dsm_Printf( BASE_FAT_TS_ID, "\t%s%s\n", g_Oem_Uni_Buf, FS_INODE_IS_DIR( FindFileData.st_mode ) ? "/" : "" );
    }

    if ( ERR_FS_NO_MORE_FILES != errCode ) 
    {
        dsm_Printf( BASE_FAT_TS_ID, "ERROR[%d] in FindNextFile\n", errCode );
        return FALSE;
    }

    dsm_Printf( BASE_FAT_TS_ID, "No more file to search\n" );
    return TRUE;
}


static BOOL shell_FS_SearchClose( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;
    
    if( argc < 2)
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }
    
    iRet = FS_FindClose( DSM_StrAToI( argv[1] ) );
    if ( ERR_SUCCESS != iRet )
    {
            dsm_Printf( BASE_FAT_TS_ID, "Search close failed[%d]\n", iRet );
    }
    return TRUE;
}

extern struct task_struct g_fs_current;
extern char g_dsm_Prompt[ 255 + 1 ];
INT32 FS_ChangeDir( PCSTR pszDirName );
static BOOL shell_FS_ChangeDir( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 iRet;
  
  if ( argc == 2 )
  {
    DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
    fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
    iRet = FS_ChangeDir( g_Oem_Uni_Buf );
    if ( ERR_SUCCESS != iRet )
    {
      dsm_Printf( BASE_FAT_TS_ID, "Change directory failed!\n" );
    }
    else
    {
        iRet = FS_GetCWD(g_dsm_Prompt, (UINT8*)argv[ 1 ]);
        if ( ERR_SUCCESS != iRet )
        {
          dsm_Printf( BASE_FAT_TS_ID, "Get current work directory failed!\n" );
          return FALSE;
        }
    
        DSM_StrLwr(g_dsm_Prompt);
    }
    return ( ERR_SUCCESS == iRet ) ? TRUE : FALSE;
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}

static BOOL shell_FS_PWD( struct cmd_entry *ent, int argc, char **argv )
{
  //INT32 iRet;
  //struct inode *pwd;
  //UINT8 path[256];
  //UINT8 *pos = NULL;

  argv = argv;

  if ( argc == 1 )
  {

      dsm_Printf( BASE_FAT_TS_ID, "%s\n", g_dsm_Prompt );

#if 0
    pwd = g_fs_current.pwd;
    memset(path, 0x00, 256);
    
    iRet = FS_GetCWD(pwd, path, 256, &pos);
    if ( ERR_SUCCESS != iRet )
    {
      dsm_Printf( BASE_FAT_TS_ID, "Get current work directory failed!\n" );
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "%s\n", pos );
    }
    return ( ERR_SUCCESS == iRet ) ? TRUE : FALSE;
#endif
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
  return TRUE;
}


static BOOL shell_FS_Open( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 fd;
  int flag = 0;
  char *p;

  if ( 2 <= argc )
  {
    if ( 3 <= argc )
    {
      for ( p = argv[ 2 ]; *p; p++ )
      {
        switch ( *p )
        {
        case 'A':
          flag  |= FS_O_APPEND;
          break;
        case 'C':
          flag  |= FS_O_CREAT;
          break;
        case 'E':
          flag  |= FS_O_EXCL;
          break;
        case 'T':
          flag  |= FS_O_TRUNC;
          break;
        case 'R':
          flag  |= ( FS_O_WRONLY == ( flag & FS_O_ACCMODE ) ) ? 
             FS_O_RDWR : FS_O_RDONLY;
          break;
        case 'W':
          flag  |= ( FS_O_RDONLY == ( flag & FS_O_ACCMODE ) ) ? 
             FS_O_RDWR : FS_O_WRONLY;
          break;
        case '+':
          flag  |= FS_O_RDWR;
          break;
        default:
          break;
        }
      }
    }
    else
    {
      flag = FS_O_RDWR;
    }

    DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
    fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
    fd = FS_Open( g_Oem_Uni_Buf, flag, 0777 );
    if ( fd >= 0 )
    {
      dsm_Printf( BASE_FAT_TS_ID, "open file with fd[%d] returned\n", fd );
      return TRUE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "open file failed! ErrorCode = %d\n",fd );
      return FALSE;
    }
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}



static BOOL shell_FS_EndOfFile( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 iRet;

  if ( 2 == argc )
  {
    iRet = FS_IsEndOfFile( DSM_StrAToI( argv[ 1 ] ) );
    if ( 1 == iRet )
    {
      dsm_Printf( BASE_FAT_TS_ID, "End of file\n" );
      return TRUE;
    }
    else if ( 0 == iRet )
    {
      dsm_Printf( BASE_FAT_TS_ID, "Not end of file\n" );
      return TRUE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "error: [%d]\n", iRet );
      return FALSE;
    }
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}


static BOOL shell_FS_Create( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 fd;

  if ( 2 == argc )
  {
    DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
    fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
    fd = FS_Create( g_Oem_Uni_Buf, 0x0 );
    if ( fd >= 0 )
    {
      dsm_Printf( BASE_FAT_TS_ID, "created file with fd[%d] returned. The file has been truncated to zero.\n", fd );
      return TRUE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "create file failed!\n" );
      return FALSE;
    }
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}

static BOOL shell_FS_CreClo( struct cmd_entry *ent, int argc, char **argv )
{
  
  INT32 fd;
  INT32 iRet;
  
  if ( 2 == argc )
  {
    DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
    fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
    fd = FS_Create( g_Oem_Uni_Buf, 0x0 );
    if ( fd >= 0 )
    {
      dsm_Printf( BASE_FAT_TS_ID, "created file successfully.\n");
      iRet = FS_Close( fd );
      if ( ERR_SUCCESS != iRet )
      {
        dsm_Printf( BASE_FAT_TS_ID, "Close file failed!\n" );
        return FALSE;
      }

      return TRUE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "create file failed!\n" );
      return FALSE;
    }
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }

}

static BOOL shell_FS_DeleteFile( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 iRet;

  if ( 2 == argc )
  {
    DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
    fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
    iRet = FS_Delete( g_Oem_Uni_Buf );
    if ( ERR_SUCCESS != iRet )
      dsm_Printf( BASE_FAT_TS_ID, "Delete file failed! error[%d]\n", iRet );

    return ( ERR_SUCCESS == iRet ) ? TRUE : FALSE;
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}

  
static BOOL shell_FS_Write( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 fd;
  int len;
  
  if ( 3 <= argc )
  {
    fd = DSM_StrAToI( argv[ 1 ] );
    len = FS_Write( fd, (UINT8*)argv[ 2 ], strlen( argv[ 2 ] ) );
    if ( len > 0 )
    {
      //dsm_Printf( BASE_FAT_TS_ID, "Write [%d] bytes to file OK\n", len );
      return TRUE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "Write file failed\n" );
      return FALSE;
    }
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}

static BOOL shell_FS_Read( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 fd;
  int len;
  int i;

  if ( 3 == argc )
  {
    fd = DSM_StrAToI( argv[ 1 ] );
    len = DSM_StrAToI( argv[ 2 ] );
    if ( len > FS_SHELL_BUFFER_LEN_MAX )
      len = FS_SHELL_BUFFER_LEN_MAX;

#ifndef _MS_VC_VER_
    if( len > TS_OUTPUT_LEN_MAX )
    {
      dsm_Printf( BASE_FAT_TS_ID, "length has tuncated as %d\n", TS_OUTPUT_LEN_MAX );
      len = TS_OUTPUT_LEN_MAX;
    }
#endif
    memset(g_shell_buffer, 0x00, VDS_SHELL_BUFFER_LEN_MAX + LEN_FOR_NULL_CHAR);
    len = FS_Read( fd, (UINT8*)g_shell_buffer, len );
    if ( len > 0 )
    {
      dsm_Printf( BASE_FAT_TS_ID, "Read file[%d bytes] OK:\n", len );
      g_shell_buffer[ len ] = NULL_CHAR;

      for ( i = 0; i < len; i++ )
      {
      if ( NULL_CHAR == g_shell_buffer[ i ] )
        g_shell_buffer[ i ] = ' ';    /*对hole，则显示空格*/
      }
      dsm_Printf( BASE_FAT_TS_ID, "%s\n", g_shell_buffer );
      return TRUE;
    }
    else if ( 0 == len )
    {
      dsm_Printf( BASE_FAT_TS_ID, "end-of-file is encountered\n" );
      return FALSE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "Read file failed\n" );
      return FALSE;
    }
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}
static BOOL shell_FS_Seek( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 fd;
  INT32 offset = 0;
  UINT8 origin = FS_SEEK_SET;
  INT32 lRet;

  if ( argc >= 3 )
  {
    fd = DSM_StrAToI( argv[ 1 ] );
    offset = DSM_StrAToI( argv[ 2 ] );
    if ( 4 == argc )
    {
      if ( !DSM_StrCmp( argv[ 3 ], "beg" ) )
      origin = FS_SEEK_SET;
      else if ( !DSM_StrCmp( argv[ 3 ], "cur" ) )
      origin = FS_SEEK_CUR;
      else if ( !DSM_StrCmp( argv[ 3 ], "end" ) )
      origin = FS_SEEK_END;
      else
      goto lseek_cmd;
    }
    lRet = FS_Seek( fd, offset, origin );
    if ( lRet < 0 )
    {
      dsm_Printf( BASE_FAT_TS_ID, "Seek failed!\n" );
      return FALSE;
    }
    return TRUE;
  }
  else
  {
lseek_cmd:
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}

static BOOL shell_FS_Close( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 iRet;

  if ( 2 == argc )
  {
    iRet = FS_Close( DSM_StrAToI( argv[ 1 ] ) );
    if ( ERR_SUCCESS != iRet )
    {
      dsm_Printf( BASE_FAT_TS_ID, "Close fd(%d) failed[%d]!\n", DSM_StrAToI( argv[ 1 ] ), iRet );
      return FALSE;
    }
    return TRUE;
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}

static BOOL shell_FS_Lwrite( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 fd;
  static char c = 'a';
  char  repeated_char = NULL_CHAR;
  int nwritten = 0, tot_len;
  int original_len, len, BuffLen;

  if ( 3 <= argc )
  {
    fd = DSM_StrAToI( argv[ 1 ] );
    tot_len = DSM_StrAToI( argv[ 2 ] );
    if ( argc > 3 )
    {
      repeated_char = argv[ 3 ][ 0 ];
    }
    BuffLen = SIZEOF(g_lwrite_buf);
    while ( tot_len > 0 )
    {
      len = MIN( tot_len, BuffLen );
      if ( repeated_char )
      {
        memset( g_lwrite_buf, repeated_char, BuffLen );
      }
      else
      {
        memset( g_lwrite_buf, 'a' + ( ( c++ ) - 'a' ) % 26, BuffLen );
      }
  /*    
      #ifdef FFS_MAIL_BUG_CHECK
      checkEvent();
      #endif
  */
      #ifndef _MS_VC_VER_
      sxr_Sleep( FFS_SLEEP_VALUE );
      #endif

      original_len = len;
      if ( original_len != ( len = FS_Write( fd, (UINT8*)g_lwrite_buf, len ) ) )
      {
        if ( len > 0 )
        {
          nwritten += len;
          dsm_Printf( BASE_FAT_TS_ID, "To write %d bytes ,only %d bytes written, Write file failed\n", len, original_len );
          dsm_Printf( BASE_FAT_TS_ID, "Only write %d bytes, Write file failed\n", nwritten );
          return FALSE;
        }

        dsm_Printf( BASE_FAT_TS_ID, "Only write %d bytes, Write file failed(%d)\n", atoi( argv[ 2 ] ) - tot_len, len );
        return FALSE;
      }

      nwritten += len;
      tot_len -= len;
    }

    dsm_Printf( BASE_FAT_TS_ID, "Write [%d] bytes to file OK\n", nwritten );
    return TRUE;
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}


#ifndef FS_PRIVATE_MEM_MANAGE_WITHOUT_GROUP
void get_mega_kilo_byte( int n, int *mega, int *kilo, int *byte )
{
    *mega = *kilo = *byte = 0;

    if ( n > 1024 * 1024 )
        *mega = n / (1024 * 1024);
    n -= (*mega) * ( 1024 * 1024 );
    if ( n > 1024 )
        *kilo = n / 1024;
    n -= (*kilo) * 1024;
    *byte = n;
}
#endif

static BOOL shell_FS_bigfile( struct cmd_entry *ent, int argc, char **argv )
{
    unsigned int c = 'A';
    INT32 fd;
    int nwritten = 0;
    int chunk_size = 199;
#ifdef _MS_VC_VER_
    int is_rand = 0;
#endif
    int mega = 0, kilo = 0, byte = 0;
    long lRet;

    if ( 3 <= argc )
    {
        DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
        fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
        fd = FS_Create( g_Oem_Uni_Buf, 0x0 );
        if( fd < 0 )
        {
        dsm_Printf( BASE_FAT_TS_ID, "open/create file failed" );
        return FALSE;
        }

        if ( 0 == DSM_StrCmp( argv[ 2 ], "random" ) )
        {
#ifdef _MS_VC_VER_
        srand( (unsigned)FS_TIME( NULL ) );
        is_rand = 1;
#endif
        }
        else if ( 0 == DSM_StrCmp( argv[ 2 ], "fixed" ) )
        {
        chunk_size = MIN( ( 4 == argc ) ? atoi( argv[ 3 ] ) : chunk_size, sizeof( g_lwrite_buf ) );
        }
        else
        {
        PRINT_CMD_USAGE( ent );
        return FALSE;
        }

        memset( g_lwrite_buf, c, sizeof( g_lwrite_buf ) );

#ifdef _MS_VC_VER_
        if ( is_rand ) chunk_size = rand() % sizeof( g_lwrite_buf );
#endif
        while( 0 <= ( lRet = FS_Write( fd, (UINT8*)g_lwrite_buf, chunk_size ) ) )
        {
#ifdef FFS_MAIL_BUG_CHECK
        checkEvent();
#endif

#ifndef _MS_VC_VER_
        sxr_Sleep( FFS_SLEEP_VALUE );
#endif

        nwritten += lRet;
        memset( g_lwrite_buf, 'A' + ( ( ( ++c ) - 'A' ) % 26 ), sizeof( g_lwrite_buf ) );

#ifdef _MS_VC_VER_
        if ( is_rand ) chunk_size = rand() % sizeof( g_lwrite_buf );
#endif
        }

        FS_Close( fd );

        get_mega_kilo_byte( nwritten, &mega, &kilo, &byte );
        dsm_Printf( BASE_FAT_TS_ID, "write %d[0x%x] ( %dM %dK %dB)bytes to the file %s\n", 
           nwritten, nwritten, mega, kilo, byte, argv[ 1 ] );
        return TRUE;
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }
}


static BOOL shell_FS_MakeDir( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 iRet;

  if ( 2 <= argc )
  {
    DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
    fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
    iRet = FS_MakeDir( g_Oem_Uni_Buf, 0777 );
    if ( ERR_SUCCESS != iRet )
    {
      dsm_Printf( BASE_FAT_TS_ID, "Make direcotry failed!,err_code[%d]\n",iRet );
    }

    return ( ERR_SUCCESS != iRet ) ? FALSE : TRUE;
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}

static BOOL shell_FS_RemoveDir( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 iRet;

  if ( 2 == argc )
  {
    DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
    fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
    iRet = FS_RemoveDir( g_Oem_Uni_Buf );
    dsm_Printf( BASE_FAT_TS_ID, "%s\n", ( ERR_SUCCESS == iRet ) ? "Remove direcotry OK" : "Remove direcotry failed!" );
    if(ERR_SUCCESS != iRet)
    {
        dsm_Printf( BASE_FAT_TS_ID,"errorcode =  %d\n", iRet);
    }
    return ( ERR_SUCCESS == iRet ) ? TRUE : FALSE;
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}

static BOOL shell_FS_Rename( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;

    if ( 3 == argc )
    {
        DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
        fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
        DSM_MemZero(g_Oem_Uni_Buf2, LEN_FOR_OEM_UNI_BUFFER);
        fat_OEM2Unicode((UINT8*)argv[ 2 ], (UINT16)DSM_StrLen((UINT8*)argv[ 2 ]), g_Oem_Uni_Buf2, &g_ULen);
        iRet = FS_Rename( g_Oem_Uni_Buf, g_Oem_Uni_Buf2 );
        if ( ERR_SUCCESS != iRet)
            dsm_Printf( BASE_FAT_TS_ID, "Rename failed[%d]!\n", iRet );

        return ( ERR_SUCCESS != iRet ) ? FALSE : TRUE;
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }
}


/*BPB: BIOS Parameter Block*/
VOID fat_dump_bpb( PCSTR pszDevName )
{
    FAT_BOOT_RECORD boot_rec;
    //UINT8* secbuf;
	INT32 iDevNo;
    INT32 iRet;
    
	iDevNo = DRV_DevName2DevNr(pszDevName);
	if(INVALID_DEVICE_NUMBER == iDevNo)
	{
        dsm_Printf( BASE_FAT_TS_ID, "Invalid device name" );
        return;
	}

    iRet = DRV_BLOCK_READ( iDevNo, 0, g_shell_buffer );
    if(_ERR_FAT_SUCCESS == iRet)
    {
        Buf2FBR(g_shell_buffer, &boot_rec);
            
        dsm_Printf(BASE_FAT_TS_ID, "BPB_BytesPerSec = %d\n",boot_rec.BPB_BytesPerSec);
        dsm_Printf(BASE_FAT_TS_ID, "BPB_TotSec32    = %d\n",boot_rec.BPB_TotSec32);
        dsm_Printf(BASE_FAT_TS_ID, "BPB_TotSec16    = %d\n",boot_rec.BPB_TotSec16);
        dsm_Printf(BASE_FAT_TS_ID, "BPB_RsvdSecCnt  = %d\n",boot_rec.BPB_RsvdSecCnt);
        dsm_Printf(BASE_FAT_TS_ID, "BPB_RootEntCnt  = %d\n",boot_rec.BPB_RootEntCnt);
        dsm_Printf(BASE_FAT_TS_ID, "BPB_FATSz16     = %d\n",boot_rec.BPB_FATSz16);
        dsm_Printf(BASE_FAT_TS_ID, "BPB_FATSz32     = %d\n",boot_rec.u.sub_rec32.BPB_FATSz32);
        dsm_Printf(BASE_FAT_TS_ID, "BPB_SecPerTrk   = %d\n",boot_rec.BPB_SecPerTrk);
        dsm_Printf(BASE_FAT_TS_ID, "BPB_NumHeads    = %d\n",boot_rec.BPB_NumHeads);
        dsm_Printf(BASE_FAT_TS_ID, "BPB_SecPerClus  = %d\n",boot_rec.BPB_SecPerClus);
        dsm_Printf(BASE_FAT_TS_ID, "BPB_NumFATs     = %d\n",boot_rec.BPB_NumFATs);
        dsm_Printf(BASE_FAT_TS_ID, "BPB_Media       = %d\n",boot_rec.BPB_Media);
        return;
    }
    else
    {
        dsm_Printf( BASE_FAT_TS_ID, "Read the first sector of the volume failed" );
    }
}

static BOOL shell_FS_FAT_Dump( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 counter;
    INT32 iRet;

    if ( argc >= 3 )
    {
        if ( 0 == DSM_StrCaselessCmp("fatlist", argv[1] ) )
        {
            iRet = fat_dump_fat_list( (UINT8*)argv[ 2 ] );
        }
        else if ( 0 == DSM_StrCaselessCmp("fatent", argv[1] ) )
        {
            if ( 3 == argc )
            {
                counter = 8;
            }
            else
            {
                counter = DSM_StrAToI(argv[ 3 ] );   
            }
            iRet = fat_dump_fat( DSM_StrAToI(argv[ 2 ] ), counter );
        }
        else if ( 0 == DSM_StrCaselessCmp("bpb", argv[1] ) )
        {
            fat_dump_bpb(argv[2]);
        }
        else
        {
            return FALSE;
        }

        return ( ERR_SUCCESS != iRet ) ? FALSE : TRUE;
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }
}
static BOOL shell_FS_GetFSInfo(struct cmd_entry *ent, int argc, char **argv)
{
    INT32 iRet;
    FS_INFO sFSInfo;
    iRet = ERR_SUCCESS;
    if(argc == 2)
    {
        iRet = FS_GetFSInfo((UINT8*)argv[1],&sFSInfo);
        if(ERR_SUCCESS != iRet)
        {
            dsm_Printf( BASE_FAT_TS_ID, "GetFsInfo failed!\n" );
            return FALSE;
        }
        else
        {
            dsm_Printf( BASE_FAT_TS_ID, "TotalSize = %d, UsedSize = %d",sFSInfo.iTotalSize, sFSInfo.iUsedSize);
            return TRUE;
        }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }
}

static BOOL shell_FS_GetFileAttr(struct cmd_entry *ent, int argc, char **argv)
{
    INT32 iRet = ERR_SUCCESS;
    FS_FILE_ATTR sFileAttr;
    struct tm* psSysTime;
    char * pctime;
    if(argc == 2)
    {
        DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
        fat_OEM2Unicode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), g_Oem_Uni_Buf, &g_ULen);
        iRet = FS_GetFileAttr( g_Oem_Uni_Buf,&sFileAttr );
        if( iRet == ERR_SUCCESS )
        {
            dsm_Printf( BASE_FAT_TS_ID, "ino=%d, mode = 0x%x, size=%d, count=%d\n",
                sFileAttr.i_no, sFileAttr.i_mode, sFileAttr.i_size, sFileAttr.i_count );

            psSysTime = localtime ((INT32*)(&sFileAttr.i_atime));
            pctime = ctime((INT32*)&sFileAttr.i_atime);
            dsm_Printf(BASE_FAT_TS_ID,"atime: %d, %d, %d %s",
                psSysTime->tm_yday, psSysTime->tm_mon, psSysTime->tm_year, pctime );

            psSysTime = localtime ((INT32*)&sFileAttr.i_ctime);
            pctime = ctime((INT32*)&sFileAttr.i_ctime);
            dsm_Printf(BASE_FAT_TS_ID,"ctime: %d, %d, %d %s",
                psSysTime->tm_yday, psSysTime->tm_mon, psSysTime->tm_year, pctime );

            psSysTime = localtime ((INT32*)&sFileAttr.i_mtime);
            pctime = ctime((INT32*)&sFileAttr.i_mtime);
            dsm_Printf(BASE_FAT_TS_ID,"mtime: %d, %d, %d %s",
                psSysTime->tm_yday, psSysTime->tm_mon, psSysTime->tm_year, pctime );
            
            return TRUE;
        }
        else
        {
            dsm_Printf(BASE_FAT_TS_ID,"FS_GetFileAttr Failed\n");
            return FALSE;
        }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }
}

static BOOL shell_FS_HasFormated(struct cmd_entry *ent, int argc, char **argv)
{
    INT32 iRet = ERR_SUCCESS;
    if(argc == 3)
    {
       iRet = FS_HasFormatted((PCSTR)argv[1],(UINT8)DSM_StrAToI(argv[2]));
       if(ERR_FS_HAS_FORMATED == iRet)
       {
           dsm_Printf(BASE_FAT_TS_ID,"fs has formatted!\n");
           return TRUE;
       }
       else
       {
           dsm_Printf(BASE_FAT_TS_ID,"fs has not formatted! errorcode = %d\n",iRet);
            return FALSE;
       }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }
}
static BOOL Shell_FS_ScanDisk(struct cmd_entry *ent, int argc, char **argv)
{
    INT32 iRet = ERR_SUCCESS;
    if(argc == 3)
    {
        iRet = FS_ScanDisk((UINT8*)argv[1],(UINT8)DSM_StrAToI(argv[2]));
        if(ERR_SUCCESS != iRet)
        {
             dsm_Printf(BASE_FAT_TS_ID,"Scan Disk Error,errorcode =  %d\n",iRet);
                return FALSE;
        }
        else
        {
            dsm_Printf(BASE_FAT_TS_ID,"SCAN DISK SUCCEED!\n");
               return TRUE;
        }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }
}


static BOOL shell_FS_GetFileName( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 fd;
  INT32 iRet;
  UINT8 fileName[LEN_FOR_OEM_UNI_BUFFER] = {0,};
  
  if ( 2 <= argc )
  {
    fd = DSM_StrAToI( argv[ 1 ] );
    iRet = FS_GetFileName( fd, LEN_FOR_OEM_UNI_BUFFER, fileName);
    if ( ERR_SUCCESS == iRet )
    {
        DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
        fat_Unicode2OEM(fileName, fat_UnicodeLen(fileName), g_Oem_Uni_Buf, &g_ULen);
        dsm_Printf( BASE_FAT_TS_ID, "%s\n", g_Oem_Uni_Buf);
        return TRUE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "FS_GetFileName failed!errirCode = %d. \n", iRet);
      return FALSE;
    }
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}


static BOOL shell_FS_Truncate( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 fd;
  UINT32 nFileSize;
  INT32 iRet;
 
  if (argc >= 3)
  {
    fd = DSM_StrAToI( argv[ 1 ] );
    nFileSize = DSM_StrAToI( argv[ 2 ] );
    iRet = FS_Truncate( fd, nFileSize);
    if ( iRet >= 0 )
    {
        return TRUE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "FS_Truncate failed!errirCode = %d. \n", iRet);
      return FALSE;
    }
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}


static BOOL shell_FS_Flush( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 fd;
  INT32 iRet;
 
  if (argc >= 2)
  {
    fd = DSM_StrAToI( argv[ 1 ] );
    iRet = FS_Flush(fd);
    if ( ERR_SUCCESS == iRet )
    {
        return TRUE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "FS_Flush failed!errirCode = %d. \n", iRet);
      return FALSE;
    }
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}


#define FS_ERROR_RESERVED -1
#define NR_MAX_DEPTH           100
#define INVALID_FD                 -1
#define LEN_PATH_BUF              512

UINT8 pcPath[LEN_PATH_BUF] = {0,};

INT32 FS_DropDirTree(const UINT8* pcDirName)
{
    INT32 i, iDepth = 0;
    INT32 iFindFd[NR_MAX_DEPTH];
    UINT8 *pOriginal, *p = pcPath;
    UINT8 str[] = "/*";
    INT32 iRet;
    UINT8 c;
    UINT32 iNameLen;
    FS_FIND_DATA sFindData;

    strcpy(pcPath, g_dsm_Prompt);

    if ('\\' == *pcDirName || '/' == *pcDirName)
    {
        // cdPath为绝对路径。
        if (!DSM_StrCmp(pcDirName, "\\") || !DSM_StrCmp(pcDirName, "/"))
        {
            //return -1; // 不能删除根目录。
        }
        else
        {
            *p = 0;
        }
    }
    else
    {
        // cdPath为相对路径，初始化p指向字符串末尾。
        while (*p)
        {
            p++;
        }
        while (('/' == (c = *(p - 1)) || '\\' == c || '>' == c) && p > pcPath)
        {
            p--;
            *p = 0;
        }
    }
    strncpy(p, "/", 1);
    p++;
    iNameLen = DSM_StrLen(pcDirName);
    strncpy(p, pcDirName, iNameLen);
    p += iNameLen;
    pOriginal = p;


    for (i = 0; i < NR_MAX_DEPTH; ++i)
    {
        iFindFd[i] = INVALID_FD;
    }

    while (TRUE)
    {
        if (INVALID_FD == iFindFd[iDepth])
        {
            if ((p + DSM_StrLen(str)) >= (pcPath + LEN_PATH_BUF))
            {
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            strcpy(p, str);
            p++;

            DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
            fat_OEM2Unicode(pcPath, (UINT16)DSM_StrLen(pcPath), g_Oem_Uni_Buf, &g_ULen);
            iFindFd[iDepth] = FS_FindFirstFile(g_Oem_Uni_Buf, &sFindData);
            if (ERR_FS_NO_MORE_FILES == iFindFd[iDepth])
            {
                ; // 不可能的情况。
            }
            else if (iFindFd[iDepth] < 0)
            {
                CSW_TRACE(MMI_TS_ID, "[#error!!]: FS_FindFirstFile return = %d.\n", iFindFd[iDepth]);
                iRet = iFindFd[iDepth];
                iDepth--;
                goto failed;
            }

            DSM_MemZero(g_Oem_Uni_Buf2, LEN_FOR_OEM_UNI_BUFFER);
            fat_Unicode2OEM(sFindData.st_name, fat_UnicodeLen(sFindData.st_name), g_Oem_Uni_Buf2, &g_ULen);
            if ((p + DSM_StrLen(g_Oem_Uni_Buf2)) >= (pcPath + LEN_PATH_BUF))
            {
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            strcpy(p, g_Oem_Uni_Buf2);

            DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
            fat_OEM2Unicode(pcPath, (UINT16)DSM_StrLen(pcPath), g_Oem_Uni_Buf, &g_ULen);
                
            if (FS_INODE_IS_REG( sFindData.st_mode ))
            {
                if ( ERR_SUCCESS != (iRet = FS_Delete(g_Oem_Uni_Buf)) )
                {
                    CSW_TRACE(MMI_TS_ID, "[#error!!]: FS_Delete111 return = %d.\n", iRet);
                    goto failed;
                }
            }
            else if (FS_INODE_IS_DIR( sFindData.st_mode ) && '.' == sFindData.st_name[0] 
                        && 0 == sFindData.st_name[1])
            {
                ;
            }
            else
            {
                iRet = FS_RemoveDir(g_Oem_Uni_Buf);
                if (ERR_SUCCESS == iRet)
                {
                    ;
                }
                else if (iRet != ERR_FS_DIR_NOT_EMPTY)
                {
                    CSW_TRACE(MMI_TS_ID, "[#error!!]: FS_RemoveDir111 return = %d.\n", iRet);
                    goto failed;
                }
                else
                {
                    goto dir_not_empty;
                }
            }
        }

        while ( ERR_SUCCESS == ( iRet = FS_FindNextFile( iFindFd[iDepth], &sFindData ) ) ) 
        {
            DSM_MemZero(g_Oem_Uni_Buf2, LEN_FOR_OEM_UNI_BUFFER);
            fat_Unicode2OEM(sFindData.st_name, fat_UnicodeLen(sFindData.st_name), g_Oem_Uni_Buf2, &g_ULen);
            if ((p + DSM_StrLen(g_Oem_Uni_Buf2)) >= (pcPath + LEN_PATH_BUF))
            {
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            strcpy(p, g_Oem_Uni_Buf2);

            DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
            fat_OEM2Unicode(pcPath, (UINT16)DSM_StrLen(pcPath), g_Oem_Uni_Buf, &g_ULen);
            
            if (FS_INODE_IS_REG( sFindData.st_mode ))
            {
                if ( ERR_SUCCESS != (iRet = FS_Delete(g_Oem_Uni_Buf)))
                {
                    CSW_TRACE(MMI_TS_ID, "[#error!!]: FS_Delete222 return = %d.\n", iRet);
                    goto failed;
                }
            }
            else if (FS_INODE_IS_DIR( sFindData.st_mode ) && '.' == sFindData.st_name[0] 
                        && 0 == sFindData.st_name[1])
            {
                ;
            }
            else
            {
                iRet = FS_RemoveDir(g_Oem_Uni_Buf);
                if (ERR_SUCCESS == iRet)
                {
                    ;
                }
                else if (iRet != ERR_FS_DIR_NOT_EMPTY)
                {
                    CSW_TRACE(MMI_TS_ID, "[#error!!]: FS_RemoveDir222 return = %d.\n", iRet);
                    goto failed;
                }
                else
                {
                    goto dir_not_empty;
                }
            }
        }

        if (iRet != ERR_FS_NO_MORE_FILES)
        {
            goto failed;
        }

        // 当前目录已空，将当前目录删除。
        FS_FindClose(iFindFd[iDepth]);
        iFindFd[iDepth] = INVALID_FD;
        iDepth--;
        p--;
        *p = (UINT8)NULL;
        DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
        fat_OEM2Unicode(pcPath, (UINT16)DSM_StrLen(pcPath), g_Oem_Uni_Buf, &g_ULen);
        if ((iRet = FS_RemoveDir(g_Oem_Uni_Buf)) != ERR_SUCCESS)
        {
            CSW_TRACE(MMI_TS_ID, "[#error!!]: FS_RemoveDir333 return = %d.\n", iRet);
            goto failed;
        }
        if (p == pOriginal)
        {
            if (iDepth != -1)
            {
                CSW_TRACE(MMI_TS_ID, "[#error!!]: p == pOriginal && iDepth != 0.\n");
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            break;
        }
        
        while ((c = *(p - 1)) != '/' && c != '\\')
        {// 返回上级目录继续搜索。
            p--;
        }
        *p = (UINT8)NULL;
        continue;
        
dir_not_empty:    
        DSM_MemZero(g_Oem_Uni_Buf2, LEN_FOR_OEM_UNI_BUFFER);
        fat_Unicode2OEM(sFindData.st_name, fat_UnicodeLen(sFindData.st_name), g_Oem_Uni_Buf2, &g_ULen);
        if ((NR_MAX_DEPTH - 1) == iDepth)
        {// 如果栈满则退出。
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }      
        iNameLen = DSM_StrLen(g_Oem_Uni_Buf2);
        p += iNameLen;
        iDepth++;
    }

    return ERR_SUCCESS;

failed:
    CSW_TRACE(MMI_TS_ID, "[#error!!]: failed+++++ iFindFd[%d] = %d \n [pcPath]:\"%s\".\n", iDepth, iFindFd[iDepth], pcPath);
    for (i = iDepth; i >= 0; --i)
    {
        FS_FindClose(iFindFd[i]);
    }
    return iRet;
        
}


#define FS_FILE_TYPE                0x001
#define FS_DIR_TYPE                 0x010
#define FS_RECURSIVE_TYPE     0x100

INT32 FS_Count(const UINT8* pcDirName, INT32 Flag)
{
    INT32 i, iDepth = 0;
    INT32 iFindFd[NR_MAX_DEPTH];
    UINT8 *pOriginal, *p = pcPath;
    UINT8 str[] = "/*";
    INT32 iRet;
    UINT8 c;
    UINT32 iNameLen;
    FS_FIND_DATA sFindData;
    INT32 iFileNum = 0;
    
    strcpy(pcPath, g_dsm_Prompt);

    if ('\\' == *pcDirName || '/' == *pcDirName)
    {
        *p = 0;
    }
    else
    {
        // cdPath为相对路径，初始化p指向字符串末尾。
        while (*p)
        {
            p++;
        }
        while (('/' == (c = *(p - 1)) || '\\' == c || '>' == c) && p > pcPath)
        {
            p--;
            *p = 0;
        }
    }
    strncpy(p, "/", 1);
    p++;
    iNameLen = DSM_StrLen(pcDirName);
    strncpy(p, pcDirName, iNameLen);
    p += iNameLen;
    pOriginal = p;

    for (i = 0; i < NR_MAX_DEPTH; ++i)
    {
        iFindFd[i] = INVALID_FD;
    }

    while (TRUE)
    {
        if (INVALID_FD == iFindFd[iDepth])
        {
            if ((p + DSM_StrLen(str)) >= (pcPath + LEN_PATH_BUF))
            {
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            strcpy(p, str);
            p++;

            DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
            fat_OEM2Unicode(pcPath, (UINT16)DSM_StrLen(pcPath), g_Oem_Uni_Buf, &g_ULen);
            iFindFd[iDepth] = FS_FindFirstFile(g_Oem_Uni_Buf, &sFindData);
            if (ERR_FS_NO_MORE_FILES == iFindFd[iDepth])
            {
                goto current_dir_empty;
            }
            else if (iFindFd[iDepth] < 0)
            {
                CSW_TRACE(MMI_TS_ID, "[#error!!]: FS_FindFirstFile return = %d.\n", iFindFd[iDepth]);
                iDepth--;
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
                         
            if (FS_INODE_IS_REG( sFindData.st_mode ))
            {
                if ( Flag & FS_FILE_TYPE )
                {
                    iFileNum++;
                }
            }
            else if (FS_INODE_IS_DIR( sFindData.st_mode )) 
            {
                if ('.'== sFindData.st_name[0] && 0 == sFindData.st_name[1])
                {
                    ;
                }
                else
                {
                    if (Flag & FS_DIR_TYPE)
                    {
                        iFileNum++;
                    }
                    if (Flag & FS_RECURSIVE_TYPE)
                    {
                        if ((p + DSM_StrLen(sFindData.st_name)) >= (pcPath + LEN_PATH_BUF))
                        {
                            iRet = FS_ERROR_RESERVED;
                            goto failed;
                        }
                        strcpy(p, sFindData.st_name);
                        goto recursive_dir;
                    }
                }
            }
        }

        while ( ERR_SUCCESS == ( iRet = FS_FindNextFile( iFindFd[iDepth], &sFindData ) ) ) 
        {
            if (FS_INODE_IS_REG( sFindData.st_mode ))
            {
                if ( Flag & FS_FILE_TYPE )
                {
                    iFileNum++;
                }
            }
            else if (FS_INODE_IS_DIR( sFindData.st_mode )) 
            {
                if ('.'== sFindData.st_name[0] && 0 == sFindData.st_name[1])
                {
                    ;
                }
                else
                {
                    if (Flag & FS_DIR_TYPE)
                    {
                        iFileNum++;
                    }
                    if (Flag & FS_RECURSIVE_TYPE)
                    {
                        DSM_MemZero(g_Oem_Uni_Buf2, LEN_FOR_OEM_UNI_BUFFER);
                        fat_Unicode2OEM(sFindData.st_name, fat_UnicodeLen(sFindData.st_name), g_Oem_Uni_Buf2, &g_ULen);
                        if ((p + DSM_StrLen(g_Oem_Uni_Buf2)) >= (pcPath + LEN_PATH_BUF))
                        {
                            iRet = FS_ERROR_RESERVED;
                            goto failed;
                        }
                        strcpy(p, g_Oem_Uni_Buf2);
                        goto recursive_dir;
                    }
                }
            }
        }

        if (iRet != ERR_FS_NO_MORE_FILES)
        {
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }

        // 已统计完当前目录。
        iRet = FS_FindClose(iFindFd[iDepth]);

current_dir_empty:
        iFindFd[iDepth] = INVALID_FD;
        iDepth--;
        p--;
        *p = (UINT8)NULL;
        
        if (p == pOriginal)
        {
            if (iDepth != -1)
            {
                CSW_TRACE(MMI_TS_ID, "[#error!!]: p == pOriginal && iDepth != 0.\n");
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            break;
        }

        while ((c = *(p - 1)) != '/' && c != '\\')
        {// 返回上级目录继续统计。
            p--;
        }
        *p = (UINT8)NULL;
        continue;
        
recursive_dir:    // 压栈，进行统计子目录。
        DSM_MemZero(g_Oem_Uni_Buf2, LEN_FOR_OEM_UNI_BUFFER);
        fat_Unicode2OEM(sFindData.st_name, fat_UnicodeLen(sFindData.st_name), g_Oem_Uni_Buf2, &g_ULen);
        if ((NR_MAX_DEPTH - 1) == iDepth)
        {// 如果栈满则退出。
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }      
        iNameLen = DSM_StrLen(g_Oem_Uni_Buf2);
        p += iNameLen;
        iDepth++;
    }
    
    return iFileNum;

failed:


    CSW_TRACE(MMI_TS_ID, "+++++failed !!!+++++.\n", iRet);
    for (i = iDepth; i >= 0; --i)
    {
        CSW_TRACE(MMI_TS_ID, "+++++FS_FindClose: iFindFd[%d] = %d +++++.\n",i, iFindFd[i]);
        FS_FindClose(iFindFd[i]);
    }
    return iRet;
        
}


static BOOL shell_DropDirTree( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 iRet;
  UINT8 fileName[LEN_FOR_OEM_UNI_BUFFER] = {0,};
  
  if ( 2 <= argc )
  {
    iRet = FS_DropDirTree( argv[ 1 ]);
    if ( ERR_SUCCESS == iRet )
    {
        return TRUE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "MMI_FS_RemoveDir failed!errirCode = %d. \n", iRet);
      return FALSE;
    }
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}


static BOOL shell_Count_file( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 iRet;
  INT32 flag = 1;
  UINT8 fileName[LEN_FOR_OEM_UNI_BUFFER] = {0,};

  if ( argc >= 2)
  {
    if (argc > 2)
    {
        flag = DSM_StrAToI( argv[2] );
    }
    if (1 == flag)
    {
        flag = FS_FILE_TYPE | FS_DIR_TYPE | FS_RECURSIVE_TYPE;
    }
    else if (2 == flag)
    {
        flag = FS_FILE_TYPE | FS_RECURSIVE_TYPE;
    }
    else if (3 == flag)
    {
        flag = FS_DIR_TYPE | FS_RECURSIVE_TYPE;
    }
    else if (4 == flag)
    {
        flag = FS_FILE_TYPE | FS_DIR_TYPE;
    }
    else if (5  == flag)
    {
        flag = FS_FILE_TYPE;
    }
    else if (6  == flag)
    {
        flag = FS_DIR_TYPE;
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }
    
    iRet = FS_Count( argv[ 1 ], flag);
    if (iRet >= 0)
    {
       dsm_Printf( BASE_FAT_TS_ID, "\t%d. \n", iRet);
        return TRUE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "FS_Count failed!errirCode = %d. \n", iRet);
      return FALSE;
    }
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}


//-----------------------------------------------------------------------------
//功能：比较两个UNICODE串，字母不区分大小写;
//返回值：0：两个串相等；1：str1 > str2；-1：str1 < str2
//Suwg
INT8 UniStrCmpi(const UINT16 *str1, const UINT16 *str2)
{
#define IS_LETTER(c)        ((c >= 'A' && c <= 'z') || (c >= 'a' && c <= 'z'))
#define IS_MATCH_LETTER(c1, c2)     (c1 - c2 == 32 || c2 - c1 == 32 || c1 -c2 == 0) 
    
    const UINT16 *p1 = str1;
    const UINT16 *p2 = str2;
    UINT8 c1, c2;

    while ( *p1 || *p2 )
    {
        if (*p1 != *p2)
        {   // 判断低八位
            c1 = ((UINT8*)p1)[1];
            c2 = ((UINT8*)p2)[1];
            if (c1 != 0 || c2 != 0)
            {// 其中一个不为字母。
                return (*p1 > *p2 ? 1 : -1);
            }
            // 两者低八位都为0，比较高八位。
            c1 = ((UINT8*)p1)[0];
            c2 = ((UINT8*)p2)[0];
            // 统一转为小写进行比较。
            if (c1 >= 'A' && c1 <= 'Z')
            {
                c1 += 32;
            }
            if (c2 >= 'A' && c2 <= 'Z')
            {
                c2 += 32;
            }
            if (!IS_LETTER(c1) || !IS_LETTER(c2) || !IS_MATCH_LETTER(c1, c2))
            {
                return (c1 > c2 ? 1 : -1);
            }
        }
        p1++;
        p2++;
    }
    return 0;
}


static BOOL shell_UniCmp( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 iRet;
  UINT8 fileName[LEN_FOR_OEM_UNI_BUFFER] = {0,};

  if ( argc >= 3)
  {
    DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
    fat_OEM2Unicode(argv[1], (UINT16)DSM_StrLen(argv[1]), g_Oem_Uni_Buf, &g_ULen);

    DSM_MemZero(g_Oem_Uni_Buf2, LEN_FOR_OEM_UNI_BUFFER);
    fat_OEM2Unicode(argv[2], (UINT16)DSM_StrLen(argv[2]), g_Oem_Uni_Buf2, &g_ULen);

    iRet = UniStrCmpi( (const UINT16*)argv[ 1 ], (const UINT16*)argv[2]);
    dsm_Printf( BASE_FAT_TS_ID, "\t%d. \n", iRet);
    return TRUE;
  }
  else
  {
    PRINT_CMD_USAGE( ent );
    return FALSE;
  }
}


struct cmd_entry g_FatCmdEnts[ ] =
{
  { "ms", shell_module_switch, "module switch for running specified module's command", "ms [FAT|VDS]" },
  { "rep", shell_repeat, "do something repetitively, %LOOP% is the name for loop variable", "rep ( base, step, counter ) command line" },
  { "debug", shell_debug, "debug switch", "debug [VDS|FAT] [on | off] [error | brief | detail]" },
  { "brep", shell_block_repeat, "do something for block repetitively", "brep {[-F]} <counter> {command line}" },
  { "help", shell_help, "print help message", NULL },
  { "version", shell_version, "show the version of the module", "version [vds | fat | reg]" },
  { "?", shell_help, "print help message", NULL },


  { "init", shell_fs_init, "initiate the file system, yes or no means formating device forcibly or not", "init [YES| NO] <root_dev_name> [quick | normal]"},
  { "format", shell_FS_Format, "Format device as a type of filesystem", "format <dev_name> <filesystem_type> <format_type>"},

  { "mountr", shell_mount_root, "mount root filesystem", "mountr <dev_name>"},
  { "mount", shell_mount, "mount filesystem", "mount [FAT|FFS|MinixFS] <dev_name> <mount_point>"},
  { "unmount", shell_unmount, "unmount filesystem(1/0 means force to unmount or not)", "unmount  <mount_point> [1 | 0]"},
  { "um", shell_unmount, "unmount filesystem(1/0 means force to unmount or not)", "unmount  <mount_point> [1 | 0]"},

  { "ls", shell_FS_Find, "print a directory listing",  "ls {<pathname>}" },
  { "dir", shell_FS_Find, "print a directory listing(same as ls)",  "dir {<pathname>}" },
  { "find", shell_FS_Find, "find files/subdirectoies", "\n\tfind <pattern>" },
  { "cd", shell_FS_ChangeDir, "change directory", "cd <pathname>" },
  { "pwd", shell_FS_PWD, "get current work directory",  "pwd" },
  { "sfirst", shell_FS_SearchFirst, "search files/subdirectoies first",  "sfirst <pathname>" },
  { "snext", shell_FS_SearchNext, "search files/subdirectoies next",  "snext <fd>" },
  { "sclose", shell_FS_SearchClose, "close search descriptor",  "sclose <fd>" },
    
  { "eof", shell_FS_EndOfFile, "check if end of file", "eof <fd>" },
  { "create", shell_FS_Create, "create a file by specifying the name", "create <filename>" },
  { "creclo", shell_FS_CreClo, "create a file by specifying the name, and close the handle",  "create <filename>" },
  { "cc",     shell_FS_CreClo, "create a file by specifying the name, and close the handle",  "create <filename>" },
  { "open", shell_FS_Open, "open an existing file to read/write with the flag","open [PathName] [Flag], Flag can be:\n A:FS_O_APPEND C:FS_O_CREAT E:FS_O_EXCL T:FS_O_TRUNC R:FS_O_RDONLY W:FS_O_WRONLY +:FS_O_RDWR" },
  { "rm", shell_FS_DeleteFile, "delete the file", "rm <filename>" },
  { "rf", shell_FS_DeleteFile, "delete the file", "rf <filename>" },
  { "read", shell_FS_Read, "read from the file associated with fd", "read <fd> <length>" },
  { "write", shell_FS_Write, "write string to the file associated with fd",  "write <fd> <string>" },
  { "seek", shell_FS_Seek, "seek the file pointer", "seek <fd> <offset> [beg | cur | end ]" },
  { "close", shell_FS_Close, "close file associated with fd", "close <fd>" },
//  { "bigfile", shell_FS_bigfile, "create a file and write data to it until the disk filled up",  "\n\tbigfile <filename> fixed <chunk-size>\n\tbigfile <filename> random" },
  { "lwrite", shell_FS_Lwrite, "write string to the file associated with fd",  "lwrite <fd> <length>" },
  
  { "md", shell_FS_MakeDir, "create a directory", "md <pathname> [ paralel | inclusive ]" },
  { "mkdir", shell_FS_MakeDir, "create a directory", "mkdir <pathname> [ paralel | inclusive ]" },
  { "rd", shell_FS_RemoveDir, "remove a directory", "rd <pathname>" },
  { "rmdir", shell_FS_RemoveDir, "remove a directory", "rmdir <pathname>" },
  { "bigfile", shell_FS_bigfile, "create a file and write data to it until the disk filled up",  "\n\tbigfile <filename> fixed <chunk-size>\n\tbigfile <filename> random" },
  { "bf", shell_FS_bigfile, "create a file and write data to it until the disk filled up",  "\n\tbigfile <filename> fixed <chunk-size>\n\tbigfile <filename> random" },
  { "rn", shell_FS_Rename, "rename a file",  "rename <oldname> <newname>" },
      
  { "dump", shell_FS_FAT_Dump, "dump data structure on the disk", "\ndump [fatlist] <filename>" 
                                                                  "\ndump [fatent] <begin_cluster_number>"
                                                                  "\ndump [bpb] <dev_name>"},
  { "info", shell_FS_GetFSInfo, "get the dev info","info <devname>"},
  { "fatt", shell_FS_GetFileAttr, "get file attribute","getattr <pathname>"},
  { "hasformat",shell_FS_HasFormated,"check if the file system was formated","hasformat<devname>[1 | 2]"},
  { "scandisk", Shell_FS_ScanDisk, "scan disk","scandisk<devname> <filesystem_type>" },
  { "getname", shell_FS_GetFileName, "get file name with fd","getname <fd>" },
  { "dtree", shell_DropDirTree, "drop dir tree","dtree <dir_path>" },
  { "count", shell_Count_file, "count file and dir","count <dir_path> <flag: FS_FILE_TYPE | FS_DIR_TYPE | FS_RECURSIVE_TYPE>" },
  { "ucmp", shell_UniCmp, "compare unicode string","ucmp <ustr1> <ustr2>" },
  { "trunc", shell_FS_Truncate, "trunctate file size","trunc <fd> <size>" },
  { "flush", shell_FS_Flush, "update file cache","flush <fd>" },
  { NULL, NULL, NULL, NULL}, 
};

#endif // VDS_SHELL_COMMAND


