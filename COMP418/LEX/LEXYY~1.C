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
  #include "lex.h"
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
  { return( TOKEN_KEY_PROGRAM ); }
break;
case 3:
      { return( TOKEN_KEY_VAR ); }
break;
case 4:
    { return( TOKEN_KEY_ARRAY ); }
break;
case 5:
       { return( TOKEN_KEY_OF ); }
break;
case 6:
  { return( TOKEN_KEY_INTEGER ); }
break;
case 7:
     { return( TOKEN_KEY_REAL ); }
break;
case 8:
 { return( TOKEN_KEY_FUNCTION ); }
break;
case 9:
{ return( TOKEN_KEY_PROCEDURE ); }
break;
case 10:
    { return( TOKEN_KEY_BEGIN ); }
break;
case 11:
      { return( TOKEN_KEY_END ); }
break;
case 12:
       { return( TOKEN_KEY_IF ); }
break;
case 13:
     { return( TOKEN_KEY_THEN ); }
break;
case 14:
     { return( TOKEN_KEY_ELSE ); }
break;
case 15:
    { return( TOKEN_KEY_WHILE ); }
break;
case 16:
       { return( TOKEN_KEY_DO ); }
break;
case 17:
      { return( TOKEN_KEY_NOT ); }
break;
case 18:
       { yylval = LVAL_OR;
            return( TOKEN_ADDOP ); }
break;
case 19:
      { yylval = LVAL_AND;
            return( TOKEN_MULOP ); }
break;
case 20:
      { yylval = LVAL_DIV;
            return( TOKEN_MULOP ); }
break;
case 21:
      { yylval = LVAL_MOD;
            return( TOKEN_MULOP ); }
break;
case 22:
     { yylval = install_id();
            return( TOKEN_ID ); }
break;
case 23:
 { yylval = install_num();
            return( TOKEN_NUM ); }
break;
case 24:
      { yylval = LVAL_LT;
            return( TOKEN_RELOP ); }
break;
case 25:
     { yylval = LVAL_LE;
            return( TOKEN_RELOP ); }
break;
case 26:
      { yylval = LVAL_EQ;
            return( TOKEN_RELOP ); }
break;
case 27:
     { yylval = LVAL_NE;
            return( TOKEN_RELOP ); }
break;
case 28:
      { yylval = LVAL_GT;
            return( TOKEN_RELOP ); }
break;
case 29:
     { yylval = LVAL_GE;
            return( TOKEN_RELOP ); }
break;
case 30:
      { yylval = LVAL_DIV_REAL;
            return( TOKEN_MULOP ); }
break;
case 31:
      { yylval = LVAL_MUL;
            return( TOKEN_MULOP ); }
break;
case 32:
      { yylval = LVAL_PLUS;
            return( TOKEN_ADDOP ); }
break;
case 33:
      { yylval = LVAL_MINUS;
            return( TOKEN_ADDOP ); }
break;
case 34:
      { while ( input() != '}' ); }
break;
case 35:
      { return( TOKEN_PERIOD ); }
break;
case 36:
     { return( TOKEN_DOTDOT ); }
break;
case 37:
      { return( TOKEN_COMMA ); }
break;
case 38:
      { return( TOKEN_SEMICOLON ); }
break;
case 39:
      { return( TOKEN_COLON ); }
break;
case 40:
     { return( TOKEN_ASSIGNOP ); }
break;
case 41:
      { return( TOKEN_PAREN_OPEN ); }
break;
case 42:
      { return( TOKEN_PAREN_CLOSE ); }
break;
case 43:
      { return( TOKEN_BRACKET_OPEN ); }
break;
case 44:
      { return( TOKEN_BRACKET_CLOSE ); }
break;
case 45:
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

45,
0,

1,
45,
0,

1,
0,

41,
45,
0,

42,
45,
0,

31,
45,
0,

32,
45,
0,

37,
45,
0,

33,
45,
0,

35,
45,
0,

30,
45,
0,

23,
45,
0,

39,
45,
0,

38,
45,
0,

24,
45,
0,

26,
45,
0,

28,
45,
0,

22,
45,
0,

43,
45,
0,

44,
45,
0,

22,
45,
0,

22,
45,
0,

22,
45,
0,

22,
45,
0,

22,
45,
0,

22,
45,
0,

22,
45,
0,

22,
45,
0,

22,
45,
0,

22,
45,
0,

22,
45,
0,

22,
45,
0,

22,
45,
0,

22,
45,
0,

34,
45,
0,

36,
0,

23,
0,

40,
0,

25,
0,

27,
0,

