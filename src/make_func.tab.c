extern char *malloc(), *realloc();

# line 2 "make_func.y"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include "config.h"
#include "mudlib_stats.h"
#include "interpret.h"
#include "mapping.h"
#define _YACC_
#include "lint.h"

#if !defined(FUNC_SPEC)
#define FUNC_SPEC 	"make list_funcs"
#endif
#define FUNC_TOKENS 	"efun_tokens.y"
#define PRE_LANG        "prelang.y"
#define POST_LANG       "postlang.y"
#define THE_LANG        "lang.y"
#ifndef BUFSIZ
#define BUFSIZ 		1024
#endif
#define NELEMS(arr) 	(sizeof arr / sizeof arr[0])

#define MAX_FUNC  	2048  /* If we need more than this we're in trouble! */
int num_buff;

/* For quick sort purposes : */
char *key[MAX_FUNC], *buf[MAX_FUNC], has_token[MAX_FUNC];

#define EFUN_TABLE "efunctions.h"
#define EFUN_PROTO "efun_protos.h"
#define OPC_TABLE "opc.h"

int min_arg = -1, limit_max = 0;

/*
 * arg_types is the types of all arguments. A 0 is used as a delimiter,
 * marking next argument. An argument can have several types.
 */
int arg_types[200], last_current_type;
/*
 * Store the types of the current efun. They will be copied into the
 * arg_types list if they were not already there (to save memory).
 */
int curr_arg_types[MAX_LOCAL], curr_arg_type_size;

void yyerror PROT((char *));
int yylex();
int yyparse();
int ungetc PROT((int c, FILE *f));
char *type_str PROT((int)), *etype PROT((int)), *etype1 PROT((int)),
   *ctype PROT((int));
#ifndef toupper
int toupper PROT((int));
#endif

void fatal(str)
    char *str;
{
    fprintf(stderr, "%s", str);
    exit(1);
}


# line 66 "make_func.y"
typedef union  {
    int number;
    char *string;
} YYSTYPE;
# define ID 257
# define VOID 258
# define INT 259
# define STRING 260
# define OBJECT 261
# define MAPPING 262
# define MIXED 263
# define UNKNOWN 264
# define DEFAULT 265
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 177 "make_func.y"


struct type {
    char *name;
    int num;
} types[] = {
{ "void", VOID },
{ "int", INT },
{ "string", STRING },
{ "object", OBJECT },
{ "mapping", MAPPING },
{ "mixed", MIXED },
{ "unknown", UNKNOWN }
};

FILE *f;
int current_line = 1;

int main(argc, argv)
    int argc;
    char **argv;
{
    int i, fdr, fdw;
    char buffer[BUFSIZ + 1];
	void make_efun_table();

    if ((f = popen(FUNC_SPEC, "r")) == NULL) { 
	perror(FUNC_SPEC);
	exit(1);
    }
    yyparse();
    /* Now sort the main_list */
    for (i = 0; i < num_buff; i++) {
       int j;
       for (j = 0; j < i; j++)
	   if (strcmp(key[i], key[j]) < 0) {
	      char *tmp;
	      int tmpi;
	      tmp = key[i]; key[i] = key[j]; key[j] = tmp;
	      tmp = buf[i]; buf[i] = buf[j]; buf[j] = tmp;
	      tmpi = has_token[i];
	      has_token[i] = has_token[j]; has_token[j] = tmpi;
           }
    }
	make_efun_table(key,num_buff);
    /* Now display it... */
    printf("{\n");
    for (i = 0; i < num_buff; i++)
       printf("%s", buf[i]);
    printf("\n};\nint efun_arg_types[] = {\n");
    for (i=0; i < last_current_type; i++) {
	if (arg_types[i] == 0)
	    printf("0,\n");
	else
	    printf("%s,", ctype(arg_types[i]));
    }
    printf("};\n");
    pclose(f);
    /*
     * Write all the tokens out.  Do this by copying the
     * pre-include portion of lang.y to lang.y, appending
     * this information, then appending the post-include
     * portion of lang.y.  It's done this way because I don't
     * know how to get YACC to #include %token files.  *grin*
     */
    if ((fdr = open(PRE_LANG, O_RDONLY)) < 0) {
       perror(PRE_LANG);
       exit(1);
    }
    unlink(THE_LANG);
    if ((fdw = open(THE_LANG, O_CREAT | O_WRONLY, 0600)) < 0) {
       perror(THE_LANG);
       exit(1);
    }
    while (i = read(fdr, buffer, BUFSIZ))
       write(fdw, buffer, i);
    close(fdr);
    for (i = 0; i < num_buff; i++) {
       if (has_token[i]) {
          char *str;   /* It's okay to mung key[*] now */
          for (str = key[i]; *str; str++)
   	     if (islower(*str)) *str = toupper(*str);
          sprintf(buffer, "%%token F_%s\n", key[i]);
          write(fdw, buffer, strlen(buffer));
       }
    }
    if ((fdr = open(POST_LANG, O_RDONLY)) < 0) {
       perror(POST_LANG);
       exit(1);
    }
    while (i = read(fdr, buffer, BUFSIZ))
       write(fdw, buffer, i);
    close(fdr), close(fdw);
    return 0;
}

