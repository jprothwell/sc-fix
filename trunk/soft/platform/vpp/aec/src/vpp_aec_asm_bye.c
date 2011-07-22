
#include "vppp_aec_asm_common.h"
#include "vppp_aec_asm_map.h"
#include "vppp_aec_asm_sections.h"

#include "voc2_library.h"
#include "voc2_define.h"

// VoC_directive: PARSER_OFF
#include <stdio.h>
// VoC_directive: PARSER_ON


#if 0

    voc_word  GLOBAL_BYE,x
    voc_short GLOBAL_BYE2,y

#endif


// ************************************************
//   Function:     vpp_HelloBye
//   Description:  Say "Bye"
//   Inputs:
//   Outputs:
//   Used :        all
//   Version 1.0   Create by Yordan  24/12/2008
//   **********************************************

void vpp_HelloBye(void)
{

#if 0

    voc_short LOCAL_BYE[4],x
    voc_word  LOCAL_BYE2,y

#endif


// VoC_directive: PARSER_OFF
    printf("The VPP Hello says Bye\n");
// VoC_directive: PARSER_ON


    VoC_lw16i(REG6,VPP_HELLO_SAYS_BYE);
    VoC_lw16i_short(REG7,0,DEFAULT);
    VoC_NOP();
    VoC_sw32_d(REG67,GLOBAL_HELLO_STATUS);

    // for test purposes
    VoC_push16(RA,DEFAULT);
    VoC_jal(vpp_HelloByeTest);
    VoC_pop16(RA,DEFAULT);
    VoC_NOP();

    VoC_return;
}

void vpp_HelloByeTest(void)
{
    // for test purposes
    VoC_sw32_d(REG67,LOCAL_BYE2);
    VoC_sw32_d(REG67,LOCAL_BYE);
    VoC_sw32_d(REG67,LOCAL_BYE+2);

    VoC_return;
}
