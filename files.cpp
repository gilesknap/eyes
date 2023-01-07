#include "stdafx.h"
#include <stdio.h>
#include "eyes.h"
#include "guts.h"
#include "optdlg.h"
#include "engine.h"

CString ConvertToHex(byte *buf, int bufsize)
{
	CString ret;
	char    *result;
	int i;

	result = new char[bufsize *3 +10];

	for(i = 0; i < bufsize; i++)
	{
		sprintf(&(result[i*3]),"%02x ",buf[i]);
	}

	ret = result;

	delete result;

	return ret;
}

void ConvertFromHex(CString hex, byte *buf, int bufsize)
{
	int i;
	CString nextByte;
	int val;

	for(i = 0; i < bufsize; i++)
	{
		nextByte = hex.Mid(i*3,3);
		if(nextByte.GetLength() == 0) return;
		sscanf(nextByte,"%2x ",&val);
		buf[i] = (byte)val;
	}
}


void SaveAnimal(CAnimal *pAnimal, CString *sFile = NULL, CString *sect = NULL)
{
	char					str[100];					
	static CFileDialog		dlg(FALSE, "ORG", NULL, 0, "Organisms (*.org)|*.org", NULL) ;
	CString					section = "Animal";
	CString					fname;
	CString					code;

	if(sFile == NULL)
	{
		if(dlg.DoModal() != IDOK)
			return;

		fname = dlg.GetPathName();
	}
	else
	{
		fname = *sFile;
	}

	if(sect != NULL)
		section = *sect;

	WritePrivateProfileString(section,"Energy",itoa(pAnimal->Energy,str,16),fname);
	WritePrivateProfileString(section,"BreedAfter",itoa(pAnimal->BreedAfter,str,16),fname);
	WritePrivateProfileString(section,"MutateRate",itoa(pAnimal->MutationRate,str,16),fname);
	WritePrivateProfileString(section,"Name",pAnimal->Name,fname);
	WritePrivateProfileString(section,"Colour",itoa(pAnimal->Colour,str,16),fname);
	WritePrivateProfileString(section,"XPos",itoa(pAnimal->XPos,str,16),fname);
	WritePrivateProfileString(section,"YPos",itoa(pAnimal->YPos,str,16),fname);

	WritePrivateProfileString(section,"ID",itoa(pAnimal->ID,str,16),fname);
	WritePrivateProfileString(section,"CreatureType",itoa(pAnimal->CreatureType,str,16),fname);
	WritePrivateProfileString(section,"Generation",itoa(pAnimal->Generation,str,16),fname);
	WritePrivateProfileString(section,"Species",itoa(pAnimal->Species,str,16),fname);
	WritePrivateProfileString(section,"YPos",itoa(pAnimal->YPos,str,16),fname);
	WritePrivateProfileString(section,"YPos",itoa(pAnimal->YPos,str,16),fname);
	WritePrivateProfileString(section,"YPos",itoa(pAnimal->YPos,str,16),fname);

	WritePrivateProfileString(section,"IP",itoa(pAnimal->IP,str,16),fname);
	WritePrivateProfileString(section,"R",itoa(pAnimal->R,str,16),fname);

	WritePrivateProfileString(section,"Vars",
		ConvertToHex((BYTE*)pAnimal->vars,NUMBER_OF_IO_VARS * sizeof(WORD)),
		fname);
	WritePrivateProfileString(section,"disassmebly",ConvertToHex(pAnimal->Code,CODE_SIZE),fname);
}


void LoadAnimal(CAnimal *pAnimal, CString *sFile = NULL, CString *sect = NULL)
{
	char					str[100];					
	static CFileDialog		dlg(TRUE, "ORG", NULL, 0, "Organisms (*.org)|*.org", NULL) ;
	CString					section = "Animal";
	CString					fname;
	CString					code;
	int						count;

	if(sFile == NULL)
	{
		if(dlg.DoModal() != IDOK)
			return;

		fname = dlg.GetPathName();
	}
	else
	{
		fname = *sFile;
	}

	if(sect != NULL)
		section = *sect;

	GetPrivateProfileString(section,"Energy","0",str,100,fname);
	sscanf(str,"%x",&pAnimal->Energy);
	GetPrivateProfileString(section,"BreedAfter","1000",str,100,fname);
	sscanf(str,"%x",&pAnimal->BreedAfter);
	GetPrivateProfileString(section,"MutateRate","50",str,100,fname);
	sscanf(str,"%x",&pAnimal->MutationRate);
	GetPrivateProfileString(section,"NAME","Unknown",pAnimal->Name.GetBuffer(100),100,fname);
	GetPrivateProfileString(section,"Colour","ff",str,100,fname);
	sscanf(str,"%x",&pAnimal->Colour);
	GetPrivateProfileString(section,"XPos","0",str,100,fname);
	sscanf(str,"%x",&pAnimal->XPos);
	GetPrivateProfileString(section,"YPos","0",str,100,fname);
	sscanf(str,"%x",&pAnimal->YPos);
	if(pAnimal->YPos >= ymax)
		pAnimal->YPos = ymax -1;
	if(pAnimal->XPos >= xmax)
		pAnimal->XPos = xmax -1;


	GetPrivateProfileString(section,"ID","1",str,100,fname);
	sscanf(str,"%x",&pAnimal->ID);
	GetPrivateProfileString(section,"CreatureType","2",str,100,fname);
	sscanf(str,"%x",&pAnimal->CreatureType);
	GetPrivateProfileString(section,"Generation","0",str,100,fname);
	sscanf(str,"%x",&pAnimal->Generation);
	GetPrivateProfileString(section,"Species","0",str,100,fname);
	sscanf(str,"%x",&pAnimal->Species);


	GetPrivateProfileString(section,"IP","0",str,100,fname);
	sscanf(str,"%x",&pAnimal->IP);
	GetPrivateProfileString(section,"R","0",str,100,fname);
	sscanf(str,"%x",&pAnimal->R);

	GetPrivateProfileString(section,"Vars","",str,100,fname);
	ConvertFromHex(str, (BYTE*)pAnimal->vars, NUMBER_OF_IO_VARS * sizeof(WORD));
	count = GetPrivateProfileString(section,"disassmebly","",code.GetBuffer(4000),4000,fname);
	code.ReleaseBuffer(count);
	ConvertFromHex(code, pAnimal->Code, CODE_SIZE);

	pAnimal->Tracing = 0 ;
	pAnimal->PrevThingHere = 0;
}



