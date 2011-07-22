#ifdef DSM_SHELL_COMMAND


#include "dsm_stdtype.h"
#include "dsm_cf.h"
#include "dsm_dbg.h"
#include "dsm_shell.h"

#include "fat_base.h"
#include "fs.h"
#include "fs_base.h"
#include "fat_local.h"
#include "tupgrade.h"

#define FS_SHELL_BUFFER_LEN_MAX 1024
#define FFS_SLEEP_VALUE   10
#define LEN_FOR_OEM_UNI_BUFFER      (FS_FILE_NAME_UNICODE_LEN + 2)

typedef struct _demo_file_info 
{
	char filename[LEN_FOR_OEM_UNI_BUFFER];
	unsigned int Attributes;
	struct _demo_file_info* next;
}DEMO_FILE_INFO;


char g_lwrite_buf[ VDS_SHELL_WRITE_BUFFER_LEN_MAX ];



extern char g_shell_buffer[ ];
extern char g_lwrite_buf[];

INT32 g_fs_has_initiated = FALSE;

#ifdef _MS_VC_VER_
extern INT32 g_dsm_errno;
#endif

DEMO_FILE_INFO* g_fssim_sort_file_list = NULL;
UINT32 g_ffsim_sort_file_count = 0;

INT8 UniStrCmp_IgnoreHAndL(const UINT16 *str1, const UINT16 *str2)

