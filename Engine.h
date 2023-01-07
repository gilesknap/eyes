
#define RANGE	4

extern int	x_dir[];
extern int	y_dir[];


#define VAR_V1 0
#define VAR_V2 1
#define VAR_V3 2
#define VAR_V4 3
#define VAR_V5 4
#define VAR_V6 5
#define VAR_V7 6
#define VAR_V8 7
#define VAR_E	8
#define VAR_X	9
#define VAR_Y	10
#define VAR_B	11
#define VAR_M	12
#define VAR_I1	13
#define VAR_I5  17
#define NUMBER_OF_VARS 18
// remember to update NUMBER_OF_IO_VARS if this changes

#define	LOADC	0	
#define	LOADV	1	
#define	ANDV	2	
#define	ORV		3
#define	JZ		4
#define	JNZ		5
#define	MOVV	6		
#define	MOVC	7	
#define	NOP		8
#define	SAVEV	9	
#define ADDV	10
#define SUBV	11		
//#define MOVRND  10
#define NUMBER_OF_INSTRUCTIONS 12



class CAnimal;

int			ExecuteCode(CAnimal* pAnimal);
CString		DisAssemble(BYTE* pByteList, int start, int end);
void		Assemble(CString SourceCode, BYTE *code, int codesize);
