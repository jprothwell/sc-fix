/***************************************************************************
 * $Id: jdi_chunk_storage.h,v 1.2 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.2 $
 * $Date: 2008/06/16 05:52:17 $
 * 
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdi_chunk_storage.h,v $
 * Revision 1.2  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.1  2007/04/20 12:41:26  shreyasdevhtmlbrow
 * Chunk Storage Interface Initial Version
 *
 *  
 ***************************************************************************/

#ifndef _JDI_CHUNK_STORAGE_H
#define _JDI_CHUNK_STORAGE_H

/*** Type definitions			***/

/**
 * @brief The details in this struture are used internally by the chunk storage module.
 */
typedef struct cs_header_details
{
	JC_BOOLEAN			bIsOccupied ; /**< Used Internally by chunk storage module */
	JC_BOOLEAN			bIsDataChunk ; /**< Used Internally by chunk storage module */
	JC_UINT16			uhChunkNumber ; /**< Used Internally by chunk storage module */
	JC_UINT16			uhNextChunk ; /**< Used Internally by chunk storage module */
	JC_UINT16			uhDataSize ; /**< Used Internally by chunk storage module */ 
} ST_CS_HEADER_DETAILS ;

/**
 * @brief The following structure specifies the details of a chunk. For each entry given by the application
 *		  the chunk storage module adds a chunk header. 
 */
typedef struct cs_chunk_header
{
	void					*pvHeader ; /**< Specifies the header information (added by the application) */
	ST_CS_HEADER_DETAILS	stHeaderDetails ; /**< Used Internally by chunk storage module */
	struct cs_chunk_header	*pstNext ; /**< Pointer to the next chunk header. */
	struct cs_chunk_header	*pstOriginalListNext ; /** Used Internally by chunk storage module */
} ST_CS_CHUNK_HEADER ;


/*** Function Prototypes			***/

/**
 * @brief	    Initializes the chunk storage module
 * 
 * @param[in]	vMemHandle Specifies the memory handle to be used by the chunk storage module.
 * @param[in]	pmFileName Specifies the file in which the data is to be stored and retrieved.
 * @param[in]	ucChunkHeaderSize Specifies the size of header (application header related to each chunk).
 * @param[in]	uhChunkSize Specifies the size of each chunk.
 * @param[out]	pvHandle Specifies chunk storage handle which will be returned by this API.
 *				The application needs to use this handle to call all the other chunk storage API's.
 * @retval		JC_OK if the Chunk storage is sucessfully initialized.
 * @retval		JC_ERR_MEMORY_ALLOCATION if a memory error occurs.
 * @see			jdi_CSDeInitialize
 *
 * The application needs to call the API to initialize the chunk storage module.
 * The API will open the file in which data is to be stored and retrieved. 
 *
 * For each record the application needs to add its own header (the same header will be used to
 * retrieve, delete the record at a later point of time). ucChunkHeaderSize will be the size of
 * this application header. 
 *
 * The size of each chunk has to be determined based on the constraints such as cost of file operations
 * (as smaller chunk will result in more chunks written and read), speed of file operations etc.
 */

JC_RETCODE jdi_CSInitialize (JC_HANDLE		vMemHandle,
							 JC_CHAR		*pmFileName,
							 JC_UINT8		ucChunkHeaderSize,
							 JC_UINT16		uhChunkSize,
							 JC_HANDLE		*pvHandle) ;

/**
 * @brief	    Adds a data information into the chunk storage file.
 * 
 * @param[in]	vHandle Specifies the Handle returned by #jdi_CSInitialize.
 * @param[in]	pvHeader Specifies the application header which will uniquely identify the
 *				data to be added.
 * @param[in]	pvData Specifies the data to be added.
 * @param[in]	uhDataSize Specifies the size of the data to be added. This should be specified
 *				in terms of bytes.
 * @param[out]	Once the data has been added the application want to perform other operations immediately such
 *				as appending (appending requires the data chunk, which can be generaly obtained by calling
 *				the API #jdi_CSRetrieveChunkHeaderList). In that case to search for the chunk again will be an overhead.
 *				So the API returns the first chunk in which this record is stored and can be used for other operations.
 * @retval		JC_OK if the entry is added successfully.
 * @retval		JC_ERR_MEMORY_ALLOCATION if a memory error occurs.
 *
 * The application needs to call the API to add an entry into the Chunk Storage file. 
 */	
					 
JC_RETCODE jdi_CSAddEntry (JC_HANDLE			vHandle,
						   void					*pvHeader,
						   void					*pvData,
						   JC_UINT16			uhDataSize,
						   ST_CS_CHUNK_HEADER	**ppstStartChunkInfo) ;

/**
 * @brief	    Retrieves the list of application header.
 * 
 * @param[in]	vHandle Specifies the Handle returned by #jdi_CSInitialize.
 * @retval		List of ST_CS_CHUNK_HEADER specifying the various appplication headers.
 *			    JC_NULL if no entry has been made.
 *
 * Each entry in the Chunk storage file is uniquely identified by the application header. The application
 * can perform operations (such as retrive data, Update, Delete etc) only if it has the appplication header
 * information. This API will return the list of Chunk headers which will include the application
 * header. 
 */	

