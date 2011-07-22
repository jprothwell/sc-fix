////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2007, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL: http://10.10.100.14/svn/developing/Sources/svc/branches/Total_BJ/vois/include/vois_m.h $ //
//	$Author: mathieu $                                                        // 
//	$Date: 2008-11-01 10:39:51 +0800 (Sat, 01 Nov 2008) $                     //   
//	$Revision: 19499 $                                                          //   
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
/// @file vois_m.h                                                            //
/// That file describes the audio VOIce Service (VOIS) interface. This        //
/// takes care of all sound related operation needed by a phone call.         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


#ifndef _VOIS_M_H_
#define _VOIS_M_H_

#include "cs_types.h"

#include "hal_aif.h"
#include "aud_m.h"


/// @page vois_mainpage  VOIce Service API
/// @mainpage VOIce Service API
/// 
/// This service provides the sound management for the phone calls. It takes care of the Rx 
/// decoding, the Tx encoding, the configuration of the audio interfaces in a fully 
/// integrated way. The only things to do is to start this service when needed 
/// (#vois_Start), to stop it when it becomes unneeded (#vois_Stop), and configure on 
/// the run when needed (#vois_Setup).
/// 
/// @par Configuration requirements
/// HAL must be poperly configured in order to use the aps service.
/// 
/// The API is detailed in the following group: @ref vois
///
/// @par Organisation
/// @image latex vois.png
/// @image html vois.png
///
/// @defgroup vois VOIce Service (VOIS)
/// @{
///  

// =============================================================================
//  MACROS
// =============================================================================

// =============================================================================
//  TYPES
// =============================================================================




// =============================================================================
// VOIS_ERR_T
// -----------------------------------------------------------------------------
/// Error types for the VOIS module.
// =============================================================================
typedef enum
{
    /// No error occured
    VOIS_ERR_NO,

    /// An attempt to access a busy resource failed
    /// (Resource use not possible)
    VOIS_ERR_RESOURCE_BUSY,

    /// Attempt to open a resource already opened,
    /// (no need to open again to use).
    VOIS_ERR_ALREADY_OPENED,
    
    /// Timeout while trying to access the resource
    VOIS_ERR_RESOURCE_TIMEOUT,
    
    /// Invalid parameter
    VOIS_ERR_BAD_PARAMETER,

    /// The specified interface does not exist
    VOIS_ERR_NO_ITF,

    /// What ?
    VOIS_ERR_UNKNOWN,


    VOIS_ERR_QTY
} VOIS_ERR_T;




// =============================================================================
// VOIS_AUDIO_CFG_T
// -----------------------------------------------------------------------------
/// Configuration structure.
/// 
/// A configuration structure allows to record a stream with the proper configuration
/// set as far as the audio interface and the decoding are concerned.
// =============================================================================
typedef struct
{
    /// Speaker level.
    AUD_SPK_LEVEL_T spkLevel;

    /// Mic level
    AUD_MIC_LEVEL_T micLevel;

    /// Side tone
    AUD_SIDE_LEVEL_T sideLevel;

    /// Tone level
    AUD_TONE_ATTENUATION_T toneLevel;
    
    /// encoder gain
    INT16 encMute;

    /// decoder gain
    INT16 decMute;
       
} VOIS_AUDIO_CFG_T;







// =============================================================================
//  FUNCTIONS
// =============================================================================


// =============================================================================
// vois_Open
// -----------------------------------------------------------------------------
/// Open the VOIS service. Must be called before any other VOIS function.
/// This function registers the VoiS context in HAL map engine.
// =============================================================================
PUBLIC VOID vois_Open(VOID);



// =============================================================================
// vois_Setup
// -----------------------------------------------------------------------------
/// Configure the VOIS service.. 
/// 
/// This functions configures the playing of the Rx stream and te recording of 
/// the Tx stream on a given audio interface:
/// gain for the side tone, the microphone and  
/// the speaker, input selection for the microphone and output selection for 
/// the speaker...
/// 
/// @param itf Interface number of the interface to setup.
/// @param cfg The configuration set applied to the audio interface. See 
/// #VOIS_AUDIO_CFG_T for more details.
/// @return #VOIS_ERR_NO, the configuration being applied.
// =============================================================================
PUBLIC VOIS_ERR_T vois_Setup(AUD_ITF_T itf, CONST VOIS_AUDIO_CFG_T* cfg);



// =============================================================================
// vois_Start
// -----------------------------------------------------------------------------
/// Start the VOIS service. 
/// 
/// This function records from the mic and outputs sound on the speaker
/// on the audio interface specified as a parameter, using the input and output
/// set by the \c parameter. \n
///
/// @param itf Interface providing the audion input and output.
/// @param cfg The configuration set applied to the audio interface
/// @return #VOIS_ERR_RESOURCE_BUSY when the driver is busy with another audio 
/// command, or if VPP is unavailable.
///         #VOIS_ERR_NO it can execute the command.
// =============================================================================
PUBLIC VOIS_ERR_T vois_Start(
                        CONST AUD_ITF_T      itf,
                        CONST VOIS_AUDIO_CFG_T* cfg);




// =============================================================================
// vois_Stop
// -----------------------------------------------------------------------------
/// This function stops the VOIS service.
/// If the function returns 
/// #VOIS_ERR_RESOURCE_BUSY, it means that the driver is busy with another 
/// audio command.
///
/// @return #VOIS_ERR_RESOURCE_BUSY when the driver is busy with another audio command,
///         #VOIS_ERR_NO if it can execute the command.
// =============================================================================
PUBLIC VOIS_ERR_T vois_Stop(VOID);




// =============================================================================
// vois_CalibUpdateValues
// -----------------------------------------------------------------------------
/// Update Vois related values depending on calibration parameters.
/// 
/// @return #VOIS_ERR_RESOURCE_BUSY, if the driver is busy with another audio command,
///         #VOIS_ERR_NO otherwise
// =============================================================================
PUBLIC VOIS_ERR_T vois_CalibUpdateValues(VOID);

#ifndef TARGET_MAGIC_SOUND_OFF
#define MAGIC_SOUND_SUPPORT
#endif

#ifdef MAGIC_SOUND_SUPPORT 
INT32 vois_set_ae( INT32 nAeID,  INT32 iValue);//jtAEParam nAeID
VOID vois_clear_ae(VOID);
#endif


///  @} <- End of the aps group 



#endif // _VOIS_M_H_

