
#include "dotkoms.h"
#include <stdio.h>
#include <stdlib.h>
#if __MSDOS__ || MSDOS
#  include <io.h>
#  define  SEGMENTED_BUFF   1  /* Segmented Scanner Buffer */
#else
#  if WIN32 || __WIN32__
#    include <io.h>
#  else
#    include <unistd.h>
#  endif
#  define  SEGMENTED_BUFF   0  /* Linear Scanner Buffer */
#endif

#ifdef NEXTSTEP   /* NextStep */
#  include <sys/file.h>
#  include <sys/types.h>
#  include <sys/uio.h>
#  define L_SET     0  /* set the seek pointer */
#  define L_INCR    1  /* increment the seek pointer */
#  define L_XTND    2  /* extend the file size */
#endif

#include <string.h>

static int STATE0[] = {0,0,0,0,0,0,0,0,0,0,10,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,13,0,2,2,2,2,2,2,2,2,2,2,0,0,
                       0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                       1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                       1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int  S_src;                 /* source file */
int  S_Line, S_Col;         /* line and column of current symbol */
int  S_Len;                 /* length of current symbol */
long S_Pos;                 /* file position of current symbol */
int  S_NextLine;            /* line of lookahead symbol */
int  S_NextCol;             /* column of lookahead symbol */
int  S_NextLen;             /* length of lookahead symbol */
long S_NextPos;             /* file position of lookahead symbol */
static unsigned char S_ch;  /* current input character */
int  S_CurrLine;            /* current input line (may be higher than line) */
long S_LineStart;           /* start position of current line */


#define Scan_Ch         S_ch
#define Scan_NextCh     S_NextCh
#define Scan_ComEols    S_ComEols
#define Scan_CurrLine   S_CurrLine
#define Scan_LineStart  S_LineStart
#define Scan_BuffPos    S_BuffPos
#define Scan_CurrCol    S_CurrCol
#define Scan_NextLen    S_NextLen

#if SEGMENTED_BUFF          /* Segmented Buffer for small (MS-DOS) systems */
#  define  BLKSIZE  16384
   static  unsigned char  *Buffer[32]; /* source buffer for low-level access */
#  define CURRENT_CH(Pos) (Buffer[(int) (Pos / BLKSIZE)])[(int) (Pos % BLKSIZE)]
#else
   static unsigned char *Buffer;
#  define CURRENT_CH(Pos) Buffer[Pos]
#endif

static int  S_ComEols;   /* number of EOLs in a comment */
static long S_BuffPos;   /* current position in buf */
static int  S_CurrCol;   /* current Column Number */
static long S_InputLen;  /* source file size */

unsigned char S_CurrentCh(long pos);

#define IGNORE_CASE  0
#define UPCASE(c)    ((c >= 'a' && c <= 'z') ? c-32 : c)

unsigned char S_CurrentCh(long pos)
{ return (unsigned char) CURRENT_CH(pos); }

#if IGNORE_CASE
#define S_NextCh() { S_BuffPos++;\
		     S_ch = CURRENT_CH(S_BuffPos);\
		     S_ch = UPCASE(S_ch);\
		     if (S_ch == TAB_CHAR) S_CurrCol += TAB_SIZE - (S_CurrCol % TAB_SIZE); else \
		     if (S_ch == LF_CHAR) {\
		       S_CurrLine++; S_CurrCol = 0;\
		       S_LineStart = S_BuffPos + 1;}\
		     S_CurrCol++;\
		   }
#else
#define S_NextCh() { S_BuffPos++;\
		     S_ch = CURRENT_CH(S_BuffPos);\
		     if (S_ch == TAB_CHAR) S_CurrCol += TAB_SIZE - (S_CurrCol % TAB_SIZE); else \
		     if (S_ch == LF_CHAR) {\
		       S_CurrLine++; S_CurrCol = 0;\
		       S_LineStart = S_BuffPos + 1;}\
		     S_CurrCol++;\
		   }
#endif

static int EqualStr (char *s)
{ long q; char c;
  if (S_NextLen != (int) strlen(s)) return 0;
  q = S_NextPos;
  while (*s) {
    c = CURRENT_CH(q); q++;
#if IGNORE_CASE
    c = UPCASE(c);
#endif
    if (c != *s++) return 0;
  }
  return 1;
}

static int CheckLiteral(int id)
{ char c;
  c = CURRENT_CH(S_NextPos);
#if IGNORE_CASE
  c = UPCASE(c);
#endif
  switch (c) {
  
  }
  return id;
}