{
    WORD ch1,ch2;
    WORD len,len1,len2;

    if(str1 == 0 || str2 == 0)
    {
        return 0;
    }
    while(str1)
    {
        if(*str1 <= 0x20)
        {
            str1 ++;
        }
        else
        {
            break;
        }
    }

    while(str2)
    {
        if(*str2 <= 0x20)
        {
            str2 ++;
        }
        else
        {
            break;
        }
    }
    
    len1 = DSM_UnicodeLen((UINT8*)str1);
    len2 = DSM_UnicodeLen((UINT8*)str2);
    len = len1 > len2 ? len2:len1;
    
    while(len > 0)
    {       
        ch1 = *str1;
        ch2 = *str2;
        if (ch1 != ch2)
        {
            // change lower  to upper.
            if((ch1 & 0xff) == ch1 && (ch1 >= 'a') && (ch1  <= 'z'))
            {
                ch1 = ch1  - ('a' - 'A');                           				
            }
			if((ch2 & 0xff) == ch2 && (ch2 >= 'a') && (ch2 <= 'z'))
            {
                ch2 = ch2  - ('a' - 'A');                           				
            }
            return ((ch1 - ch2) > 0 ? 1 : -1);
        }
        str1++;
        str2++;
        len --;
    } 
    
    if(len1 > len2)
    {
        return -1;
    }
    else if(len1 < len2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void AppendFile(DEMO_FILE_INFO* p)
{
	
	if(!p)
	{
		return;
	}
	if(!g_fssim_sort_file_list)
	{
        g_ffsim_sort_file_count = 0;
	}
	p->next = g_fssim_sort_file_list;
    g_fssim_sort_file_list = p; 
    g_ffsim_sort_file_count ++;
}

void ReleaseAllChain(void)
{
	DEMO_FILE_INFO* p = NULL;
    DEMO_FILE_INFO* pTmp = NULL;
    
	p = g_fssim_sort_file_list;
	while(p)
	{
        pTmp = p;		
		p = pTmp->next;
        DSM_Free(pTmp);
	}	
	g_fssim_sort_file_list = NULL;
    g_ffsim_sort_file_count = 0;
}

void PrintAllChain(void)
{
	DEMO_FILE_INFO* p = NULL;
	UINT8* pOemName = NULL;
	UINT32 uOemLen = 0;
    
	p = g_fssim_sort_file_list;
	while(p)
	{      
		if(pOemName)
		{
			DSM_Free(pOemName);
			pOemName = NULL;
		}
        DSM_Unicode2OEM(p->filename, DSM_UnicodeLen(p->filename), &pOemName, &uOemLen,NULL);      
		dsm_Printf(BASE_VDS_TS_ID,"#debug sort>attrib = 0x%x,filename = %s.\n",p->Attributes,pOemName);
		if(pOemName)
		{
			DSM_Free(pOemName);
			
		}
		pOemName = NULL;
		p = p->next;
		
	}	   
}


    
#define FSSIM_FILE_INFO_NODE DEMO_FILE_INFO
#define COS_MALLOC malloc
#define COS_FREE free
void SortByName_1(UINT32 len)
{  
   #if 0
   INT32 exchange = 0;
   FSSIM_FILE_INFO_NODE** node = NULL;
   FSSIM_FILE_INFO_NODE** dir_node = NULL;
   UINT32 dir_count = 0;
   FSSIM_FILE_INFO_NODE** file_node = NULL;
   UINT32 file_count = 0;
   FSSIM_FILE_INFO_NODE* p = NULL; 
   UINT32 i= 0,j = 0;

   UINT8* file_name = NULL;
   UINT32 f_len = 0;   

   
   node = (FSSIM_FILE_INFO_NODE**)COS_MALLOC(SIZEOF(UINT32)*len);
    
   if(NULL == node)
   {
        return;
   }
//   ML_SetCodePage("CP936");
   // get pointer list.
   p = g_fssim_sort_file_list;
   i = 0;
   while(p)
   {
	  if(i >= len || p == NULL)
	  {
		  dsm_Printf(BASE_VDS_TS_ID,"sort name node number[%d] > len[%d],p = 0x%x.\n", i,len,p); 
		  break;
	  }
         
        node[i] = (FSSIM_FILE_INFO_NODE*)p;	  
        iResult = ML_Unicode2LocalLanguage(p->filename,(UINT32)DSM_UnicodeLen(p->filename),&file_name,&f_len,NULL);
        if(iResult == 0)
       {                        
            dsm_Printf(BASE_VDS_TS_ID,"#debug SortByName[%d] file = %s.\n", i,file_name); 
            if(file_name)
            {
                COS_FREE(file_name);
                file_name = NULL;
            }
        }
        else
        {
            dsm_Printf(BASE_VDS_TS_ID,"#debug SortByName[%d] file = unknown.n", i); 
        }
        
        sxr_Sleep(5);
        p = p->next;
	 i++;
   }
   
   // find dir list and file list.
 
   for(i = 0; i < len; i++)
   {
	  exchange = 0;
         for(j = i; j < len - i -1; j++)
	  {
               if((node[j]->Attributes  & FS_ATTR_DIR) < (node[j+1]->Attributes  & FS_ATTR_DIR))
		 {
			 p = node[j];
			 node[j] = node[j + 1];
			 node[j + 1] = p;
			 exchange = 1;
		 }
	  }
	  if(exchange == 0)
	  {
		  break;
	  }
   }
   dir_node = node;
   p = node[0];

   // get dir list and file list.
   for(i = 0; i < len; i++)
   {
      if((node[i]->Attributes & FS_ATTR_DIR) == 0) 
	  {
	       file_node = &node[i];
              break;
		
	  }
      dir_count ++;
   }
   file_count = len - dir_count;

   // dir sort by name 
   for(i = 0 ; i < dir_count; i++)
   {
	   exchange = 0;	   
	   for(j = 0; j < dir_count - i - 1; j++)
	   {		   
		   
		   if (UniStrCmp_IgnoreHAndL((UINT16*)dir_node[j]->filename, (UINT16*)dir_node[j+1]->filename) > 0)
		   {	
			    p = dir_node[j];
			    dir_node[j] = dir_node[j + 1];
			    dir_node[j + 1] = p;
			    exchange = 1;			  
		   }
		   else
		   {
			   // do nothing.
		   }
	   }
	   if(exchange == 0)
	   {
		   break;
	   }        
   }

   // file sort by name 
   for(i = 0 ; i < file_count; i++)
   {
	   exchange = 0;	   
	   for(j = 0; j < file_count - i -1; j++)
	   {		   
		   
		   if (UniStrCmp_IgnoreHAndL((UINT16*)file_node[j]->filename, (UINT16*)file_node[j+1]->filename) > 0)
		   {	
			    p = file_node[j];
			    file_node[j] = file_node[j + 1];
			    file_node[j + 1] = p;
			    exchange = 1;			  
		   }
		   else
		   {
			   // do nothing.
		   }
	   }
	   if(exchange == 0)
	   {
		   break;
	   }        
   }
   
   // Create chain.

   // add dir list to chain.
   p = *dir_node;
   if(p)
   {       
       for(i = 1; i < dir_count; i++)
       {
          p->next = dir_node[i];
    	  p = dir_node[i]->next;	  
       }

       if(p)
       {
           // add file list to chain.
           for(i = 0; i < file_count; i++)
           {
              p->next = file_node[i];
        	  p = file_node[i]->next;
           }        
           // set the last node 
           if(p)
           {
               p->next = NULL;
           }
        }
    }
   g_fssim_sort_file_list = *dir_node;
   if(node)
   {
	   COS_FREE(node);
   }
   return;
   #endif
}


void SortByName(INT32 len)
{

    DEMO_FILE_INFO *node1 = NULL, *node2 = NULL, *dir_list = NULL,*prenode = NULL;
    DEMO_FILE_INFO *file_list = NULL;
	
    int i,j;
    int dir_count = 0;
    int file_count = 0;
    int exchange = 0;

    // classify de.
    // de_chain = dir_chain + file_chain
    prenode = g_fssim_sort_file_list;
    for(i = 0; i < len; i++)
    {
        exchange = 0;
        node1 = g_fssim_sort_file_list;
        for(j = 0; j < len - i; j++)
        {
            
            if(node1->next == 0)
            {
                break;
            }
			node2 = node1->next;
            if((node1->Attributes & FS_ATTR_DIR) < (node2->Attributes & FS_ATTR_DIR))
            {	
                if(node1 == g_fssim_sort_file_list)
                {					
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    g_fssim_sort_file_list = node2;				
                    node1 = prenode->next;
                                        	
                }
                else
                {					
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;				
                    node1 = prenode->next;
                }				
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }

    }

    // find the file_list header.
    // accumulative total of directory number.
    dir_list = g_fssim_sort_file_list;	
    while(dir_list) 
    {
        if((dir_list->Attributes & FS_ATTR_DIR))
        {
            dir_count ++;
        }
        if(dir_list->next == 0)
        {
            break;
        }
        if((dir_list->next->Attributes & FS_ATTR_DIR) == 0)
        {
            file_list = dir_list->next;
            break;
        }        
        
        dir_list = dir_list->next;
    }
    
    file_count = len - dir_count;

    // sort directory list by name. 
    prenode = g_fssim_sort_file_list;
    for(i = 0; i < dir_count; i++)
    {
        exchange = 0;
        node1 = g_fssim_sort_file_list;
        for(j = 0; j < dir_count - i; j++)
        {
            
            if(node1->next == 0)
            {
                break;
            }
			node2 = node1->next;
            if (UniStrCmp_IgnoreHAndL((UINT16*)node1->filename, (UINT16*)node2->filename) > 0)
            {	
				if(node1 == g_fssim_sort_file_list)
				{
				
					prenode = node2;					
					node1->next = node2->next;
					node2->next = node1;
					g_fssim_sort_file_list = node2;	
					node1 = prenode->next;
					
				}
				else
				{
					prenode->next = node2;					
					node1->next = node2->next;
					node2->next = node1;
					prenode = node2;		
					node1 = prenode->next;
				}
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }        
    }

    // sort file list by name. 
    prenode = dir_list;
    for(i = 0; i < file_count; i++)
    {
        exchange = 0;
        node1 = file_list;
		if(!node1)
		{
			break;
		}
        for(j = 0; j < file_count - i; j++)
        {
            
            if(node1->next == 0)
            {
				prenode->next = file_list;
                break;
            }
			node2 = node1->next;
            if(UniStrCmp_IgnoreHAndL((UINT16*)node1->filename, (UINT16*)node2->filename) > 0)
            {	
				if(node1 == file_list)
				{
					prenode = node2;					
					node1->next = node2->next;
					node2->next = node1;
					file_list = node2;
					node1 = prenode->next;
					
				}
				else
				{
					prenode->next = node2;					
					node1->next = node2->next;
					node2->next = node1;
					prenode = node2;		
					node1 = prenode->next;
				}
                exchange = 1;	
                
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }        
    }
    return ;
}


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
	 iFsType = (UINT8)(DSM_StrCmp(argv[2],"FAT") == 0 ? 1:2 );
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

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
static BOOL shell_VDS_Flush( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;
	UINT32 uUniLen = 0;
    UINT8 uFsType = 0;
    UINT8* pUniName = NULL;
    if (1 <= argc )
    {
        VDS_CacheFlushAlloc();
        do 
        {
            iRet = VDS_FlushCache();
            
            if ( ERR_SUCCESS != iRet )
            {
                //dsm_Printf( BASE_FAT_TS_ID, "vds flush failed: %d!\n", iRet );
                break;
            }
        	
        } while(ERR_SUCCESS == iRet);
		
    }
    else
    {
        PRINT_CMD_USAGE( ent );
    }
    
    return TRUE;
}

static BOOL shell_VDS_Flush_All( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;
	UINT32 uUniLen = 0;
    UINT8 uFsType = 0;
    UINT8* pUniName = NULL;
    if (1 <= argc )
    {
        VDS_CacheFlushAlloc();
        iRet = VDS_FlushCacheAll();
        if ( ERR_SUCCESS != iRet )
        {
           dsm_Printf( BASE_FAT_TS_ID, "vds flush failed: %d!\n", iRet );
        }       
        VDS_CacheFlushAlloc();
    }
    else
    {
        PRINT_CMD_USAGE( ent );
    }
    
    return TRUE;
}
#endif

/*mount [FAT|FFS|MinixFS] <dev_name> <mount_point>*/
static BOOL shell_mount( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;
	UINT32 uUniLen = 0;
    UINT8 uFsType = 0;
    UINT8* pUniName = NULL;
    if ( 4 <= argc )
    {
		uFsType = (UINT8)(DSM_StrCmp(argv[1],"FAT") == 0 ? FS_TYPE_FAT: FS_TYPE_FFS);
        DSM_OEM2Uincode((UINT8*)argv[ 3 ], (UINT16)DSM_StrLen((UINT8*)argv[ 3 ]), &pUniName, &uUniLen,NULL);
        iRet = FS_Mount( (UINT8*)argv[ 2 ], pUniName, 0, uFsType );
        if ( ERR_SUCCESS != iRet )
        {
            dsm_Printf( BASE_FAT_TS_ID, "mount failed: %d!\n", iRet );
        }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
    }
    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
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
	UINT32 uUniLen = 0;
    UINT8* pUniName = NULL;
    BOOL bRet = FALSE;

    if ( argc < 2 )
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }

    if ( argc >= 3 )
    {
        bForce = DSM_StrAToI( argv[ 2 ] );
    }

    DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName, &uUniLen,NULL);
    iRet = FS_Unmount( pUniName, bForce );
    if ( ERR_SUCCESS != iRet )
    {
        dsm_Printf( BASE_FAT_TS_ID, "Unmount failed: [%d]!\n", iRet );
		bRet = FALSE;
    }
	else
	{
		bRet = TRUE;
	}

    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }
    return bRet;
}

static BOOL shell_FS_Find( struct cmd_entry *ent, int argc, char **argv )
{
    #define FS_SHELL_DIR_FILENAME_LEN 256
    FS_FIND_DATA FindFileData;
    INT32 fd = -1;
    long errCode;
    char filename[ FS_SHELL_DIR_FILENAME_LEN ] = { 0, };
    long filename_len;
    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    UINT8* pOemName = NULL;
    UINT32 uOemLen = 0;
    BOOL bRet = FALSE;
    DEMO_FILE_INFO* p = NULL; 
    
    if( 0 == DSM_StrCaselessCmp( "find", argv[0] ) && argc < 2)
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }
    
    if ( 0 == DSM_StrCaselessCmp( "ls", argv[0] ) || 0 == DSM_StrCaselessCmp( "dir", argv[0] ) )
    {
        if ( 1 == argc )
        {
            DSM_StrCpy( filename,  "./*" );
        }
        else
        {
            if('-' == argv[1][0])
            {
                DSM_StrCpy( filename,  "./*" );
            }
            else
            {
            filename_len = DSM_StrLen( argv[ 1 ] );
            if ( filename_len > FS_SHELL_DIR_FILENAME_LEN - 2 ) // 2 is for the length of "/*" 
            {
                dsm_Printf( BASE_FAT_TS_ID, "Invalid pathname: too long\n" );
                return FALSE;
            }

            DSM_StrCpy( filename, argv[1] );
            DSM_StrCat( filename, "/*" );
            } 
        }       
        DSM_OEM2Uincode((UINT8*)filename, (UINT16)DSM_StrLen(filename), &pUniName, &uUniLen,NULL);
    }
    else if (0 == DSM_StrCaselessCmp( "find", argv[0] ))
    {
         DSM_OEM2Uincode((UINT8*)argv[ 1], (UINT16)DSM_StrLen((UINT8*)argv[ 1]), &pUniName, &uUniLen,NULL);
    }
    else
    {
        dsm_Printf( BASE_FAT_TS_ID, "Invalid command.\n" );
        bRet = FALSE;
        goto func_end;
    }
    DSM_MemSet( FindFileData.st_name, NULL_CHAR, sizeof( FindFileData.st_name ) );

    // Find the first file.
    fd = FS_FindFirstFile( pUniName , &FindFileData );
    if ( fd < 0 ) 
    {
        if ( ERR_FS_NO_MORE_FILES == fd ) 
        {
            dsm_Printf( BASE_FAT_TS_ID, "No Matching entry is found\n" );
            bRet = TRUE;
        }
        else
        {
            dsm_Printf( BASE_FAT_TS_ID, "Find failed: [%d]\n", fd );
            bRet = FALSE;
        }
        
        goto func_end;
    }

    if(pOemName)
    {
           DSM_Free(pOemName);
           pOemName = NULL;
    }
    
    if ( !( FS_IS_DIR( FindFileData.st_mode ) && 
         ( 0 == DSM_StrCmp( ".", FindFileData.st_name ) || 0 == DSM_StrCmp( "..", FindFileData.st_name ))))
    {
       DSM_Unicode2OEM(FindFileData.st_name, DSM_UnicodeLen(FindFileData.st_name), &pOemName, &uOemLen,NULL);      
       if(!FS_IS_DIR( FindFileData.st_mode ))
       {
           
           if( 2 == argc && DSM_StrCaselessCmp(argv[1],"-l") == 0||
               3 == argc && DSM_StrCaselessCmp(argv[2],"-l") == 0)
           {
               dsm_Printf( BASE_FAT_TS_ID, "\t%s", pOemName);
               dsm_Printf( BASE_FAT_TS_ID, "\t%08d%s\n", 
                   FindFileData.st_size > 1024 ? FindFileData.st_size/1024 : FindFileData.st_size, 
                   FindFileData.st_size > 1024 ? "kb" : "byte");
           }
           else
           {
               dsm_Printf( BASE_FAT_TS_ID, "\t%s%s\n", pOemName, FS_IS_DIR( FindFileData.st_mode ) ? "/" : "" );
           }
       }
       else
       {
           dsm_Printf( BASE_FAT_TS_ID, "\t%s%s\n", pOemName, FS_IS_DIR( FindFileData.st_mode ) ? "/" : "" );
       }	
    }
	
	p = DSM_MAlloc(sizeof(DEMO_FILE_INFO));
       if(NULL == p)
       {
           DSM_ASSERT(0,"shell_FS_Find: 1.malloc(0x%x) failed.",sizeof(DEMO_FILE_INFO)); 
       }
	tcscpy((PCWSTR)p->filename,(PCWSTR)FindFileData.st_name);
	p->Attributes = FindFileData.st_mode;
	p->next = NULL;
	AppendFile(p);

    while ( ERR_SUCCESS == ( errCode = FS_FindNextFile( fd, &FindFileData ) ) ) 
    {
        if(pOemName)
        {
               DSM_Free(pOemName);
               pOemName = NULL;
        }
        if ( !( FS_IS_DIR( FindFileData.st_mode ) && 
             ( 0 == DSM_StrCmp( ".", FindFileData.st_name ) || 0 == DSM_StrCmp( "..", FindFileData.st_name ))))
        {
            DSM_Unicode2OEM(FindFileData.st_name, DSM_UnicodeLen(FindFileData.st_name), &pOemName, &uOemLen,NULL);
            if(!FS_IS_DIR( FindFileData.st_mode ))
            {
                
                if( 2 == argc && DSM_StrCaselessCmp(argv[1],"-l") == 0||
                    3 == argc && DSM_StrCaselessCmp(argv[2],"-l") == 0)
                {
                    dsm_Printf( BASE_FAT_TS_ID, "\t%s", pOemName);
                    dsm_Printf( BASE_FAT_TS_ID, "\t%08d%s\n", 
                        FindFileData.st_size > 1024 ? FindFileData.st_size/1024 : FindFileData.st_size, 
                        FindFileData.st_size > 1024 ? "kb" : "byte");
                }
                else
                {
                    dsm_Printf( BASE_FAT_TS_ID, "\t%s%s\n", pOemName, FS_IS_DIR( FindFileData.st_mode ) ? "/" : "" );
                }
            }
            else
            {
                dsm_Printf( BASE_FAT_TS_ID, "\t%s%s\n", pOemName, FS_IS_DIR( FindFileData.st_mode ) ? "/" : "" );
            }	
        }	
        /*
		p = DSM_MAlloc(sizeof(DEMO_FILE_INFO));
		if(p)
		{
        		tcscpy((PCWSTR)p->filename,(PCWSTR)FindFileData.st_name);
        		p->Attributes = FindFileData.st_mode;
        		p->next = NULL;
        		AppendFile(p);
    		}
        */
		
    }
    
//	SortByName_1(g_ffsim_sort_file_count);
//	PrintAllChain();
//	ReleaseAllChain();
    if ( ERR_FS_NO_MORE_FILES != errCode ) 
    {
        dsm_Printf( BASE_FAT_TS_ID, "ERROR[%d] in FindNextFile\n", errCode );
        FS_FindClose( fd );
        bRet = FALSE;
        goto func_end;
    }

    dsm_Printf( BASE_FAT_TS_ID, "\n" );
    FS_FindClose( fd );
    goto func_end;

func_end:
    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }
    if(pOemName)
    {
           DSM_Free(pOemName);
           pOemName = NULL;
    }
    return bRet;
}


