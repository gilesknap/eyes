#include "stdafx.h"
#include "eyes.h"
#include "engine.h"
#include "guts.h"
#include "optdlg.h"

int colmapsize = 20;
int colmap[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};


CMapWordToPtr	SpeciesInfo;

CPtrList	Herbivores;
BYTE		Universe[XACROSS*MAX_UNI][YACROSS*MAX_UNI];
int			xmax = XACROSS;
int			ymax = YACROSS;
int			scale = 5;

int			gnLives;
int			gnDeaths;
int			lIterations;
int			gnMaxGenerations;
int			gnMutations;

int			gnSpeed;
int			gGrassPerTurn = GRASS_PER_TURN;

BOOL		gbDisplay = TRUE;
BOOL		bAutoRestart = TRUE;

void CreateHerbivores();
void BreedHerbivore(CAnimal *pAnimal);
void ClearUniverse();
void AddGrass();
void AddNewSod(BOOL bUsePropgation = TRUE);
void AddHerbivores();
int FieldOfVision(int x, int y);
int nFieldOfVision2(int x, int y, CAnimal *pAnimal);
BOOL bMoveHerbivore(CAnimal *pAnimal);
BOOL bFoodFound(int x, int y);
void Startup();
void ShowHerbivoreEyes();
COLORREF GetFirstAvailableColour();	

/*
CAnimal & CAnimal::operator = ( const CAnimal &o)
{
		// do a memory copy first;
	char * co = (char*) &o;
	char * cn = (char*) this;
	for(int i = 0; i < sizeof(CAnimal); i++)
		cn[i] = co[i];

	// now sort out any nasties
	Name = 
cstring
	return *this;
}
*/




// we cheat with painting - directly draw changes instead of going
// through WM_PAINT. Variables below used for this purpose
CPen penBackgound(PS_SOLID, 1, RGB(192,192,192));
CBrush brushBackgound(RGB(200,200,200));
CPen penGrass(PS_SOLID, 1, RGB(0,127,0));
CBrush brushGrass(RGB(0,64,0));
CPen penCreature(PS_SOLID, 1, RGB(0,0,255));
CBrush brushCreature(RGB(0,0,255));
CPen penEyes(PS_SOLID, 1, RGB(255,255,255));
CBrush brushCarnivore(RGB(255,255,255));

CAnimal* FindAnimal(int x, int y, CAnimal *NotMe = NULL)
{
	CAnimal		*pCurrentAnimal;
	POSITION	r;

	if(!Herbivores.IsEmpty())
		r = Herbivores.GetTailPosition();
    while(r)
	{
		pCurrentAnimal = (CAnimal*)	Herbivores.GetPrev(r);
        if(pCurrentAnimal->XPos == x && pCurrentAnimal->YPos == y)
		{
			if(pCurrentAnimal != NotMe)
				return pCurrentAnimal;
		}
	}

	return NULL;

}

void DoDraw(int x, int y, CAnimal *pAnimal)
{
	if(!gbDisplay)
		return;

	//CRect		rectDraw;
	register int	s = scale;
	static CClientDC	WDC(((CEyesApp *)AfxGetApp())->m_pMainWnd);
	static CClientDC	GrassDC(((CEyesApp *)AfxGetApp())->m_pMainWnd);
	static CClientDC	BackDropDC(((CEyesApp *)AfxGetApp())->m_pMainWnd);
	static BOOL			bFirst = TRUE;
	CBrush		*oldBrush = NULL;
	CBrush		brushLocalCreature;

	if(bFirst)
	{
		bFirst = FALSE;
		GrassDC.SelectObject(&brushGrass);
		BackDropDC.SelectObject(&brushBackgound);
	}

	switch (Universe[x][y])
	{
	case ID_CREATURE:
	case ID_CARNIVORE:
			if(!pAnimal)
			{
				// this will be slow but only happens on a repaint which is
				// slow anyway
				pAnimal = FindAnimal(x,y);
			}
			if(pAnimal)
			{
				if(pAnimal->Tracing == 1)
					brushLocalCreature.CreateSolidBrush(RGB(255,255,255));
				else
					brushLocalCreature.CreateSolidBrush(pAnimal->Colour);

				oldBrush = WDC.SelectObject(&brushLocalCreature);
				if(s > 1)
					WDC.PatBlt( x*s + 1, y*s + 1,s - 1 ,s - 1 ,PATCOPY );
				else
					WDC.PatBlt( x*s, y*s,s,s,PATCOPY );
				if(pAnimal->CreatureType == ID_CARNIVORE)
				{
					WDC.SelectObject(&brushCarnivore);
					WDC.PatBlt(x*s + 2, y*s + 2,s - 3 ,s - 3 ,PATCOPY);
				}
			}
			else
			{
				oldBrush = WDC.SelectObject(&brushCreature);
				WDC.PatBlt( x*s, y*s,s ,s ,PATCOPY );
			}
			break;
		case ID_GRASS:
			if(s > 1)
				GrassDC.PatBlt( x*s + 1, y*s + 1,s - 1 ,s - 1 ,PATCOPY );
			else
				GrassDC.PatBlt( x*s, y*s,s,s,PATCOPY );
			break;
		case ID_BACKGROUND:
			BackDropDC.PatBlt( x*s, y*s,s ,s ,PATCOPY );
			break;
	}

	if(oldBrush != NULL)
	{
		WDC.SelectObject(oldBrush);
	}
}

