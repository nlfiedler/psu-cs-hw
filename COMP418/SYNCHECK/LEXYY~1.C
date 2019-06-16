# include "stdio.h"
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX BUFSIZ
# define output(c) putc(c,yyout)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
# define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng; extern char yytext[];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin = {stdin}, *yyout = {stdout};
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
  #include "symtab.h"
  #include "syncheck.h"
# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:
     { }
break;
case 2:
  { curLine++; }
break;
case 3:
   { printf( "Missing fraction on real number.\n" ); }
break;
case 4:
   { printf( "Missing exponent on real number.\n" ); }
break;
case 5:
   { printf( "Missing integer portion of real number.\n" ); }
break;
case 6:
  { return( TOKEN_KEY_PROGRAM ); }
break;
case 7:
      { return( TOKEN_KEY_VAR ); }
break;
case 8:
    { return( TOKEN_KEY_ARRAY ); }
break;
case 9:
       { return( TOKEN_KEY_OF ); }
break;
case 10:
  { return( TOKEN_KEY_INTEGER ); }
break;
case 11:
     { return( TOKEN_KEY_REAL ); }
break;
case 12:
 { return( TOKEN_KEY_FUNCTION ); }
break;
case 13:
{ return( TOKEN_KEY_PROCEDURE ); }
break;
case 14:
    { return( TOKEN_KEY_BEGIN ); }
break;
case 15:
      { return( TOKEN_KEY_END ); }
break;
case 16:
       { return( TOKEN_KEY_IF ); }
break;
case 17:
     { return( TOKEN_KEY_THEN ); }
break;
case 18:
     { return( TOKEN_KEY_ELSE ); }
break;
case 19:
    { return( TOKEN_KEY_WHILE ); }
break;
case 20:
       { return( TOKEN_KEY_DO ); }
break;
case 21:
      { return( TOKEN_KEY_NOT ); }
break;
case 22:
       { yylval = LVAL_OR;
            return( TOKEN_ADDOP ); }
break;
case 23:
      { yylval = LVAL_AND;
            return( TOKEN_MULOP ); }
break;
case 24:
      { yylval = LVAL_DIV;
            return( TOKEN_MULOP ); }
break;
case 25:
      { yylval = LVAL_MOD;
            return( TOKEN_MULOP ); }
break;
case 26:
     { yylval = install_id();
            return( TOKEN_ID ); }
break;
case 27:
 { yylval = install_num();
            return( TOKEN_NUM ); }
break;
case 28:
      { yylval = LVAL_LT;
            return( TOKEN_RELOP ); }
break;
case 29:
     { yylval = LVAL_LE;
            return( TOKEN_RELOP ); }
break;
case 30:
      { yylval = LVAL_EQ;
            return( TOKEN_RELOP ); }
break;
case 31:
     { yylval = LVAL_NE;
            return( TOKEN_RELOP ); }
break;
case 32:
      { yylval = LVAL_GT;
            return( TOKEN_RELOP ); }
break;
case 33:
     { yylval = LVAL_GE;
            return( TOKEN_RELOP ); }
break;
case 34:
      { yylval = LVAL_DIV_REAL;
            return( TOKEN_MULOP ); }
break;
case 35:
      { yylval = LVAL_MUL;
            return( TOKEN_MULOP ); }
break;
case 36:
      { yylval = LVAL_PLUS;
            return( TOKEN_ADDOP ); }
break;
case 37:
      { yylval = LVAL_MINUS;
            return( TOKEN_ADDOP ); }
break;
case 38:
      { while ( input() != '}' ); }
break;
case 39:
      { return( TOKEN_PERIOD ); }
break;
case 40:
     { return( TOKEN_DOTDOT ); }
break;
case 41:
      { return( TOKEN_COMMA ); }
break;
case 42:
      { return( TOKEN_SEMICOLON ); }
break;
case 43:
      { return( TOKEN_COLON ); }
break;
case 44:
     { return( TOKEN_ASSIGNOP ); }
break;
case 45:
      { return( TOKEN_PAREN_OPEN ); }
break;
case 46:
      { return( TOKEN_PAREN_CLOSE ); }
break;
case 47:
      { return( TOKEN_BRACKET_OPEN ); }
break;
case 48:
      { return( TOKEN_BRACKET_CLOSE ); }
break;
case 49:
        { printf( "Character not part of a token.\n" ); }
break;
case -1:
break;
default:
fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */

/********************************************************************
 *                 install_id
 ********************************************************************
 * SYNOPSIS:     Insert the current lexeme and token into the
 *               symbol table.
 * RETURNS:      Index - index into symbol table, 0 if keyword.
 * STRATEGY:     See if the symbol is already in the table. If so,
 *               return either it's index, or 0 if it's a keyword.
 *******************************************************************/
