# Microsoft Developer Studio Project File - Name="NetworkSimulator" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=NetworkSimulator - Win32 Dual Sim Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NetworkSimulator.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NetworkSimulator.mak" CFG="NetworkSimulator - Win32 Dual Sim Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NetworkSimulator - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NetworkSimulator - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NetworkSimulator - Win32 Dual Sim Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NetworkSimulator - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "UNICODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\SimGreenstone2\application\coolmmi\CoolSimulator\Bin\NetworkSimulator.dll"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Release\NetworkSimulator.lib ..\CoolSimulator\Lib\NetworkSimulatorR.lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "NetworkSimulator - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\CoolSimulator\Bin\NetworkSimulator.dll" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Debug\NetworkSimulator.lib ..\CoolSimulator\Lib\NetworkSimulatorD.lib
# End Special Build Tool

!ELSEIF  "$(CFG)" == "NetworkSimulator - Win32 Dual Sim Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "NetworkSimulator___Win32_Dual_Sim_Debug"
# PROP BASE Intermediate_Dir "NetworkSimulator___Win32_Dual_Sim_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Dual_Sim_Debug"
# PROP Intermediate_Dir "Dual_Sim_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "UNICODE" /D "_DUAL_SIM_" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\SimGreenstone2\soft\application\coolmmi\CoolSimulator\Bin\NetworkSimulator.dll" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"..\CoolSimulator\Bin\NetworkSimulator.dll" /pdbtype:sept
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Dual_Sim_Debug\NetworkSimulator.lib ..\CoolSimulator\Lib\NetworkSimulatorD.lib
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "NetworkSimulator - Win32 Release"
# Name "NetworkSimulator - Win32 Debug"
# Name "NetworkSimulator - Win32 Dual Sim Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Simulator\AmLog.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\Application.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\AsyncMessageSeq.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\BootMessageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\BootUpMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\CallMgmtIncoming.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\CoolModule.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\Data.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\DataList.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\DataType.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\DCMLHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\Format.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\FreqUsedMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\GCMLGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\IncomingMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\InPlaceSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\ListDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MainSimulator.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\MemAllocator.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\Message.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\MessageManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\Messages.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\MyEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkSimulator.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkSimulator.def
# End Source File
# Begin Source File

SOURCE=.\NetworkSimulator.rc
# End Source File
# Begin Source File

SOURCE=.\Simulator\NSRThread.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\NSWThread.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\NWGCMLHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\NWSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\ObjectSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\OutgoingMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\PhoneBook.cpp
# End Source File
# Begin Source File

SOURCE=.\Dialog\SelectMessageDlg.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\Dialog\SendmessageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\Settings.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\SimAppToolkit.cpp
# End Source File
# Begin Source File

SOURCE=.\SimulatorStart.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Dialog\TreeCtrlEnhanced.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\Value.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\Var.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator\XMLVerifier.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Simulator\AmLog.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\Application.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\AsyncMessageSeq.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\BootMessageDlg.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\BootUpMsg.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\CallMgmtStruct.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\CoolModule.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\Data.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\DataList.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\DataType.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\DCMLHandler.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\Format.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\FreqUsedMsg.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\GCMLDefines.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\GCMLGenerator.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\GlobalConstants.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\GlobalDefines.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\IncomingMsg.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\InPlaceSelect.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\ListDrop.h
# End Source File
# Begin Source File

SOURCE=.\MainDialog.h
# End Source File
# Begin Source File

SOURCE=.\MainSimulator.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\MemAllocator.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\Message.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\MessageManager.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\Messages.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\MyEdit.h
# End Source File
# Begin Source File

SOURCE=.\NetworkSimulator.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\NSRThread.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\NSWThread.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\NWGCMLHandler.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\NWSocket.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\ObjectSearch.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\OutgoingMessage.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\PhoneBookAPI.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\ProtocolEvents.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\QueueGprot.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SelectMessageDlg.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\SendmessageDlg.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\Settings.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\SimAppToolkit.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorStart.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\TaskInit.h
# End Source File
# Begin Source File

SOURCE=.\Dialog\TreeCtrlEnhanced.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\Value.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\Var.h
# End Source File
# Begin Source File

SOURCE=.\Simulator\XMLVerifier.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ToolBitmap\2ndCall.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\battery.bmp
# End Source File
# Begin Source File

SOURCE=.\bootup.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\bootup.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\call_barr_resp.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\call_fwd.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\call_hist.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\CBSMS.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\CHLD.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\class0.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\cphs.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\dial_ind.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\dropcall.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\dtmf.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\fwd_resp.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\gpio.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\incall.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\incoming.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\li.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NetworkSimulator.rc2
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\new_sms.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\no_sim.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\NWTZCHGINFO.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\outgoing.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\outgoing_connect.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\PBook.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\plnm_status.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\pnl_ind.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\SAT.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\sat_disp.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\sat_inkey.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\sat_input.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\sat_sel.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\sgt_ind.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\sl_ind.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\SS_Not.bmp
# End Source File
# Begin Source File

SOURCE=.\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\USB.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\USSD.bmp
# End Source File
# Begin Source File

SOURCE=.\ToolBitmap\wtg_msg.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