static BOOL shell_FS_TUpdate( struct cmd_entry *ent, int argc, char **argv )
{   
    long errCode;
    BOOL bRet = FALSE;
    UINT8 filename[FS_SHELL_DIR_FILENAME_LEN]; 
    UINT32 filename_len;
    
    if(argc > 2)
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }
    
    if ( 0 == DSM_StrCaselessCmp( "tupdate", argv[0] ))
    {
        if ( 1 == argc )
        {
            DSM_StrCpy( filename,  "/t/updata.lod" );
        }
        else
        {
            filename_len = DSM_StrLen( argv[ 1 ] );
            if ( filename_len > FS_SHELL_DIR_FILENAME_LEN - 2 ) // 2 is for the length of "/*" 
            {
                dsm_Printf( BASE_FAT_TS_ID, "Invalid pathname: too long\n" );
                return FALSE;
            }

            DSM_StrCpy( filename, argv[1] );     
        }               
    }
    else
    {
        dsm_Printf( BASE_FAT_TS_ID, "Invalid command.\n" );
        bRet = FALSE;
        goto func_end;
    }

    // Find the first file.
    errCode = T_UP_Main(NULL);
    if ( errCode != 0 ) 
    {
        dsm_Printf( BASE_FAT_TS_ID, "Tupdate failed: [%d]\n", errCode );
        bRet = FALSE;   
        goto func_end;
    }
    goto func_end;

func_end:    
    return bRet;
}


static BOOL shell_FS_SearchFirst( struct cmd_entry *ent, int argc, char **argv )
{
    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    UINT8* pOemName = NULL;
    UINT32 uOemLen = 0;
    FS_FIND_DATA FindFileData;
    INT32 fd = -1;

    if( argc < 2 )
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }

    DSM_MemSet( FindFileData.st_name, NULL_CHAR, sizeof( FindFileData.st_name ) );
    DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName, &uUniLen,NULL);
    fd = FS_FindFirstFile( pUniName , &FindFileData );//使用FindFirstFile函数来开始文件查找

    if ( fd >= 0) 
    {
        dsm_Printf( BASE_FAT_TS_ID, "search descriptor[%d]\n", fd );
        
        DSM_Unicode2OEM(FindFileData.st_name, DSM_UnicodeLen(FindFileData.st_name), &pOemName, &uOemLen,NULL);
        dsm_Printf( BASE_FAT_TS_ID, "\t%s%s\n", pOemName, FS_IS_DIR( FindFileData.st_mode ) ? "/" : "" );
    }
    else
        dsm_Printf( BASE_FAT_TS_ID, "search file failed[%d]\n", fd );
    
    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }
    
    if(pOemName)
    {
           DSM_Free(pOemName);
           pOemName = NULL;
    }
    return TRUE;
}


static BOOL shell_FS_SearchNext( struct cmd_entry *ent, int argc, char **argv )
{
    UINT8* pOemName = NULL;
    UINT32 uOemLen = 0;
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
        DSM_Unicode2OEM(FindFileData.st_name, DSM_UnicodeLen(FindFileData.st_name), &pOemName, &uOemLen,NULL);       
        dsm_Printf( BASE_FAT_TS_ID, "\t%s%s\n", pOemName, FS_IS_DIR( FindFileData.st_mode ) ? "/" : "" );
    }

    if ( ERR_FS_NO_MORE_FILES != errCode ) 
    {       
        dsm_Printf( BASE_FAT_TS_ID, "ERROR[%d] in FindNextFile\n", errCode );
        if(pOemName)
        {
               DSM_Free(pOemName);
               pOemName = NULL;
        }
        return FALSE;
    }

    dsm_Printf( BASE_FAT_TS_ID, "No more file to search\n" );

    if(pOemName)
    {
           DSM_Free(pOemName);
           pOemName = NULL;
    }
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
char g_dsm_Prompt[ 255 + 1 ] = { '/', '>', };
INT32 FS_ChangeDir( PCSTR pszDirName );

static BOOL shell_FS_ChangeDir( struct cmd_entry *ent, int argc, char **argv )
{
  UINT8* pUniName = NULL;
  UINT32 uUniLen = 0;
  INT32 iRet;
  BOOL bRet = FALSE;
  
  if ( argc == 2 )
  {
        DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName, &uUniLen,NULL);    
        iRet = FS_ChangeDir( pUniName );
        if ( ERR_SUCCESS != iRet )
        {
            dsm_Printf( BASE_FAT_TS_ID, "Change directory failed!\n" );
            bRet = FALSE;
        }
        else
        {
             bRet = TRUE;
        }
  }
  else
  {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
  }


   if(NULL != pUniName)
   {
        DSM_Free(pUniName);
        pUniName = NULL;
   }
   return bRet;
}

static BOOL shell_FS_PWD( struct cmd_entry *ent, int argc, char **argv )
{
    
    UINT8* pOemName = NULL;
    UINT32 uOemLen = 0;
    BOOL bRet = FALSE;

	argv = argv;
	
    if ( argc == 1 )
    {
        FS_GetCurDir(SIZEOF(g_dsm_Prompt),(UINT8*)g_dsm_Prompt);
        DSM_Unicode2OEM((UINT8*)g_dsm_Prompt, (UINT16)DSM_UnicodeLen((UINT8*)g_dsm_Prompt), &pOemName, &uOemLen,NULL);
        dsm_Printf( BASE_FAT_TS_ID, "%s\n", pOemName) ;
        bRet = TRUE;
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        return FALSE;
    }

    if(NULL != pOemName)
    {
        DSM_Free(pOemName);
        pOemName = NULL;
    }
    return bRet;
}


static BOOL shell_FS_Open( struct cmd_entry *ent, int argc, char **argv )
{
    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    BOOL bRet = FALSE;
    INT32 fd;
    UINT32 flag = 0;
    UINT32 mode = 0; 
    char *p;

    if ( 2 <= argc )
    {
        // flag
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
            // mode 
            if(4 <= argc)
            {
                for ( p = argv[ 3 ]; *p; p++ )
                {
                    switch ( *p )
                    {
                        case 'A':
                        mode  |= FS_ATTR_ARCHIVE;
                        break;
                        case 'R':
                        mode  |= FS_ATTR_RO;
                        break;
                        case 'S':
                        mode  |= FS_ATTR_SYSTEM;
                        break;
                        case 'H':
                        mode  |= FS_ATTR_HIDDEN;
                        break;        
                        case '|':
                        break;          
                        default:
                        break;
                    }
                }
            }
            else
            {
                mode = 0;
            }
        }
        else
        {
            flag = FS_O_RDWR;
        }

        DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName, &uUniLen,NULL);    

        fd = FS_Open( pUniName, flag, mode );
        if ( fd >= 0 )
        {
            dsm_Printf( BASE_FAT_TS_ID, "open file with fd[%d] returned\n", fd );
            bRet = TRUE;
        }
        else
        {
            dsm_Printf( BASE_FAT_TS_ID, "open file failed! ErrorCode = %d\n",fd );
            bRet = FALSE;
        }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
    }

    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }
    return bRet;
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
    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    UINT32 mode = 0;
    UINT8* p;
    BOOL bRet = FALSE;       
  
    
    if ( 2 <= argc )
    {
       // mode 
        if(3 <= argc)
        {
            for ( p = argv[2]; *p; p++ )
            {
                switch ( *p )
                {
                    case 'A':
                    mode  |= FS_ATTR_ARCHIVE;
                    break;
                    case 'R':
                    mode  |= FS_ATTR_RO;
                    break;
                    case 'S':
                    mode  |= FS_ATTR_SYSTEM;
                    break;
                    case 'H':
                    mode  |= FS_ATTR_HIDDEN;
                    break;        
                    case '|':
                    break;          
                    default:
                    break;
                }
            }
        }
        else
        {
            mode = 0;
        }
        DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName, &uUniLen,NULL); 
        fd = FS_Create( pUniName, mode );
        if ( fd >= 0 )
        {
            dsm_Printf( BASE_FAT_TS_ID, "created file with fd[%d] returned. The file has been truncated to zero.\n", fd );
            bRet = TRUE;
        }
        else
        {
            dsm_Printf( BASE_FAT_TS_ID, "create file failed!\n" );
            bRet = FALSE;
        }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
    }
  
    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }
    return bRet;  
}

static BOOL shell_FS_CreClo( struct cmd_entry *ent, int argc, char **argv )
{
  
    INT32 fd;
    INT32 iRet;
    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    BOOL bRet = FALSE;
       
    if ( 2 == argc )
    {
        DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName, &uUniLen,NULL);   
        fd = FS_Create( pUniName, 0x0 );
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
    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }
    return bRet;
}

static BOOL shell_FS_DeleteFile( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;
    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    BOOL bRet = FALSE;
        
    if ( 2 == argc )
    {
        DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName, &uUniLen,NULL);
        iRet = FS_Delete( pUniName);
        if ( ERR_SUCCESS != iRet )
        {
            dsm_Printf( BASE_FAT_TS_ID, "Delete file failed! error[%d]\n", iRet );
        }
        bRet = ( ERR_SUCCESS == iRet ) ? TRUE : FALSE;
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
    }
    
    if(NULL != pUniName)
    {
    DSM_Free(pUniName);
    pUniName = NULL;
    }
    return bRet;
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
    DSM_MemSet(g_shell_buffer, 0x00, VDS_SHELL_BUFFER_LEN_MAX + LEN_FOR_NULL_CHAR);
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

/*
#ifndef FS_PRIVATE_MEM_MANAGE
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
*/
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
    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    BOOL bRet = FALSE;

     
    if ( 3 <= argc )
    {
        DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName, &uUniLen,NULL);
        fd = FS_Create( pUniName, 0x0 );
        if( fd < 0 )
        {
            dsm_Printf( BASE_FAT_TS_ID, "open/create file failed" );
            bRet = FALSE;
            goto func_end;
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
            bRet = FALSE;
            goto func_end;
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
            DSM_MemSet( g_lwrite_buf, 'A' + ( ( ( ++c ) - 'A' ) % 26 ), sizeof( g_lwrite_buf ) );

#ifdef _MS_VC_VER_
            if ( is_rand ) chunk_size = rand() % sizeof( g_lwrite_buf );
#endif
        }

        FS_Close( fd );
#ifdef FS_PRIVATE_MEM_MANAGE
        get_mega_kilo_byte( nwritten, &mega, &kilo, &byte );
#endif
        dsm_Printf( BASE_FAT_TS_ID, "write %d[0x%x] ( %dM %dK %dB)bytes to the file %s\n", 
               nwritten, nwritten, mega, kilo, byte, argv[ 1 ] );
        bRet = TRUE;
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
    }