void ShowLives()
{
	CString deaths,lives,iterates,generations,species;

	deaths.Format("%d",gnDeaths);
	lives.Format("%d",Herbivores.GetCount());
	iterates.Format("%d",lIterations);
	generations.Format("%d",gnMaxGenerations);
	species.Format("%d",gnMutations);

	((CEyesApp *)AfxGetApp())->m_pMainWnd->GetDlgItem(IDC_DEATHS)->SetWindowText(deaths);
	((CEyesApp *)AfxGetApp())->m_pMainWnd->GetDlgItem(IDC_LIVING)->SetWindowText(lives);
	((CEyesApp *)AfxGetApp())->m_pMainWnd->GetDlgItem(IDC_TIME)->SetWindowText(iterates);
	((CEyesApp *)AfxGetApp())->m_pMainWnd->GetDlgItem(IDC_GENERATIONS)->SetWindowText(generations);
	((CEyesApp *)AfxGetApp())->m_pMainWnd->GetDlgItem(IDC_SPECIES)->SetWindowText(species);

}

void CreateHerbivores()
{
    int i,j;
	CAnimal *pNewAnimal;

    for( i = 0; i < gOptions.m_StartHerbivores + gOptions.m_StartCarnivores; i++)
	{
		pNewAnimal = new CAnimal;

		pNewAnimal->ID = gnLives++;
		if(i <= gOptions.m_StartHerbivores)
		{
			pNewAnimal->CreatureType = ID_CREATURE;
			pNewAnimal->Energy = gOptions.m_StartEnergy;
			pNewAnimal->MutationRate = gOptions.m_StartMutate;
			pNewAnimal->BreedAfter = gOptions.m_StartBreed;
		}
		else
		{
			pNewAnimal->CreatureType = ID_CARNIVORE;
			pNewAnimal->Energy = gOptions.m_StartEnergy2;
			pNewAnimal->MutationRate = gOptions.m_StartMutate2;
			pNewAnimal->BreedAfter = gOptions.m_StartBreed2;
		}
		pNewAnimal->Name = "Natural";
		pNewAnimal->Generation = 0;
		pNewAnimal->Species = 0;
        pNewAnimal->XPos = (int)(rand() / ((double)RAND_MAX + 1) * (double)(xmax - XMIN)) + XMIN;
        pNewAnimal->YPos = (int)(rand() / ((double)RAND_MAX + 1) * (double)(ymax - YMIN)) + YMIN;
        pNewAnimal->Colour = RGB(0,0,255);
		pNewAnimal->Tracing = 0;
		pNewAnimal->IP = pNewAnimal->R = 0;
        for(j = 0; j < CODE_SIZE; j++)
		{
            pNewAnimal->Code[j] = (int)((rand() / ((double)RAND_MAX + 1)) * 255);
        }
		Herbivores.AddTail((CObject *)pNewAnimal);
	}

}

COLORREF MakeColour(int Generation)
{ 

	COLORREF r;

	r =  RGB(((Generation / 4)*64) % 256, ((Generation / 2)*64) % 256, ((Generation) *64) % 256);
	
	if(r == 0)
		r = RGB(100,100,100);
	if(r == RGB(255,255,255))
		r = RGB(200,200,200);
	
	return r;
}