void SaveUniverse()
{
	char					str[100];					
	static CFileDialog		dlg(FALSE, "UNI", NULL, 0, "Universes (*.uni)|*.uni", NULL) ;
	CString					section = "Universe";
	CString					fname;

	if(dlg.DoModal() != IDOK)
		return;

	fname = dlg.GetPathName();

	WritePrivateProfileString(section,"lIterations",itoa(lIterations,str,16),fname);
	WritePrivateProfileString(section,"ymax",itoa(ymax,str,16),fname);
	WritePrivateProfileString(section,"xmax",itoa(xmax,str,16),fname);
	WritePrivateProfileString(section,"scale",itoa(scale,str,16),fname);
	WritePrivateProfileString(section,"gnSpeed",itoa(gnSpeed,str,16),fname);
	WritePrivateProfileString(section,"gnLives",itoa(gnLives,str,16),fname);
	WritePrivateProfileString(section,"gnMutations",itoa(gnMutations,str,16),fname);
	WritePrivateProfileString(section,"gGrassPerTurn",itoa(gGrassPerTurn,str,16),fname);
	WritePrivateProfileString(section,"gnMaxGenerations",itoa(gnMaxGenerations,str,16),fname);
	WritePrivateProfileString(section,"gGrassPerTurn",itoa(gGrassPerTurn,str,16),fname);

	WritePrivateProfileString(section,"Size",itoa(gOptions.m_Size,str,16),fname);
	WritePrivateProfileString(section,"GrassStart",itoa(gOptions.m_GrassStart,str,16),fname);
	WritePrivateProfileString(section,"GrassPoints",itoa(gOptions.m_GrassPoints,str,16),fname);
	WritePrivateProfileString(section,"RestCost",itoa(gOptions.m_RestCost,str,16),fname);
	WritePrivateProfileString(section,"MoveCost",itoa(gOptions.m_MoveCost,str,16),fname);
	WritePrivateProfileString(section,"MaxBreed",itoa(gOptions.m_MaxBreed,str,16),fname);
	WritePrivateProfileString(section,"MaxMutate",itoa(gOptions.m_MaxMutate,str,16),fname);
	WritePrivateProfileString(section,"MinBreed",itoa(gOptions.m_MinBreed,str,16),fname);
	WritePrivateProfileString(section,"MinMutate",itoa(gOptions.m_MinMutate,str,16),fname);
	WritePrivateProfileString(section,"StartBreed",itoa(gOptions.m_StartBreed,str,16),fname);
	WritePrivateProfileString(section,"StartMutate",itoa(gOptions.m_StartMutate,str,16),fname);
	WritePrivateProfileString(section,"StartEnergy",itoa(gOptions.m_StartEnergy,str,16),fname);
	WritePrivateProfileString(section,"StartHerbivores",itoa(gOptions.m_StartHerbivores,str,16),fname);
	WritePrivateProfileString(section,"StartCarnivores",itoa(gOptions.m_StartCarnivores,str,16),fname);
	WritePrivateProfileString(section,"CarnEase",itoa(gOptions.m_CarnEase,str,16),fname);
	WritePrivateProfileString(section,"Ominvore",itoa(gOptions.m_Ominvore,str,16),fname);
	WritePrivateProfileString(section,"FightCost",itoa(gOptions.m_FightCost,str,16),fname);

	BYTE compressed[XACROSS*MAX_UNI*YACROSS*MAX_UNI*sizeof(BYTE)];
	int p = 0;
	for(int x = 0; x < xmax; x++)
		for(int y = 0; y < ymax ;y++)
			compressed[p++] = (BYTE)Universe[x][y];

	WritePrivateProfileString(section,"WorldGrid",
		ConvertToHex(compressed,xmax*ymax*sizeof(BYTE)),
		fname);

	int i = 0;
	CAnimal *pCurrentAnimal;
	POSITION r;

	if(!Herbivores.IsEmpty())
	{
		r = Herbivores.GetTailPosition();
		while(r)
		{
			section.Format("Animal%d",i++);
			pCurrentAnimal = (CAnimal*)	Herbivores.GetPrev(r);
			SaveAnimal(pCurrentAnimal,&fname,&section);
		}
	}

}