func_end:
    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }
    return bRet;
    
}


static BOOL shell_FS_MakeDir( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;
    //UINT8 name[] = {0x28,0x06, 0x0, 0x0};
    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    BOOL bRet = FALSE;
    UINT32 mode = 0;
    UINT8* p;

    if ( 2 <= argc )
    {
        DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName, &uUniLen,NULL);
        // mode 
        if(3 <= argc)
        {
            for ( p = argv[2]; *p; p++ )
            {
                switch ( *p )
                {
                    case 'A':
                    mode  |= FS_ATTR_ARCHIVE;
                    break;
                    case 'R':
                    mode  |= FS_ATTR_RO;
                    break;
                    case 'S':
                    mode  |= FS_ATTR_SYSTEM;
                    break;
                    case 'H':
                    mode  |= FS_ATTR_HIDDEN;
                    break;        
                    case '|':
                    break;          
                    default:
                    break;
                }
            }
        }
        else
        {
            mode = 0;
        }
        iRet = FS_MakeDir(pUniName, mode);
        if ( ERR_SUCCESS != iRet )
        {
            dsm_Printf( BASE_FAT_TS_ID, "Make direcotry failed!,iResult[%d]\n",iRet );
            bRet = FALSE;
        }
        else
        {
            bRet = TRUE;
        }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
    }
    
    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }
    return bRet;
}

static BOOL shell_FS_RemoveDir( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;
    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    BOOL bRet = FALSE;

    if ( 2 == argc )
    {
        DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName, &uUniLen,NULL);
        iRet = FS_RemoveDir( pUniName );
        dsm_Printf( BASE_FAT_TS_ID, "%s\n", ( ERR_SUCCESS == iRet ) ? "Remove direcotry OK" : "Remove direcotry failed!" );
        if(ERR_SUCCESS != iRet)
        {
            dsm_Printf( BASE_FAT_TS_ID,"errorcode =  %d\n", iRet);
            bRet = FALSE;
        }
        else
        {
            bRet =  TRUE;
        }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet =  FALSE;
    }

    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }
    return bRet;    
}

static BOOL shell_FS_Rename( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 iRet;
    UINT8* pUniName1 = NULL;
    UINT32 uUniLen1 = 0;
    UINT8* pUniName2 = NULL;
    UINT32 uUniLen2 = 0;
    BOOL bRet = FALSE;


    if ( 3 == argc )
    {
        DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName1, &uUniLen1,NULL);
        DSM_OEM2Uincode((UINT8*)argv[ 2 ], (UINT16)DSM_StrLen((UINT8*)argv[ 2 ]), &pUniName2, &uUniLen2,NULL);
        iRet = FS_Rename(pUniName1,pUniName2 );
        if ( ERR_SUCCESS != iRet)
        {
            dsm_Printf( BASE_FAT_TS_ID, "Rename failed[%d]!\n", iRet );
            bRet = FALSE;
        }
        else
        {
            bRet = TRUE;
        }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
    }
    
    if(NULL != pUniName1)
    {
        DSM_Free(pUniName1);
        pUniName1 = NULL;
    }
    
    if(NULL != pUniName2)
    {
        DSM_Free(pUniName2);
        pUniName2 = NULL;
    }   
    return bRet;
}


/*BPB: BIOS Parameter Block*/
VOID fat_dump_bpb( PCSTR pszDevName )
{
    FAT_BOOT_RECORD boot_rec;
    //UINT8* secbuf;
    UINT32 uDevNo;
    INT32 iRet;
    
	uDevNo = DSM_DevName2DevNo(pszDevName);
	if(INVALID_DEVICE_NUMBER == uDevNo)
	{
        dsm_Printf( BASE_FAT_TS_ID, "Invalid device name" );
        return;
	}

    iRet = DRV_BLOCK_READ( uDevNo, 0, (UINT8*)g_shell_buffer );
    if(_ERR_FAT_SUCCESS == iRet)
    {
        Buf2FBR((UINT8*)g_shell_buffer, &boot_rec);
            
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

VOID fat_dump_file( PCSTR pszResName, PCSTR pszDestName)
{

    UINT8* pUniName = NULL;
    UINT32 iUniLen = 0;
    INT32 iRet;
    INT32 fd_res = -1;
    FILE* fp_dest = NULL;
    //UINT8* dest_file = "";
    UINT8 szBuff[512];
	UINT32 r_size = 0;
	UINT32 w_size = 0;

    iRet = DSM_OEM2Uincode(pszResName,DSM_StrLen(pszResName),&pUniName,&iUniLen,NULL);

    if(iRet != 0)
    {
       dsm_Printf( BASE_FAT_TS_ID, "to unicode failed.err_code = %d.",iRet );
       goto dump_file_end;
    }

    fd_res = FS_Open(pUniName, FS_O_RDONLY,0);
    if(fd_res < 0)
    {
       dsm_Printf( BASE_FAT_TS_ID, "open res file(%s) failed.err_code = %d.",pszResName,fd_res ); 
	   goto dump_file_end;
    }

    fp_dest = fopen(pszDestName, "w+");
    if(fp_dest == NULL)
    {
       dsm_Printf( BASE_FAT_TS_ID, "open res file(%s) failed.",pszDestName); 
	   goto dump_file_end;
    }
    do
	{
        r_size = FS_Read(fd_res,szBuff,512);
		if(r_size > 0)
		{
			w_size = fwrite(szBuff,1,r_size,fp_dest);
			if(w_size != r_size)
			{
				goto dump_file_end;
			}
		}
    } while(r_size > 0);
    


dump_file_end:
    if(pUniName)
	{
		DSM_Free(pUniName);
		pUniName = NULL;
	}
	if(fd_res >= 0)
	{
		FS_Close(fd_res);
		fd_res = -1;
	}
     
	if(fp_dest >= 0)
	{
		fclose(fp_dest);
		fp_dest = NULL;
	}
	
    return;
    
}


/*BPB: BIOS Parameter Block*/
VOID fat_dump_dir( PCSTR pszResName, PCSTR pszDestName)
{

    UINT8* pUniName = NULL;
    UINT32 iUniLen = 0;
    INT32 iRet;
    INT32 fd_res = -1;
    FILE* fp_dest = NULL;
    //UINT8* dest_file = "";
    UINT8 szBuff[512];
	UINT32 r_size = 0;
	UINT32 w_size = 0;

    iRet = DSM_OEM2Uincode(pszResName,DSM_StrLen(pszResName),&pUniName,&iUniLen,NULL);

    if(iRet != 0)
    {
       dsm_Printf( BASE_FAT_TS_ID, "to unicode failed.err_code = %d.",iRet );
       goto dump_file_end;
    }

    fd_res = FS_Open(pUniName, FS_O_RDONLY,0);
    if(fd_res < 0)
    {
       dsm_Printf( BASE_FAT_TS_ID, "open res file(%s) failed.err_code = %d.",pszResName,fd_res ); 
	   goto dump_file_end;
    }

    fp_dest = fopen(pszDestName, "w+");
    if(fp_dest == NULL)
    {
       dsm_Printf( BASE_FAT_TS_ID, "open res file(%s) failed.",pszDestName); 
	   goto dump_file_end;
    }
    do
	{
        r_size = FS_Read(fd_res,szBuff,512);
		if(r_size > 0)
		{
			w_size = fwrite(szBuff,1,r_size,fp_dest);
			if(w_size != r_size)
			{
				goto dump_file_end;
			}
		}
    } while(r_size > 0);
    


dump_file_end:
    if(pUniName)
	{
		DSM_Free(pUniName);
		pUniName = NULL;
	}
	if(fd_res >= 0)
	{
		FS_Close(fd_res);
		fd_res = -1;
	}
     
	if(fp_dest >= 0)
	{
		fclose(fp_dest);
		fp_dest = NULL;
	}
	
    return;
    
}
static BOOL shell_FS_FAT_Dump( struct cmd_entry *ent, int argc, char **argv )
{
    INT32 counter;
    INT32 iRet;
	UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
	BOOL bRet = FALSE;

    if ( argc >= 3 )
    {
        if ( 0 == DSM_StrCaselessCmp("fatlist", argv[1] ) )
        {
			DSM_OEM2Uincode((UINT8*)argv[ 2 ], (UINT16)DSM_StrLen((UINT8*)argv[ 2 ]), &pUniName, &uUniLen,NULL);
            iRet = fat_dump_fat_list( (UINT8*)pUniName );
			if(ERR_SUCCESS == iRet)
			{
				bRet = TRUE;
			}
			else
			{
				bRet = FALSE;
			}
			if(NULL != pUniName)
			{
				DSM_Free(pUniName);
				pUniName = NULL;
			}
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
			if(ERR_SUCCESS == iRet)
			{
				bRet = TRUE;
			}
			else
			{
				bRet = FALSE;
			}
        }
        else if ( 0 == DSM_StrCaselessCmp("bpb", argv[1] ) )
        {
            fat_dump_bpb((PCSTR)argv[2]);			
			bRet = TRUE;			
        }
        else if ( 0 == DSM_StrCaselessCmp("file", argv[1] ) )
        {
			if(argc >= 4)
			{
               fat_dump_file((PCSTR)argv[2],(PCSTR)argv[3]);	
			   bRet = TRUE;
			}
			else if(argc >= 3)
			{
               fat_dump_file((PCSTR)argv[2],(PCSTR)argv[2]);	
			   bRet = TRUE;
			}
            			
        }
        else
        {
            return FALSE;
        }        
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
    }
	return bRet;
}
static BOOL shell_FS_GetFSInfo(struct cmd_entry *ent, int argc, char **argv)
{
    INT32 iRet;
    FS_INFO sFSInfo;
	UINT8 szUsed[32];
	UINT8 szTotal[32];

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
			ui64toa(sFSInfo.iTotalSize,szTotal,10); 
			ui64toa(sFSInfo.iUsedSize,szUsed,10);
            dsm_Printf( BASE_FAT_TS_ID, "total_size = %s,used size = %s.",
				szTotal,
				szUsed);
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
    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    BOOL bRet = FALSE;
	

    if(argc == 2)
    {
     	DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName, &uUniLen,NULL);
        iRet = FS_GetFileAttr( pUniName,&sFileAttr );
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
            
            bRet = TRUE;
        }
        else
        {
            dsm_Printf(BASE_FAT_TS_ID,"FS_GetFileAttr Failed\n");
            bRet = FALSE;
        }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
    }

	if(NULL != pUniName)
	{
		DSM_Free(pUniName);
		pUniName = NULL;
	}
	
	return bRet;
}