void MutateAnimal(CAnimal *pAnimal, CAnimal *pOldAnimal)
{
	// new colour for this new species!!!
	pAnimal->Colour = MakeColour(gnMutations++);
	pAnimal->Species = gnMutations;

	/*
	// maybe mutate into carniviore! (2% chance even at 100% mutate rate)
	if( ((rand() / ((double)RAND_MAX + 1) * 
			10000.0 / (double)pAnimal->MutationRate)) < 2) 
		pAnimal->CreatureType = ID_CARNIVORE;
	*/

	// Mutation Amount is % between minus half MutationRate % 
	//   and plus half MutationRate %
	int MutationAmount = (int)((rand() / ((double)RAND_MAX + 1) * 
							(double)pAnimal->MutationRate) - 
							pAnimal->MutationRate / 2);

	pAnimal->BreedAfter += (pAnimal->BreedAfter * MutationAmount) / 100;

	MutationAmount = (int)((rand() / ((double)RAND_MAX + 1) * 
							(double)pAnimal->MutationRate) - 
							pAnimal->MutationRate / 2);


    pAnimal->MutationRate += (pAnimal->MutationRate * MutationAmount) / 100;
	if(pAnimal->CreatureType == ID_CARNIVORE)
	{
		if(pAnimal->MutationRate > gOptions.m_MaxMutate2)
			pAnimal->MutationRate = gOptions.m_MaxMutate2;
		if(pAnimal->MutationRate < gOptions.m_MinMutate2)
			pAnimal->MutationRate = gOptions.m_MinMutate2;
		if(pAnimal->BreedAfter > gOptions.m_MaxBreed2)
			pAnimal->BreedAfter = gOptions.m_MaxBreed2;
		if(pAnimal->BreedAfter < gOptions.m_MinBreed2)
			pAnimal->BreedAfter = gOptions.m_MinBreed2;
	}
	else
	{
		if(pAnimal->MutationRate > gOptions.m_MaxMutate)
			pAnimal->MutationRate = gOptions.m_MaxMutate;
		if(pAnimal->MutationRate < gOptions.m_MinMutate)
			pAnimal->MutationRate = gOptions.m_MinMutate;
		if(pAnimal->BreedAfter > gOptions.m_MaxBreed)
			pAnimal->BreedAfter = gOptions.m_MaxBreed;
		if(pAnimal->BreedAfter < gOptions.m_MinBreed)
			pAnimal->BreedAfter = gOptions.m_MinBreed;
	}

	// now mutate the the code!
	int copyfrom = 0;
	for(int j = 0; j < CODE_SIZE; j++)
	{
		pAnimal->Code[j] = pOldAnimal->Code[copyfrom++];
		// Maybe mutate this value !!
		// (deliberately lower chances of mutation in each line of code 200 fold)
		if((int)((rand() / ((double)RAND_MAX + 1)) * 20000) < pAnimal->MutationRate)
		{
			pAnimal->Code[j] = (BYTE)(rand());	
		}
		// Maybe Mutate where the rest of the code is copied from !!
		if((int)((rand() / ((double)RAND_MAX + 1)) * 20000) < pAnimal->MutationRate)
		{
			copyfrom = (int)((rand() / ((double)RAND_MAX + 1)) * (double)CODE_SIZE);	
		}
		if(copyfrom >= CODE_SIZE)
			copyfrom = 0;
	}
}

void BreedHerbivore(CAnimal *pOldAnimal)
{
	CAnimal *pNewAnimal;

	pNewAnimal = new CAnimal;
	
    pOldAnimal->Energy = pOldAnimal->Energy / 2;
	*pNewAnimal = *pOldAnimal;
 	pNewAnimal->ID = gnLives++;
	pNewAnimal->Generation++;
	pNewAnimal->IP = pNewAnimal->R = 0;
	pNewAnimal->Tracing = 0;

	if(pNewAnimal->Generation > gnMaxGenerations)
		gnMaxGenerations = pNewAnimal->Generation;
   
   // Mutate Herbivore if threshold exceeded
    if((int)((rand() / ((double)RAND_MAX + 1)*100.0)) < pNewAnimal->MutationRate)
	{
		MutateAnimal(pNewAnimal,pOldAnimal);
    }
	else
	{
		for(int j = 0; j < CODE_SIZE; pNewAnimal->Code[j] =
				pOldAnimal->Code[j++]);
	}
	Herbivores.AddTail((CObList*)pNewAnimal);
}

void ClearUniverse()
{
	int x,y;

	for(x = 0; x < xmax; x++)
	{
		for(y = 0; y < ymax; y++)
		{
			Universe[x][y] = ID_BACKGROUND;
		}
	}
}

void AddGrass()
{
    int i;
    for(i = 0; i < gOptions.m_GrassStart; i++)
        AddNewSod(FALSE);
}

