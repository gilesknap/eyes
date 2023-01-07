#include "stdafx.h"
#include "eyes.h"
#include "engine.h"
#include "guts.h"

int		x_dir[] = {0,1,1,1,0,-1,-1,-1};
int		y_dir[] = {-1,-1,0,1,1,1,0,-1};

char		CR[3] = {13,10,0};


WORD GetConstant(CAnimal* pAnimal)
{
	WORD res = 0;

	res += pAnimal->Code[pAnimal->IP++];
	res += pAnimal->Code[pAnimal->IP++] * 256;

	return res;
}

WORD GetVariable(CAnimal* pAnimal)
{
	WORD var = pAnimal->Code[pAnimal->IP++] % NUMBER_OF_VARS;

	if(var >= VAR_V1 && var <= VAR_V8)
	{
		// this is a vision variable, pick up the data from the universe
		// around us
		WORD res = 0;
		int x,y;
		for(int dist = RANGE; dist > 0; dist--)
		{
			x = pAnimal->XPos + x_dir[var - VAR_V1] * dist;
			y = pAnimal->YPos + y_dir[var - VAR_V1] * dist;
			if(x >= XMIN && x < xmax && y>= YMIN && y < ymax)
				res += Universe[x][y] << (RANGE - dist) * 16/RANGE;
		}
		return res;
	}
	else if(var >= VAR_I1)
	{
		return pAnimal->vars[var - VAR_I1];
	}
	else
	{
		switch(var)
		{
		case VAR_X:
			return pAnimal->XPos;
		case VAR_Y:
			return pAnimal->YPos;
		case VAR_E:
			return pAnimal->Energy;
		case VAR_B:
			return pAnimal->BreedAfter;
		case VAR_M:
			return pAnimal->MutationRate;
		}
	}
	return 0;
}

void SetVariable(CAnimal* pAnimal, WORD value)
{
	WORD var = pAnimal->Code[pAnimal->IP++] % NUMBER_OF_IO_VARS;

	pAnimal->vars[var] = value;

}

int ExecuteCode(CAnimal* pAnimal)
{
	BYTE	nextInst;
	int		DirectionToMove = -1;
	int		ip;

	//wrap the instruction pointer
	if(pAnimal->IP > CODE_SIZE - 3)
		pAnimal->IP = 0;

	nextInst = pAnimal->Code[pAnimal->IP++];

	nextInst %= NUMBER_OF_INSTRUCTIONS;

	switch(nextInst)
	{
	case LOADC:
		pAnimal->R = GetConstant(pAnimal);
		break;
	case LOADV:
		pAnimal->R = GetVariable(pAnimal);
		break;
	case ANDV:
		pAnimal->R &= GetVariable(pAnimal);
		break;
	case ORV:
		pAnimal->R |= GetVariable(pAnimal);
		break;
	case JZ:
		if(pAnimal->R == 0)
		{
			ip = pAnimal->IP - 1;
			pAnimal->IP = (ip + GetConstant(pAnimal)) % CODE_SIZE;
		}
		else
			GetConstant(pAnimal);
		break;
	case JNZ:
		if(pAnimal->R != 0)
		{
			ip = pAnimal->IP - 1;
			pAnimal->IP = (ip + GetConstant(pAnimal)) % CODE_SIZE;
		}
		else
			GetConstant(pAnimal);
		break;
	case MOVV:
		DirectionToMove = GetVariable(pAnimal) % 8;
		break;
	case MOVC:
		DirectionToMove = GetConstant(pAnimal) % 8;
		break;
	case NOP:
		break;
	case SAVEV:
		SetVariable(pAnimal,pAnimal->R);
		break;
	case ADDV:
		pAnimal->R += GetVariable(pAnimal);
		break;
	case SUBV:
		pAnimal->R -= GetVariable(pAnimal);
		break;
//	case MOVRND:
//		DirectionToMove = (int)(rand()/((double)RAND_MAX + 1) * 8) % 8;
	}

	return DirectionToMove;
}


CString	instSet[] = {"LOADC","LOADV","ANDV","ORV","JZ",
					"JNZ","MOVV","MOVC","NOP","SAVEV","ADDV","SUBV"};
CString varSet[] = {"V1","V2","V3","V4","V5","V6","V7","V8",
					"E","X","Y","B","M","I1","I2","I3","I4","I5"};



