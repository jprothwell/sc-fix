/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: drvi.h,v 1.3 2005/04/27 19:35:10 hubbe Exp $
 * 
 * REALNETWORKS CONFIDENTIAL--NOT FOR DISTRIBUTION IN SOURCE CODE FORM
 * Portions Copyright (c) 1995-2002 RealNetworks, Inc.
 * All Rights Reserved.
 * 
 * The contents of this file, and the files included with this file,
 * are subject to the current version of the Real Format Source Code
 * Porting and Optimization License, available at
 * https://helixcommunity.org/2005/license/realformatsource (unless
 * RealNetworks otherwise expressly agrees in writing that you are
 * subject to a different license).  You may also obtain the license
 * terms directly from RealNetworks.  You may not use this file except
 * in compliance with the Real Format Source Code Porting and
 * Optimization License. There are no redistribution rights for the
 * source code of this file. Please see the Real Format Source Code
 * Porting and Optimization License for the rights, obligations and
 * limitations governing use of the contents of the file.
 * 
 * RealNetworks is the developer of the Original Code and owns the
 * copyrights in the portions it created.
 * 
 * This file, and the files included with this file, is distributed and
 * made available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS ALL
 * SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT
 * OR NON-INFRINGEMENT.
 * 
 * Technology Compatibility Kit Test Suite(s) Location:
 * https://rarvcode-tck.helixcommunity.org
 * 
 * Contributor(s):
 * 
 * ***** END LICENSE BLOCK ***** */

/*/////////////////////////////////////////////////////////////////////////// */
/*    RealNetworks, Inc. Confidential and Proprietary Information. */
/* */
/*    Copyright (c) 1995-2002 RealNetworks, Inc. */
/*    All Rights Reserved. */
/* */
/*    Do not redistribute. */
/* */
/* */
/*/////////////////////////////////////////////////////////////////////////// */

/* Define a structure and message id that can be used to extract the */
/* version of the API supported by the decoder. */
/* The version is packed into a ULONG32 using PN_ENCODE_PROD_VERSION. */
/* Only the MAJOR (4-bits) and MINOR (8-bits) fields are relevant. */
/* The RELEASE and BUILD fields will be set to 0 for consistency. */
/* These fields are interpreted as follows: */
/* */
/* The MINOR field will be incremented whenever an API change occurs that */
/* is compatible with the previous API.  That is, a front end that uses */
/* the previous API will still function properly. */
/* */
/* The MAJOR field will be incremented whenever an API change occurs that */
/* renders the new API incompatible with the old one. */

#define RV_MSG_ID_Get_Decoder_API_Version           0


#define RVDecoder_API_Tromso_Major       2
#define RVDecoder_API_Tromso_Minor       0
    /* The underlying codec changed over to the Tromso code base, in the */
    /* first half of 2000.  Many of the ILVC custom messages are no longer */
    /* applicable. */

#define RVDecoder_API_Margate_Major       3
#define RVDecoder_API_Margate_Minor       0
    /* The underlying codec changed over to the Margate (RV9) code base, in the */
    /* first half of 2002.  Many of the ILVC custom messages are no longer */
    /* applicable. */

typedef struct
{
    ULONG32             message_id;
        /* message_id must be RV_MSG_ID_Get_Decoder_API_Version */

    ULONG32             version;
        /* The version is returned here */
} RVDecoder_API_Version_Message;



/* Define a structure and message id that can be used to change the */
/* dimensions of the output image.  This will fail if called before */
/* RV20toYUV420Init().  It will also fail if the new dimensions */
/* are not supported.  The new dimensions will take effect on the */
/* next call to RV20toYUV420Transform(). */

#define RV_MSG_ID_Change_Decoder_Output_Dimensions  1

typedef struct
{
    ULONG32             message_id;
        /* message_id must be RV_MSG_ID_Change_Decoder_Output_Dimensions */

    ULONG32             width;
    ULONG32             height;
        /* specifies the desired dimensions of the output image. */
} RVDecoder_Output_Dimensions_Message;


/* Define a message id that can be used to tell the decoder that it is */
/* starting a new video sequence.  This is intended to be called after */
/* a seek, for instance. */

#define RV_MSG_ID_Decoder_Start_Sequence    20