void AddNewSod(BOOL bUsePropgation)
{
	int x,y,chance;

	//chance alogrithm makes grass grow more near where it already is!!
	if(bUsePropgation)
	{
		for(int i = 0 ; i < 10; i++)
		{
			x = ((int)(rand() / ((double)RAND_MAX + 1) * (double)(xmax - XMIN) + (double)XMIN));
			y = ((int)(rand() / ((double)RAND_MAX + 1) * (double)(ymax - YMIN) + (double)YMIN));
			
			chance = 8;
			if(Universe[x-1][y] != ID_GRASS) chance--;
			if(Universe[x-1][y-1] != ID_GRASS) chance--;
			if(Universe[x-1][y+1] != ID_GRASS) chance--;
			if(Universe[x+1][y] != ID_GRASS) chance--;
			if(Universe[x+1][y-1] != ID_GRASS) chance--;
			if(Universe[x+1][y+1] != ID_GRASS) chance--;
			if(Universe[x][y-1] != ID_GRASS) chance--;
			if(Universe[x][y+1] != ID_GRASS) chance--;

			if(chance > (int)(rand() / ((double)RAND_MAX + 1) * 8))
			{
				Universe[x][y] = ID_GRASS;
				DoDraw(x,y);
			}
		}
	}
	else
	{
		x = ((int)(rand() / ((double)RAND_MAX + 1) * (double)(xmax - XMIN) + (double)XMIN));
		y = ((int)(rand() / ((double)RAND_MAX + 1) * (double)(ymax - YMIN) + (double)YMIN));
		Universe[x][y] = ID_GRASS;
		DoDraw(x,y);
	}
}


void AddHerbivores()
{
    CAnimal *pCurrentAnimal = NULL;
	POSITION r = NULL;
	if(!Herbivores.IsEmpty())
	r = Herbivores.GetHeadPosition();
    while(r)
	{
		pCurrentAnimal = (CAnimal*)	Herbivores.GetNext(r);
        Universe[pCurrentAnimal->XPos][pCurrentAnimal->YPos]  = ID_CREATURE;
	}
}

void FightItOut(CAnimal* pA1, CAnimal* pA2)
{
	int Winner = 0;

	// never eat own species
	if(pA1->Species == pA2->Species) 
		return;

	//determine the winner
	if(pA1->CreatureType == ID_CARNIVORE && pA2->CreatureType == ID_CREATURE) 
		Winner = 1;
	else if(pA1->CreatureType == ID_CREATURE && pA2->CreatureType == ID_CARNIVORE) 
		Winner = 2;
	else if(pA1->CreatureType == ID_CREATURE && pA2->CreatureType == ID_CREATURE &&
					gOptions.m_Ominvore)
	{
		if(pA1->Energy > pA2->Energy)
			Winner = 1;
		else
			Winner = 2;
	}
						
	if(Winner == 1)					
	{
		pA1->Energy += pA2->Energy - gOptions.m_FightCost;
		pA2->Energy = 0;
	}
	else if(Winner == 2)					
	{
		pA2->Energy += pA1->Energy - gOptions.m_FightCost;
		pA1->Energy = 0;
	}
}


BOOL bMoveHerbivore(CAnimal *pAnimal)
{
    if(pAnimal->Energy > 0)
	{
		int Direction = ExecuteCode(pAnimal);
		if(pAnimal->PrevThingHere == ID_GRASS && 
			pAnimal->CreatureType == ID_CARNIVORE)
			Universe[pAnimal->XPos][pAnimal->YPos] = ID_GRASS;	
		else
			Universe[pAnimal->XPos][pAnimal->YPos] = ID_BACKGROUND;
		if(Direction != -1)
		{
			DoDraw(pAnimal->XPos,pAnimal->YPos);
			
			if(!gOptions.m_RoundWorld)
			{
				if(pAnimal->XPos + x_dir[Direction] >= XMIN &&
					pAnimal->XPos + x_dir[Direction] < xmax)
						pAnimal->XPos += x_dir[Direction];
				if(pAnimal->YPos + y_dir[Direction] >= YMIN &&
					pAnimal->YPos + y_dir[Direction] < ymax)
						pAnimal->YPos += y_dir[Direction];
			}
			else
			{
				pAnimal->XPos = (pAnimal->XPos+ x_dir[Direction]) % xmax;
				pAnimal->YPos = (pAnimal->YPos+ y_dir[Direction]) % ymax;
				if(pAnimal->XPos < 0) pAnimal->XPos = xmax;				
				if(pAnimal->YPos < 0) pAnimal->YPos = ymax;
			}

			pAnimal->PrevThingHere = Universe[pAnimal->XPos][pAnimal->YPos];
			pAnimal->Energy = pAnimal->Energy - 
				(pAnimal->CreatureType == ID_CARNIVORE ? 
					gOptions.m_MoveCost2 : gOptions.m_MoveCost);

			if((pAnimal->CreatureType != ID_CARNIVORE && 
				bFoodFound(pAnimal->XPos, pAnimal->YPos)))
			{
				pAnimal->Energy = pAnimal->Energy + gOptions.m_GrassPoints ;
			}
			
			if(Universe[pAnimal->XPos][pAnimal->YPos] == ID_CREATURE ||
					Universe[pAnimal->XPos][pAnimal->YPos] == ID_CARNIVORE)
			{
				// come across another bestie - fight it out!      
				CAnimal *pFightAnimal = FindAnimal(pAnimal->XPos,pAnimal->YPos,pAnimal);
				if(pFightAnimal)
				{
					FightItOut(pAnimal,pFightAnimal);
				}
			}
        }
		else
		{
			pAnimal->Energy = pAnimal->Energy - 
				(pAnimal->CreatureType == ID_CARNIVORE ? 
					gOptions.m_RestCost2 : gOptions.m_RestCost);

		}

		if(pAnimal->Energy > 0)
		{
			Universe[pAnimal->XPos][pAnimal->YPos] = (BYTE)pAnimal->CreatureType;
			if(Direction != -1)
				DoDraw(pAnimal->XPos,pAnimal->YPos,pAnimal);
			if(pAnimal->Energy > pAnimal->BreedAfter)
			{
				BreedHerbivore(pAnimal);
			}
			return TRUE;
		}

	 }				
	DoDraw(pAnimal->XPos,pAnimal->YPos,pAnimal);
	return FALSE;
}