void yyerror(str)
    char *str;
{
    fprintf(stderr, "%s:%d: %s\n", FUNC_SPEC, current_line, str);
    exit(1);
}

int ident(c)
    int c;
{
    char buff[100];
    int len, i;

    for (len=0; isalnum(c) || c == '_'; c = getc(f)) {
	buff[len++] = c;
	if (len + 1 >= sizeof buff)
	    fatal("Local buffer in ident() too small!\n");
	if (len == sizeof buff - 1) {
	    yyerror("Too long indentifier");
	    break;
	}
    }
    (void)ungetc(c, f);
    buff[len] = '\0';
    for (i=0; i < NELEMS(types); i++) {
	if (strcmp(buff, types[i].name) == 0) {
	    yylval.number = types[i].num;
	    return types[i].num;
	}
    }
    if (strcmp(buff, "default") == 0)
	return DEFAULT;
    yylval.string = (char *)malloc(strlen(buff)+1);
    strcpy(yylval.string, buff);
    return ID;
}

char *type_str(n)
    int n;
{
    int i, type = n & 0xffff;

    for (i=0; i < NELEMS(types); i++) {
	if (types[i].num == type) {
	    if (n & 0x10000) {
		static char buff[100];
		if (strlen(types[i].name) + 3 > sizeof buff)
		    fatal("Local buffer too small in type_str()!\n");
		sprintf(buff, "%s *", types[i].name);
		return buff;
	    }
	    return types[i].name;
	}
    }
    return "What?";
}

int yylex1() {
    register int c;
    
    for(;;) {
	switch(c = getc(f)) {
	case ' ':
	case '\t':
	    continue;
	case '#':
	{
#ifdef sun /* no prototype in <stdio.h> *sigh* */
	    extern int fscanf PROT((FILE *, char *, ...));
#endif
	    int line;
	    /* does any operating system support longer pathnames? */
	    char aBuf[2048];

		fgets(aBuf, 2047, f);
	    if (sscanf(aBuf, "%d", &line) == 2)
		current_line = line;
	    current_line++;
	    continue;
	}
	case '\n':
	    current_line++;
	    continue;
	case EOF:
	    return -1;
	default:
	    if (isalpha(c))
		return ident(c);
	    return c;
	}
    }
}

int yylex() {
    return yylex1();
}

char *etype1(n)
    int n;
{
    if (n & 0x10000)
	return "T_POINTER";
    switch(n) {
    case INT:
	return "T_NUMBER";
    case OBJECT:
	return "T_OBJECT";
    case MAPPING:
        return "T_MAPPING";
    case STRING:
	return "T_STRING";
    case MIXED:
	return "0";	/* 0 means any type */
    default:
	yyerror("Illegal type for argument");
    }
    return "What?";
}

char *etype(n)
    int n;
{
    int i;
    int local_size = 100;
    char *buff = (char *)malloc(local_size);

    for (i=0; i < curr_arg_type_size; i++) {
	if (n == 0)
	    break;
	if (curr_arg_types[i] == 0)
	    n--;
    }
    if (i == curr_arg_type_size)
	return "0";
    buff[0] = '\0';
    for(; curr_arg_types[i] != 0; i++) {
	char *p;
	if (curr_arg_types[i] == VOID)
	    continue;
	if (buff[0] != '\0')
	    strcat(buff, "|");
	p = etype1(curr_arg_types[i]);
	/*
	 * The number 2 below is to include the zero-byte and the next
	 * '|' (which may not come).
	 */
	if (strlen(p) + strlen(buff) + 2 > local_size) {
	    fprintf(stderr, "Buffer overflow!\n");
	    exit(1);
	}
	strcat(buff, etype1(curr_arg_types[i]));
    }
    if (!strcmp(buff, "")) 
      strcpy(buff, "T_ANY");
    return buff;
}

