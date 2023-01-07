#if !defined(AFX_TWIDDLE_H__70255C43_BCA8_11D0_BC4E_00C04FE00E9B__INCLUDED_)
#define AFX_TWIDDLE_H__70255C43_BCA8_11D0_BC4E_00C04FE00E9B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Twiddle.h : header file
//

#include "guts.h"

/////////////////////////////////////////////////////////////////////////////
// CTwiddle dialog

class CTwiddle : public CDialog
{
// Construction
public:
	CTwiddle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTwiddle)
	enum { IDD = IDD_TWIDDLE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTwiddle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	void OnUpdate();
	void ShowAnimal(CAnimal* pAnimal);

	// Generated message map functions
	//{{AFX_MSG(CTwiddle)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowcode();
	afx_msg void OnNew();
	afx_msg void OnSavefile();
	afx_msg void OnLoadfile();
	afx_msg void OnDone();
	afx_msg void OnColour();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TWIDDLE_H__70255C43_BCA8_11D0_BC4E_00C04FE00E9B__INCLUDED_)
