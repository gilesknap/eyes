// Optdlg.cpp : implementation file
//

#include "stdafx.h"
#include "eyes.h"
#include "Optdlg.h"
#include "guts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptdlg dialog

COptdlg gOptions;


COptdlg::COptdlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptdlg)
	m_Size = 5;
	m_GrassStart = 6000;
	m_GrassPoints = 180;
	m_RestCost = 3;
	m_MoveCost = 6;
	m_MaxBreed = 15000;
	m_MaxMutate = 100;
	m_MinBreed = 150;
	m_MinMutate = 1;
	m_StartBreed = 3000;
	m_StartMutate = 90;
	m_StartEnergy = 2100;
	m_StartHerbivores = 10;
	m_StartCarnivores = 0;
	m_CarnEase = 2;
	m_Ominvore = FALSE;
	m_FightCost = 30;
	m_RestCost2 = 1;
	m_MoveCost2 = 2;
	m_MaxBreed2 = 20000;
	m_MaxMutate2 = 100;
	m_MinBreed2 = 50;
	m_MinMutate2 = 1;
	m_StartBreed2 = 4500;
	m_StartMutate2 = 90;
	m_StartEnergy2 = 3000;
	m_RoundWorld = FALSE;
	//}}AFX_DATA_INIT
/*	m_Size = 0;
	m_GrassStart = 0;
	m_GrassPoints = 0;
	m_RestCost = 0;
	m_MoveCost = 0;
	m_MaxBreed = 0;
	m_MaxMutate = 0;
	m_MinBreed = 0;
	m_MinMutate = 0;
	m_StartBreed = 0;
	m_StartMutate = 0;
	m_StartEnergy = 0;
*/
}


void COptdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptdlg)
	DDX_Text(pDX, IDC_UNI_SIZE, m_Size);
	DDV_MinMaxInt(pDX, m_Size, 1, 10);
	DDX_Text(pDX, IDC_GRASS, m_GrassStart);
	DDV_MinMaxInt(pDX, m_GrassStart, 0, 100000);
	DDX_Text(pDX, IDC_GRASS_POINTS, m_GrassPoints);
	DDV_MinMaxInt(pDX, m_GrassPoints, 1, 10000);
	DDX_Text(pDX, IDC_REST_POINTS, m_RestCost);
	DDV_MinMaxInt(pDX, m_RestCost, 0, 100000);
	DDX_Text(pDX, IDC_MOVE_POINTS, m_MoveCost);
	DDV_MinMaxInt(pDX, m_MoveCost, 0, 100000);
	DDX_Text(pDX, IDC_MAXBREED, m_MaxBreed);
	DDV_MinMaxInt(pDX, m_MaxBreed, 0, 100000);
	DDX_Text(pDX, IDC_MAXMUTE, m_MaxMutate);
	DDV_MinMaxInt(pDX, m_MaxMutate, 0, 100);
	DDX_Text(pDX, IDC_MINBREED, m_MinBreed);
	DDV_MinMaxInt(pDX, m_MinBreed, 0, 100000);
	DDX_Text(pDX, IDC_MINMUTE, m_MinMutate);
	DDV_MinMaxInt(pDX, m_MinMutate, 0, 100);
	DDX_Text(pDX, IDC_STARTBREED, m_StartBreed);
	DDV_MinMaxInt(pDX, m_StartBreed, 0, 100000);
	DDX_Text(pDX, IDC_STARTMUTE, m_StartMutate);
	DDV_MinMaxInt(pDX, m_StartMutate, 0, 100);
	DDX_Text(pDX, IDC_START_ENERGY, m_StartEnergy);
	DDV_MinMaxInt(pDX, m_StartEnergy, 10, 100000);
	DDX_Text(pDX, IDC_HERB, m_StartHerbivores);
	DDV_MinMaxInt(pDX, m_StartHerbivores, 0, 10000);
	DDX_Text(pDX, IDC_CARN, m_StartCarnivores);
	DDV_MinMaxInt(pDX, m_StartCarnivores, 0, 10000);
	DDX_Check(pDX, IDC_OMNI, m_Ominvore);
	DDX_Text(pDX, IDC_FIGHT, m_FightCost);
	DDV_MinMaxInt(pDX, m_FightCost, 0, 10000);
	DDX_Text(pDX, IDC_REST_POINTS2, m_RestCost2);
	DDV_MinMaxInt(pDX, m_RestCost2, 0, 100000);
	DDX_Text(pDX, IDC_MOVE_POINTS2, m_MoveCost2);
	DDV_MinMaxInt(pDX, m_MoveCost2, 0, 100000);
	DDX_Text(pDX, IDC_MAXBREED2, m_MaxBreed2);
	DDV_MinMaxInt(pDX, m_MaxBreed2, 0, 100000);
	DDX_Text(pDX, IDC_MAXMUTE2, m_MaxMutate2);
	DDV_MinMaxInt(pDX, m_MaxMutate2, 0, 100);
	DDX_Text(pDX, IDC_MINBREED2, m_MinBreed2);
	DDV_MinMaxInt(pDX, m_MinBreed2, 0, 100000);
	DDX_Text(pDX, IDC_MINMUTE2, m_MinMutate2);
	DDV_MinMaxInt(pDX, m_MinMutate2, 0, 100);
	DDX_Text(pDX, IDC_STARTBREED2, m_StartBreed2);
	DDV_MinMaxInt(pDX, m_StartBreed2, 0, 100000);
	DDX_Text(pDX, IDC_STARTMUTE2, m_StartMutate2);
	DDV_MinMaxInt(pDX, m_StartMutate2, 0, 100);
	DDX_Text(pDX, IDC_START_ENERGY2, m_StartEnergy2);
	DDV_MinMaxInt(pDX, m_StartEnergy2, 10, 100000);
	DDX_Check(pDX, IDC_ROUND, m_RoundWorld);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptdlg, CDialog)
	//{{AFX_MSG_MAP(COptdlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptdlg message handlers

BOOL COptdlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	switch(wParam)
	{
	case IDOK:
		extern int lIterations;
		if(lIterations == 0)
		{
			UpdateData(TRUE);
			Startup();
		}
	}
	
	return CDialog::OnCommand(wParam, lParam);
}