BOOL bFoodFound(int x, int y)
{
    if(Universe[x][y] == ID_GRASS)
	{
		return TRUE;
	}
    else
    {
		return FALSE;
	}
}

void Shutdown()
{
	POSITION	r;
	CAnimal		*pCurrentAnimal;

	if(!Herbivores.IsEmpty())
	{
		r = Herbivores.GetTailPosition();
		while(r)
		{
			pCurrentAnimal = (CAnimal*)	Herbivores.GetPrev(r);
			delete pCurrentAnimal;
		}
	}
	Herbivores.RemoveAll();
}

void Startup()
{	
	lIterations = 0;
	gnDeaths = 0;
	gnLives = 0;
	gnMaxGenerations = 0;
	gnMutations = 1;

	scale = MAX_UNI + 1 - gOptions.m_Size;
	xmax = (int)( (double)MAX_UNI / (double) scale  * (double)XACROSS);
	ymax = (int)( (double)MAX_UNI / (double) scale  * (double)YACROSS);

	Herbivores.RemoveAll();
	srand( (unsigned)time( NULL ) );
	(((CEyesApp *)AfxGetApp())->m_pMainWnd)->InvalidateRect(NULL);
    ClearUniverse();
    CreateHerbivores();
    AddGrass();
    AddHerbivores();

}



void CALLBACK OneGo(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime )
{
	static int	counter = 0;
	int			i, trips;
    CAnimal		*pCurrentAnimal = NULL;
	POSITION	r = NULL;
	unsigned int startTime = (unsigned)time( NULL );

	if(gbDisplay)
	{
		trips = 1;
		counter++;
		if(counter < gnSpeed) 
			return; 
		else 
			counter = 0;
	}
	else 
	{
		trips = 1000;
	}

	do
	{
		lIterations++;

		for(i = 1; i <= gGrassPerTurn; i++)
			AddNewSod();

		if(!Herbivores.IsEmpty())
		{
			r = Herbivores.GetTailPosition();
			while(r)
			{
				pCurrentAnimal = (CAnimal*)	Herbivores.GetPrev(r);
				if(!bMoveHerbivore(pCurrentAnimal))
				{
					POSITION rnext = r;
					if(rnext == NULL)
						rnext = Herbivores.GetHeadPosition();
					else
						Herbivores.GetNext(rnext);
					// if tracing is on let the debugger delete this animal
					if(pCurrentAnimal->Tracing == 1)
						pCurrentAnimal->Tracing = 2;
					else
						delete pCurrentAnimal;
					Herbivores.RemoveAt(rnext);
					gnDeaths++;
				}
			}
		}
		else
		{
			if(bAutoRestart)
				Startup();
		}
		// dont allow display off mode to sit in this loop forever
		// (high populations make this take a long time!)
		if((unsigned)time( NULL ) - startTime > 1)
			break;
	} while (lIterations % trips != 0);

	if(lIterations % 10 == 0)
		ShowLives();
}

