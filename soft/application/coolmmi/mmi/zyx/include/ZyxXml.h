/********************************************************************
* NOTE: ONLY SUPPORT two type element : prolog, data.
* NOTE: ONLY support utf-8 encoding.
********************************************************************/

#ifndef __ZYXML_H__
#define __ZYXML_H__

#define XML_TAG_PROLOG_START        "<?xml"
#define XML_TAG_PROLOG_END          "?>"
// #define XML_TAG_DOCTYPE_START       "<!DOCTYPE"
// #define XML_TAG_ELEMENTTYPE_START   "<!ELEMENT"
// #define XML_TAG_ATTLISTTYPE_START   "<!ATTLIST"
// #define XML_TAG_ENTITYTYPE_START    "<!ENTITY"
// #define XML_TAG_NOTATIONTYPE_START  "<!NOTATION"
// 
// 
// #define XML_TAG_CDATA_START         "<![CDATA["
// #define XML_TAG_CDATA_END           "]]>"
// #define XML_TAG_PI_START            "<?"

#define XML_TAG_ELEMENT_START       "<"
#define XML_TAG_ELEMENT_END         "</"
#define XML_TAG_SINGLE_END          "/>"
#define XML_TAG_GENERAL_END         ">"
// #define XML_SPACE                   " "
// #define XML_QUOTE                   "'"
// #define XML_DOUBLE_QUOTE            "\""
// #define XML_SQUARE_LEFT_BRAKET      "["
// #define XML_SQUARE_RIGHT_BRAKET     "]"

//////////////////////////////////////////////////////////////////////////
#define XML_BUFFER_LENGTH		1024		//old=128 by lijb

#define XML_MAX_ATTR_COUNT		16
#define XML_MAX_NAME_LENGTH		32		 

//////////////////////////////////////////////////////////////////////////
typedef void (*XML_start_elem_hdlr) (void *data,  char *el,  char **attr);
typedef void (*XML_end_elem_hdlr) (void *data,  char *el);
typedef void (*XML_data_hdlr) (void *resv,  char *el,  char *data, int len);

typedef enum
{
    XML_RESULT_OUT_OF_MEMORY = -3,
        XML_RESULT_ENCODING_ERROR = -2,
        XML_RESULT_FAIL = -1,
        XML_RESULT_OK = 0
} XML_RESULT_ENUM;

typedef enum
{
    XML_ENCODING_UTF8,
        XML_ENCODING_UTF16,
        XML_ENCODING_UCS2,
        XML_ENCODING_UCS4,
        XML_ENCODING_UNKNOWN
} XML_ENCODING_TYPE_ENUM;

//////////////////////////////////////////////////////////////////////////
// public functions.

// step 1. create one parse handle.
// if FAIL, return NULL, else return HANDLE.
void *zy_xml_new_parser(void);

// step 2. register element handler(start+end)
void zy_xml_register_element_handler(
                                     void *handle,
                                     XML_start_elem_hdlr start_hdlr,
                                     XML_end_elem_hdlr end_hdlr);

// step 3. register data handler
void zy_xml_register_data_handler(
                                  void *handle, 
                                  XML_data_hdlr data_hdlr);

// step 4. parse the file.
// RETURN: if success, return XML_RESULT_OK.
int zy_xml_parse(void *handle, char *file_name);

// step 5. close the handle. 
void zy_xml_close_parser(void *handle);

//////////////////////////////////////////////////////////////////////////

#endif