static int Comment(void)
{ int Level, StartLine, OldCol;
  long OldLineStart;

  Level = 1; StartLine = S_CurrLine;
  OldLineStart = S_LineStart; OldCol = S_CurrCol;
  
  return 0;
}

int S_Get(void)
{ int state, ctx;
  start:
  while (Scan_Ch == ' ') Scan_NextCh();
  

  S_Pos  = S_NextPos;   S_NextPos  = S_BuffPos;
  S_Col  = S_NextCol;   S_NextCol  = S_CurrCol - 1;
  S_Line = S_NextLine;  S_NextLine = S_CurrLine;
  S_Len  = S_NextLen;   S_NextLen  = 0; ctx = 0;

  if (S_ch == EOF_CHAR) return EOF_Sym;
  state = STATE0[S_ch];
  while(1) {
    S_NextCh(); S_NextLen++;
    switch (state) {
     /* State 0; valid STATE0 Table
    case 0:
    	if (Scan_Ch >= 'A' && Scan_Ch <= 'Z' ||
        Scan_Ch >= 'a' && Scan_Ch <= 'z') state = 1; else
    	if (Scan_Ch >= '0' && Scan_Ch <= '9') state = 2; else
    	if (Scan_Ch == 13) state = 9; else
    	if (Scan_Ch == 10) state = 10; else
    	if (Scan_Ch == '#') state = 11; else
    	if (Scan_Ch == '.') state = 13; else
    	return No_Sym;
    	break;
     --------- End State0 --------- */
    case 1:
    	if (Scan_Ch >= '0' && Scan_Ch <= '9' ||
    	    Scan_Ch >= 'A' && Scan_Ch <= 'Z' ||
    	    Scan_Ch >= 'a' && Scan_Ch <= 'z') /*same state*/; else
    	return identSym;
    	break;
    case 2:
    	if (Scan_Ch >= '0' && Scan_Ch <= '9') /*same state*/; else
    	return numberSym;
    	break;
    case 7:
    	return nlSym;
    case 8:
    	return nlSym;
    case 9:
    	if (Scan_Ch == 10) state = 7; else
    	return nlSym;
    	break;
    case 10:
    	if (Scan_Ch == 13) state = 8; else
    	return nlSym;
    	break;
    case 11:
    	if (Scan_Ch == '!') state = 12; else
    	return No_Sym;
    	break;
    case 12:
    	return HashBangSym;
    case 13:
    	if (Scan_Ch == ',') state = 14; else
    	if (Scan_Ch == 's') state = 15; else
    	return No_Sym;
    	break;
    case 14:
    	return PointCommaSym;
    case 15:
    	if (Scan_Ch == 'h') state = 16; else
    	return No_Sym;
    	break;
    case 16:
    	return PointshSym;
    
    default: return No_Sym; /* S_NextCh already done */
    }
  }
}

void S_GetString (long pos, int len, char *s, int max)
{ while (len-- && max--) { *s++ = CURRENT_CH(pos); pos++; }
  *s = '\0';
}

void S_GetName (long pos, int len, char *s, int max)
{ while (len-- && max--) {
    *s = CURRENT_CH(pos);
#if IGNORE_CASE
    *s = UPCASE(*s);
#endif
    s++; pos++;
  }
  *s = '\0';
}

void S_Reset(void)
/* assert: S_src has been opened */
{ long len;
  int  i = 0, n;

  S_InputLen = len = lseek(S_src, 0L, SEEK_END);
  (void) lseek(S_src, 0L, SEEK_SET);

#if SEGMENTED_BUFF
  while (len > BLKSIZE) {
    Buffer[i] = (unsigned char *) malloc(BLKSIZE);
    n = read(S_src, (void *) Buffer[i], (unsigned) BLKSIZE);
    len -= n;
    i++;
  }
  Buffer[i] = (unsigned char *) malloc((unsigned) len + 1);
  n = read(S_src, (void *) Buffer[i], (unsigned) len);
  (Buffer[i])[n] = EOF_CHAR;
#else 
  Buffer = (unsigned char *) malloc(len+1);
  n = read(S_src, (void *) Buffer, len);
  Buffer[n] = EOF_CHAR;
#endif

  S_CurrLine = 1; S_LineStart = 0; S_BuffPos = -1; S_CurrCol = 0;
  S_NextPos  = 0; S_NextCol = 0; S_NextLine = 0; S_NextLen = 0;
  S_ComEols = 0;
  S_NextCh();
}

