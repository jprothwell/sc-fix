/*******************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2001
*
*******************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *  gdi_bytestream.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Bytestream. Abstract file and memory data source.
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#include "gdi_internal.h"
#include "drm_gprot.h"


#if 0    //remove by elisa for not used in our platform on 2008_10_9
#ifdef CSD_TARGET
#pragma arm section rwdata = "INTERNRW" , rodata = "INTERNCONST" , zidata = "INTERNZI"
#endif 
#endif

/* Function pointer table */
U8(*gdi_bytestream_get_byte) (void);
U8(*gdi_bytestream_peek_byte) (void);
U8(*gdi_bytestream_peek_byte_n) (int index);
U16(*gdi_bytestream_get_word) (void);
U32(*gdi_bytestream_get_3bytes) (void);
U32(*gdi_bytestream_get_dword) (void);
void (*gdi_bytestream_get_array) (U8 *array, U32 size);
void (*gdi_bytestream_flush) (U32 count);
void (*gdi_bytestream_seek) (S32 offset);

S32(*gdi_bytestream_tell) (void);
U32(*gdi_bytestream_get_size) (void);
BOOL(*gdi_bytestream_is_eof) (void);    /* Deprecated. We use longjmp() to handle truncated data source. */

/* source is from memory */
static U8 *gdi_bytestream_cache_buf_ptr;
static U32 gdi_bytestream_cache_size;
static U32 gdi_bytestream_cache_offset;

/* source is from file */
INT32 gdi_bytestream_file_size;
FS_HANDLE gdi_bytestream_handle;

INT32 gdi_bytestream_buffer_content_size;
int gdi_bytestream_file_offset;
U8 *gdi_bytestream_buffer_reader;

/* Common bytestream data */
typedef enum
{
    GDI_BYTESTREAM_TYPE_NONE,
    GDI_BYTESTREAM_TYPE_MEM,
    GDI_BYTESTREAM_TYPE_FILE
} gdi_bytestream_type_enum;

gdi_bytestream_type_enum gdi_bytestream_source_type = GDI_BYTESTREAM_TYPE_NONE;

#if  0
#ifdef CSD_TARGET
#pragma arm section rwdata, rodata , zidata
#endif
#endif //remove by elisa for not used in our platform on 2008_10_9

U8 gdi_bytestream_buffer[BYTESTREAM_BUFFER_SIZE];


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_free
 * DESCRIPTION
 *  free a bytestream
 * PARAMETERS
 *  void
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_bytestream_free(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gdi_bytestream_source_type == GDI_BYTESTREAM_TYPE_FILE)
    {
        (void)DRM_close_file(gdi_bytestream_handle);
    }
    else
    {
        GDI_ASSERT(gdi_bytestream_source_type == GDI_BYTESTREAM_TYPE_MEM);
    }

    gdi_bytestream_source_type = GDI_BYTESTREAM_TYPE_NONE;

    return GDI_SUCCEED;
}

#if 0
/* Start of internal RAM */
#ifdef CSD_TARGET
#pragma arm section code = "INTERNCODE"
#endif 
#endif //remove by elisa for not used in our platform on 2008_10_9

/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_get_byte_mem
 * DESCRIPTION
 *  get a U8 from memory
 * PARAMETERS
 *  void
 * RETURNS
 *  U8
 *****************************************************************************/
