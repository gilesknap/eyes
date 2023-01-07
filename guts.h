
#define XACROSS				45
#define YACROSS				40
#define MAX_UNI				10
#define XMIN				0
#define YMIN				0
#define INIT_ENERGY			800
#define GRASS_PER_TURN		2
#define START_HERBIVORES	10

#define CODE_SIZE 1000
#define NUMBER_OF_IO_VARS 5

// Types of item in the universe
#define ID_BACKGROUND			0
#define ID_GRASS				1
#define	ID_CREATURE				2
#define ID_CARNIVORE			4


extern BYTE	Universe[XACROSS*MAX_UNI][YACROSS*MAX_UNI];
extern int		lIterations;
extern int		ymax,xmax;
extern int		scale;
extern int		gnSpeed;
extern int		gnLives;
extern int      gnMutations;
extern int		gGrassPerTurn;
extern int		gnMaxGenerations;
extern BOOL		gbDisplay;
extern CPtrList	Herbivores;



class CAnimal 
{
	public:
		int			ID;
		CString		Name;
		short		CreatureType;			
		int			Generation;
		int			Species;
		int			Energy;
		int			XPos;
		int			YPos;
		COLORREF	Colour;
		int			BreedAfter;
		int			MutationRate;

		// program instructions and state
		BYTE		Code[CODE_SIZE];
		WORD		vars[NUMBER_OF_IO_VARS];
		WORD		IP,R;

		BYTE		Tracing; // (0 = off, 1 = on, 2 = killed!)
		int			PrevThingHere;
//		CAnimal  operator = ( const CAnimal n);
};


void Startup();
void Shutdown();
void CALLBACK OneGo(HWND hWnd,      // handle of CWnd that called SetTimer
					UINT nMsg,      // WM_TIMER
					UINT nIDEvent,   // timer identification
					DWORD dwTime );
void DoDraw(int x, int y, CAnimal *pAnimal = NULL);

