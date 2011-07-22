#ifndef CFW_SIM_SAT_H
#define CFW_SIM_SAT_H

#define RESPONSEBASE            0x0C
#define RESPONSEITEM            0x03
#define RESPONSETEXT            0x03
#define RESPONSEDURA            0x04
#define RESPONSELOCAL           0x09
#define RESPONSEUSSD            0x03

#define RESPONSECHANNEL         0x04
#define RESPONSEIMEI            0x0A
#define RESPONSEMEASURE         0x12
#define RESPONSEDATETIME        0X09
#define RESPONSELANGUAGE        0x04
#define RESPONSETADVANCE        0x04

#define RESPONSECARDID          0x03
#define RESPONSECARDSTATUS      0x03


#define ComType                 0x03
#define ComQual                 0x04
#define ComResult               0x0B

#define TextLength              0x01
#define TextScheme              0x02

#define ItemSelection           0x02

#define ItemNumber              0x08        //if Length occupy two byte,this value should add one.

#define MCC_MNC                 0x02
#define LAC                     0x05
#define CellID                  0x07

#define ChannelStatus           0x02
#define IMEI                    0x02
#define Measurement             0x02
#define DateTime                0x02
#define Language                0x02
#define MEStatus                0x02
#define TimingAdvance           0x03

#define CardID_Length           0x01
#define CardID                  0x02

#define CardStatus              0x02

//typedef struct SAT_CardStatus
//{
//    UINT8 Status;   //bits 1-3    = identity of card reader x.
//                    //bit 4        0 = Card reader is not removable
//                    //            1 = Card reader is removable
//                    //bit 5        0 = Card reader is not present
//                    //            1 = Card reader is present
//                    //bit 6        0 = Card reader present is not ID-1 size 
//                    //            1 = Card reader present is ID-1 size
//                    //bit 7        0 = No card present 
//                    //            1 = Card is present in reader 
//                    //bit 8        0 = No card powered
//                    //            1 = Card in reader is powered
//}SAT_CardStatus_t;


//Proactive command definition
#define SAT_PRO_COM_SCALL           0x10
#define SAT_PRO_COM_SSMS            0X13
#define SAT_PRO_COM_DTEXT           0x21
#define SAT_PRO_COM_GINPUT          0x23
#define SAT_PRO_COM_SITEM           0x24
#define SAT_PRO_COM_SMENU           0x25
#define SAT_PRO_COM_PLINFO          0x26
#define SAT_PRO_COM_GRSTATUS        0x33
#define SAT_PRO_COM_GCSTATUS        0x44

//Status of a proactive command
#define SAT_STATUS_SUCCESS_NOMAL    0x00
#define SAT_STATUS_SUCCESS_PARTIAL  0x01
#define SAT_STATUS_SUCCESS_MISSINFO 0x02
#define SAT_STATUS_SUCCESS_LIMITSER 0x06
#define SAT_STATUS_TERMINAL_USER    0x10
#define SAT_STATUS_BACKWARDMOVE     0x11
#define SAT_STATUS_NORESPONSE       0x12
#define SAT_STATUS_HELP_INFO        0x13

#define SAT_STATUS_ME_NOPROCESS     0x20
#define SAT_STATUS_NETWORK_NOPRO    0x21
#define SAT_STATUS_USER_NOT_ACCEPT  0x22

#define SAT_STATUS_COM_BEYONG_ME    0x30
#define SAT_STATUS_COMTYPE_UNKOWN   0x31
#define SAT_STATUS_COMDATA_UNKOWN   0x32
#define SAT_STATUS_COMNUM_UNKOWN    0x33


#define SM_ACT_INIT                 0xFF
#define SM_ACT_IDLE                 0x01
#define SM_ACT_TERMINALPROFILE      0x02
#define SM_ACT_FETCH                0x03
#define SM_ACT_TERMINALRESPONSE     0x04



#define SM_RESP_INIT                0xFF
#define SM_RESP_IDLE                0x01
#define SM_RESP_TERMINALRESPONSE    0x02
#define SM_RESP_FETCH               0x03
#define SM_RESP_TERMINALENVELOPE    0x04

extern CFW_PROFILE nMEProfile;



#endif

