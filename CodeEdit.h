#if !defined(AFX_CODEEDIT_H__70255C44_BCA8_11D0_BC4E_00C04FE00E9B__INCLUDED_)
#define AFX_CODEEDIT_H__70255C44_BCA8_11D0_BC4E_00C04FE00E9B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CodeEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCodeEdit dialog

class CCodeEdit : public CDialog
{
// Construction
public:
	CCodeEdit(CWnd* pParent = NULL);   // standard constructor
	CAnimal *pCurrentAnimal;

// Dialog Data
	//{{AFX_DATA(CCodeEdit)
	enum { IDD = IDD_CODE_EDIT };
	CString	m_CodeText;
	CString	m_I1;
	CString	m_I2;
	CString	m_I3;
	CString	m_I4;
	CString	m_I5;
	CString	m_IP;
	CString	m_R;
	CString	m_M;
	CString	m_X;
	CString	m_Y;
	CString	m_E;
	CString	m_B;
	CString	m_CodeStart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCodeEdit)
	afx_msg void OnStep();
	afx_msg void OnSave();
	afx_msg void OnSetstart();
	afx_msg void OnRun();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEEDIT_H__70255C44_BCA8_11D0_BC4E_00C04FE00E9B__INCLUDED_)