void LoadUniverse()
{
	char					str[100];					
	static CFileDialog		dlg(TRUE, "UNI", NULL, 0, "Universes (*.uni)|*.uni", NULL) ;
	CString					section = "Universe";
	CString					fname;
	CString					unistring;
	int						count,size;

	if(dlg.DoModal() != IDOK)
		return;

	fname = dlg.GetPathName();


	GetPrivateProfileString(section,"lIterations","10",str,100,fname);
	sscanf(str,"%x",&lIterations);
	GetPrivateProfileString(section,"ymax","10",str,100,fname);
	sscanf(str,"%x",&ymax);
	GetPrivateProfileString(section,"xmax","10",str,100,fname);
	sscanf(str,"%x",&xmax);
	GetPrivateProfileString(section,"scale","10",str,100,fname);
	sscanf(str,"%x",&scale);
	GetPrivateProfileString(section,"gnSpeed","10",str,100,fname);
	sscanf(str,"%x",&gnSpeed);
	GetPrivateProfileString(section,"gnLives","10",str,100,fname);
	sscanf(str,"%x",&gnLives);
	GetPrivateProfileString(section,"gnMutations","10",str,100,fname);
	sscanf(str,"%x",&gnMutations);
	GetPrivateProfileString(section,"gGrassPerTurn","10",str,100,fname);
	sscanf(str,"%x",&gGrassPerTurn);
	GetPrivateProfileString(section,"gnMaxGenerations","10",str,100,fname);
	sscanf(str,"%x",&gnMaxGenerations);
	GetPrivateProfileString(section,"gGrassPerTurn","10",str,100,fname);
	sscanf(str,"%x",&gGrassPerTurn);

	GetPrivateProfileString(section,"Size","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_Size);
	GetPrivateProfileString(section,"GrassStart","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_GrassStart);
	GetPrivateProfileString(section,"GrassPoints","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_GrassPoints);
	GetPrivateProfileString(section,"RestCost","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_RestCost);
	GetPrivateProfileString(section,"MoveCost","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_MoveCost);
	GetPrivateProfileString(section,"MaxBreed","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_MaxBreed);
	GetPrivateProfileString(section,"MaxMutate","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_MaxMutate);
	GetPrivateProfileString(section,"MinBreed","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_MinBreed);
	GetPrivateProfileString(section,"MinMutate","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_MinMutate);
	GetPrivateProfileString(section,"StartBreed","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_StartBreed);
	GetPrivateProfileString(section,"StartMutate","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_StartMutate);
	GetPrivateProfileString(section,"StartEnergy","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_StartEnergy);
	GetPrivateProfileString(section,"StartHerbivores","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_StartHerbivores);
	GetPrivateProfileString(section,"StartCarnivores","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_StartCarnivores);
	GetPrivateProfileString(section,"CarnEase","1",str,100,fname);
	sscanf(str,"%x",&gOptions.m_CarnEase);
	GetPrivateProfileString(section,"Ominvore","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_Ominvore);
	GetPrivateProfileString(section,"FightCost","10",str,100,fname);
	sscanf(str,"%x",&gOptions.m_FightCost);


	size = xmax * ymax ;
	BYTE compressed[XACROSS*MAX_UNI*YACROSS*MAX_UNI*sizeof(BYTE)];
	
	count = GetPrivateProfileString(section,"WorldGrid","",
		unistring.GetBuffer(size * 3 + 10),size * 3 + 10,fname);
	unistring.ReleaseBuffer(count);
	ConvertFromHex(unistring, compressed, size);


	int p = 0;
	for(int x = 0; x < xmax; x++)
		for(int y = 0; y < ymax ;y++)
			Universe[x][y] = compressed[p++];


	
	Herbivores.RemoveAll();

	BOOL bDone = FALSE;
	CAnimal *pCurrentAnimal;
	p = 0;

	while(!bDone)
	{
		section.Format("Animal%d",p++);
		pCurrentAnimal = new CAnimal;
		LoadAnimal(pCurrentAnimal,&fname,&section);
		if(pCurrentAnimal->Energy != 0)
		{
			Herbivores.AddHead(pCurrentAnimal);
		}
		else
		{
			delete pCurrentAnimal;
			bDone = TRUE;
		}
	}

	srand( (unsigned)time( NULL ) );
	(((CEyesApp *)AfxGetApp())->m_pMainWnd)->InvalidateRect(NULL);


}