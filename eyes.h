// eyes.h : main header file for the EYES application
//

#if !defined(AFX_EYES_H__789B1185_B803_11D0_BC44_00C04FE00E9B__INCLUDED_)
#define AFX_EYES_H__789B1185_B803_11D0_BC44_00C04FE00E9B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEyesApp:
// See eyes.cpp for the implementation of this class
//

class CEyesApp : public CWinApp
{
public:
	CEyesApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEyesApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

	BOOL	bRunning;
	int     hTimer;

// Implementation

	//{{AFX_MSG(CEyesApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EYES_H__789B1185_B803_11D0_BC44_00C04FE00E9B__INCLUDED_)