Index install_id
  (
  void
  )
{
  Index  index;

  index = lookup( yytext );
  if ( index == SYMBOL_NOT_FOUND ) {
    return( insert( yytext, TOKEN_ID ) );
  }
  else {
    return( index );
  }
} /* install_id */

/********************************************************************
 *                 install_num
 ********************************************************************
 * SYNOPSIS:     Insert the current number and appropriate token
 *               into the symbol table.
 * RETURNS:      Index - index into symbol table.
 * STRATEGY:     See if the number is already there. If not,
 *               insert it and return index. If it's there,
 *               return the index.
 *******************************************************************/
Index install_num
  (
  void
  )
{
     /* Index to entry containing number. */
  Index index;

  index = lookup( yytext );
  if ( index == SYMBOL_NOT_FOUND ) {
    return( insert( yytext, TOKEN_NUM ) );
  }
  else {
    return( index );
  }
} /* install_num */

/********************************************************************
 *                 yywrap
 ********************************************************************
 * SYNOPSIS:     Returns 1 when yylex reaches the end of the file.
 * RETURNS:      int - 1 no matter what
 * STRATEGY:     Return 1 no matter what.
 *******************************************************************/
int yywrap
  (
  void
  )
{
  return( 1 );
} /* yywrap */
int yyvstop[] = {
0,

49,
0,

1,
49,
0,

2,
0,

45,
49,
0,

46,
49,
0,

35,
49,
0,

36,
49,
0,

41,
49,
0,

37,
49,
0,

39,
49,
0,

34,
49,
0,

27,
49,
0,

43,
49,
0,

42,
49,
0,

28,
49,
0,

30,
49,
0,

32,
49,
0,

26,
49,
0,

47,
49,
0,

48,
49,
0,

26,
49,
0,

26,
49,
0,

26,
49,
0,

26,
49,
0,

26,
49,
0,

26,
49,
0,

26,
49,
0,

26,
49,
0,

26,
49,
0,

26,
49,
0,

26,
49,
0,

26,
49,
0,

26,
49,
0,

26,
49,
0,

38,
49,
0,

1,
0,

40,
0,

27,
0,

44,
0,

29,
0,

31,
0,

33,
0,

26,
0,

26,
0,

26,
0,

26,
0,

26,
0,

20,
26,
0,

26,
0,

26,
0,

26,
0,

26,
0,

16,
26,
0,

26,
0,

26,
0,

26,
0,

9,
26,
0,

22,
26,
0,

26,
0,

26,
0,

26,
0,

26,
0,

26,
0,

27,
0,

27,
0,

23,
26,
0,

26,
0,

26,
0,

24,
26,
0,

26,
0,

15,
26,
0,

26,
0,

26,
0,

26,
0,

25,
26,
0,

21,
26,
0,

26,
0,

26,
0,

26,
0,

7,
26,
0,

26,
0,

26,
0,

26,
0,

18,
26,
0,

26,
0,

26,
0,

26,
0,

26,
0,

26,
0,

11,
26,
0,

17,
26,
0,

26,
0,

8,
26,
0,

14,
26,
0,

26,
0,

26,
0,

26,
0,

26,
0,

26,
0,

19,
26,
0,

3,
26,
0,

4,
26,
0,

5,
26,
0,

26,
0,

26,
0,

26,
0,

26,
0,

26,
0,

10,
26,
0,

26,
0,

6,
26,
0,

12,
26,
0,

26,
0,

13,
26,
0,
0};
# define YYTYPE char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
4,38,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	4,38,	
0,0,	0,0,	0,0,	0,0,	
0,0,	1,6,	1,7,	1,8,	
1,9,	1,10,	1,11,	1,12,	
1,13,	1,14,	12,39,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	1,15,	
1,16,	1,17,	1,18,	1,19,	
15,43,	14,40,	1,20,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	14,41,	14,41,	14,41,	
14,41,	17,44,	17,45,	19,46,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
14,42,	0,0,	0,0,	0,0,	
1,21,	0,0,	1,22,	0,0,	
0,0,	0,0,	1,23,	1,24,	
0,0,	1,25,	1,26,	1,27,	
24,50,	0,0,	1,28,	0,0,	
28,57,	0,0,	1,29,	1,30,	
1,31,	1,32,	23,48,	1,33,	
28,58,	1,34,	23,49,	1,35,	
1,36,	2,6,	2,7,	2,8,	
1,37,	2,10,	2,11,	2,12,	
2,13,	25,51,	26,53,	27,56,	
26,54,	29,59,	30,60,	25,52,	
26,55,	31,61,	32,63,	2,15,	
2,16,	2,17,	2,18,	2,19,	
33,64,	34,65,	35,66,	36,67,	
48,71,	31,62,	40,68,	40,68,	
40,68,	40,68,	40,68,	40,68,	
40,68,	40,68,	40,68,	40,68,	
49,72,	50,73,	51,74,	53,75,	
54,76,	55,77,	56,78,	58,79,	
59,80,	60,81,	63,82,	64,83,	
2,21,	65,84,	2,22,	66,85,	
67,86,	68,42,	2,23,	2,24,	
72,87,	2,25,	2,26,	2,27,	
73,88,	75,89,	2,28,	77,90,	
78,91,	79,92,	2,29,	2,30,	
2,31,	2,32,	82,93,	2,33,	
83,95,	2,34,	82,94,	2,35,	
2,36,	84,96,	86,97,	87,98,	
2,37,	20,47,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
20,47,	20,47,	20,47,	88,99,	
90,100,	91,101,	92,102,	93,103,	
94,104,	97,105,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
100,106,	100,107,	100,108,	101,109,	
102,110,	103,111,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
20,47,	20,47,	20,47,	20,47,	
42,69,	104,112,	42,69,	109,113,	
110,114,	42,70,	42,70,	42,70,	
42,70,	42,70,	42,70,	42,70,	
42,70,	42,70,	42,70,	69,70,	
69,70,	69,70,	69,70,	69,70,	
69,70,	69,70,	69,70,	69,70,	
69,70,	111,115,	112,116,	113,117,	
115,118,	118,119,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-81,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+3,	0,		yyvstop+3,
yycrank+0,	0,		yyvstop+6,
yycrank+0,	0,		yyvstop+8,
yycrank+0,	0,		yyvstop+11,
yycrank+0,	0,		yyvstop+14,
yycrank+0,	0,		yyvstop+17,
yycrank+0,	0,		yyvstop+20,
yycrank+0,	0,		yyvstop+23,
yycrank+4,	0,		yyvstop+26,
yycrank+0,	0,		yyvstop+29,
yycrank+19,	0,		yyvstop+32,
yycrank+3,	0,		yyvstop+35,
yycrank+0,	0,		yyvstop+38,
yycrank+16,	0,		yyvstop+41,
yycrank+0,	0,		yyvstop+44,
yycrank+18,	0,		yyvstop+47,
yycrank+157,	0,		yyvstop+50,
yycrank+0,	0,		yyvstop+53,
yycrank+0,	0,		yyvstop+56,
yycrank+4,	yysvec+20,	yyvstop+59,
yycrank+3,	yysvec+20,	yyvstop+62,
yycrank+24,	yysvec+20,	yyvstop+65,
yycrank+22,	yysvec+20,	yyvstop+68,
yycrank+14,	yysvec+20,	yyvstop+71,
yycrank+6,	yysvec+20,	yyvstop+74,
yycrank+22,	yysvec+20,	yyvstop+77,
yycrank+23,	yysvec+20,	yyvstop+80,
yycrank+35,	yysvec+20,	yyvstop+83,
yycrank+24,	yysvec+20,	yyvstop+86,
yycrank+43,	yysvec+20,	yyvstop+89,
yycrank+41,	yysvec+20,	yyvstop+92,
yycrank+49,	yysvec+20,	yyvstop+95,
yycrank+43,	yysvec+20,	yyvstop+98,
yycrank+0,	0,		yyvstop+101,
yycrank+0,	yysvec+4,	yyvstop+104,
yycrank+0,	0,		yyvstop+106,
yycrank+102,	0,		0,	
yycrank+0,	yysvec+14,	yyvstop+108,
yycrank+237,	0,		0,	
yycrank+0,	0,		yyvstop+110,
yycrank+0,	0,		yyvstop+112,
yycrank+0,	0,		yyvstop+114,
yycrank+0,	0,		yyvstop+116,
yycrank+0,	yysvec+20,	yyvstop+118,
yycrank+48,	yysvec+20,	yyvstop+120,
yycrank+46,	yysvec+20,	yyvstop+122,
yycrank+58,	yysvec+20,	yyvstop+124,
yycrank+44,	yysvec+20,	yyvstop+126,
yycrank+0,	yysvec+20,	yyvstop+128,
yycrank+48,	yysvec+20,	yyvstop+131,
yycrank+64,	yysvec+20,	yyvstop+133,
yycrank+51,	yysvec+20,	yyvstop+135,
yycrank+56,	yysvec+20,	yyvstop+137,
yycrank+0,	yysvec+20,	yyvstop+139,
yycrank+51,	yysvec+20,	yyvstop+142,
yycrank+68,	yysvec+20,	yyvstop+144,
yycrank+53,	yysvec+20,	yyvstop+146,
yycrank+0,	yysvec+20,	yyvstop+148,
yycrank+0,	yysvec+20,	yyvstop+151,
yycrank+59,	yysvec+20,	yyvstop+154,
yycrank+74,	yysvec+20,	yyvstop+156,
yycrank+72,	yysvec+20,	yyvstop+158,
yycrank+61,	yysvec+20,	yyvstop+160,
yycrank+71,	yysvec+20,	yyvstop+162,
yycrank+108,	yysvec+40,	yyvstop+164,
yycrank+247,	0,		0,	
yycrank+0,	yysvec+69,	yyvstop+166,
yycrank+0,	yysvec+20,	yyvstop+168,
yycrank+83,	yysvec+20,	yyvstop+171,
yycrank+79,	yysvec+20,	yyvstop+173,
yycrank+0,	yysvec+20,	yyvstop+175,
yycrank+84,	yysvec+20,	yyvstop+178,
yycrank+0,	yysvec+20,	yyvstop+180,
yycrank+76,	yysvec+20,	yyvstop+183,
yycrank+89,	yysvec+20,	yyvstop+185,
yycrank+88,	yysvec+20,	yyvstop+187,
yycrank+0,	yysvec+20,	yyvstop+189,
yycrank+0,	yysvec+20,	yyvstop+192,
yycrank+95,	yysvec+20,	yyvstop+195,
yycrank+88,	yysvec+20,	yyvstop+197,
yycrank+91,	yysvec+20,	yyvstop+199,
yycrank+0,	yysvec+20,	yyvstop+201,
yycrank+94,	yysvec+20,	yyvstop+204,
yycrank+82,	yysvec+20,	yyvstop+206,
yycrank+105,	yysvec+20,	yyvstop+208,
yycrank+0,	yysvec+20,	yyvstop+210,
yycrank+102,	yysvec+20,	yyvstop+213,
yycrank+101,	yysvec+20,	yyvstop+215,
yycrank+115,	yysvec+20,	yyvstop+217,
yycrank+118,	yysvec+20,	yyvstop+219,
yycrank+106,	yysvec+20,	yyvstop+221,
yycrank+0,	yysvec+20,	yyvstop+223,
yycrank+0,	yysvec+20,	yyvstop+226,
yycrank+120,	yysvec+20,	yyvstop+229,
yycrank+0,	yysvec+20,	yyvstop+231,
yycrank+0,	yysvec+20,	yyvstop+234,
yycrank+199,	yysvec+20,	yyvstop+237,
yycrank+146,	yysvec+20,	yyvstop+239,
yycrank+151,	yysvec+20,	yyvstop+241,
yycrank+153,	yysvec+20,	yyvstop+243,
yycrank+184,	yysvec+20,	yyvstop+245,
yycrank+0,	yysvec+20,	yyvstop+247,
yycrank+0,	yysvec+20,	yyvstop+250,
yycrank+0,	yysvec+20,	yyvstop+253,
yycrank+0,	yysvec+20,	yyvstop+256,
yycrank+172,	yysvec+20,	yyvstop+259,
yycrank+170,	yysvec+20,	yyvstop+261,
yycrank+188,	yysvec+20,	yyvstop+263,
yycrank+197,	yysvec+20,	yyvstop+265,
yycrank+197,	yysvec+20,	yyvstop+267,
yycrank+0,	yysvec+20,	yyvstop+269,
yycrank+194,	yysvec+20,	yyvstop+272,
yycrank+0,	yysvec+20,	yyvstop+274,
yycrank+0,	yysvec+20,	yyvstop+277,
yycrank+208,	yysvec+20,	yyvstop+280,
yycrank+0,	yysvec+20,	yyvstop+282,
0,	0,	0};
struct yywork *yytop = yycrank+309;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,011 ,012 ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
011 ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,'+' ,01  ,'+' ,01  ,01  ,
'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,
'0' ,'0' ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,01  ,01  ,01  ,01  ,01  ,
01  ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,'A' ,
'A' ,'A' ,'A' ,01  ,01  ,01  ,01  ,01  ,
0};
char yyextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
#ifndef lint
static	char ncform_sccsid[] = "@(#)ncform 1.6 88/02/08 SMI"; /* from S5R2 1.2 */
#endif

int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
char yytext[YYLMAX];
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
yylook(){
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych, yyfirst;
	struct yywork *yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	yyfirst=1;
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank && !yyfirst){  /* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
			*yylastch++ = yych = input();
			yyfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (int)yyt > (int)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)yyt < (int)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
yyback(p, m)
	int *p;
{
if (p==0) return(0);
while (*p)
	{
	if (*p++ == m)
		return(1);
	}
return(0);
}
	/* the following are only used in the lex library */
yyinput(){
	return(input());
	}
yyoutput(c)
  int c; {
	output(c);
	}
yyunput(c)
   int c; {
	unput(c);
	}