static U8 gdi_bytestream_get_byte_mem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 v = (U8) gdi_bytestream_cache_buf_ptr[gdi_bytestream_cache_offset++];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gdi_bytestream_cache_offset <= gdi_bytestream_cache_size)
    {
        return v;
    }
    else
    {
        GDI_RAISE(1);
        return 0;   /* Unreached */
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_peek_byte_mem
 * DESCRIPTION
 *  peek a U8 from memory
 * PARAMETERS
 *  void
 * RETURNS
 *  U8
 *****************************************************************************/
U8 gdi_bytestream_peek_byte_mem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gdi_bytestream_cache_offset + 1 <= gdi_bytestream_cache_size)
    {
        return (U8) gdi_bytestream_cache_buf_ptr[gdi_bytestream_cache_offset];
    }
    else
    {
        GDI_RAISE(1);
        return 0;   /* Unreached */
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_peek_byte_n_mem
 * DESCRIPTION
 *  peek one U8 after n bytes
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  U8
 *****************************************************************************/
U8 gdi_bytestream_peek_byte_n_mem(int index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int offset = gdi_bytestream_cache_offset + index;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (offset + 1 <= (int)gdi_bytestream_cache_size)
    {
        return (U8) gdi_bytestream_cache_buf_ptr[offset];
    }
    else
    {
        GDI_RAISE(1);
        return 0;   /* Unreached */
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_get_word_mem
 * DESCRIPTION
 *  peek a U16 from memory
 * PARAMETERS
 *  void
 * RETURNS
 *  U16
 *****************************************************************************/
U16 gdi_bytestream_get_word_mem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 U80;
    U32 U81;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    U80 = gdi_bytestream_cache_buf_ptr[gdi_bytestream_cache_offset++];
    U81 = gdi_bytestream_cache_buf_ptr[gdi_bytestream_cache_offset++];

    if (gdi_bytestream_cache_offset <= gdi_bytestream_cache_size)
    {
        return ((U16) ((U81 << 8) | U80));
    }
    else
    {
        GDI_RAISE(1);
        return 0;   /* Unreached */
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_get_3bytes_mem
 * DESCRIPTION
 *  peek a U16 from memory
 * PARAMETERS
 *  void
 * RETURNS
 *  U16
 *****************************************************************************/
U32 gdi_bytestream_get_3bytes_mem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 U80;
    U32 U81;
    U32 U82;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    U80 = gdi_bytestream_cache_buf_ptr[gdi_bytestream_cache_offset++];
    U81 = gdi_bytestream_cache_buf_ptr[gdi_bytestream_cache_offset++];
    U82 = gdi_bytestream_cache_buf_ptr[gdi_bytestream_cache_offset++];

    if (gdi_bytestream_cache_offset <= gdi_bytestream_cache_size)
    {
        return ((U32) ((U82 << 16) | (U81 << 8) | U80));
    }
    else
    {
        GDI_RAISE(1);
        return 0;   /* Unreached */
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_get_dword_mem
 * DESCRIPTION
 *  peek a U32 from memory
 * PARAMETERS
 *  void
 * RETURNS
 *  U32
 *****************************************************************************/
U32 gdi_bytestream_get_dword_mem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 U80;
    U32 U81;
    U32 U82;
    U32 U83;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    U80 = gdi_bytestream_cache_buf_ptr[gdi_bytestream_cache_offset++];
    U81 = gdi_bytestream_cache_buf_ptr[gdi_bytestream_cache_offset++];
    U82 = gdi_bytestream_cache_buf_ptr[gdi_bytestream_cache_offset++];
    U83 = gdi_bytestream_cache_buf_ptr[gdi_bytestream_cache_offset++];

    if (gdi_bytestream_cache_offset <= gdi_bytestream_cache_size)
    {
        return ((U32) (U83 << 24)) | ((U32) (U82 << 16)) | ((U32) (U81 << 8)) | (U32) (U80);
    }
    else
    {
        GDI_RAISE(1);
        return 0;   /* Unreached code */
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_get_array_mem
 * DESCRIPTION
 *  
 * PARAMETERS
 *  array       [?]         
 *  size        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_bytestream_get_array_mem(U8 *array, U32 size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gdi_bytestream_cache_offset + size <= gdi_bytestream_cache_size)
    {
        memcpy(array, gdi_bytestream_cache_buf_ptr + gdi_bytestream_cache_offset, size);
        gdi_bytestream_cache_offset += size;
    }
    else
    {
        GDI_RAISE(1);
        return; /* Unreached code */
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_is_eof_mem
 * DESCRIPTION
 *  chcek if is eof
 * PARAMETERS
 *  void
 * RETURNS
 * BOOL
 *  
 *****************************************************************************/
BOOL gdi_bytestream_is_eof_mem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (BOOL) (gdi_bytestream_cache_offset >= gdi_bytestream_cache_size);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_flush_mem
 * DESCRIPTION
 *  flush memory
 * PARAMETERS
 *  count       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_bytestream_flush_mem(U32 count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* It allows to stop at the end of stream plus one U8 */
    if ((gdi_bytestream_cache_offset += count) > gdi_bytestream_cache_size)
    {
        GDI_RAISE(1);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_seek_mem
 * DESCRIPTION
 *  seek memory
 * PARAMETERS
 *  offset      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_bytestream_seek_mem(S32 offset)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* It allows to stop at the end of stream plus one U8 */
    if (offset <= (S32) gdi_bytestream_cache_size)
    {
        gdi_bytestream_cache_offset = offset;
    }
    else
    {
        GDI_RAISE(1);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_tell_mem
 * DESCRIPTION
 *  tell memory
 * PARAMETERS
 *  void
 *  S32(?)      [IN]        Offset
 * RETURNS
 *  S32
 *****************************************************************************/
S32 gdi_bytestream_tell_mem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_bytestream_cache_offset;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_get_size_mem
 * DESCRIPTION
 *  get size of memory
 * PARAMETERS
 *  void
 *  S32(?)      [IN]        Offset
 * RETURNS
 *  U32
 *****************************************************************************/
U32 gdi_bytestream_get_size_mem(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_bytestream_cache_size;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_buffer_load_file
 * DESCRIPTION
 *  
 * PARAMETERS
 *  at_least        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_bytestream_buffer_load_file(int at_least)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int offset;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    offset = gdi_bytestream_buffer_content_size;
    if (offset)
    {
        memcpy(gdi_bytestream_buffer, gdi_bytestream_buffer_reader, gdi_bytestream_buffer_content_size);
    }

    if ((DRM_read_file(
            gdi_bytestream_handle,
            gdi_bytestream_buffer + offset,
            sizeof(gdi_bytestream_buffer) - offset,
            (INT32*)(&gdi_bytestream_buffer_content_size)) == FS_NO_ERROR)
        && gdi_bytestream_buffer_content_size > 0)
    {
        gdi_bytestream_buffer_reader = gdi_bytestream_buffer;
        gdi_bytestream_buffer_content_size += offset;

        if (gdi_bytestream_buffer_content_size >= at_least)
        {
            return;
        }
    }

    GDI_RAISE(1);
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_buffer_load_file_widthout_exception
 * DESCRIPTION
 *  
 * PARAMETERS
 *  at_least        [IN]        
 * RETURNS
 *  
 *****************************************************************************/
BOOL gdi_bytestream_buffer_load_file_widthout_exception(int at_least)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int offset;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    offset = gdi_bytestream_buffer_content_size;
    if (offset)
    {
        memcpy(gdi_bytestream_buffer, gdi_bytestream_buffer_reader, gdi_bytestream_buffer_content_size);
    }

    if ((DRM_read_file(
            gdi_bytestream_handle,
            gdi_bytestream_buffer + offset,
            sizeof(gdi_bytestream_buffer) - offset,
            (INT32*) & gdi_bytestream_buffer_content_size) == FS_NO_ERROR)
        && gdi_bytestream_buffer_content_size > 0)
    {
        gdi_bytestream_buffer_reader = gdi_bytestream_buffer;
        gdi_bytestream_buffer_content_size += offset;

        if (gdi_bytestream_buffer_content_size >= at_least)
        {
            return TRUE;
        }
    }
    return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_get_byte_file
 * DESCRIPTION
 *  get a U8 from file
 * PARAMETERS
 *  void
 * RETURNS
 *  U8
 *****************************************************************************/
U8 gdi_bytestream_get_byte_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!(gdi_bytestream_buffer_content_size))
    {
        gdi_bytestream_buffer_load_file(1);
    }

    gdi_bytestream_buffer_content_size--;

    gdi_bytestream_file_offset++;

    return *gdi_bytestream_buffer_reader++;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_peek_byte_file
 * DESCRIPTION
 *  peek a U8 from file
 * PARAMETERS
 *  void
 * RETURNS
 *  U8
 *****************************************************************************/
U8 gdi_bytestream_peek_byte_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (!gdi_bytestream_buffer_content_size)
    {
        gdi_bytestream_buffer_load_file(1);
    }

    return *gdi_bytestream_buffer_reader;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_peek_byte_n_file
 * DESCRIPTION
 *  peek n U8 from file
 * PARAMETERS
 *  index       [IN]        
 * RETURNS
 *  U8
 *****************************************************************************/
U8 gdi_bytestream_peek_byte_n_file(int index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gdi_bytestream_buffer_content_size < index)
    {
        gdi_bytestream_buffer_load_file(index);
    }

    return gdi_bytestream_buffer_reader[index];
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_get_word_file
 * DESCRIPTION
 *  peek a U16 from file (16 bit)
 * PARAMETERS
 *  void
 * RETURNS
 *  U16
 *****************************************************************************/
U16 gdi_bytestream_get_word_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gdi_bytestream_buffer_content_size < 2)
    {
        gdi_bytestream_buffer_load_file(2);
    }

    gdi_bytestream_buffer_content_size -= 2;
    gdi_bytestream_file_offset += 2;

    result = *gdi_bytestream_buffer_reader++;
    result |= (*gdi_bytestream_buffer_reader++) << 8;
    return result;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_get_3bytes_file
 * DESCRIPTION
 *  peek a U16 from file (16 bit)
 * PARAMETERS
 *  void
 * RETURNS
 *  U16
 *****************************************************************************/
U32 gdi_bytestream_get_3bytes_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gdi_bytestream_buffer_content_size < 3)
    {
        gdi_bytestream_buffer_load_file(3);
    }

    gdi_bytestream_buffer_content_size -= 3;
    gdi_bytestream_file_offset += 3;

    result = *gdi_bytestream_buffer_reader++;
    result |= (*gdi_bytestream_buffer_reader++) << 8;
    result |= (*gdi_bytestream_buffer_reader++) << 16;
    return result;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_get_dword_file
 * DESCRIPTION
 *  get a U32 from file (32 bit)
 * PARAMETERS
 *  void
 *  int(?)      [IN]        Index
 * RETURNS
 *  U32
 *****************************************************************************/
U32 gdi_bytestream_get_dword_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 result;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gdi_bytestream_buffer_content_size < 4)
    {
        gdi_bytestream_buffer_load_file(4);
    }

    gdi_bytestream_buffer_content_size -= 4;
    gdi_bytestream_file_offset += 4;

    result = *gdi_bytestream_buffer_reader++;
    result |= (*gdi_bytestream_buffer_reader++) << 8;
    result |= (*gdi_bytestream_buffer_reader++) << 16;
    result |= (*gdi_bytestream_buffer_reader++) << 24;
    return result;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_get_array_file
 * DESCRIPTION
 *  get a U32 from file (32 bit)
 * PARAMETERS
 *  array       [?]         
 *  size        [IN]        
 *  int(?)      [IN]        Index
 * RETURNS
 *  dword(?)
 *****************************************************************************/
void gdi_bytestream_get_array_file(U8 *array, U32 size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (size > 0)
    {
        int n;

        if (gdi_bytestream_buffer_content_size == 0)
        {
            gdi_bytestream_buffer_load_file(1);
        }

        n = GDI_MIN((int)size, gdi_bytestream_buffer_content_size);
        memcpy(array, gdi_bytestream_buffer_reader, n);
        array += n;
        gdi_bytestream_buffer_content_size -= n;
        size -= n;
        gdi_bytestream_file_offset += n;
        gdi_bytestream_buffer_reader += n;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_is_eof_file
 * DESCRIPTION
 *  check is eof
 * PARAMETERS
 *  void
 * RETURNS
 * BOOL
 *  
 *****************************************************************************/
BOOL gdi_bytestream_is_eof_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (gdi_bytestream_buffer_content_size)
    {
        return FALSE;
    }

    if (gdi_bytestream_buffer_load_file_widthout_exception(1))
    {
        return FALSE;
    }

    return TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_seek_file
 * DESCRIPTION
 *  seek file bytestream
 * PARAMETERS
 *  offset      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void gdi_bytestream_seek_file(S32 offset)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int ret;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ret = DRM_seek_file(gdi_bytestream_handle, offset, FS_FILE_BEGIN);
    gdi_bytestream_buffer_content_size = 0;
    gdi_bytestream_file_offset = offset;

    if ((ret < 0) || (ret != (int)offset))
    {
        GDI_RAISE(1);
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_flush_file
 * DESCRIPTION
 *  flush file bytestream
 * PARAMETERS
 *  count       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
/* Note: FSAL does not check the availability of data until issuing read/write operations */
void gdi_bytestream_flush_file(U32 count)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_bytestream_file_offset += count;
    if (count >= BYTESTREAM_BUFFER_SIZE)
    {
        gdi_bytestream_seek_file(gdi_bytestream_file_offset);
        return;
    }
    while (count)
    {
        if (gdi_bytestream_buffer_content_size < (int)count)
        {
            count -= gdi_bytestream_buffer_content_size;
            gdi_bytestream_buffer_content_size = 0;
            gdi_bytestream_buffer_load_file(1);
        }
        else
        {
            gdi_bytestream_buffer_content_size -= count;
            gdi_bytestream_buffer_reader += count;
            count = 0;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_tell_file
 * DESCRIPTION
 *  tell file bytestream
 * PARAMETERS
 *  void
 * RETURNS
 *  S32
 *****************************************************************************/
S32 gdi_bytestream_tell_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_bytestream_file_offset;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_get_size_file
 * DESCRIPTION
 *  get size of file bytestream
 * PARAMETERS
 *  void
 * RETURNS
 *  U32
 *****************************************************************************/
U32 gdi_bytestream_get_size_file(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return gdi_bytestream_file_size;
}

#if 0  //remove by elisa for not used in our platform on 2008_10_9
/* End of Internal SRAM */
#ifdef CSD_TARGET
#pragma arm section code
#endif 
#endif


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_create
 * DESCRIPTION
 *  create a memory bytestream
 * PARAMETERS
 *  buf_ptr         [IN]        
 *  buf_size        [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_bytestream_create(U8 *buf_ptr, U32 buf_size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* check if someone didnt free bytestream correctly */
    GDI_ASSERT(gdi_bytestream_source_type == GDI_BYTESTREAM_TYPE_NONE);

    if (buf_ptr == NULL)
    {
        return GDI_BYTESTREAM_ERR_OPEN_BUFFER_FAILED;
    }

    gdi_bytestream_cache_size = buf_size;
    gdi_bytestream_cache_offset = 0;
    gdi_bytestream_cache_buf_ptr = buf_ptr;

    gdi_bytestream_get_byte = gdi_bytestream_get_byte_mem;
    gdi_bytestream_peek_byte = gdi_bytestream_peek_byte_mem;
    gdi_bytestream_peek_byte_n = gdi_bytestream_peek_byte_n_mem;
    gdi_bytestream_get_word = gdi_bytestream_get_word_mem;
    gdi_bytestream_get_3bytes = gdi_bytestream_get_3bytes_mem;
    gdi_bytestream_get_dword = gdi_bytestream_get_dword_mem;
    gdi_bytestream_get_array = gdi_bytestream_get_array_mem;
    gdi_bytestream_flush = gdi_bytestream_flush_mem;
    gdi_bytestream_is_eof = gdi_bytestream_is_eof_mem;
    gdi_bytestream_seek = gdi_bytestream_seek_mem;
    gdi_bytestream_tell = gdi_bytestream_tell_mem;
    gdi_bytestream_get_size = gdi_bytestream_get_size_mem;

    /* flag to check if someone didnt call free */
    gdi_bytestream_source_type = GDI_BYTESTREAM_TYPE_MEM;
    return GDI_SUCCEED;
}


/*****************************************************************************
 * FUNCTION
 *  gdi_bytestream_create_file
 * DESCRIPTION
 *  create a file bytestream
 * PARAMETERS
 *  file_name       [IN]        
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
GDI_RESULT gdi_bytestream_create_file(const S8 *file_name)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* check if someone didnt free bytestream correctly */
    GDI_ASSERT(gdi_bytestream_source_type == GDI_BYTESTREAM_TYPE_NONE);

    if (file_name == NULL)
    {
        return GDI_BYTESTREAM_ERR_OPEN_FILE_FAILED;
    }

    gdi_bytestream_handle = DRM_open_file((UINT8*) file_name, FS_READ_ONLY | FS_OPEN_NO_DIR, 0);
    if (gdi_bytestream_handle < 0)
    {
        return GDI_BYTESTREAM_ERR_OPEN_FILE_FAILED;
    }

    gdi_bytestream_buffer_reader = gdi_bytestream_buffer;
    gdi_bytestream_buffer_content_size = 0;
    gdi_bytestream_file_offset = 0;
    if (DRM_file_size(gdi_bytestream_handle, (INT32*) & gdi_bytestream_file_size) < 0)
    {
        gdi_bytestream_file_size = 0;
    }

    gdi_bytestream_get_byte = gdi_bytestream_get_byte_file;
    gdi_bytestream_peek_byte = gdi_bytestream_peek_byte_file;
    gdi_bytestream_peek_byte_n = gdi_bytestream_peek_byte_n_file;
    gdi_bytestream_get_word = gdi_bytestream_get_word_file;
    gdi_bytestream_get_3bytes = gdi_bytestream_get_3bytes_file;
    gdi_bytestream_get_dword = gdi_bytestream_get_dword_file;
    gdi_bytestream_get_array = gdi_bytestream_get_array_file;
    gdi_bytestream_flush = gdi_bytestream_flush_file;
    gdi_bytestream_is_eof = gdi_bytestream_is_eof_file;
    gdi_bytestream_seek = gdi_bytestream_seek_file;
    gdi_bytestream_tell = gdi_bytestream_tell_file;
    gdi_bytestream_get_size = gdi_bytestream_get_size_file;

    /* flag to check if someone didnt call free */
    gdi_bytestream_source_type = GDI_BYTESTREAM_TYPE_FILE;
    return GDI_SUCCEED;
}