char *ctype(n)
    int n;
{
    static char buff[100];	/* 100 is such a comfortable size :-) */
    char *p;

    if (n & 0x10000)
	strcpy(buff, "TYPE_MOD_POINTER|");
    else
	buff[0] = '\0';
    n &= ~0x10000;
    switch(n) {
    case VOID: p = "TYPE_VOID"; break;
    case STRING: p = "TYPE_STRING"; break;
    case INT: p = "TYPE_NUMBER"; break;
    case OBJECT: p = "TYPE_OBJECT"; break;
    case MAPPING: p = "TYPE_MAPPING"; break;
    case MIXED: p = "TYPE_ANY"; break;
    case UNKNOWN: p = "TYPE_UNKNOWN"; break;
    default: yyerror("Bad type!");
    }
    strcat(buff, p);
    if (strlen(buff) + 1 > sizeof buff)
	fatal("Local buffer overwritten in ctype()");
    return buff;
}

void make_efun_table(key,num_buff)
char *key[];
int num_buff;
{
	FILE *fp, *fp2, *fp3;
	char buffer[100];
	int i;

	fp = fopen(EFUN_TABLE,"w");
	if (!fp) {
		fprintf(stderr,"make_func: unable to open %s\n",EFUN_TABLE);
		exit(-1);
	}
	fp2 = fopen(OPC_TABLE, "w");
	if (!fp2) {
		fprintf(stderr,"make_func: unable to open %s\n",OPC_TABLE);
		exit(-2);
	}
	fprintf(fp,"/*\n\tThis file is automatically generated by make_func.\n");
	fprintf(fp,"\tdo not make any manual changes to this file.\n*/\n\n");
	fprintf(fp2,"/*\n\tThis file is automatically generated by make_func.\n");
	fprintf(fp2,"\tdo not make any manual changes to this file.\n*/\n\n");
	fprintf(fp,
		"\ntypedef void (*func_t) PROT((int, int));\n\n");
	fprintf(fp2,"\ntypedef struct opc_s { char *name; int count; } opc_t;\n\n");
	strcpy(buffer,key[0]);
	for (i = 0; buffer[i]; i++)
		buffer[i] = (buffer[i] == '_') ? '_' : toupper(buffer[i]);
	fp3 = fopen("base.h", "w");
	fprintf(fp3,"#define BASE F_%s\n\n",buffer); 
	fclose(fp3);
	fprintf(fp,"#include \"base.h\"\n\n");
	fprintf(fp2,"#include \"base.h\"\n\n");
	fprintf(fp,"func_t efun_table[] = {\n");
	fprintf(fp2,"opc_t opc_efun[] = {\n");
	for (i = 0; i < (num_buff - 1); i++) {
		fprintf(fp,"\tf_%s,\n",key[i]);
		fprintf(fp2,"\"%s\", 0,\n",key[i]);
	}
	fprintf(fp,"\tf_%s};\n",key[num_buff - 1]);
	fprintf(fp2,"\"%s\", 0};\n",key[num_buff - 1]);
	fclose(fp);
	fclose(fp2);
	for (i = 0; i < num_buff; i++) {
		fprintf(fp,"void f_%s PROT((int, int));\n",key[i]);
	}
	fp = fopen(EFUN_PROTO,"w");
	fprintf(fp,"/*\n\tThis file is automatically generated by make_func.\n");
	fprintf(fp,"\tdo not make any manual changes to this file.\n*/\n\n");
	if (!fp) {
		fprintf(stderr,"make_func: unable to open %s\n",EFUN_TABLE);
		exit(-1);
	}
	for (i = 0; i < num_buff; i++) {
		fprintf(fp,"void f_%s PROT((int, int));\n",key[i]);
	}
	fclose(fp);
}
int yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 25
# define YYLAST 230
int yyact[]={

    21,     5,     6,     7,    10,    11,     8,     9,    24,    34,
    15,    12,    26,    32,    33,    30,    27,    13,    28,    16,
    20,    18,    22,     2,     3,     1,    23,    14,    19,     4,
    17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,    29,    31,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     5,     6,     7,    10,    11,     8,     9,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    25 };