static BOOL shell_FS_SetFileAttr(struct cmd_entry *ent, int argc, char **argv)
{   
    INT32 iRet = ERR_SUCCESS;
    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    BOOL bRet = FALSE;
    UINT32 mode = 0;
    UINT8* p;
	

    if(argc >= 2)
    {
         // mode 
        if(3 <= argc)
        {
            for ( p = argv[2]; *p; p++ )
            {
                switch ( *p )
                {
                    case 'A':
                    mode  |= FS_ATTR_ARCHIVE;
                    break;
                    case 'R':
                    mode  |= FS_ATTR_RO;
                    break;
                    case 'S':
                    mode  |= FS_ATTR_SYSTEM;
                    break;
                    case 'H':
                    mode  |= FS_ATTR_HIDDEN;
                    break;        
                    case '|':
                    break;          
                    default:
                    break;
                }
            }
        }
        else
        {
            mode = 0;
        }
     	 DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName, &uUniLen,NULL);
        iRet = FS_SetFileAttr( pUniName,mode);
        if( iRet == ERR_SUCCESS )
        {                     
            bRet = TRUE;
        }
        else
        {
            dsm_Printf(BASE_FAT_TS_ID,"FS_SetFileAttr Failed\n");
            bRet = FALSE;
        }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
    }

	if(NULL != pUniName)
	{
		DSM_Free(pUniName);
		pUniName = NULL;
	}
	
	return bRet;
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
    UINT8* pOemName = NULL;
    UINT32 uOemLen = 0;
    BOOL bRet = FALSE;

    if ( 2 <= argc )
    {
        fd = DSM_StrAToI( argv[ 1 ] );
        iRet = FS_GetFileName( fd, LEN_FOR_OEM_UNI_BUFFER, fileName);
        if ( ERR_SUCCESS == iRet )
        {
            DSM_Unicode2OEM((UINT8*)fileName, (UINT16)DSM_UnicodeLen((UINT8*)fileName), &pOemName, &uOemLen,NULL);
            dsm_Printf( BASE_FAT_TS_ID, "%s\n", pOemName);
            bRet = TRUE;
        }
        else
        {
            dsm_Printf( BASE_FAT_TS_ID, "FS_GetFileName failed!errirCode = %d. \n", iRet);
            bRet = FALSE;
        }
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
    }

    if(NULL != pOemName)
    {
        DSM_Free(pOemName);
        pOemName = NULL;
    }
    return bRet;
}