ST_CS_CHUNK_HEADER * jdi_CSRetrieveChunkHeaderList (JC_HANDLE	vHandle) ;

/**
 * @brief	    Retrieves the size of the data.
 * 
 * @param[in]	vHandle Specifies the Handle returned by #jdi_CSInitialize.
 * @retval		pstChunkHeader Specifies the chunk whose data size is required.
 *
 * While performing the retrieve data operation (#jdi_CSRetrieveData) the application needs to
 * allocate memory for the data to be retrieved. So before retrieving application needs to know the
 * size of the data. For that this API can be used.
 */	
JC_UINT16 jdi_CSGetDataSize (JC_HANDLE				vHandle,
							 ST_CS_CHUNK_HEADER		*pstChunkHeader) ;
							
/**
 * @brief	    Retrieves Data from file.
 * 
 * @param[in]	vHandle Specifies the Handle returned by #jdi_CSInitialize.
 * @param[in]	pstChunkHeader Specifies the Chunk header which contains the information
 *				related to data to be retrieved (obtained using #jdi_CSRetrieveChunkHeaderList)
 * @param[in]	uhStartIndex Specifies the starting index of the data to be copied.
 * @param[in]	uhDataSize Specifies the size of the data to be copied.
 * @param[in]	pvData The API will return the data in this parameter.
 * @retval		JC_OK if the data is successfully retrieved.
 *
 * This API will retrieve the data from the file. The application needs to specify the chunk header
 * which will give information to chunk storage module to retrieve the correct data. 
 *
 * The API allows the application to retrieve part of the data that it wants. The Start index and the
 * data size parameter should be used for this purpose.
 * 
 * The Application has to allocate memory for pvData before using this API. The application can 
 * find out the size of the data related to the chunk header using the API #jdi_CSGetDataSize.
 */

JC_RETCODE jdi_CSRetrieveData (JC_HANDLE			vHandle,
							   ST_CS_CHUNK_HEADER	*pstChunkHeader,
							   JC_UINT16			uhStartIndex,
							   JC_UINT16			uhDataSize,
							   void					*pvData) ;

/**
 * @brief	    Updates the application header information.
 * 
 * @param[in]	vHandle Specifies the Handle returned by #jdi_CSInitialize.
 * @param[in]	pstChunkHeader Specifies the Chunk header which contains the header information
 *				to be updated (obtained using #jdi_CSRetrieveChunkHeaderList)
 * @param[in]	pvHeader Specifies the new header information.
 * @retval		JC_OK if the header is successfully updated.
 *
 * This API will update the application header. In certain cases only the application header needs to
 * be updated (without changing the data). In that case this API needs to be used.
 */

JC_RETCODE jdi_CSUpdateHeader (JC_HANDLE			vHandle,
							   ST_CS_CHUNK_HEADER	*pstChunkHeader,
							   void					*pvHeader) ;

/**
 * @brief	    Updates the data information.
 * 
 * @param[in]	vHandle Specifies the Handle returned by #jdi_CSInitialize.
 * @param[in]	pstChunkHeader Specifies the Chunk header which contains the information
 *				to get the data to be updated (obtained using #jdi_CSRetrieveChunkHeaderList)
 * @param[in]	pvData Specifies the new data information
 * @param[in]	uhDataSize Specifies the size of the new data.
 * @param[in]	bIsAppendMode Specifies whether the data is to be updated or replaced.
 * @retval		JC_OK if the data is successfully updated.
 *
 * This API will update the data header. The application can either replace the entire data 
 * information or just update the data to the existing data.
 */ 

JC_RETCODE jdi_CSUpdateEntry (JC_HANDLE				vHandle,
							  ST_CS_CHUNK_HEADER	*pstChunkHeader,
							  void					*pvData,
							  JC_UINT16				uhDataSize,
							  JC_BOOLEAN			bIsAppendMode) ;

/**
 * @brief	    Deletes data information from the chunk storage file.
 * 
 * @param[in]	vHandle Specifies the Handle returned by #jdi_CSInitialize.
 * @param[in]	pstChunkHeader Specifies the Chunk header which contains the information
 *				to the data to be deleted (obtained using #jdi_CSRetrieveChunkHeaderList).
 * @retval		JC_OK if the data is deleted successfully.
 *
 * This API will delete the data information from the chunk storage file.
 */ 

JC_RETCODE jdi_CSDeleteEntry (JC_HANDLE				vHandle,
							  ST_CS_CHUNK_HEADER	*pstChunkHeader) ;

/**
 * @brief	    Performs clean up of the chunk storage file.
 * 
 * @param[in]	vHandle Specifies the Handle returned by #jdi_CSInitialize.
 * @retval		JC_OK if the file is cleaned up successfully..
 *
 * This API is currently not implemented.
 */ 

JC_RETCODE jdi_CSCleanUp (JC_HANDLE		vHandle) ;

/**
 * @brief	    Deinitializes the chunk storage module.
 * 
 * @param[in]	vHandle Specifies the Handle returned by #jdi_CSInitialize.
 * @retval		void.
 *
 * This API will deinitialize the chunk storage module.
 */ 

void jdi_CSDeInitialize (JC_HANDLE		vHandle) ;

#endif

/** END OF FILE **/