29,
0,

22,
0,

22,
0,

22,
0,

22,
0,

22,
0,

16,
22,
0,

22,
0,

22,
0,

22,
0,

12,
22,
0,

22,
0,

22,
0,

22,
0,

5,
22,
0,

18,
22,
0,

22,
0,

22,
0,

22,
0,

22,
0,

22,
0,

23,
0,

23,
0,

19,
22,
0,

22,
0,

22,
0,

20,
22,
0,

22,
0,

11,
22,
0,

22,
0,

22,
0,

21,
22,
0,

17,
22,
0,

22,
0,

22,
0,

22,
0,

3,
22,
0,

22,
0,

22,
0,

22,
0,

14,
22,
0,

22,
0,

22,
0,

22,
0,

22,
0,

7,
22,
0,

13,
22,
0,

22,
0,

4,
22,
0,

10,
22,
0,

22,
0,

22,
0,

22,
0,

22,
0,

15,
22,
0,

22,
0,

22,
0,

22,
0,

22,
0,

22,
0,

6,
22,
0,

22,
0,

2,
22,
0,

8,
22,
0,

22,
0,

9,
22,
0,
0};
# define YYTYPE char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
4,5,	4,5,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	4,5,	
0,0,	0,0,	0,0,	0,0,	
0,0,	1,6,	1,7,	1,8,	
1,9,	1,10,	1,11,	1,12,	
1,13,	1,14,	12,38,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	1,15,	
1,16,	1,17,	1,18,	1,19,	
15,42,	14,39,	1,20,	14,40,	
14,40,	14,40,	14,40,	14,40,	
14,40,	14,40,	14,40,	14,40,	
14,40,	17,43,	17,44,	19,45,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
14,41,	0,0,	0,0,	0,0,	
1,21,	0,0,	1,22,	0,0,	
0,0,	0,0,	1,23,	1,24,	
0,0,	1,25,	1,26,	1,27,	
24,49,	0,0,	1,28,	0,0,	
28,55,	0,0,	1,29,	1,30,	
1,31,	1,32,	23,47,	1,33,	
28,56,	1,34,	23,48,	1,35,	
1,36,	2,6,	2,7,	2,8,	
1,37,	2,10,	2,11,	2,12,	
2,13,	25,50,	26,52,	27,54,	
26,53,	29,57,	30,58,	25,51,	
31,59,	32,61,	33,62,	2,15,	
2,16,	2,17,	2,18,	2,19,	
34,63,	35,64,	36,65,	47,69,	
31,60,	39,66,	39,66,	39,66,	
39,66,	39,66,	39,66,	39,66,	
39,66,	39,66,	39,66,	48,70,	
49,71,	50,72,	52,73,	53,74,	
54,75,	56,76,	57,77,	58,78,	
61,79,	62,80,	63,81,	64,82,	
2,21,	65,83,	2,22,	66,41,	
70,84,	71,85,	2,23,	2,24,	
73,86,	2,25,	2,26,	2,27,	
75,87,	76,88,	2,28,	80,91,	
81,92,	83,93,	2,29,	2,30,	
2,31,	2,32,	79,89,	2,33,	
84,94,	2,34,	79,90,	2,35,	
2,36,	85,95,	87,96,	88,97,	
2,37,	20,46,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
20,46,	20,46,	20,46,	89,98,	
90,99,	93,100,	96,101,	97,102,	
98,103,	99,104,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
101,105,	102,106,	103,107,	104,108,	
105,109,	107,110,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
20,46,	20,46,	20,46,	20,46,	
41,67,	110,111,	41,67,	0,0,	
0,0,	41,68,	41,68,	41,68,	
41,68,	41,68,	41,68,	41,68,	
41,68,	41,68,	41,68,	67,68,	
67,68,	67,68,	67,68,	67,68,	
67,68,	67,68,	67,68,	67,68,	
67,68,	0,0,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		0,	
yycrank+-81,	yysvec+1,	0,	
yycrank+0,	0,		yyvstop+1,
yycrank+3,	0,		yyvstop+3,
yycrank+0,	yysvec+4,	yyvstop+6,
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
yycrank+34,	yysvec+20,	yyvstop+83,
yycrank+23,	yysvec+20,	yyvstop+86,
yycrank+37,	yysvec+20,	yyvstop+89,
yycrank+40,	yysvec+20,	yyvstop+92,
yycrank+48,	yysvec+20,	yyvstop+95,
yycrank+42,	yysvec+20,	yyvstop+98,
yycrank+0,	0,		yyvstop+101,
yycrank+0,	0,		yyvstop+104,
yycrank+101,	0,		0,	
yycrank+0,	yysvec+14,	yyvstop+106,
yycrank+237,	0,		0,	
yycrank+0,	0,		yyvstop+108,
yycrank+0,	0,		yyvstop+110,
yycrank+0,	0,		yyvstop+112,
yycrank+0,	0,		yyvstop+114,
yycrank+0,	yysvec+20,	yyvstop+116,
yycrank+47,	yysvec+20,	yyvstop+118,
yycrank+45,	yysvec+20,	yyvstop+120,
yycrank+57,	yysvec+20,	yyvstop+122,
yycrank+43,	yysvec+20,	yyvstop+124,
yycrank+0,	yysvec+20,	yyvstop+126,
yycrank+47,	yysvec+20,	yyvstop+129,
yycrank+63,	yysvec+20,	yyvstop+131,
yycrank+54,	yysvec+20,	yyvstop+133,
yycrank+0,	yysvec+20,	yyvstop+135,
yycrank+49,	yysvec+20,	yyvstop+138,
yycrank+66,	yysvec+20,	yyvstop+140,
yycrank+51,	yysvec+20,	yyvstop+142,
yycrank+0,	yysvec+20,	yyvstop+144,
yycrank+0,	yysvec+20,	yyvstop+147,
yycrank+57,	yysvec+20,	yyvstop+150,
yycrank+72,	yysvec+20,	yyvstop+152,
yycrank+69,	yysvec+20,	yyvstop+154,
yycrank+57,	yysvec+20,	yyvstop+156,
yycrank+68,	yysvec+20,	yyvstop+158,
yycrank+106,	yysvec+39,	yyvstop+160,
yycrank+247,	0,		0,	
yycrank+0,	yysvec+67,	yyvstop+162,
yycrank+0,	yysvec+20,	yyvstop+164,
yycrank+79,	yysvec+20,	yyvstop+167,
yycrank+72,	yysvec+20,	yyvstop+169,
yycrank+0,	yysvec+20,	yyvstop+171,
yycrank+79,	yysvec+20,	yyvstop+174,
yycrank+0,	yysvec+20,	yyvstop+176,
yycrank+85,	yysvec+20,	yyvstop+179,
yycrank+84,	yysvec+20,	yyvstop+181,
yycrank+0,	yysvec+20,	yyvstop+183,
yycrank+0,	yysvec+20,	yyvstop+186,
yycrank+95,	yysvec+20,	yyvstop+189,
yycrank+79,	yysvec+20,	yyvstop+191,
yycrank+78,	yysvec+20,	yyvstop+193,
yycrank+0,	yysvec+20,	yyvstop+195,
yycrank+81,	yysvec+20,	yyvstop+198,
yycrank+75,	yysvec+20,	yyvstop+200,
yycrank+91,	yysvec+20,	yyvstop+202,
yycrank+0,	yysvec+20,	yyvstop+204,
yycrank+86,	yysvec+20,	yyvstop+207,
yycrank+100,	yysvec+20,	yyvstop+209,
yycrank+114,	yysvec+20,	yyvstop+211,
yycrank+102,	yysvec+20,	yyvstop+213,
yycrank+0,	yysvec+20,	yyvstop+215,
yycrank+0,	yysvec+20,	yyvstop+218,
yycrank+116,	yysvec+20,	yyvstop+221,
yycrank+0,	yysvec+20,	yyvstop+223,
yycrank+0,	yysvec+20,	yyvstop+226,
yycrank+113,	yysvec+20,	yyvstop+229,
yycrank+118,	yysvec+20,	yyvstop+231,
yycrank+120,	yysvec+20,	yyvstop+233,
yycrank+124,	yysvec+20,	yyvstop+235,
yycrank+0,	yysvec+20,	yyvstop+237,
yycrank+137,	yysvec+20,	yyvstop+240,
yycrank+135,	yysvec+20,	yyvstop+242,
yycrank+133,	yysvec+20,	yyvstop+244,
yycrank+142,	yysvec+20,	yyvstop+246,
yycrank+142,	yysvec+20,	yyvstop+248,
yycrank+0,	yysvec+20,	yyvstop+250,
yycrank+139,	yysvec+20,	yyvstop+253,
yycrank+0,	yysvec+20,	yyvstop+255,
yycrank+0,	yysvec+20,	yyvstop+258,
yycrank+180,	yysvec+20,	yyvstop+261,
yycrank+0,	yysvec+20,	yyvstop+263,
0,	0,	0};
struct yywork *yytop = yycrank+304;
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