static BOOL shell_FS_ChangeSize( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 fd;
  UINT32 nFileSize;
  INT32 iRet;
 
  if (argc >= 3)
  {
    fd = DSM_StrAToI( argv[ 1 ] );
    nFileSize = DSM_StrAToI( argv[ 2 ] );
    iRet = FS_ChangeSize( fd, nFileSize);
    if ( iRet >= 0 )
    {
        return TRUE;
    }
    else
    {
      dsm_Printf( BASE_FAT_TS_ID, "FS_ChangeSize failed!errirCode = %d. \n", iRet);
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
	BOOL bCheat = TRUE;

    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    UINT8* pOemName = NULL;
    UINT32 uOemLen = 0;


    DSM_Unicode2OEM((UINT8*)g_dsm_Prompt, (UINT16)DSM_UnicodeLen((UINT8*)g_dsm_Prompt), &pOemName, &uOemLen,NULL);
    DSM_StrCpy(pcPath, pOemName);

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
        while (('/' == (c = *(p - 1)) || '\\' == c ) && p > pcPath)
        {
            p--;
            *p = 0;
        }
    }
    DSM_StrNCpy(p, "/", 1);
    p++;
    iNameLen = DSM_StrLen(pcDirName);
    DSM_StrNCpy(p, pcDirName, iNameLen);
    p += iNameLen;
    pOriginal = p;


    for (i = 0; i < NR_MAX_DEPTH; ++i)
    {
        iFindFd[i] = INVALID_FD;
    }

    while (TRUE == bCheat)
    {
        if (INVALID_FD == iFindFd[iDepth])
        {
            if ((p + DSM_StrLen(str)) >= (pcPath + LEN_PATH_BUF))
            {
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            DSM_StrCpy(p, str);
            p++;

            DSM_OEM2Uincode((UINT8*)pcPath, (UINT16)DSM_StrLen((UINT8*)pcPath), &pUniName, &uUniLen,NULL);
            iFindFd[iDepth] = FS_FindFirstFile(pUniName, &sFindData);
            if (ERR_FS_NO_MORE_FILES == iFindFd[iDepth])
            {
                ; // 不可能的情况。
            }
            else if (iFindFd[iDepth] < 0)
            {
                CSW_TRACE(BASE_FFS_TS_ID, "[#error!!]: FS_FindFirstFile return = %d.\n", iFindFd[iDepth]);
                iRet = iFindFd[iDepth];
                iDepth--;
                goto failed;
            }
            if(NULL != pOemName)
            {
               DSM_Free(pOemName);
               pOemName = NULL;
            }
             DSM_Unicode2OEM((UINT8*)sFindData.st_name, (UINT16)DSM_UnicodeLen((UINT8*)sFindData.st_name), &pOemName, &uOemLen,NULL);
            if ((p + DSM_StrLen(pOemName)) >= (pcPath + LEN_PATH_BUF))
            {
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            DSM_StrCpy(p, pOemName);

            if(NULL != pUniName)
            {
                DSM_Free(pUniName);
                pUniName = NULL;
            }
            DSM_OEM2Uincode((UINT8*)pcPath, (UINT16)DSM_StrLen((UINT8*)pcPath), &pUniName, &uUniLen,NULL);
                     
            if (!FS_IS_DIR( sFindData.st_mode ))
            {
                if ( ERR_SUCCESS != (iRet = FS_Delete(pUniName)) )
                {
                    CSW_TRACE(BASE_FFS_TS_ID, "[#error!!]: FS_Delete 1 return = %d.\n", iRet);
                    goto failed;
                }
            }
            else if (FS_IS_DIR( sFindData.st_mode ) && '.' == sFindData.st_name[0] 
                        && 0 == sFindData.st_name[1])
            {
                ;
            }
            else
            {
                iRet = FS_RemoveDir(pUniName);
                if (ERR_SUCCESS == iRet)
                {
                    ;
                }
                else if (iRet != ERR_FS_DIR_NOT_EMPTY)
                {
                    CSW_TRACE(BASE_FFS_TS_ID, "[#error!!]: FS_RemoveDir 1 return = %d.\n", iRet);
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
            if(NULL != pOemName)
            {
                DSM_Free(pOemName);
                pOemName = NULL;
            }
            DSM_Unicode2OEM((UINT8*)sFindData.st_name, (UINT16)DSM_UnicodeLen((UINT8*)sFindData.st_name), &pOemName, &uOemLen,NULL);            
            if ((p + DSM_StrLen(pOemName)) >= (pcPath + LEN_PATH_BUF))
            {
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            DSM_StrCpy(p, pOemName);

            if(NULL != pUniName)
            {
                DSM_Free(pUniName);
                pUniName = NULL;
            }
            DSM_OEM2Uincode((UINT8*)pcPath, (UINT16)DSM_StrLen((UINT8*)pcPath), &pUniName, &uUniLen,NULL);

            // Delete file.
            if (!FS_IS_DIR( sFindData.st_mode ))
            {
                if ( ERR_SUCCESS != (iRet = FS_Delete(pUniName)))
                {
                    CSW_TRACE(BASE_FFS_TS_ID, "[#error!!]: FS_Delete 2 return = %d.\n", iRet);
                    goto failed;
                }
            }
            else if ( '.' == sFindData.st_name[0] &&
                         0 == sFindData.st_name[1])
            {
                // Do nothing.            
            }
            else
            {
                iRet = FS_RemoveDir(pUniName);
                if (ERR_SUCCESS == iRet)
                {
                    // Do nothing.
                }
                else if (iRet != ERR_FS_DIR_NOT_EMPTY)
                {
                    CSW_TRACE(BASE_FFS_TS_ID, "[#error!!]: FS_RemoveDir 2 return = %d.\n", iRet);
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
        *p = 0;
        
        if(NULL != pUniName)
        {
            DSM_Free(pUniName);
            pUniName = NULL;
        }
        DSM_OEM2Uincode((UINT8*)pcPath, (UINT16)DSM_StrLen((UINT8*)pcPath), &pUniName, &uUniLen,NULL);
        
        if ((iRet = FS_RemoveDir(pUniName)) != ERR_SUCCESS)
        {
            CSW_TRACE(BASE_FFS_TS_ID, "[#error!!]: FS_RemoveDir333 return = %d.\n", iRet);
            goto failed;
        }
        if (p == pOriginal)
        {
            if (iDepth != -1)
            {
                CSW_TRACE(BASE_FFS_TS_ID, "[#error!!]: p == pOriginal && iDepth != 0.\n");
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            break;
        }
        
        while ((c = *(p - 1)) != '/' && c != '\\')
        {// 返回上级目录继续搜索。
            p--;
        }
        *p = 0;
        continue;
        
dir_not_empty:    
        if(NULL != pOemName)
        {
            DSM_Free(pOemName);
            pOemName = NULL;
        }
        DSM_Unicode2OEM((UINT8*)sFindData.st_name, (UINT16)DSM_UnicodeLen((UINT8*)sFindData.st_name), &pOemName, &uOemLen,NULL);
        if ((NR_MAX_DEPTH - 1) == iDepth)
        {
            // 如果栈满则退出。
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }      
        iNameLen = DSM_StrLen(pOemName);
        p += iNameLen;
        iDepth++;
    }

    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }  

    if(NULL != pOemName)
    {
        DSM_Free(pOemName);
        pOemName = NULL;
    }
    return ERR_SUCCESS;

failed:
    CSW_TRACE(BASE_FFS_TS_ID, "[#error!!]: failed+++++ iFindFd[%d] = %d \n [pcPath]:\"%s\".\n", iDepth, iFindFd[iDepth], pcPath);
    for (i = iDepth; i >= 0; --i)
    {
        FS_FindClose(iFindFd[i]);
    }
    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }

    if(NULL != pOemName)
    {
        DSM_Free(pOemName);
        pOemName = NULL;
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
    UINT8* pUniName = NULL;
    UINT32 uUniLen = 0;
    UINT8* pOemName = NULL;
    UINT32 uOemLen = 0;
    BOOL bCheat = TRUE;

    DSM_Unicode2OEM((UINT8*)g_dsm_Prompt, (UINT16)DSM_UnicodeLen((UINT8*)g_dsm_Prompt), &pOemName, &uOemLen,NULL); 
    DSM_StrCpy(pcPath, pOemName);

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
        while (('/' == (c = *(p - 1)) || '\\' == c) && p > pcPath)
        {
            p--;
            *p = 0;
        }
    }
    DSM_StrNCpy(p, "/", 1);
    p++;
    iNameLen = DSM_StrLen(pcDirName);
    DSM_StrNCpy(p, pcDirName, iNameLen);
    p += iNameLen;
    pOriginal = p;

    for (i = 0; i < NR_MAX_DEPTH; ++i)
    {
        iFindFd[i] = INVALID_FD;
    }

    while (TRUE == bCheat)
    {
        if (INVALID_FD == iFindFd[iDepth])
        {
            if ((p + DSM_StrLen(str)) >= (pcPath + LEN_PATH_BUF))
            {
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            DSM_StrCpy(p, str);
            p++;
            if(NULL != pUniName)
            {
               DSM_Free(pUniName);
               pUniName = NULL;
            }
            DSM_OEM2Uincode((UINT8*)pcPath, (UINT16)DSM_StrLen((UINT8*)pcPath), &pUniName, &uUniLen,NULL);
            iFindFd[iDepth] = FS_FindFirstFile(pUniName, &sFindData);
            if (ERR_FS_NO_MORE_FILES == iFindFd[iDepth])
            {
                goto current_dir_empty;
            }
            else if (iFindFd[iDepth] < 0)
            {
                CSW_TRACE(BASE_FFS_TS_ID, "[#error!!]: FS_FindFirstFile return = %d.\n", iFindFd[iDepth]);
                iDepth--;
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
                         
            if (!FS_IS_DIR( sFindData.st_mode ))
            {
                if ( Flag & FS_FILE_TYPE )
                {
                    iFileNum++;
                }
            }
            else
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
                        DSM_StrCpy(p, sFindData.st_name);
                        goto recursive_dir;
                    }
                }
            }
        }

        while ( ERR_SUCCESS == ( iRet = FS_FindNextFile( iFindFd[iDepth], &sFindData ) ) ) 
        {
            if (!FS_IS_DIR( sFindData.st_mode ))
            {
                if ( Flag & FS_FILE_TYPE )
                {
                    iFileNum++;
                }
            }
            else
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
                        if(NULL != pOemName)
                        {
                            DSM_Free(pOemName);
                            pOemName = NULL;
                        }
                        DSM_Unicode2OEM((UINT8*)sFindData.st_name, (UINT16)DSM_UnicodeLen((UINT8*)sFindData.st_name), &pOemName, &uOemLen,NULL);
                        if ((p + DSM_StrLen(pOemName)) >= (pcPath + LEN_PATH_BUF))
                        {
                            iRet = FS_ERROR_RESERVED;
                            goto failed;
                        }
                        DSM_StrCpy(p, pOemName);
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
        *p = 0;
        
        if (p == pOriginal)
        {
            if (iDepth != -1)
            {
                CSW_TRACE(BASE_FFS_TS_ID, "[#error!!]: p == pOriginal && iDepth != 0.\n");
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            break;
        }

        while ((c = *(p - 1)) != '/' && c != '\\')
        {// 返回上级目录继续统计。
            p--;
        }
        *p = 0;
        continue;
        
recursive_dir:    // 压栈，进行统计子目录。

         if(NULL != pOemName)
         {
            DSM_Free(pOemName);
            pOemName = NULL;
         }
        DSM_Unicode2OEM((UINT8*)sFindData.st_name, (UINT16)DSM_UnicodeLen((UINT8*)sFindData.st_name), &pOemName, &uOemLen,NULL);
        
        if ((NR_MAX_DEPTH - 1) == iDepth)
        {// 如果栈满则退出。
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }      
        iNameLen = DSM_StrLen(pOemName);
        p += iNameLen;
        iDepth++;
    }

    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }
    if(NULL != pOemName)
    {
        DSM_Free(pOemName);
        pOemName = NULL;
    }
    return iFileNum;

failed:

    CSW_TRACE(BASE_FFS_TS_ID, "+++++failed !!!+++++.\n", iRet);
    for (i = iDepth; i >= 0; --i)
    {
        CSW_TRACE(BASE_FFS_TS_ID, "+++++FS_FindClose: iFindFd[%d] = %d +++++.\n",i, iFindFd[i]);
        FS_FindClose(iFindFd[i]);
    }

    if(NULL != pUniName)
    {
        DSM_Free(pUniName);
        pUniName = NULL;
    }
    
    if(NULL != pOemName)
    {
        DSM_Free(pOemName);
        pOemName = NULL;
    }
    return iRet;
        
}


static BOOL shell_DropDirTree( struct cmd_entry *ent, int argc, char **argv )
{
  INT32 iRet;
   
  if ( 2 <= argc )
  {
    iRet = FS_DropDirTree((const UINT8*)argv[ 1 ]);
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
    
    iRet = FS_Count((const UINT8*)argv[ 1 ], flag);
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
INT32 UniStrCmpi(const UINT16 *str1, const UINT16 *str2)
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
    UINT8* pUniName1 = NULL;
    UINT32 uUniLen1 = 0;
    UINT8* pUniName2 = NULL;
    UINT32 uUniLen2 = 0;
    BOOL bRet = FALSE;

    if ( argc >= 3)
    {
        DSM_OEM2Uincode((UINT8*)argv[ 1 ], (UINT16)DSM_StrLen((UINT8*)argv[ 1 ]), &pUniName1, &uUniLen1,NULL);

        DSM_OEM2Uincode((UINT8*)argv[ 2 ], (UINT16)DSM_StrLen((UINT8*)argv[ 2 ]), &pUniName2, &uUniLen2,NULL);

        iRet = UniStrCmpi( (UINT16*)pUniName1, (UINT16*)pUniName2);
        dsm_Printf( BASE_FAT_TS_ID, "\t%d. \n", iRet);
        bRet = TRUE;
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
    }

    if(NULL != pUniName1)
    {
        DSM_Free(pUniName1);
        pUniName1 = NULL;
    }
    
    if(NULL != pUniName2)
    {
        DSM_Free(pUniName2);
        pUniName2 = NULL;
    }

    return bRet;
}



static BOOL shell_FS_GetDevInfo( struct cmd_entry *ent, int argc, char **argv )
{
    UINT32 i;
    INT32 iRet;
    UINT32 uDeviceCount = 0;
    FS_DEV_INFO *pFSDevInfo = NULL;
    BOOL bRet;
    
	argv = argv;
    if(argc > 1)
    {
          dsm_Printf( BASE_FAT_TS_ID, "The devinfo command none parameter.\n");
          return FALSE;
    }
    iRet = FS_GetDeviceInfo(&uDeviceCount, &pFSDevInfo);
    if(ERR_SUCCESS == iRet)
    {
        for(i = 0; i < uDeviceCount; i++)
        {
            dsm_Printf( BASE_FAT_TS_ID, "name = %s,type = %d,is_root = %d.\n",
                pFSDevInfo[i].dev_name,
                pFSDevInfo[i].dev_type,
                pFSDevInfo[i].is_root
                );
        }
        bRet = TRUE;
    }
    else
    {
        PRINT_CMD_USAGE( ent );
        bRet = FALSE;
    }
    return bRet;
}


INT32 fat_dump_fat_list( PCSTR pathname )
{
    struct inode *inode;
    FAT_SB_INFO* sb;
    UINT32 clus_nr;
    UINT32 next_clus_nr;
    INT32 counter = 0;
    BOOL is_last_cluster;

    inode = namei((PCWSTR) pathname );
    if( !inode )
    {
        return ERR_FS_NO_DIR_ENTRY;
    }

    sb = &(inode->i_sb->u.fat_sb);
    clus_nr = inode->u.fat_i.entry.DIR_FstClusLO;

    is_last_cluster = fat_is_last_cluster(sb, clus_nr);
    while(!is_last_cluster)
    {
        
        fat_get_next_cluster(sb, clus_nr, &next_clus_nr);
        is_last_cluster = fat_is_last_cluster(sb, next_clus_nr);

        dsm_Printf( BASE_DSM_TS_ID, "%3d%s", clus_nr, !is_last_cluster ? "-->" : ""  );
        clus_nr = next_clus_nr;
        counter++;
        
        if ( 0 == counter % 8 )
        {
            dsm_Printf( BASE_DSM_TS_ID, "\n" );
        }
    }
    dsm_Printf( BASE_DSM_TS_ID, "\n" );

    iput( inode );
    return ERR_SUCCESS;
}


INT32 fat_dump_fat(UINT32 begin_clus_nr, UINT32 counter )
{
    struct inode *root_i;
    FAT_SB_INFO *sb_info;
    UINT32 entry_ofs_in_fat;
    UINT32 sec_nr;
    UINT32 entry_ofs_in_sec = 0, old_sec_nr = 0;
    UINT32 clus_nr = 0;
    UINT8*  secbuf = NULL;
    UINT32 i;
    INT32 iResult = _ERR_FAT_SUCCESS;
    INT32 iRet = _ERR_FAT_SUCCESS;

    root_i = namei( (PCWSTR)"/" );
    if (!root_i)
    {
        return ERR_FS_INVALID_DIR_ENTRY;
    }

    sb_info = &(root_i->i_sb->u.fat_sb);
    iput(root_i);
    
    secbuf = FAT_SECT_BUF_ALLOC();   
    if(NULL == secbuf)
    {
        D(( DL_FATERROR,"in fat_dump_fat,1.FAT_SECT_BUF_ALLOC() failed.")); 
        DSM_ASSERT(0,"in fat_dump_fat,1.FAT_SECT_BUF_ALLOC() failed.");
        return _ERR_FAT_MALLOC_FAILED;
    }      
    i = begin_clus_nr;
        
    dsm_Printf( BASE_DSM_TS_ID, "\n%2x: ", 0 );
    if(sb_info->iFATn == FAT12)
    {
        do 
        {
            entry_ofs_in_fat = i + (i / 2);// 根据cluster number算出它在FAT中的字节偏移量
            sec_nr = sb_info->iFATStartSec  + (entry_ofs_in_fat / sb_info->iBytesPerSec);
            entry_ofs_in_sec = entry_ofs_in_fat % sb_info->iBytesPerSec;

            if(old_sec_nr < sec_nr) 
            {
                DSM_MemSet(secbuf, 0x00, DEFAULT_SECSIZE);
                DSM_MemSet(secbuf+DEFAULT_SECSIZE, 0x00, DEFAULT_SECSIZE);

                iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, sec_nr, secbuf );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    D( ( DL_FATERROR, "In fat_dump_fat DRV_BLOCK_READ_1 failed iResult = %d\n",iResult));
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    goto end;
                }

                iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, sec_nr + 1, secbuf+DEFAULT_SECSIZE );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    D( ( DL_FATERROR, "In fat_dump_fat DRV_BLOCK_READ_2 failed iResult = %d\n",iResult));
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    goto end;
                }
                
                old_sec_nr = sec_nr;
            }
            
            clus_nr = MAKEFATUINT16(secbuf[entry_ofs_in_sec], secbuf[entry_ofs_in_sec+1]);
            if(i & 0x0001)
            {
                clus_nr = ((clus_nr>>4) & 0x0fff);
            }
            else
            {
                clus_nr = clus_nr & 0x0fff;
            }

            if ( i != begin_clus_nr && 0 == ( (i - begin_clus_nr) % 8 ) )
            {
                dsm_Printf( BASE_DSM_TS_ID, "\n%2x: ", (i - begin_clus_nr) / 8);
            }
            dsm_Printf( BASE_DSM_TS_ID, "%04x ", clus_nr );

            counter--;
            if ( 0 == counter )
                break;

            i++; // 连续扫描每个后面的cluster
        }while(sec_nr < sb_info->iFATStartSec  + sb_info->iFATSize);

        if(sec_nr >= sb_info->iFATStartSec  + sb_info->iFATSize)
        {
            iRet = _ERR_FAT_DISK_FULL;
        }
    }
    else if(sb_info->iFATn == FAT16)
    {
        do 
        {
            entry_ofs_in_fat = i*2;  // 根据cluster number算出它在FAT中的字节偏移量
            sec_nr = sb_info->iFATStartSec  + (entry_ofs_in_fat / sb_info->iBytesPerSec);
            entry_ofs_in_sec = entry_ofs_in_fat % sb_info->iBytesPerSec;

            if(old_sec_nr < sec_nr) 
            {
                DSM_MemSet(secbuf, 0x00, DEFAULT_SECSIZE);
                iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, sec_nr, secbuf );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    D( ( DL_FATERROR, "In fat_dump_fat DRV_BLOCK_READ_2 failed iResult = %d\n",iResult));
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    goto end;
                }
                
                old_sec_nr = sec_nr;
            }
            
            clus_nr = MAKEFATUINT16(secbuf[entry_ofs_in_sec], secbuf[entry_ofs_in_sec+1]);
            if ( i != begin_clus_nr && 0 == ( (i - begin_clus_nr) % 8 ) )
            {
                dsm_Printf( BASE_DSM_TS_ID, "\n%2x: ", (i - begin_clus_nr) / 8);
            }
            dsm_Printf( BASE_DSM_TS_ID, "%04x ", clus_nr );
            
            counter--;
            if ( 0 == counter )
                break;
            
            i++;
        }while(sec_nr < sb_info->iFATStartSec + sb_info->iFATSize);   

        if(sec_nr >= sb_info->iFATStartSec + sb_info->iFATSize)
        {
            iRet = _ERR_FAT_DISK_FULL;
        }
    }
    else // FAT32
    {
        entry_ofs_in_fat = begin_clus_nr*4;
        iRet = _ERR_FAT_NOT_SUPPORT;
    }

    iRet = _ERR_FAT_SUCCESS;

end:    
    if(NULL != secbuf)
    {
        FAT_SECT_BUF_FREE((SECT_BUF*)secbuf);
    }

    return iRet;
}



struct cmd_entry g_FatCmdEnts[ ] =
{
  { "ms", shell_module_switch, "module switch for running specified module's command", "ms [FAT|VDS]" },
  { "rep", shell_repeat, "do something repetitively, %LOOP% is the name for loop variable", "rep ( base, step, counter ) command line" },
  { "debug", shell_debug, "debug switch", "debug [VDS|FAT] [on | off] [error | brief | detail]" },
  { "brep", shell_block_repeat, "do something for block repetitively", "brep {[-F]} <counter> {command line}" },
  { "help", shell_help, "print help message", NULL },
  { "version", shell_version, "show the version of the module", "version [vds | fat |vfs | reg | sms]" },
  { "?", shell_help, "print help message", NULL },


  { "init", shell_fs_init, "initiate the file system, yes or no means formating device forcibly or not", "init [YES| NO] <root_dev_name> [quick | normal]"},
  { "format", shell_FS_Format, "Format device as a type of filesystem", "format <dev_name> <filesystem_type[FAT|FFS|MinixFS]> <format_type>"},

  { "mountr", shell_mount_root, "mount root filesystem", "mountr <dev_name>"},
  { "mount", shell_mount, "mount filesystem", "mount [FAT|FFS|MinixFS] <dev_name> <mount_point>"},
  { "unmount", shell_unmount, "unmount filesystem(1/0 means force to unmount or not)", "unmount  <mount_point> [1 | 0]"},
  { "um", shell_unmount, "unmount filesystem(1/0 means force to unmount or not)", "unmount  <mount_point> [1 | 0]"},

  { "ls", shell_FS_Find, "print a directory listing",  "ls {<pathname> [-l]}" },
  { "dir", shell_FS_Find, "print a directory listing(same as ls)",  "dir {<pathname>}" },
  { "find", shell_FS_Find, "find files/subdirectoies", "\n\tfind <pattern>" },
  { "cd", shell_FS_ChangeDir, "change directory", "cd <pathname>" },
  { "pwd", shell_FS_PWD, "get current work directory",  "pwd" },
  { "sfirst", shell_FS_SearchFirst, "search files/subdirectoies first",  "sfirst <pathname>" },
  { "snext", shell_FS_SearchNext, "search files/subdirectoies next",  "snext <fd>" },
  { "sclose", shell_FS_SearchClose, "close search descriptor",  "sclose <fd>" },
    
  { "eof", shell_FS_EndOfFile, "check if end of file", "eof <fd>" },
  { "create", shell_FS_Create, "create a file by specifying the name", "create <filename> [mode] \n mode can be: A:FS_ATTR_ARCHIVE R: FS_ATTR_RO S: FS_ATTR_SYSTEM H: FS_ATTR_HIDDEN" },
  { "creclo", shell_FS_CreClo, "create a file by specifying the name, and close the handle",  "create <filename> [mode] \n mode can be: A:FS_ATTR_ARCHIVE R: FS_ATTR_RO S: FS_ATTR_SYSTEM H: FS_ATTR_HIDDEN" },
  { "cc",     shell_FS_CreClo, "create a file by specifying the name, and close the handle",  "create <filename>" },
  { "open", shell_FS_Open, "open an existing file to read/write with the flag","open [PathName] [Flag] [mode ], \n Flag can be: A:FS_O_APPEND C:FS_O_CREAT E:FS_O_EXCL T:FS_O_TRUNC R:FS_O_RDONLY W:FS_O_WRONLY +:FS_O_RDWR \n mode can be: A:FS_ATTR_ARCHIVE R: FS_ATTR_RO S: FS_ATTR_SYSTEM H: FS_ATTR_HIDDEN" },
  { "rm", shell_FS_DeleteFile, "delete the file", "rm <filename>" },
  { "rf", shell_FS_DeleteFile, "delete the file", "rf <filename>" },
  { "read", shell_FS_Read, "read from the file associated with fd", "read <fd> <length>" },
  { "write", shell_FS_Write, "write string to the file associated with fd",  "write <fd> <string>" },
  { "seek", shell_FS_Seek, "seek the file pointer", "seek <fd> <offset> [beg | cur | end ]" },
  { "close", shell_FS_Close, "close file associated with fd", "close <fd>" },
//  { "bigfile", shell_FS_bigfile, "create a file and write data to it until the disk filled up",  "\n\tbigfile <filename> fixed <chunk-size>\n\tbigfile <filename> random" },
  { "lwrite", shell_FS_Lwrite, "write string to the file associated with fd",  "lwrite <fd> <length>" },
  
  { "md", shell_FS_MakeDir, "create a directory", "md <pathname> [mode] \n mode can be: A:FS_ATTR_ARCHIVE R: FS_ATTR_RO S: FS_ATTR_SYSTEM H: FS_ATTR_HIDDEN" },
  { "mkdir", shell_FS_MakeDir, "create a directory", "mkdir <pathname> [ mode] \n mode can be: A:FS_ATTR_ARCHIVE R: FS_ATTR_RO S: FS_ATTR_SYSTEM H: FS_ATTR_HIDDEN" },
  { "rd", shell_FS_RemoveDir, "remove a directory", "rd <pathname>" },
  { "rmdir", shell_FS_RemoveDir, "remove a directory", "rmdir <pathname>" },
  { "bigfile", shell_FS_bigfile, "create a file and write data to it until the disk filled up",  "\n\tbigfile <filename> fixed <chunk-size>\n\tbigfile <filename> random" },
  { "bf", shell_FS_bigfile, "create a file and write data to it until the disk filled up",  "\n\tbigfile <filename> fixed <chunk-size>\n\tbigfile <filename> random" },
  { "rn", shell_FS_Rename, "rename a file",  "rename <oldname> <newname>" },
      
  { "dump", shell_FS_FAT_Dump, "dump data structure on the disk", "\ndump [fatlist] <filename>" 
                                                                  "\ndump [fatent] <begin_cluster_number>"
                                                                  "\ndump [bpb] <dev_name>"
																  "\ndump [file] <res_file_name> [res_file_name]"},
  { "info", shell_FS_GetFSInfo, "get the dev info","info <devname>"},
  { "gfatt", shell_FS_GetFileAttr, "get file attribute","getattr <pathname>"},
  { "sfatt", shell_FS_SetFileAttr, "get file attribute","getattr <pathname> [mode] \n mode can be: A:FS_ATTR_ARCHIVE R: FS_ATTR_RO S: FS_ATTR_SYSTEM H: FS_ATTR_HIDDEN"},
  { "hasformat",shell_FS_HasFormated,"check if the file system was formated","hasformat<devname>[1 | 2]"},
  { "scandisk", Shell_FS_ScanDisk, "scan disk","scandisk<devname> <filesystem_type>" },
  { "getname", shell_FS_GetFileName, "get file name with fd","getname <fd>" },
  { "dtree", shell_DropDirTree, "drop dir tree","dtree <dir_path>" },
  { "count", shell_Count_file, "count file and dir","count <dir_path> <flag: FS_FILE_TYPE | FS_DIR_TYPE | FS_RECURSIVE_TYPE>" },
  { "ucmp", shell_UniCmp, "compare unicode string","ucmp <ustr1> <ustr2>" },
  { "chsize", shell_FS_ChangeSize, "change file size","chsize <fd> <size>" },
  { "flush", shell_FS_Flush, "update file cache","flush <fd>" },
  { "devinfo", shell_FS_GetDevInfo, "Get the deviece information","devinfo" },
  { "tupdate", shell_FS_TUpdate, "Update lod from T-Flash",  "tupdate {<pathname>}" },
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
  { "vds_flush", shell_VDS_Flush, "Flush vds cache node",  "vds_flush<nodenumber>" },
  { "vds_flushall",shell_VDS_Flush_All, "Flush all vds cache node","vds_flushall"},
#endif  
  { NULL, NULL, NULL, NULL}, 
};


#if 0
// Print mode defining.

// Print to file.
#define TST_PRINT_MODE_F            0X01

// Print to trace.
#define TST_PRINT_MODE_T            0X02

// Print to file and trace.
#define TST_PRINT_MODE_FT          0X03

// T-Flash device mount point defining.
// Mount the t - flash to root dir.
#define TST_T_MOUNT_ROOT       "/"

// Mount the t - flash to "/t"
#define TST_T_MOUNT_T             "/t"

// Test result file.


#define TEST_PRINT_LEN 512

// Test result base file name .
CONST UINT8* g_pcTestBase =  "csw_test_result";

// Test Print buffer.
UINT8 g_szTestPrintBuf[TEST_PRINT_LEN + 1];

// Test result file name with path.
UINT8 g_szTestResultFile[FS_FILE_NAME_LEN] = {0,};

// The fd of opened file for print.
INT32 g_fp = -1;


//-------------------------------------------------------------------------------------------------------------
// tst_Print
// Print the trace to file or trace tool.
// Parameter
// uMode: Output mode. This parameter has flowing option:
// TST_PRINT_MODE_F: Print to file.
// TST_PRINT_MODE_T: Print to trace.
// TST_PRINT_MODE_FT:Print to file and trace.
// szFormat: print format.
// return value:
//   VOID
//------------------------------------------------------------------------------------------------------------
VOID tst_Print(UINT32 uiMode,CONST INT8* szFormat, ... )
{
    INT32 iResult;
    va_list va_format;
    UINT32 uiLen = 0;

    // Make the output string.
    va_start( va_format, szFormat );            
    DSM_MemSet( g_szTestPrintBuf, 0, TEST_PRINT_LEN );
    uiLen = SUL_StrVPrint( g_szTestPrintBuf, szFormat, va_format );
    if ( uiLen < 0 )
    {
        va_end( va_format );
        return;
    }
    
    // Add a enter character.
    DSM_StrCat(g_szTestPrintBuf,"\n");

    // write to file.
    if(uiMode&TST_PRINT_MODE_F)
    {               
        if(g_fp >= 0)
        {
            FS_Write(g_fp,g_szTestPrintBuf,DSM_StrLen(g_szTestPrintBuf));                    
            FS_Flush(g_fp);
        }
    }         
    va_end( va_format );

    // write to trace. 
    if(uiMode&TST_PRINT_MODE_T || g_fp < 0)
    {
          CSW_TRACE(BASE_DSM_TS_ID, "%s",g_szTestPrintBuf);
    }
    return ;
}


//-------------------------------------------------------------------------------------------------------------
// tst_DsmInit
// FS initalize function. realized flowing processing flash driver init, mult-language init, vds init, reg init,sms-dm init,
// fs - init,mount t-flash device,create csw test print file etc.
// return value:
//         TRUE:  Success.
//         FALSE: Fail.
//------------------------------------------------------------------------------------------------------------
BOOL tst_DsmInit(VOID)
{
    UINT8 i = 0;
    INT32 iResult = 0x00;
    UINT32 uiDevCount = 0;
    FS_DEV_INFO* pDevInfo = NULL;
    FS_DEV_INFO* pRootDevInfo = NULL;
    UINT32 uiRootDevCount = 0;
    UINT8* pUName = NULL;
    UINT8 szTFPoint[100];
    UINT32 uiULen = 0;
    BOOL bIsFormated = FALSE;
    BOOL bIsMountRoot = FALSE;
    BOOL bIsMountT = FALSE;
    BOOL bRet = TRUE;    

   // Initialize flash
    DRV_FlashPowerUp();
    DRV_FlashInit();

   // Initialize Mult-Language.    
    ML_Init();
    ML_SetCodePage("cp936");

    // Initialize VDS.
    iResult = VDS_Init();   
    if(ERR_SUCCESS == iResult)
    {
        CSW_TRACE(BASE_FFS_TS_ID,"VDS_Init() OK.\n");        
    }
    else
    {
        CSW_TRACE(BASE_FFS_TS_ID,"VDS_Init() ERROR, Error code: %d.\n", iResult);
        bRet = FALSE;
    }
    
    // Initialize device.
    iResult = DSM_DevInit();
    if(ERR_SUCCESS == iResult)
    {
        CSW_TRACE(BASE_FFS_TS_ID,"DSM_DevInit OK.\n");
    }
    else
    {
        CSW_TRACE(BASE_FFS_TS_ID,"DSM_DevInit ERROR, Error code: %d. \n",iResult);
        bRet = FALSE;
    }

    // Initialize REG.
    iResult = REG_Init();
    if(ERR_SUCCESS == iResult)
    {
        CSW_TRACE(BASE_FFS_TS_ID, TSTXT("REG_Init() OK.\n"));
    }
    else
    {
        CSW_TRACE(BASE_FFS_TS_ID, TSTXT("REG_Init() Fail!Error code:%d.\n"),iResult);
        bRet = FALSE;
    }

    // Initialize SMS_DM.
    iResult = SMS_DM_Init();
    if(ERR_SUCCESS == iResult)
    {
        CSW_TRACE(BASE_FFS_TS_ID, TSTXT("SMS_DM_Init OK.\n"));
    }
    else
    {
        CSW_TRACE(BASE_FFS_TS_ID, TSTXT("SMS_DM_Init ERROR, Error code: %d.\n"), iResult);
        bRet = FALSE;
    }

    // FS power on. 
    iResult = FS_PowerOn();
    if(ERR_SUCCESS == iResult)
    {
        CSW_TRACE(BASE_FFS_TS_ID,"FS Power On Check OK.\n");
    }
    else
    {
        CSW_TRACE(BASE_FFS_TS_ID,"FS Power On Check ERROR, Error code: %d. \n",iResult);
        bRet = FALSE;
    }

   // Get FS device table.
   iResult = FS_GetDeviceInfo(&uiDevCount, &pDevInfo);
   if(ERR_SUCCESS != iResult)
   {
        CSW_TRACE(BASE_FFS_TS_ID,"Device not register.\n");
        uiDevCount = 0;
        bRet = FALSE;
    }

    // Format the flash device.if the deivce has not format,shell foramt it ,else do noting. 
    // mount the root divice.    
    for(i = 0; i < uiDevCount; i++)
    {
        // Format the flash device if it not format. 
        iResult = FS_HasFormatted(pDevInfo[i].dev_name,FS_TYPE_FAT);
        if(ERR_FS_HAS_FORMATED == iResult)
        {
            CSW_TRACE(BASE_FFS_TS_ID,"The flash device %s has formated.\n",pDevInfo[i].dev_name);
            bIsFormated = TRUE;
        }
        else if(ERR_FS_NOT_FORMAT == iResult)
        { 
            CSW_TRACE(BASE_FFS_TS_ID,"The flash device %s not format.\n",pDevInfo[i].dev_name);
            if(FS_DEV_TYPE_FLASH == pDevInfo[i].dev_type)
            {
                iResult = FS_Format(pDevInfo[i].dev_name,FS_TYPE_FAT,FS_FORMAT_QUICK);
                if(iResult == ERR_SUCCESS)
                {
                    CSW_TRACE(BASE_FFS_TS_ID,"The flash device %s format ok.\n",pDevInfo[i].dev_name);
                    bIsFormated = TRUE;
                }
                else
                {
                    CSW_TRACE(BASE_FFS_TS_ID,"The flash device %s format error.Error code:%d.\n",pDevInfo[i].dev_name,iResult);                                
                    bRet = FALSE;
                }                
            }
        }
        else 
        {
            CSW_TRACE(BASE_FFS_TS_ID,"The flash device %s has formated error.Error code:%d.\n",pDevInfo[i].dev_name,iResult);
            bRet = FALSE;
        }  
        
        // Mount root device.
        if(TRUE == pDevInfo[i].is_root)
        {            
             if(TRUE == bIsFormated)
             {
                 if(uiRootDevCount > 0)
                 {
                      CSW_TRACE(BASE_FFS_TS_ID,"Warning:The FS root device too more:%d.\n",uiRootDevCount);
                 }
                 
                 if(FALSE == bIsMountRoot)
                 {
                    iResult = FS_MountRoot(pDevInfo[i].dev_name);
                    if(ERR_SUCCESS == iResult)
                    {
                        CSW_TRACE(BASE_FFS_TS_ID,"FS MountRoot(%s) OK.\n",pDevInfo[i].dev_name);
                        pRootDevInfo = pDevInfo + i;
                        bIsMountRoot = TRUE;
                        if(pRootDevInfo->dev_type == FS_DEV_TYPE_FLASH)
                        {
                            bIsMountT = TRUE;
                            SUL_StrCopy(szTFPoint,TST_T_MOUNT_ROOT);  
                        }
                    }
                    else
                    {
                        CSW_TRACE(BASE_FFS_TS_ID,"FS MountRoot(%s) ERROR, Error code: %d. \n",pDevInfo[i].dev_name,iResult);
                        bRet = FALSE;
                    }
                 }
                 
                 uiRootDevCount ++;
             }
             else
             {
                 CSW_TRACE(BASE_FFS_TS_ID,"Warning:The device %s not format,so can't mount root device\n",pDevInfo[i].dev_name);
                 bRet = FALSE;
             }
        }            
    }

    // Make a dircetory for T-Flash device  mount point.
    // Mount the t-flash device.
    if(pRootDevInfo)
    {
        if(FS_DEV_TYPE_FLASH == pRootDevInfo->dev_type)
        {
            SUL_StrCopy(szTFPoint,TST_T_MOUNT_T);   
            iResult= ML_LocalLanguage2UnicodeBigEnding((const UINT8*)szTFPoint,SUL_Strlen(szTFPoint),&pUName, &uiULen,NULL);
            if(ERR_SUCCESS == iResult)
            {
                iResult = FS_MakeDir(pUName,pUName);
                if(ERR_SUCCESS == iResult || ERR_FS_FILE_EXIST == iResult)
                {
                    iResult = FS_Mount(pRootDevInfo->dev_name,pUName,0,FS_TYPE_FAT);
                    if(ERR_SUCCESS == iResult)
                    {
                        CSW_TRACE(BASE_FFS_TS_ID,"Mount the t-flash device[%s] success.",pDevInfo[i].dev_name);
                    }
                    else
                    {
                        CSW_TRACE(BASE_FFS_TS_ID,"Mount the t-flash device[%s] fail.err code = %d.",pDevInfo[i].dev_name,iResult);
                        bRet = FALSE;
                    }
                }
                else
                {
                    CSW_TRACE(BASE_FFS_TS_ID,"Mount the t-flash device[%s] fail.err code = %d.",pDevInfo[i].dev_name,iResult);
                    bRet = FALSE;
                }
            }
            else
            {
                CSW_TRACE(BASE_FFS_TS_ID,"Local2Unicode fail.err code = %d.",iResult);
                bRet = FALSE;
            }
        }
    }

    // Create the print file.
    if(TRUE == bIsMountT)
    {
            if(NULL != pUName)
            {
                CSW_FFS_FREE(pUName);
                pUName = NULL;
            }

            // Make the print file name with path.
            TM_SYSTEMTIME sSystemTime;
            TM_GetSystemTime(&sSystemTime);             
            SUL_StrPrint(g_szTestResultFile,"%s/%s%d-%d-%d-%d-%d.txt",
                szTFPoint,
                g_pcTestBase,
                sSystemTime.uYear,
                sSystemTime.uMonth,
                sSystemTime.uDay,
                sSystemTime.uHour,
                sSystemTime.uMinute);
            iResult= ML_LocalLanguage2UnicodeBigEnding((const UINT8*)g_szTestResultFile,SUL_Strlen(g_szTestResultFile),&pUName, &uiULen,NULL);
            if(ERR_SUCCESS == iResult)
        
 {
                // Create the print file.
                g_fp = FS_Create(pUName,0);
                if(g_fp < 0)
                {
                    CSW_TRACE(BASE_FFS_TS_ID,"Open the print file fail.err code = %d.",iResult);       
                }
                else
                {
                    CSW_TRACE(BASE_FFS_TS_ID,"Open the print file success.g_fp = %d.",g_fp);
                    bRet = FALSE;
                }                
            }
    }
    
    if(NULL != pUName)
    {
        CSW_FFS_FREE(pUName);
        pUName = NULL;
    }
    return TRUE;
}
#endif

    
#endif // VDS_SHELL_COMMAND


