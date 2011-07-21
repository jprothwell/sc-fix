; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=7
Class1=CNetworkSimulatorApp
LastClass=CSelectMessageDlg
NewFileInclude2=#include "NetworkSimulator.h"
ResourceCount=7
NewFileInclude1=#include "stdafx.h"
Class2=CSendMessageDlg
LastTemplate=CDialog
Class3=CMainDialog
Resource1=IDD_DLG_ASYNC_MESSAGE
Class4=CGenericInput
Resource2=IDR_MAINFRAME
Resource3=IDD_DIALOG_GENERIC_INPUT
Resource4=IDD_DLG_BOOT_MESSAGE
Class5=CAsyncMessageDlg
Class6=CBootMessageDlg
Resource5=IDD_DLG_SEND_MESSAGE
Resource6=IDD_DLG_NETWORK_SIMULATOR
Class7=CSelectMessageDlg
Resource7=IDD_DLG_SELECT_MESSAGE

[CLS:CNetworkSimulatorApp]
Type=0
HeaderFile=NetworkSimulator.h
ImplementationFile=NetworkSimulator.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CNetworkSimulatorApp

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_MENU_EXIT
Command2=IDM_SEND_MESSAGES
Command3=IDM_MODIFY_BOOT_MSG
Command4=IDM_MODIFY_ASYNC_MESSAGES_SEQUENCE
Command5=ID_HELP_CONTENTS
Command6=ID_ABOUT_ME
CommandCount=6

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_BUTTON_MSG1
Command2=ID_BUTTON_MSG2
Command3=ID_BUTTON_MSG3
Command4=ID_BUTTON_MSG4
Command5=ID_BUTTON_MSG5
Command6=ID_BUTTON_MSG6
Command7=ID_BUTTON_MSG7
Command8=ID_BUTTON_MSG8
Command9=ID_BUTTON_MSG9
Command10=ID_BUTTON_MSG10
CommandCount=10

[DLG:IDD_DIALOG_GENERIC_INPUT]
Type=1
Class=CGenericInput
ControlCount=14
Control1=ID_BUTTON_MAKEBUFFER,button,1342242817
Control2=IDC_LIST_DATASET_DETAILS,SysListView32,1350633473
Control3=IDC_EDIT_OUTGOING_MSG_NAME,edit,1484849280
Control4=IDC_EDIT_OUTGOING_MSGID,edit,1484849280
Control5=IDC_EDIT_INCOMING_MSG_NAME,edit,1484849280
Control6=IDC_EDIT_INCOMING_MSG_ID,edit,1484849280
Control7=IDC_STATIC_OUTGOING_MSG_NAME,static,1342308352
Control8=IDC_STATIC_OUTGOING_MSG_ID,static,1342308352
Control9=IDC_STATIC_INCOMING_MSG_NAME,static,1342308352
Control10=IDC_STATIC_INCOMING_MSG_ID,static,1342308352
Control11=IDC_STATIC_DS_DETAILS,static,1342308352
Control12=IDC_STATIC_INC_MSG_DETAILS,button,1342177287
Control13=IDC_STATIC_OUTGOING_MSG_DETAILS,button,1342177287
Control14=IDC_BUTTON_ONCLOSE,button,1342242816

[CLS:CGenericInput]
Type=0
HeaderFile=GenericInput.h
ImplementationFile=GenericInput.cpp
BaseClass=CDialog
Filter=W
LastObject=CGenericInput
VirtualFilter=dWC

[DLG:IDD_DLG_ASYNC_MESSAGE]
Type=1
Class=CAsyncMessageDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LISTE_STOCK_MESSAGE,listbox,1352728841
Control4=IDC_LISTE_ASYNC_MESSAGE,listbox,1352728841
Control5=IDC_BTN_ADD,button,1476460544
Control6=IDC_BTN_DELETE,button,1476460544
Control7=IDC_STATIC,static,1342308352
Control8=IDC_COMBO_AYSNC_NAME,combobox,1344339971
Control9=IDC_BTN_ADD_MESSAGE,button,1342242816
Control10=IDC_BTN_DELETE_MESSAGE,button,1342242816

[DLG:IDD_DLG_BOOT_MESSAGE]
Type=1
Class=CBootMessageDlg
ControlCount=4
Control1=IDC_STATIC_GROUP,button,1342177287
Control2=IDC_TREE_BOOT,SysTreeView32,1350631719
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[DLG:IDD_DLG_SEND_MESSAGE]
Type=1
Class=CSendMessageDlg
ControlCount=4
Control1=IDC_STATIC_GROUP,button,1342177287
Control2=IDC_TREE_BOOT,SysTreeView32,1350631463
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816

[CLS:CAsyncMessageDlg]
Type=0
HeaderFile=AsyncMessageDlg.h
ImplementationFile=AsyncMessageDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CAsyncMessageDlg

[CLS:CBootMessageDlg]
Type=0
HeaderFile=dialog\bootmessagedlg.h
ImplementationFile=dialog\bootmessagedlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CBootMessageDlg

[CLS:CSendMessageDlg]
Type=0
HeaderFile=dialog\sendmessagedlg.h
ImplementationFile=dialog\sendmessagedlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CSendMessageDlg

[DLG:IDD_DLG_NETWORK_SIMULATOR]
Type=1
Class=CMainDialog
ControlCount=4
Control1=IDC_STATIC_INC_MSGS,button,1342177287
Control2=IDC_LIST_INCOMING_MESSAGES,SysListView32,1350631425
Control3=IDC_STATIC_OUTGOING_MSGS,button,1342177287
Control4=IDC_LIST_OUTGOING_MESSAGES,SysListView32,1350631425

[CLS:CMainDialog]
Type=0
HeaderFile=MainDialog.h
ImplementationFile=MainDialog.cpp
BaseClass=CDialog
Filter=W
LastObject=CMainDialog
VirtualFilter=dWC

[DLG:IDD_DLG_SELECT_MESSAGE]
Type=1
Class=CSelectMessageDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_MESSAGE_ID,edit,1484849280
Control4=IDC_COMBO_MSG_RESPONSE,combobox,1344340226
Control5=IDC_STATIC_RESP_MSG,static,1342308352
Control6=IDC_STATIC_MSGID,static,1342308352
Control7=IDC_COMBO_MSG_RESPONSE_VALUE,combobox,1344340226
Control8=IDC_STATIC_RESP_VAL,static,1342308352
Control9=IDC_STATIC_MSG_DETAIL,button,1342177287
Control10=IDC_COMBO_MSG_RESPONSE2,combobox,1344340226

[CLS:CSelectMessageDlg]
Type=0
HeaderFile=Dialog\SelectMessageDlg.h
ImplementationFile=Dialog\SelectMessageDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CSelectMessageDlg

