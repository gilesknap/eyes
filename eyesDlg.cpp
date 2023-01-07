// eyesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eyes.h"
#include "eyesDlg.h"
#include "twiddle.h"
#include "optdlg.h"
#include "files.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEyesDlg dialog

CEyesDlg::CEyesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEyesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEyesDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEyesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEyesDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEyesDlg, CDialog)
	//{{AFX_MSG_MAP(CEyesDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DISPLAY, OnDisplay)
	ON_BN_CLICKED(IDC_TWIDDLE, OnTwiddle)
	ON_BN_CLICKED(IDC_START, OnStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEyesDlg message handlers

BOOL CEyesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	((CSliderCtrl *)GetDlgItem( IDC_SLIDER ))->SetRange(0,30);
	((CSliderCtrl *)GetDlgItem( IDC_GRASS ))->SetRange(0,30);
	((CSliderCtrl *)GetDlgItem( IDC_SLIDER ))->SetPos(0);
	((CSliderCtrl *)GetDlgItem( IDC_GRASS ))->SetPos(GRASS_PER_TURN);

	Startup();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEyesDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}



// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEyesDlg::OnPaint() 
{
	int s = scale;

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting

		CRect RectPaint(XMIN*s,YMIN*s,(xmax-1)*s,(ymax-1)*s);

		RectPaint.IntersectRect(&(dc.m_ps.rcPaint),RectPaint);

		register int x,y;		
		for(x = RectPaint.left/s; x <= RectPaint.right/s; x++)
		{
			for(y = RectPaint.top/s; y <= RectPaint.bottom/s; y++)
			{
				DoDraw(x,y);
			}
		}

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEyesDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


BOOL CEyesDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if(wParam == IDC_SLIDER)
	{
		gnSpeed = ((CSliderCtrl *)GetDlgItem( IDC_SLIDER ))->GetPos();
	}
	else if(wParam == IDC_GRASS)
	{
		gGrassPerTurn = ((CSliderCtrl *)GetDlgItem( IDC_GRASS ))->GetPos();
	}
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}



void CEyesDlg::OnStart() 
{
	srand( (unsigned)time( NULL ) );

	if(((CEyesApp *)AfxGetApp())->bRunning)
	{
		((CEdit *)GetDlgItem( IDC_START ))->SetWindowText("Start");
		((CEyesApp *)AfxGetApp())->bRunning = FALSE;
		KillTimer(((CEyesApp *)AfxGetApp())->hTimer);
	}
	else
	{
		((CEdit *)GetDlgItem( IDC_START ))->SetWindowText("Stop");
		((CEyesApp *)AfxGetApp())->bRunning = TRUE;
		((CEyesApp *)AfxGetApp())->hTimer =  SetTimer(
			1, 8, OneGo );
	}
}

void CEyesDlg::OnDisplay() 
{
	gbDisplay = !gbDisplay;	
	if(gbDisplay)
	{
		((CEdit *)GetDlgItem( IDC_DISPLAY ))->SetWindowText("Display Off");
	}
	else
	{
		((CEdit *)GetDlgItem( IDC_DISPLAY ))->SetWindowText("Display On");
	}
	InvalidateRect(NULL,FALSE);
}


void CEyesDlg::OnTwiddle() 
{
	CTwiddle	twiddle;
	
	if(((CEyesApp *)AfxGetApp())->bRunning)
		KillTimer(((CEyesApp *)AfxGetApp())->hTimer);
	twiddle.DoModal();
	if(((CEyesApp *)AfxGetApp())->bRunning)
		((CEyesApp *)AfxGetApp())->hTimer =  SetTimer(
			1, 8, OneGo );
}



void CEyesDlg::OnClose() 
{
	if(AfxMessageBox("Do you really want to destroy an entire microcosm?",MB_OKCANCEL) != IDOK)
		return;
	KillTimer(((CEyesApp *)AfxGetApp())->hTimer);
	Shutdown();
	CDialog::OnClose();
}

BOOL CEyesDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if(((CEyesApp *)AfxGetApp())->bRunning)
		KillTimer(((CEyesApp *)AfxGetApp())->hTimer);
	
	switch(wParam)
	{
	case IDC_RESET:
		Startup();
		InvalidateRect(NULL,FALSE);
		break;
	case ID_FILE_EXIT:
		OnClose();
		break;
	case ID_FILE_STARTUPSETTINGS:
		gOptions.DoModal();
		break;
	case ID_FILE_SAVE:
		SaveUniverse();
		break;
	case ID_FILE_LOAD:
		LoadUniverse();
		break;
	case IDM_ABOUT:
		CAboutDlg dlg;
		dlg.DoModal();
		break;
	}
	
	if(((CEyesApp *)AfxGetApp())->bRunning)
		((CEyesApp *)AfxGetApp())->hTimer =  SetTimer(
			1, 8, OneGo );
	
	return CDialog::OnCommand(wParam, lParam);
}