CString DisAssemble(BYTE* pByteList, int start, int end)
{
	int			ip;
	CString		Assembly;		
	int			constant;
	CString		tmp;
	BYTE		nextInst;

	for(ip = start; ip < end; ip++)
	{
		nextInst = pByteList[ip] % NUMBER_OF_INSTRUCTIONS;
		tmp.Format("%x",ip);
		Assembly += tmp + "\t" + instSet[nextInst] + "\t\t";

		switch(nextInst)
		{
		case MOVC:
			ip++;
			constant = 	(pByteList[ip] + pByteList[ip+1] * 256) % 8;
			ip++;
			tmp.Format("%x",constant);
			Assembly += tmp;
			break;
		case LOADC:
			ip++;
			constant = 	pByteList[ip] + pByteList[ip+1] *256;
			ip++;
			tmp.Format("%x",constant);
			Assembly += tmp;
			break;
		case SAVEV:
			ip++;
			Assembly += varSet[(pByteList[ip] % NUMBER_OF_IO_VARS) + VAR_I1] ;
			break;
		case LOADV:
		case ANDV:
		case ORV:
		case MOVV:
		case ADDV:
		case SUBV:
			ip++;
			Assembly += varSet[pByteList[ip] % NUMBER_OF_VARS];
			break;
		case JZ:
		case JNZ:
			constant = 	(pByteList[ip+ 1] + pByteList[ip+2] * 256 + ip) % CODE_SIZE;
			ip++; ip++;
			tmp.Format("%x",constant);
			Assembly += tmp;
			break;
		case NOP:
			break;
		}
		Assembly += CR;
	}

	return Assembly;
}


CString GetNextLine(CString &Source)
{
	int			posn;
	CString		Line;

	posn = Source.Find(CR);
	
	if(posn != -1)
	{
		Line = Source.Left(posn);
		Source = Source.Right(Source.GetLength() - posn - 2);
	}
	else
	{
		Line = Source;
		Source.Empty();
	}
	return Line;
}

void GetParms(int operatorNum, CString Operand, BYTE *code, int &ip)
{
	int constant, varNum;

	switch(operatorNum)
		{
		case MOVC:
		case LOADC:
		case JZ:
		case JNZ:
			sscanf(Operand,"%x", &constant);
			if(operatorNum == JZ || operatorNum == JNZ)
				constant = (constant - (ip - 1) + CODE_SIZE) % CODE_SIZE;
			code[ip++] = constant;
			code[ip++] = constant / 256;
			break;
		case SAVEV:
			for(varNum = VAR_I1; varNum < VAR_I1 + NUMBER_OF_IO_VARS; varNum ++)
			{
				if(Operand.CompareNoCase(varSet[varNum]) == 0)
					break;
			}
			code[ip++] = varNum - VAR_I1;
			break;
		case LOADV:
		case ANDV:
		case ORV:
		case MOVV:
		case ADDV:
		case SUBV:
			for(varNum = 0; varNum < NUMBER_OF_VARS; varNum ++)
			{
				if(Operand.CompareNoCase(varSet[varNum]) == 0)
					break;
			}
			code[ip++] = varNum;
			break;
		case NOP:
//		case MOVRND:
			break;
		}

}

void Assemble(CString SourceCode, BYTE *code, int codesize)
{
	CString		Line;
	CString		LineNumber;
	int			ip, operatorNum;
	int			LineNum, scanned;
	CString		Operator,Operand;
	BOOL		bIgnoreLineNos = FALSE;

	ip = 0;
	
	while(ip < CODE_SIZE)
	{
		Line = GetNextLine(SourceCode);
		Operator.Empty(); Operand.Empty();
		scanned = sscanf(Line,"%9s %9s %9s",
					LineNumber.GetBuffer(10),
					Operator.GetBuffer(10),
					Operand.GetBuffer(10));
		if(scanned == 3)
		{
			sscanf(LineNumber,"%x",&LineNum);
		}
		else
		{
			LineNum = ip;
			sscanf(Line,"%9s %9s",Operator,Operand);
		}


		if(strlen(Operator) > 0)
		{
			if(ip > LineNum && !bIgnoreLineNos)
			{
				if(AfxMessageBox("Your code as conflicting Line No.s. Choose Yes to overwrite previous lines of code. Choose No to ignore all future line numbers.",
					MB_YESNO) == IDNO)
					bIgnoreLineNos = TRUE;
			}

			for(operatorNum = 0; operatorNum < NUMBER_OF_INSTRUCTIONS; operatorNum++)
			{
				if(Operator.CompareNoCase(instSet[operatorNum]) == 0)
				{
					if(scanned > 0 && !bIgnoreLineNos)
					{
						ip = LineNum;
					}

					code[ip++] = operatorNum;
					GetParms(operatorNum, Operand, code, ip);
					break;
				}
			}
		}
		if(SourceCode.GetLength() == 0)
			break;
	}
}