int yypact[]={

 -1000,  -257, -1000,  -246,   -25, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000,  -247, -1000,   -21, -1000,   -46,   -36, -1000,  -112,
 -1000,   -30, -1000,   -23,   -46,   -43,  -257,   -33,   -45, -1000,
  -248, -1000, -1000, -1000, -1000 };
int yypgo[]={

     0,    22,    30,    29,    28,    20,    21,    27,    26,    25,
    23 };
int yyr1[]={

     0,     9,     9,     7,     7,     8,     8,    10,     1,     1,
     3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
     6,     5,     4,     4,     4 };
int yyr2[]={

     0,     0,     4,     2,     1,     7,     1,    17,     2,     5,
     2,     2,     2,     2,     2,     2,     2,     1,     3,     7,
     3,     3,     3,     7,     7 };
int yychk[]={

 -1000,    -9,   -10,    -1,    -3,   258,   259,   260,   263,   264,
   261,   262,   257,    42,    -7,   257,    40,    -2,    -6,    -4,
    -5,    46,    -1,    -8,    44,   265,   124,    46,    41,    -6,
    58,    -5,    46,    59,   257 };
int yydef[]={

     1,    -2,     2,     0,     8,    10,    11,    12,    13,    14,
    15,    16,     4,     9,     0,     3,    17,     6,    18,    20,
    22,     0,    21,     0,     0,     0,     0,     0,     0,    19,
     0,    23,    24,     7,     5 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"ID",	257,
	"VOID",	258,
	"INT",	259,
	"STRING",	260,
	"OBJECT",	261,
	"MAPPING",	262,
	"MIXED",	263,
	"UNKNOWN",	264,
	"DEFAULT",	265,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"funcs : /* empty */",
	"funcs : funcs func",
	"optional_ID : ID",
	"optional_ID : /* empty */",
	"optional_default : DEFAULT ':' ID",
	"optional_default : /* empty */",
	"func : type ID optional_ID '(' arg_list optional_default ')' ';'",
	"type : basic",
	"type : basic '*'",
	"basic : VOID",
	"basic : INT",
	"basic : STRING",
	"basic : MIXED",
	"basic : UNKNOWN",
	"basic : OBJECT",
	"basic : MAPPING",
	"arg_list : /* empty */",
	"arg_list : typel2",
	"arg_list : arg_list ',' typel2",
	"typel2 : typel",
	"arg_type : type",
	"typel : arg_type",
	"typel : typel '|' arg_type",
	"typel : '.' '.' '.'",
};
#endif /* YYDEBUG */
#line 1 "/usr/lib/yaccpar"
/*	@(#)yaccpar 1.10 89/04/04 SMI; from S5R3 1.10	*/

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	{ free(yys); free(yyv); return(0); }
#define YYABORT		{ free(yys); free(yyv); return(1); }
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-1000)

/*
** static variables used by the parser
*/
static YYSTYPE *yyv;			/* value stack */
static int *yys;			/* state stack */

static YYSTYPE *yypv;			/* top of value stack */
static int *yyps;			/* top of state stack */

static int yystate;			/* current state */
static int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */

int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */


