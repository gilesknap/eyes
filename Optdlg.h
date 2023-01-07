#if !defined(AFX_OPTDLG_H__16F5BB53_BE45_11D0_BC4F_000000000000__INCLUDED_)
#define AFX_OPTDLG_H__16F5BB53_BE45_11D0_BC4F_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Optdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptdlg dialog

class COptdlg : public CDialog
{
// Construction
public:
	COptdlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptdlg)
	enum { IDD = IDD_OPTIONS };
	int		m_Size;
	int		m_GrassStart;
	int		m_GrassPoints;
	int		m_RestCost;
	int		m_MoveCost;
	int		m_MaxBreed;
	int		m_MaxMutate;
	int		m_MinBreed;
	int		m_MinMutate;
	int		m_StartBreed;
	int		m_StartMutate;
	int		m_StartEnergy;
	int		m_StartHerbivores;
	int		m_StartCarnivores;
	int		m_CarnEase;
	BOOL	m_Ominvore;
	int		m_FightCost;
	int		m_RestCost2;
	int		m_MoveCost2;
	int		m_MaxBreed2;
	int		m_MaxMutate2;
	int		m_MinBreed2;
	int		m_MinMutate2;
	int		m_StartBreed2;
	int		m_StartMutate2;
	int		m_StartEnergy2;
	BOOL	m_RoundWorld;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptdlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptdlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern COptdlg gOptions;

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTDLG_H__16F5BB53_BE45_11D0_BC4F_000000000000__INCLUDED_)


