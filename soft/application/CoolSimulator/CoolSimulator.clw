; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLayerWnd
LastTemplate=generic CWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "CoolSimulator.h"

ClassCount=5
Class1=CCoolSimulatorApp
Class2=CCoolSimulatorDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_COOLSIMULATOR_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Resource4=IDD_LAYER_DLG
Class4=CLayerDlg
Class5=CLayerWnd
Resource5=IDR_MENU_PHONE

[CLS:CCoolSimulatorApp]
Type=0
HeaderFile=CoolSimulator.h
ImplementationFile=CoolSimulator.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CCoolSimulatorApp

[CLS:CCoolSimulatorDlg]
Type=0
HeaderFile=CoolSimulatorDlg.h
ImplementationFile=CoolSimulatorDlg.cpp
Filter=D
LastObject=CCoolSimulatorDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=CoolSimulatorDlg.h
ImplementationFile=CoolSimulatorDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_COOLSIMULATOR_DIALOG]
Type=1
Class=CCoolSimulatorDlg
ControlCount=2
Control1=IDC_STATIC_COORDINATE,static,1342308352
Control2=IDC_COOLPHONECTRL,{AB9D831B-277B-4927-8F89-C1CBBEB9E611},1342242816

[MNU:IDR_MENU_PHONE]
Type=1
Class=?
Command1=IDM_FILE_SAVE_WINDOW_POS
Command2=IDM_FILE_NETWORK_SIMULATOR
Command3=ID_FILE_Exit
Command4=IDM_TOOL_LOAD_SKIN
Command5=IDM_TOOLS_LAYER_WATCH
Command6=IDM_TOOL_INFOMONITOR
Command7=IDM_TOOLS_EXT_KEYPAD
Command8=IDD_ABOUTBOX
CommandCount=8

[DLG:IDD_LAYER_DLG]
Type=1
Class=CLayerDlg
ControlCount=4
Control1=IDC_STATIC_LAYER1,static,1342177286
Control2=IDC_STATIC_LAYER2,static,1342177286
Control3=IDC_STATIC_LAYER3,static,1342177286
Control4=IDC_STATIC_LAYER4,static,1342177286

[CLS:CLayerDlg]
Type=0
HeaderFile=LayerDlg.h
ImplementationFile=LayerDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_STATIC_LAYER1
VirtualFilter=dWC

[CLS:CLayerWnd]
Type=0
HeaderFile=LayerWnd.h
ImplementationFile=LayerWnd.cpp
BaseClass=CWnd
Filter=W
VirtualFilter=WC