/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
int
yyparse()
{
	register YYSTYPE *yypvt;	/* top of value stack for $vars */
	unsigned yymaxdepth = YYMAXDEPTH;

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yyv = (YYSTYPE*)malloc(yymaxdepth*sizeof(YYSTYPE));
	yys = (int*)malloc(yymaxdepth*sizeof(int));
	if (!yyv || !yys)
	{
		yyerror( "out of memory" );
		return(1);
	}
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

	goto yystack;
	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			(void)printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				(void)printf( "end-of-file\n" );
			else if ( yychar < 0 )
				(void)printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				(void)printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int yyps_index = (yy_ps - yys);
			int yypv_index = (yy_pv - yyv);
			int yypvt_index = (yypvt - yyv);
			yymaxdepth += YYMAXDEPTH;
			yyv = (YYSTYPE*)realloc((char*)yyv,
				yymaxdepth * sizeof(YYSTYPE));
			yys = (int*)realloc((char*)yys,
				yymaxdepth * sizeof(int));
			if (!yyv || !yys)
			{
				yyerror( "yacc stack overflow" );
				return(1);
			}
			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			(void)printf( "Received token " );
			if ( yychar == 0 )
				(void)printf( "end-of-file\n" );
			else if ( yychar < 0 )
				(void)printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				(void)printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				(void)printf( "Received token " );
				if ( yychar == 0 )
					(void)printf( "end-of-file\n" );
				else if ( yychar < 0 )
					(void)printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					(void)printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
				yynerrs++;
			skip_init:
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						(void)printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					(void)printf( "Error recovery discards " );
					if ( yychar == 0 )
						(void)printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						(void)printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						(void)printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			(void)printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 4:
# line 86 "make_func.y"
{ yyval.string = ""; } break;
case 5:
# line 88 "make_func.y"
{ yyval.string = yypvt[-0].string; } break;
case 6:
# line 88 "make_func.y"
{ yyval.string="0"; } break;
case 7:
# line 91 "make_func.y"
{
	char buff[500];
	char f_name[500];
	int i;
	if (min_arg == -1)
	    min_arg = yypvt[-3].number;
	if (yypvt[-5].string[0] == '\0') {
	    int len;
	    if (strlen(yypvt[-6].string) + 1 + 2 > sizeof f_name)
		fatal("A local buffer was too small!(1)\n");
	    sprintf(f_name, "F_%s", yypvt[-6].string);
	    len = strlen(f_name);
	    for (i=0; i < len; i++) {
		if (islower(f_name[i]))
		    f_name[i] = toupper(f_name[i]);
	    }
	    has_token[num_buff]=1;
	} else {
	    if (strlen(yypvt[-5].string) + 1 > sizeof f_name)
		fatal("A local buffer was too small(2)!\n");
	    strcpy(f_name, yypvt[-5].string);
	    has_token[num_buff]=0;
	}
	for(i=0; i < last_current_type; i++) {
	    int j;
	    for (j = 0; j+i<last_current_type && j < curr_arg_type_size; j++)
	    {
		if (curr_arg_types[j] != arg_types[i+j])
		    break;
	    }
	    if (j == curr_arg_type_size)
		break;
	}
	if (i == last_current_type) {
	    int j;
	    for (j=0; j < curr_arg_type_size; j++) {
		arg_types[last_current_type++] = curr_arg_types[j];
		if (last_current_type == NELEMS(arg_types))
		    yyerror("Array 'arg_types' is too small");
	    }
	}
	sprintf(buff, "{\"%s\",%s,%d,%d,%s,%s,%s,%d,%s},\n",
		yypvt[-6].string, f_name, min_arg, limit_max ? -1 : yypvt[-3].number, ctype(yypvt[-7].number),
		etype(0), etype(1), i, yypvt[-2].string);
        if (strlen(buff) > sizeof buff)
     	    fatal("Local buffer overwritten !\n");
        key[num_buff] = (char *) malloc(strlen(yypvt[-6].string) + 1);
	strcpy(key[num_buff], yypvt[-6].string);
	buf[num_buff] = (char *) malloc(strlen(buff) + 1);
	strcpy(buf[num_buff], buff);
    num_buff++;
	min_arg = -1;
	limit_max = 0;
	curr_arg_type_size = 0;
    } break;
case 9:
# line 147 "make_func.y"
{ yyval.number = yypvt[-1].number | 0x10000; } break;
case 17:
# line 151 "make_func.y"
{ yyval.number = 0; } break;
case 18:
# line 152 "make_func.y"
{ yyval.number = 1; if (yypvt[-0].number) min_arg = 0; } break;
case 19:
# line 153 "make_func.y"
{ yyval.number = yypvt[-2].number + 1; if (yypvt[-0].number) min_arg = yyval.number - 1; } break;
case 20:
# line 156 "make_func.y"
{
	yyval.number = yypvt[-0].number;
	curr_arg_types[curr_arg_type_size++] = 0;
	if (curr_arg_type_size == NELEMS(curr_arg_types))
	    yyerror("Too many arguments");
    } break;
case 21:
# line 164 "make_func.y"
{
	if (yypvt[-0].number != VOID) {
	    curr_arg_types[curr_arg_type_size++] = yypvt[-0].number;
	    if (curr_arg_type_size == NELEMS(curr_arg_types))
		yyerror("Too many arguments");
	}
	yyval.number = yypvt[-0].number;
    } break;
case 22:
# line 173 "make_func.y"
{ yyval.number = (yypvt[-0].number == VOID && min_arg == -1); } break;
case 23:
# line 174 "make_func.y"
{ yyval.number = (min_arg == -1 && (yypvt[-2].number || yypvt[-0].number == VOID));} break;
case 24:
# line 175 "make_func.y"
{ yyval.number = min_arg == -1 ; limit_max = 1; } break;
	}
	goto yystack;		/* reset registers in driver code */
}
