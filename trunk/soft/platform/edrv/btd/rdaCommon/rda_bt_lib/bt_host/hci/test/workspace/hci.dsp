# Microsoft Developer Studio Project File - Name="hci" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=hci - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hci.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hci.mak" CFG="hci - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hci - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "hci - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hci - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x1809 /d "NDEBUG"
# ADD RSC /l 0x1809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "DEBUG" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x1809 /d "_DEBUG"
# ADD RSC /l 0x1809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "hci - Win32 Release"
# Name "hci - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=D:\devel\bt_host\hci\hci_callbacks.c

!IF  "$(CFG)" == "hci - Win32 Release"

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\hci\hci_flowctrl.c

!IF  "$(CFG)" == "hci - Win32 Release"

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\hci\hci_internals.c

!IF  "$(CFG)" == "hci - Win32 Release"

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\hci\hci_primitives.c

!IF  "$(CFG)" == "hci - Win32 Release"

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\hci\hci_tables.c

!IF  "$(CFG)" == "hci - Win32 Release"

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\hci\test\hci_test.c

!IF  "$(CFG)" == "hci - Win32 Release"

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\windows\win32\hci_transport\hci_transport_win32.c

!IF  "$(CFG)" == "hci - Win32 Release"

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\generic\host_buf.c

!IF  "$(CFG)" == "hci - Win32 Release"

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\manager\mgr_core.c

!IF  "$(CFG)" == "hci - Win32 Release"

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\manager\mgr_devdb.c

!IF  "$(CFG)" == "hci - Win32 Release"

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\manager\mgr_utils.c

!IF  "$(CFG)" == "hci - Win32 Release"

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\windows\win32\papi\papi_win32.c

!IF  "$(CFG)" == "hci - Win32 Release"

!ELSEIF  "$(CFG)" == "hci - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=D:\devel\bt_host\hci\include\hci_callbacks.h
# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\hci\include\hci_constants.h
# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\hci\include\hci_internals.h
# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\hci\include\hci_primitives.h
# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\hci\include\hci_tables.h
# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\hci\include\hci_transport.h
# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\include\host_error.h
# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\include\host_types.h
# End Source File
# Begin Source File

SOURCE=D:\devel\bt_host\include\papi.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
