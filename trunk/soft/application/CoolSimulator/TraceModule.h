// TraceModule.h: interface for the CTraceModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACEMODULE_H__2B0FC51A_554D_407D_8A04_D35808A864E0__INCLUDED_)
#define AFX_TRACEMODULE_H__2B0FC51A_554D_407D_8A04_D35808A864E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTraceModule  
{
public:
	CTraceModule();
	virtual ~CTraceModule();
public:
  static void DebugWindowAlloc(HWND hWnd);
  static BOOL CtrlHandler(unsigned int fdwCtrlType);
  static void DisplayToDebugwindow(char *nPrintableStr);
};

#endif // !defined(AFX_TRACEMODULE_H__2B0FC51A_554D_407D_8A04_D35808A864E0__INCLUDED_)
