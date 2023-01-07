// CodeEdit.cpp : implementation file
//

#include "stdafx.h"
#include "eyes.h"
#include "guts.h"
#include "CodeEdit.h"
#include "engine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeEdit dialog


CCodeEdit::CCodeEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CCodeEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCodeEdit)
	m_CodeText = _T("");
	m_I1 = _T("");
	m_I2 = _T("");
	m_I3 = _T("");
	m_I4 = _T("");
	m_I5 = _T("");
	m_IP = _T("");
	m_R = _T("");
	m_M = _T("");
	m_X = _T("");
	m_Y = _T("");
	m_E = _T("");
	m_B = _T("");
	m_CodeStart = _T("0");
	//}}AFX_DATA_INIT
}


void CCodeEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	if(!pDX->m_bSaveAndValidate)
	{
		m_I1.Format("%x",pCurrentAnimal->vars[0]);
		m_I2.Format("%x",pCurrentAnimal->vars[1]);
		m_I3.Format("%x",pCurrentAnimal->vars[2]);
		m_I4.Format("%x",pCurrentAnimal->vars[3]);
		m_I5.Format("%x",pCurrentAnimal->vars[4]);
		m_IP.Format("%x",pCurrentAnimal->IP);
		m_R.Format("%x",pCurrentAnimal->R);
		m_M.Format("%x",pCurrentAnimal->MutationRate);
		m_X.Format("%x",pCurrentAnimal->XPos);
		m_Y.Format("%x",pCurrentAnimal->YPos);
		m_E.Format("%x",pCurrentAnimal->Energy);
		m_B.Format("%x",pCurrentAnimal->BreedAfter);
	}
	//{{AFX_DATA_MAP(CCodeEdit)
	DDX_Text(pDX, IDC_CODE_TEXT, m_CodeText);
	DDX_Text(pDX, IDC_I1, m_I1);
	DDX_Text(pDX, IDC_I2, m_I2);
	DDX_Text(pDX, IDC_I3, m_I3);
	DDX_Text(pDX, IDC_I4, m_I4);
	DDX_Text(pDX, IDC_I5, m_I5);
	DDX_Text(pDX, IDC_IP, m_IP);
	DDX_Text(pDX, IDC_R, m_R);
	DDX_Text(pDX, IDC_M, m_M);
	DDX_Text(pDX, IDC_X, m_X);
	DDX_Text(pDX, IDC_Y, m_Y);
	DDX_Text(pDX, IDC_E, m_E);
	DDX_Text(pDX, IDC_B, m_B);
	DDX_Text(pDX, ID_START_POINT, m_CodeStart);
	//}}AFX_DATA_MAP

	if(pDX->m_bSaveAndValidate)
	{

		sscanf(m_I1,"%x",&pCurrentAnimal->vars[0]);
		sscanf(m_I2,"%x",&pCurrentAnimal->vars[1]);
		sscanf(m_I3,"%x",&pCurrentAnimal->vars[2]);
		sscanf(m_I4,"%x",&pCurrentAnimal->vars[3]);
		sscanf(m_I5,"%x",&pCurrentAnimal->vars[4]);
		sscanf(m_IP,"%x",&pCurrentAnimal->IP);
		sscanf(m_R,"%x",&pCurrentAnimal->R);
		sscanf(m_M,"%x",&pCurrentAnimal->MutationRate);
		sscanf(m_X,"%x",&pCurrentAnimal->XPos);
		sscanf(m_Y,"%x",&pCurrentAnimal->YPos);
		sscanf(m_E,"%x",&pCurrentAnimal->Energy);
		sscanf(m_B,"%x",&pCurrentAnimal->BreedAfter);
	}



}


BEGIN_MESSAGE_MAP(CCodeEdit, CDialog)
	//{{AFX_MSG_MAP(CCodeEdit)
	ON_BN_CLICKED(IDSTEP, OnStep)
	ON_BN_CLICKED(ID_SAVE, OnSave)
	ON_BN_CLICKED(IDC_SETSTART, OnSetstart)
	ON_BN_CLICKED(IDRUN, OnRun)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDDONE, OnDone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeEdit message handlers



void CCodeEdit::OnStep() 
{
	int end;
	BOOL d = gbDisplay;

	UpdateData(TRUE);

	gbDisplay = TRUE;
	pCurrentAnimal->Tracing = 1;
	OneGo(NULL,0,0,0);
	if(pCurrentAnimal->Tracing == 2)
	{
		delete pCurrentAnimal;
		EndDialog(TRUE);
	}
	pCurrentAnimal->Tracing = 0;
	gbDisplay = d;

	end = pCurrentAnimal->IP + 50;
	if(end > CODE_SIZE -2) end = CODE_SIZE - 2;
	m_CodeText = DisAssemble(pCurrentAnimal->Code,pCurrentAnimal->IP,end);
	UpdateData(FALSE);
}

void CCodeEdit::OnSave() 
{
	UpdateData(TRUE);
	Assemble(m_CodeText, pCurrentAnimal->Code, CODE_SIZE+3);
}

void CCodeEdit::OnSetstart() 
{
	int		start;
	CString	Text;

	((CEdit*)GetDlgItem(ID_START_POINT))->GetWindowText(Text);
	sscanf(Text,"%x",&start);

	m_CodeText = DisAssemble(pCurrentAnimal->Code,start,CODE_SIZE-2);
	UpdateData(FALSE);
	
}

void CCodeEdit::OnRun() 
{
	static UINT hTimer;
	static BOOL bRunning = FALSE;

	if(bRunning)
	{
		((CEdit *)GetDlgItem( IDRUN ))->SetWindowText("Run");
		bRunning = FALSE;
		KillTimer(hTimer);
	}
	else
	{
		((CEdit *)GetDlgItem( IDRUN ))->SetWindowText("Stop");
		bRunning = TRUE;
		hTimer = SetTimer(10, 10, NULL);
	}
}

void CCodeEdit::OnTimer(UINT nIDEvent) 
{
	OnStep();
	
	CDialog::OnTimer(nIDEvent);
}

void CCodeEdit::OnDone() 
{
	UpdateData(TRUE);
	Assemble(m_CodeText, pCurrentAnimal->Code, CODE_SIZE+3);
	EndDialog(1);	

}
