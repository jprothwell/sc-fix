/*************************************************************************/
/*                                                                                                                            */
/*            Copyright (C) 2007, Coolsand Technologies, Inc.                                          */
/*                            All Rights Reserved                                                                    */
/*                                                                                                                            */
/*************************************************************************/
/* BY jixj******************************************************************/

#ifndef _FM_DRV_I2C_H


#define _FM_DRV_I2C_H
//
//-------------------------------------------------------------------
// Function:  HWInitI2C
// Purpose:  This function is used to init I2C port of the FM device   
// In: 
// Return:    bool

//-------------------------------------------------------------------
bool HWInitI2C(u8 scl, u8 sda);



//
//-------------------------------------------------------------------
// Function:  I2SendData
// Purpose:  This function is used to Send a bunch of data to I2C bus 
// In: pSendBuff, the buffer save the data ready to send
//     iSendCount, the byte count ready to send 
//     iDeviceAddr, the device Addr  
//     iDeviceSubAddr, the device sub Addr
//     bHaveSubAddr,  check if need to seek the sub Addr
// Return:   pSendCount ,the count actually send
             

//-------------------------------------------------------------------
bool I2CSendData(u8 iDeviceAddr,u8 iDeviceSubAddr,u8 *pSendBuff,u8 iSendCount,u8 *pSendCount,bool bHaveSubAddr);

//-------------------------------------------------------------------
// Function:  I2RecvData
// Purpose:  This function is used to receive a bunch of data from I2C bus 
// In: pRecvBuff, the buffer save the recv data
//     iRecvCount, the byte count prepare to receive 
//     iDeviceAddr, the device Addr  
//     iDeviceSubAddr, the device sub Addr
//     bHaveSubAddr,  check if need to seek the sub Addr
// Return:   pSendCount ,the count actually send
//--------------------------------------------------------------------
bool I2CRecvData(u8 iDeviceAddr,u8 iDeviceSubAddr,u8 *pRecvBuff,u8 iRecvCount,bool bHaveSubAddr);

#endif
