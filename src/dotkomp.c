
#include "dotkoms.h"
#include "dotkomp.h"

typedef char Name[1024]; 

typedef struct OUTPUT{
char **incl;
char **main;
} CODE;
#define P1 pos1=S_NextPos
#define P2 pos2=S_Pos
#define GS(a) S_GetString(pos1, pos2-pos1,a,sizeof(a)-1)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long pos1, pos2, slen;
char Str[1024]; 


Error_Func Custom_Error = 0L;

static int Sym;
static int errors = 0;                /*number of detected errors*/
static int ErrDist = MinErrDist;

#define MAXSYM		1


/* Production prototypes */

static void dotkoma(void);
static void instruct(void);
static void heredoc(void);


#define NSETBITS        16

static unsigned short int SymSet[][MAXSYM] = {
  /*EOF_Sym */
  {0x1},
  {0x0}
};

void GenError(int errno)
{ if (ErrDist >= MinErrDist) {
    if (Custom_Error != 0L)
      (*Custom_Error) (errno, S_NextLine, S_NextCol, S_NextPos);
    errors++;
  }
  ErrDist = 0;
}

void SynError(int errno)
{ if (errno <= MAXERROR) errno = MAXERROR;
  if (ErrDist >= MinErrDist) {
    if (Custom_Error != 0L)
      (*Custom_Error) (errno, S_NextLine, S_NextCol, S_NextPos);
    errors++;
  }
  ErrDist = 0;
}

void SemError(int errno)
{ if (errno <= MAXERROR) errno = MAXERROR;
  if (ErrDist >= MinErrDist) {
    if (Custom_Error != 0L)
      (*Custom_Error) (errno, S_Line, S_Col, S_Pos);
    errors++;
  }
  ErrDist = 0;
}

static void Get(void)
{ do {
    Sym = S_Get();
    if (Sym <= MAXT) ErrDist ++;
    else {
      /* Empty Stmt */ ;
      S_NextPos  = S_Pos;  S_NextCol = S_Col;
      S_NextLine = S_Line; S_NextLen = S_Len;
    }
  } while (Sym > MAXT);
}

static int In (unsigned short int *SymbolSet, int i)
{ return SymbolSet[i / NSETBITS] & (1 << (i % NSETBITS)); }

static void Expect (int n)
{ if (Sym == n) Get(); else GenError(n); }

static void ExpectWeak (int n, int follow)
{ if (Sym == n) Get();
  else {
    GenError(n);
    while (!(In(SymSet[follow], Sym) || In(SymSet[0], Sym))) Get();
  }
}

static int WeakSeparator (int n, int syFol, int repFol)
{ unsigned short int s[MAXSYM];
  int i;

  if (Sym == n) { Get(); return 1; }
  if (In(SymSet[repFol], Sym)) return 0;
  for (i = 0; i < MAXSYM; i++)
    s[i] = SymSet[0][i] | SymSet[syFol][i] | SymSet[repFol][i];
  GenError(n);
  while (!In(s, Sym)) Get();
  return In(SymSet[syFol], Sym);
}

int Successful(void)
{ return errors == 0; }

/* Productions */

static void dotkoma(void)
{
	if (Sym == HashBangSym) {
		Get();
		while (Sym >= identSym && Sym <= numberSym ||
		       Sym >= HashBangSym && Sym <= No_Sym) {
			Get();
		}
		Expect(nlSym);
	}
	while (Sym >= PointCommaSym && Sym <= PointshSym) {
		if (Sym == PointshSym) {
			instruct();
		} else if (Sym == PointCommaSym) {
			heredoc();
			Expect(nlSym);
		} else GenError(8);
	}
}

static void instruct(void)
{
	Expect(PointshSym);
	Expect(identSym);
	
	Name prog;
	LexName(prog, sizeof(prog));
	
	P1;
	while (Sym >= identSym && Sym <= numberSym ||
	       Sym >= HashBangSym && Sym <= No_Sym) {
		Get();
	}
	Expect(nlSym);
	P2; 
	char argu[1024];
	char comstr[1024];
	char space[1]=" ";
	char capall[10]=" 2>&1";
	GS(argu);
	//   execv(prog,argu);
	strcat(prog, space);
	strcat(prog, argu);
	strcat(prog, capall);
	//   system(prog);
	
	FILE *fp;
	char path[1035];
	
	/* Open the command for reading. */
	fp = popen(prog, "r");
	if (fp == NULL) {
	printf("Failed to run command\n" );
	exit(1);
	}
	
	/* Read the output a line at a time - output it. */
	while (fgets(path, sizeof(path), fp) != NULL) {
	printf("%s", path);
	}
	
	/* close */
	pclose(fp);
	
	
	
	// printf("execute %s \n", prog);   
}

static void heredoc(void)
{
	Expect(PointCommaSym);
	P1;
	while (Sym >= identSym && Sym <= HashBangSym ||
	       Sym >= PointshSym && Sym <= No_Sym) {
		Get();
	}
	Expect(PointCommaSym);
	P2;
	char hdc[1024];
	GS(hdc);
	printf("%s", hdc);
}



void Parse(void)
{ S_Reset(); Get();
  dotkoma();
}

