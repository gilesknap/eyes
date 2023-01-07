// Twiddle.cpp : implementation file
//

#include "stdafx.h"
#include "eyes.h"
#include "Twiddle.h"
#include "CodeEdit.h"
#include "engine.h"
#include "files.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTwiddle dialog
POSITION	gCurrentPos;
int			gnPos;
CAnimal*	pCurrentAnimal;

CTwiddle::CTwiddle(CWnd* pParent /*=NULL*/)
	: CDialog(CTwiddle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTwiddle)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CTwiddle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTwiddle)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTwiddle, CDialog)
	//{{AFX_MSG_MAP(CTwiddle)
	ON_BN_CLICKED(IDC_SHOWCODE, OnShowcode)
	ON_BN_CLICKED(IDC_COPY, OnNew)
	ON_BN_CLICKED(IDC_SAVEFILE, OnSavefile)
	ON_BN_CLICKED(IDC_LOADFILE, OnLoadfile)
	ON_BN_CLICKED(IDDONE, OnDone)
	ON_BN_CLICKED(IDC_COLOUR, OnColour)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTwiddle message handlers

BOOL CTwiddle::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!Herbivores.IsEmpty())
	{
		gCurrentPos = Herbivores.GetHeadPosition();	
		gnPos = 1;
		pCurrentAnimal = (CAnimal*) Herbivores.GetNext(gCurrentPos);
		ShowAnimal(pCurrentAnimal);
	}
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetRange(0, Herbivores.GetCount() - 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTwiddle::ShowAnimal(CAnimal* pAnimal)
{

	CWnd		*pic = GetDlgItem(IDC_PICTURE);
	CClientDC	WDC(pic);
	CBrush		brush,*oldBrush;
	CPen		pen,*oldPen;
	CRect		rectDraw;

	brush.CreateSolidBrush(pAnimal->Colour);
	pen.CreatePen(PS_SOLID, 1, pAnimal->Colour);
	oldBrush = WDC.SelectObject(&brush);
	oldPen = WDC.SelectObject(&pen);
	pic->GetClientRect(&rectDraw);
	WDC.Rectangle(&rectDraw);
	WDC.SelectObject(oldBrush);
	WDC.SelectObject(oldPen);

	CString tmp;

	tmp.Format("%d",gnPos);
	GetDlgItem(IDC_Number)->SetWindowText(tmp);
	tmp.Format("%d",pAnimal->ID);
	GetDlgItem(IDC_CreatureID)->SetWindowText(tmp);
	tmp.Format("%d",pAnimal->Generation);
	GetDlgItem(IDC_CreatureGeneration)->SetWindowText(tmp);
	tmp.Format("%d",pAnimal->Species);
	GetDlgItem(IDC_CreatureSpecies)->SetWindowText(tmp);
	GetDlgItem(IDC_NAME)->SetWindowText(pAnimal->Name);

	tmp.Format("%d",pAnimal->Energy);
	GetDlgItem(IDC_ENERGY)->SetWindowText(tmp);
	tmp.Format("%d",pAnimal->XPos);
	GetDlgItem(IDC_X)->SetWindowText(tmp);
	tmp.Format("%d",pAnimal->YPos);
	GetDlgItem(IDC_Y)->SetWindowText(tmp);
	tmp.Format("%d",pAnimal->BreedAfter);
	GetDlgItem(IDC_BREED)->SetWindowText(tmp);
	tmp.Format("%d",pAnimal->MutationRate);
	GetDlgItem(IDC_MUTATE)->SetWindowText(tmp);

	
}



BOOL CTwiddle::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{

	OnUpdate();
	switch(wParam)
	{
		case IDC_SLIDER:
				if(!Herbivores.IsEmpty())
				{
					POSITION r = Herbivores.GetHeadPosition();
					gnPos = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->GetPos();
					for(int i = 0; i <= gnPos; i++)
						pCurrentAnimal = (CAnimal*) Herbivores.GetNext(r);
					ShowAnimal(pCurrentAnimal);
				}
				break;
	}
	

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CTwiddle::OnShowcode() 
{
	CCodeEdit	dlg;
	int			SavePos;	

	OnUpdate();

	dlg.m_CodeText = DisAssemble(pCurrentAnimal->Code,0,CODE_SIZE - 2);
	dlg.pCurrentAnimal = pCurrentAnimal;
	ShowWindow(SW_HIDE);
	dlg.DoModal();
	ShowWindow(SW_SHOW);

	// refresh the lot 'cos code window may have done debugging and 
	// our animal list may be invalid now.
	SavePos = gnPos;
	OnInitDialog();
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetPos(SavePos);
	InvalidateRect(NULL);

}

void CTwiddle::OnNew() 
{
	int		j;
	char	Text[20];
	CAnimal *pNewAnimal;

	OnUpdate();

	pNewAnimal = new CAnimal;

	pNewAnimal->Tracing = 0;
	pNewAnimal->ID = gnLives++;
	pNewAnimal->Generation = pCurrentAnimal->Generation + 1;
	pNewAnimal->Species = gnMutations++;
	pNewAnimal->Name = pCurrentAnimal->Name;
	GetDlgItem(IDC_ENERGY)->GetWindowText(Text,19);
    pNewAnimal->Energy = atoi(Text);
    pNewAnimal->XPos = (int)(rand() / ((double)RAND_MAX + 1) * (double)(xmax - XMIN)) + XMIN;
    pNewAnimal->YPos = (int)(rand() / ((double)RAND_MAX + 1) * (double)(ymax - YMIN)) + YMIN;
    pNewAnimal->Colour = pCurrentAnimal->Colour;
	GetDlgItem(IDC_MUTATE)->GetWindowText(Text,19);
    pNewAnimal->MutationRate = atoi(Text);
	GetDlgItem(IDC_BREED)->GetWindowText(Text,19);
    pNewAnimal->BreedAfter = atoi(Text);
	pNewAnimal->IP = pCurrentAnimal->IP;
	pNewAnimal->R = pCurrentAnimal->R;
    for(j = 0; j < CODE_SIZE; j++)
	{
        pNewAnimal->Code[j] = pCurrentAnimal->Code[j];
    }
	Herbivores.AddTail((CObject *)pNewAnimal);

	if(pNewAnimal->Generation > gnMaxGenerations)
		gnMaxGenerations = pNewAnimal->Generation;

	OnInitDialog();
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetPos(Herbivores.GetCount());
	InvalidateRect(NULL);
}

void CTwiddle::OnUpdate() 
{
	char	Text[20];

	Universe[pCurrentAnimal->XPos][pCurrentAnimal->YPos] = ID_BACKGROUND;
	GetDlgItem(IDC_ENERGY)->GetWindowText(Text,19);
    pCurrentAnimal->Energy = atoi(Text);
	GetDlgItem(IDC_X)->GetWindowText(Text,19);
    pCurrentAnimal->XPos = atoi(Text);
	GetDlgItem(IDC_Y)->GetWindowText(Text,19);
    pCurrentAnimal->YPos = atoi(Text);
	GetDlgItem(IDC_MUTATE)->GetWindowText(Text,19);
    pCurrentAnimal->MutationRate = atoi(Text);
	GetDlgItem(IDC_BREED)->GetWindowText(Text,19);
    pCurrentAnimal->BreedAfter = atoi(Text);
	GetDlgItem(IDC_NAME)->GetWindowText(Text,19);
    pCurrentAnimal->Name = Text;
	if(pCurrentAnimal->YPos >= ymax)
		pCurrentAnimal->YPos = ymax -1;
	if(pCurrentAnimal->XPos >= xmax)
		pCurrentAnimal->XPos = xmax -1;

}

void CTwiddle::OnSavefile() 
{
	OnUpdate();
	SaveAnimal(pCurrentAnimal);
}

void CTwiddle::OnLoadfile() 
{
	Universe[pCurrentAnimal->XPos][pCurrentAnimal->YPos] = ID_BACKGROUND;
	LoadAnimal(pCurrentAnimal);	
	int SavePos = gnPos;
	OnInitDialog();
    ((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetPos(SavePos);
	InvalidateRect(NULL);
}

void CTwiddle::OnDone() 
{
	OnUpdate();
	EndDialog(1);	
}

void CTwiddle::OnColour() 
{
	CColorDialog dlg(pCurrentAnimal->Colour);
	
	dlg.DoModal();
	
	pCurrentAnimal->Colour = dlg.GetColor();

	ShowAnimal(pCurrentAnimal);
}
