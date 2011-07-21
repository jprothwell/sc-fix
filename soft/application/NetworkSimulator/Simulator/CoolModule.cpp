// CoolModule.cpp: implementation of the CCoolModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoolModule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoolModule::CCoolModule()
{

}

CCoolModule::~CCoolModule()
{

}

CString CCoolModule::GetModulePath()
{
  //获得当前可执行程序的目录
  char szFileName[_MAX_PATH] = {0};
	char szDir[_MAX_PATH] = {0};
	char szDriver[_MAX_PATH] = {0};
	GetModuleFileNameA(NULL, szFileName, _MAX_PATH-1);
	_splitpath(szFileName, szDriver, szDir, NULL, NULL);
	CString strExeDir = CString(szDriver) + szDir;

  return strExeDir;
}
