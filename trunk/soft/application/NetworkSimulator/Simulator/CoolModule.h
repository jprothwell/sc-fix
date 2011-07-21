// CoolModule.h: interface for the CCoolModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COOLMODULE_H__521DE7A5_5BCF_4DE6_A87B_BB2EC8B5559D__INCLUDED_)
#define AFX_COOLMODULE_H__521DE7A5_5BCF_4DE6_A87B_BB2EC8B5559D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCoolModule  
{
public:
	CCoolModule();
	virtual ~CCoolModule();
public:
  static CString GetModulePath();       //Get the module path
};

#endif // !defined(AFX_COOLMODULE_H__521DE7A5_5BCF_4DE6_A87B_BB2EC8B5559D__INCLUDED_)
