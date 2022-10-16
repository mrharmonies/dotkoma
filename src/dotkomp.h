
#ifndef PARSER_HEADER_INCLUDE
#define PARSER_HEADER_INCLUDE

typedef void (*Error_Func) (int nr, int line, int col, long pos);

#define MinErrDist 2
#define MAXERROR   9

void Parse(void);
/* Parse the source */

int Successful(void);
/* Returns 1 if no errors have been recorded while parsing */

void SynError(int errno);
/* Report Syntactic Error errno */

void SemError(int errno);
/* Report Semantic Error errno */

#define LexString(lex, size)    S_GetString(S_Pos, S_Len, lex, size)
/* Retrieves Lex as exact spelling of current token */

#define LexName(lex, size)      S_GetName(S_Pos, S_Len, lex, size)
/* Retrieves Lex as name of current token (capitalized if IGNORE CASE) */

#define LookAheadString(lex, size)      S_GetString(S_NextPos, S_NextLen, lex, size)
/* Retrieves Lex as exact spelling of lookahead token */

#define LookAheadName(lex, size)        S_GetName(S_NextPos, S_NextLen, lex, size)
/* Retrieves Lex as name of lookahead token (capitalized if IGNORE CASE) */

#endif /* PARSER_HEADER_INCLUDE */

