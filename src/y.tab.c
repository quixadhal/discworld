extern char *malloc(), *realloc();

# line 2 "lang.y"
# line 3 "prelang.y"
/* NOTE: this is the 3.1.1 version with 3.0.53-A3.1 fixes patched in */
/* The above line is to give proper line number references. Please mail me
 * if your compiler complains about it.
 */
/*
 * This is the grammar definition of LPC. The token table is built
 * automatically by make_func. The lang.y is constructed from this file,
 * the generated token list and post_lang.y. The reason for this is that there
 * is no #include statement that yacc recognizes.
 */
#include <string.h>
#include <stdio.h>
#include <memory.h>
#if defined(sun)
#include <alloca.h>
#endif
#ifdef NeXT
#include <stdlib.h>
#endif

#include "mudlib_stats.h"
#include "interpret.h"
#include "config.h"
#define _YACC_
#include "lint.h"
#include "object.h"
#include "exec.h"
#include "instrs.h"
#include "incralloc.h"
#include "switch.h"
#include "base.h"

#define YYMAXDEPTH	600

/* NUMPAREAS areas are saved with the program code after compilation.
 */
#define A_PROGRAM		0
#define A_FUNCTIONS		1
#define A_STRINGS		2
#define A_VARIABLES		3
#define A_LINENUMBERS		4
#define A_INHERITS		5
#define A_ARGUMENT_TYPES	6
#define A_ARGUMENT_INDEX	7
#define NUMPAREAS		8
#define A_CASE_NUMBERS		8
#define A_CASE_STRINGS		9
#define A_CASE_LABELS	       10
#define NUMAREAS	       11

#define BREAK_ON_STACK		0x40000
#define BREAK_FROM_CASE		0x80000

#define SWITCH_STACK_SIZE  100

/* make sure that this struct has a size that is a power of two */
struct case_heap_entry { int key; short addr; short line; };
#define CASE_HEAP_ENTRY_ALIGN(offset) offset &= -sizeof(struct case_heap_entry)

static struct mem_block mem_block[NUMAREAS];

/*
   these three variables used to properly adjust the 'break_sp' stack in
   the event a 'continue' statement is issued from inside a 'switch'.
*/
static short switches = 0;
static int switch_sptr = 0;
static short switch_stack[SWITCH_STACK_SIZE];

/*
 * Some good macros to have.
 */

#define BASIC_TYPE(e,t) ((e) == TYPE_ANY ||\
			 (e) == (t) ||\
			 (t) == TYPE_ANY)

#define TYPE(e,t) (BASIC_TYPE((e) & TYPE_MOD_MASK, (t) & TYPE_MOD_MASK) ||\
		   (((e) & TYPE_MOD_POINTER) && ((t) & TYPE_MOD_POINTER) &&\
		    BASIC_TYPE((e) & (TYPE_MOD_MASK & ~TYPE_MOD_POINTER),\
			       (t) & (TYPE_MOD_MASK & ~TYPE_MOD_POINTER))))

#define FUNCTION(n) ((struct function *)mem_block[A_FUNCTIONS].block + (n))
#define VARIABLE(n) ((struct variable *)mem_block[A_VARIABLES].block + (n))

#define align(x) (((x) + 3) & ~3)

/*
 * If the type of the function is given, then strict types are
 * checked and required.
 */
static int exact_types;
extern int pragma_strict_types;	/* Maintained by lex.c */
extern int pragma_save_types;	/* Also maintained by lex.c */
int approved_object;		/* How I hate all these global variables */

extern int total_num_prog_blocks, total_prog_block_size;

extern int num_parse_error;
extern int d_flag;

extern int function_index_found;
extern struct program *function_prog_found;
extern unsigned short function_type_mod_found;
extern int function_inherit_found;

static int current_break_address;
static int current_continue_address;
static int current_case_number_heap;
static int current_case_string_heap;
#define SOME_NUMERIC_CASE_LABELS 0x40000
#define NO_STRING_CASE_LABELS    0x80000
static int zero_case_label;
static int current_type;

static int last_push_indexed;
static int last_push_local;
static int last_push_identifier;

/*
 * There is always function starting at address 0, which will execute
 * the initialization code. This code is spread all over the program,
 * with jumps to next initializer. The next variable keeps track of
 * the previous jump. After the last initializer, the jump will be changed
 * into a return(0) statement instead.
 *
 * A function named '__INIT' will be defined, which will contain the
 * initialization code. If there was no initialization code, then the
 * function will not be defined. That is the usage of the
 * first_last_initializer_end variable.
 *
 * When inheriting from another object, a call will automatically be made
 * to call __INIT in that code from the current __INIT.
 */
static int last_initializer_end;
static int first_last_initializer_end;

static struct program NULL_program; /* marion - clean neat empty struct */

void epilog();
static int check_declared PROT((char *str));
static void prolog();
static char *get_two_types PROT((int type1, int type2));
void free_all_local_names(),
    add_local_name PROT((char *, int)), smart_log PROT((char *, int, char *,int));
extern int yylex();
static int verify_declared PROT((char *));
static void copy_variables();
static void copy_inherits PROT((struct program *, int, int));
void type_error PROT((char *, int));

char *string_copy();

extern int current_line;
/*
 * 'inherit_file' is used as a flag. If it is set to a string
 * after yyparse(), this string should be loaded as an object,
 * and the original object must be loaded again.
 */
extern char *current_file, *inherit_file;

/*
 * The names and types of arguments and auto variables.
 */
char *local_names[MAX_LOCAL];
unsigned short type_of_locals[MAX_LOCAL];
int current_number_of_locals = 0;
int current_break_stack_need = 0  ,max_break_stack_need = 0;

/*
 * The types of arguments when calling functions must be saved,
 * to be used afterwards for checking. And because function calls
 * can be done as an argument to a function calls,
 * a stack of argument types is needed. This stack does not need to
 * be freed between compilations, but will be reused.
 */
static struct mem_block type_of_arguments;

struct program *prog;	/* Is returned to the caller of yyparse */

/*
 * Compare two types, and return true if they are compatible.
 */

static int compatible_types(t1, t2)
    int t1, t2;
{
    if (t1 == TYPE_UNKNOWN || t2 == TYPE_UNKNOWN)
#ifdef CAST_CALL_OTHERS      
	return 0;
#else  
	return 1;
#endif
    if (t1 == t2)
	return 1;
    if (t1 == TYPE_ANY || t2 == TYPE_ANY)
	return 1;
    if ((t1 & TYPE_MOD_POINTER) && (t2 & TYPE_MOD_POINTER)) {
	if ((t1 & TYPE_MOD_MASK) == (TYPE_ANY|TYPE_MOD_POINTER) ||
	    (t2 & TYPE_MOD_MASK) == (TYPE_ANY|TYPE_MOD_POINTER))
	    return 1;
    }
    return 0;
}

/*
 * Add another argument type to the argument type stack
 */
static void add_arg_type(type)
    unsigned short type;
{
    struct mem_block *mbp = &type_of_arguments;
    while (mbp->current_size + sizeof type > mbp->max_size) {
	mbp->max_size <<= 1;
	mbp->block = (char *)REALLOC((char *)mbp->block, mbp->max_size);
    }
    memcpy(mbp->block + mbp->current_size, &type, sizeof type);
    mbp->current_size += sizeof type;
}

/*
 * Pop the argument type stack 'n' elements.
 */
INLINE
static void pop_arg_stack(n)
    int n;
{
    type_of_arguments.current_size -= sizeof (unsigned short) * n;
}

/*
 * Get type of argument number 'arg', where there are
 * 'n' arguments in total in this function call. Argument
 * 0 is the first argument.
 */
INLINE
int get_argument_type(arg, n)
    int arg, n;
{
    return
	((unsigned short *)
	 (type_of_arguments.block + type_of_arguments.current_size))[arg - n];
}

INLINE
static void add_to_mem_block(n, data, size)
    int n, size;
    char *data;
{
    struct mem_block *mbp = &mem_block[n];
    while (mbp->current_size + size > mbp->max_size) {
	mbp->max_size <<= 1;
	mbp->block = (char *)REALLOC((char *)mbp->block, mbp->max_size);
    }
    memcpy(mbp->block + mbp->current_size, data, size);
    mbp->current_size += size;
}

static void ins_byte(b)
    char b;
{
    add_to_mem_block(A_PROGRAM, &b, 1);
}

/*
 * Store a 2 byte number. It is stored in such a way as to be sure
 * that correct byte order is used, regardless of machine architecture.
 * Also beware that some machines can't write a word to odd addresses.
 */
static void ins_short(l)
    short l;
{
    add_to_mem_block(A_PROGRAM, (char *)&l + 0, 1);
    add_to_mem_block(A_PROGRAM, (char *)&l + 1, 1);
}

static void upd_short(offset, l)
    int offset;
    short l;
{
    mem_block[A_PROGRAM].block[offset + 0] = ((char *)&l)[0];
    mem_block[A_PROGRAM].block[offset + 1] = ((char *)&l)[1];
}

static short read_short(offset)
    int offset;
{
    short l;

    ((char *)&l)[0] = mem_block[A_PROGRAM].block[offset + 0];
    ((char *)&l)[1] = mem_block[A_PROGRAM].block[offset + 1];
    return l;
}

/*
 * Store a 4 byte number. It is stored in such a way as to be sure
 * that correct byte order is used, regardless of machine architecture.
 */
static void ins_long(l)
    int l;
{
    add_to_mem_block(A_PROGRAM, (char *)&l+0, 1);
    add_to_mem_block(A_PROGRAM, (char *)&l+1, 1);
    add_to_mem_block(A_PROGRAM, (char *)&l+2, 1);
    add_to_mem_block(A_PROGRAM, (char *)&l+3, 1);
}

static void ins_f_byte(b)
    unsigned int b;
{
    ins_byte((char)(b - F_OFFSET));
}

/*
 * Return 1 on success, 0 on failure.
 */
static int defined_function(s)
    char *s;
{
    int offset;
    int inh;
    struct function *funp;
    extern char *findstring PROT((char *));

    s = findstring (s);
    if (!s) return 0;
    for (offset = 0; offset < mem_block[A_FUNCTIONS].current_size;
	 offset += sizeof (struct function)) {
	funp = (struct function *)&mem_block[A_FUNCTIONS].block[offset];
        /* Only index, prog, and type will be defined. */
        if (s == funp->name) {
          function_index_found = offset / sizeof (struct function);
          function_prog_found = 0;
          function_type_mod_found = funp->type & ~TYPE_MOD_MASK;
          return 1;
          }
        }
    /* Look for the function in the inherited programs */
    for (inh = mem_block[A_INHERITS].current_size / sizeof (struct inherit) -1;
         inh >= 0; inh--) {
      if (search_for_function (s,
            ((struct inherit *)mem_block[A_INHERITS].block)[inh].prog)) {
        /* Adjust for inherit-type */
        function_type_mod_found |=
           ((struct inherit *)mem_block[A_INHERITS].block)[inh].type
           & ~TYPE_MOD_MASK;
        if (function_type_mod_found & TYPE_MOD_PUBLIC)
          function_type_mod_found &= ~TYPE_MOD_PRIVATE;
        return 1;
        }
      }
    return 0;
}

/*
 * A mechanism to remember addresses on a stack. The size of the stack is
 * defined in config.h.
 */
static int comp_stackp;
static int comp_stack[COMPILER_STACK_SIZE];

static void push_address() {
    if (comp_stackp >= COMPILER_STACK_SIZE) {
	yyerror("Compiler stack overflow");
	comp_stackp++;
	return;
    }
    comp_stack[comp_stackp++] = mem_block[A_PROGRAM].current_size;
}

static void push_explicit(address)
    int address;
{
    if (comp_stackp >= COMPILER_STACK_SIZE) {
	yyerror("Compiler stack overflow");
	comp_stackp++;
	return;
    }
    comp_stack[comp_stackp++] = address;
}

static int pop_address() {
    if (comp_stackp == 0)
	fatal("Compiler stack underflow.\n");
    if (comp_stackp > COMPILER_STACK_SIZE) {
	--comp_stackp;
	return 0;
    }
    return comp_stack[--comp_stackp];
}

/*
 * Patch a function definition of an inherited function, to what it really
 * should be.
 * The name of the function can be one of:
 *    object::name
 *    ::name
 * Where 'object' is the name of the superclass.
 */
static int find_inherited_function(real_name)
    char *real_name;
{
    int i;
    struct inherit *ip;
    int super_length;
    char *super_name = 0, *p;
    extern char *findstring PROT((char *));

    if (real_name[0] == ':')
	real_name = real_name + 2;	/* There will be exactly two ':' */
    else if (p = strchr(real_name, ':')) {
        super_name = real_name;
        real_name = p+2;
	super_length = real_name - super_name - 2;
    }
    real_name = findstring (real_name);
    /* If it's not in the string table it can't be inherited */
    if (!real_name) return 0;
    ip = (struct inherit *)
         (mem_block[A_INHERITS].block + mem_block[A_INHERITS].current_size)
         - 1;
    for (; ip >= (struct inherit *)mem_block[A_INHERITS].block; 
           /* Skip indirect inherits, they have been searched already */
           ip -= (ip->prog->p.i.num_inherited + 1)) {
        /* It is not possible to refer to indirectly inherited programs
         * by name. Should it be? */
	if (super_name) {
	    int l = strlen(ip->prog->name);	/* Including .c */
	    if (l - 2 < super_length)
		continue;
	    if (strncmp(super_name, ip->prog->name + l - 2 - super_length,
			super_length) != 0)
		continue;
	}
        if (search_for_function (real_name, ip->prog) &&
	    !(function_type_mod_found & TYPE_MOD_PRIVATE)) {
	  /* Function was found in an inherited program.
           * Treat the inherit_found as an offset in our inherit list */
	  function_inherit_found += 
              (ip - (struct inherit *)mem_block[A_INHERITS].block)
              - ip->prog->p.i.num_inherited;
          return 1;
          }
	}
    return 0;
} /* find_inherited_function() */

/*
 * Define a new function. Note that this function is called at least twice
 * for alll function definitions. First as a prototype, then as the real
 * function. Thus, there are tests to avoid generating error messages more
 * than once by looking at (flags & NAME_PROTOTYPE).
 */
static void define_new_function(name, num_arg, num_local, offset, flags, type)
  char *name;
  int num_arg, num_local;
  int offset, flags, type;
{
  struct function fun;
  struct function *funp;
  unsigned short argument_start_index;

  if (defined_function (name)) {
    /* The function is already defined.
     *   If it was defined by inheritance, make a new definition,
     *   unless nomask applies.
     *   If it was defined in the current program, use that definition.
     */
    /* Point to the function definition found */
    if (function_prog_found)
      funp = &function_prog_found->p.i.functions[function_index_found];
    else
      funp = FUNCTION(function_index_found);

    /* If it was declared in the current program, and not a prototype,
     * it is a double definition. */
    if (!funp->flags & NAME_PROTOTYPE &&
        !function_prog_found) {
      char buff[500];
      sprintf (buff, "Redeclaration of function %s", name);
      yyerror (buff);
      return;
      }
    /* If neither the new nor the old definition is a prototype,
     * it must be a redefinition of an inherited function.
     * Check for nomask. */
    if ((funp->type & TYPE_MOD_NO_MASK) &&
        !(funp->flags & NAME_PROTOTYPE) &&
        !(flags & NAME_PROTOTYPE)) {
      char buff[500];
      sprintf (buff, "Illegal to redefine nomask function %s", name);
      yyerror (buff);
      return;
      }
    /* Check types */
    if (exact_types && funp->type != TYPE_UNKNOWN) {
      if (funp->num_arg != num_arg && !(funp->type & TYPE_MOD_VARARGS)) {
        yyerror("Incorrect number of arguments");
        return;
        }
      else if (!(funp->flags & NAME_STRICT_TYPES)) {
        yyerror("Function called not compiled with type testing");
        return;
        }
#if 0
      else {
        int i;
        /* Now check argument types */
        for (i=0; i < num_arg; i++) {
          }
        }
#endif
      }
    /* If it is a prototype for a function that has already been defined,
     * we don't need it. */
    if (flags & NAME_PROTOTYPE)
      return;
    /* If the function was defined in an inherited program, we need to
     * make a new definition here. */
    if (function_prog_found)
      funp = &fun;
    }
  else /* Function was not defined before, we need a new definition */
    funp = &fun;
  funp->name = make_shared_string (name);
  funp->offset = offset;
  funp->flags = flags;
  funp->num_arg = num_arg;
  funp->num_local = num_local;
  funp->type = type;
  if (exact_types)
    funp->flags |= NAME_STRICT_TYPES;
  if (funp == &fun) {
    add_to_mem_block (A_FUNCTIONS, (char *)&fun, sizeof fun);
    if (!exact_types || num_arg == 0)
      argument_start_index = INDEX_START_NONE;
    else {
      int i;
      /*
       * Save the start of argument types.
       */
      argument_start_index =
          mem_block[A_ARGUMENT_TYPES].current_size /
              sizeof (unsigned short);
      for (i=0; i < num_arg; i++)
        add_to_mem_block(A_ARGUMENT_TYPES, &type_of_locals[i],
                         sizeof type_of_locals[i]);
      }
    add_to_mem_block(A_ARGUMENT_INDEX, &argument_start_index,
                   sizeof argument_start_index);
    }
  return;
} /* define_new_function() */


/* 'prog' is a freshly inherited program.
 * The current program may not have defined functions that are 'nomask' in 
 * the inherited program. Neither may the inherited program define functions
 * that are 'nomask' in already inherited programs.
 *
 * I assume the program has not yet defined many functions,
 * because inherit statements are usually near the top. 
 *                                                 /Dark
 */
static void check_nomask_functions (prog)
  struct program *prog;
{
  int fix;
  struct inherit *ip;
  int num_functions = mem_block[A_FUNCTIONS].current_size /
	              sizeof (struct function);
  for (fix = 0; fix < num_functions;  fix++) {
    if (FUNCTION(fix)->flags & NAME_PROTOTYPE) continue;
    if (search_for_function (FUNCTION(fix)->name, prog) &&
        (function_prog_found->p.i.functions[function_index_found].type &
         TYPE_MOD_NO_MASK) ) {
      char buff[200];
      sprintf (buff, "Redefinition of nomask function %.40s", 
               FUNCTION(fix)->name);
      yyerror (buff);
      }
    }
  for (ip = (struct inherit *)mem_block[A_INHERITS].block +
             mem_block[A_INHERITS].current_size / sizeof (struct inherit) - 1;
       ip >= (struct inherit *)mem_block[A_INHERITS].block; ip--) {
    for (fix = 0; fix < ip->prog->p.i.num_functions; fix++) {
      if (ip->prog->p.i.functions[fix].type & TYPE_MOD_NO_MASK &&
	  search_for_function (ip->prog->p.i.functions[fix].name, prog)) {
        char buff[200];
        sprintf (buff, "Redefinition of inherited nomask function %.40s",
		 ip->prog->p.i.functions[fix].name);
        yyerror (buff);
        }
      }
    }
} /* check_nomask_functions() */

static int is_simul_efun (name)
  char *name;
{
  struct object *s_ob;
  extern char *simul_efun_file_name;
  extern char *findstring PROT((char *));
  if (simul_efun_file_name) {
    s_ob = find_object2 (simul_efun_file_name);
    if (s_ob != 0 && search_for_function (findstring (name), s_ob->prog)) 
      return 1;
    }
  return 0;
}   /* is_simul_efun() */

static void define_variable(name, type, flags)
    char *name;
    int type;
    int flags;
{
    struct variable dummy;
    int n;

    n = check_declared(name);
    if (n != -1 && (VARIABLE(n)->type & TYPE_MOD_NO_MASK)) {
	char p[2048];

	sprintf(p, "Illegal to redefine 'nomask' variable \"%s\"", name);
	yyerror(p);
    }
    dummy.name = make_shared_string(name);
    dummy.type = type;
    dummy.flags = flags;
    add_to_mem_block(A_VARIABLES, (char *)&dummy, sizeof dummy);
}

short store_prog_string(str)
    char *str;
{
    short i;
    char **p;

    p = (char **) mem_block[A_STRINGS].block;
    str = make_shared_string(str);
    for (i=mem_block[A_STRINGS].current_size / sizeof str -1; i>=0; --i)
	if (p[i] == str)  {
	    free_string(str); /* Needed as string is only free'ed once. */
	    return i;
	}

    add_to_mem_block(A_STRINGS, &str, sizeof str);
    return mem_block[A_STRINGS].current_size / sizeof str - 1;
}

void add_to_case_heap(block_index,entry)
    int block_index;
    struct case_heap_entry *entry;
{
    char *heap_start;
    int offset,parent;
    int current_heap;

    if ( block_index == A_CASE_NUMBERS )
        current_heap = current_case_number_heap;
    else
        current_heap = current_case_string_heap;
    offset = mem_block[block_index].current_size - current_heap;
    add_to_mem_block(block_index, (char*)entry, sizeof(*entry) );
    heap_start = mem_block[block_index].block + current_heap;
    for ( ; offset; offset = parent ) {
        parent = ( offset - sizeof(struct case_heap_entry) ) >> 1 ;
        CASE_HEAP_ENTRY_ALIGN(parent);
        if ( ((struct case_heap_entry*)(heap_start+offset))->key <
             ((struct case_heap_entry*)(heap_start+parent))->key )
        {
            *(struct case_heap_entry*)(heap_start+offset) =
            *(struct case_heap_entry*)(heap_start+parent);
            *(struct case_heap_entry*)(heap_start+parent) = *entry;
        }
    }
}

/*
 * Arrange a jump to the current position for the initialization code
 * to continue.
 */
static void transfer_init_control() {
    if (mem_block[A_PROGRAM].current_size - 2 == last_initializer_end)
	mem_block[A_PROGRAM].current_size -= 3;
    else {
	/*
	 * Change the address of the last jump after the last
	 * initializer to this point.
	 */
	upd_short(last_initializer_end,
		  mem_block[A_PROGRAM].current_size);
    }
}

void add_new_init_jump();

# line 906 "lang.y"
typedef union 
{
	int number;
	unsigned int address;	/* Address of an instruction */
	char *string;
	short type;
	struct { int key; char block; } case_label;
	struct function *funp;
} YYSTYPE;
# define F_JUMP 257
# define F_JUMP_WHEN_ZERO 258
# define F_JUMP_WHEN_NON_ZERO 259
# define F_POP_VALUE 260
# define F_DUP 261
# define F_STORE 262
# define F_CALL_DOWN 263
# define F_CALL_SELF 264
# define F_PUSH_IDENTIFIER_LVALUE 265
# define F_PUSH_LOCAL_VARIABLE_LVALUE 266
# define F_PUSH_INDEXED_LVALUE 267
# define F_INDIRECT 268
# define F_INDEX 269
# define F_CONST0 270
# define F_CONST1 271
# define F_CLASS_MEMBER_LVALUE 272
# define F_CALL_EFUN 273
# define F_CASE 274
# define F_DEFAULT 275
# define F_RANGE 276
# define F_IF 277
# define F_IDENTIFIER 278
# define F_LAND 279
# define F_LOR 280
# define F_STATUS 281
# define F_RETURN 282
# define F_STRING 283
# define F_INC 284
# define F_DEC 285
# define F_POST_INC 286
# define F_POST_DEC 287
# define F_COMMA 288
# define F_NUMBER 289
# define F_ASSIGN 290
# define F_INT 291
# define F_ADD 292
# define F_SUBTRACT 293
# define F_MULTIPLY 294
# define F_DIVIDE 295
# define F_LT 296
# define F_GT 297
# define F_EQ 298
# define F_GE 299
# define F_LE 300
# define F_NE 301
# define F_ADD_EQ 302
# define F_SUB_EQ 303
# define F_DIV_EQ 304
# define F_MULT_EQ 305
# define F_NEGATE 306
# define F_SUBSCRIPT 307
# define F_WHILE 308
# define F_BREAK 309
# define F_POP_BREAK 310
# define F_DO 311
# define F_FOR 312
# define F_SWITCH 313
# define F_SSCANF 314
# define F_PARSE_COMMAND 315
# define F_STRING_DECL 316
# define F_LOCAL_NAME 317
# define F_ELSE 318
# define F_CONTINUE 319
# define F_MOD 320
# define F_MOD_EQ 321
# define F_INHERIT 322
# define F_COLON_COLON 323
# define F_STATIC 324
# define F_ARROW 325
# define F_AGGREGATE 326
# define F_AGGREGATE_ASSOC 327
# define F_COMPL 328
# define F_AND 329
# define F_AND_EQ 330
# define F_OR 331
# define F_OR_EQ 332
# define F_XOR 333
# define F_XOR_EQ 334
# define F_LSH 335
# define F_LSH_EQ 336
# define F_RSH 337
# define F_RSH_EQ 338
# define F_CATCH 339
# define F_END_CATCH 340
# define F_MAPPING 341
# define F_OBJECT 342
# define F_VOID 343
# define F_MIXED 344
# define F_PRIVATE 345
# define F_NO_MASK 346
# define F_NOT 347
# define F_PROTECTED 348
# define F_PUBLIC 349
# define F_VARARGS 350
# define F_ACTIONS_DEFINED 351
# define F_ADD_ACTION 352
# define F_ADD_WORTH 353
# define F_ALL_INVENTORY 354
# define F_ALLOCATE 355
# define F_ALLOCATE_MAPPING 356
# define F_AUTHOR_STATS 357
# define F_BREAK_POINT 358
# define F_CACHE_STATS 359
# define F_CALL_OTHER 360
# define F_CALL_OUT 361
# define F_CALL_OUT_INFO 362
# define F_CAPITALIZE 363
# define F_CAT 364
# define F_CHILDREN 365
# define F_CLEAR_BIT 366
# define F_CLONE_OBJECT 367
# define F_CLONEP 368
# define F_COMMAND 369
# define F_CP 370
# define F_CRYPT 371
# define F_CTIME 372
# define F_DEBUG_INFO 373
# define F_DEEP_INHERIT_LIST 374
# define F_DEEP_INVENTORY 375
# define F_DESTRUCT 376
# define F_DISABLE_COMMANDS 377
# define F_DOMAIN_STATS 378
# define F_DUMP_FILE_DESCRIPTORS 379
# define F_DUMP_SOCKET_STATUS 380
# define F_DUMPALLOBJ 381
# define F_EACH 382
# define F_ED 383
# define F_ENABLE_COMMANDS 384
# define F_ENABLE_WIZARD 385
# define F_ENVIRONMENT 386
# define F_EVENT 387
# define F_EXEC 388
# define F_EXPLODE 389
# define F_EXPORT_UID 390
# define F_EXTRACT 391
# define F_FILE_LENGTH 392
# define F_FILE_NAME 393
# define F_FILE_SIZE 394
# define F_FILTER_ARRAY 395
# define F_FIND_CALL_OUT 396
# define F_FIND_LIVING 397
# define F_FIND_OBJECT 398
# define F_FIND_PLAYER 399
# define F_FIRST_INVENTORY 400
# define F_FUNCTION_EXISTS 401
# define F_GET_CHAR 402
# define F_GET_DIR 403
# define F_GETEUID 404
# define F_GETUID 405
# define F_IMPLODE 406
# define F_IN_EDIT 407
# define F_IN_INPUT 408
# define F_INHERIT_LIST 409
# define F_INPUT_TO 410
# define F_INTERACTIVE 411
# define F_INTP 412
# define F_KEYS 413
# define F_LINK 414
# define F_LIVING 415
# define F_LOCALTIME 416
# define F_LOG_FILE 417
# define F_LOWER_CASE 418
# define F_MALLOC_STATUS 419
# define F_MAP_ARRAY 420
# define F_MAP_DELETE 421
# define F_MAPP 422
# define F_MASTER 423
# define F_MEMBER_ARRAY 424
# define F_MESSAGE 425
# define F_MKDIR 426
# define F_MOVE_OBJECT 427
# define F_MUD_NAME 428
# define F_MUD_STATUS 429
# define F_NEW 430
# define F_NEXT_INVENTORY 431
# define F_NEXT_LIVING 432
# define F_NOTIFY_FAIL 433
# define F_NULLP 434
# define F_OBJECTP 435
# define F_OPCPROF 436
# define F_PLURALIZE 437
# define F_POINTERP 438
# define F_PRESENT 439
# define F_PREVIOUS_OBJECT 440
# define F_PRINTF 441
# define F_PROCESS_STRING 442
# define F_QUERY_HOST_NAME 443
# define F_QUERY_IDLE 444
# define F_QUERY_IP_NAME 445
# define F_QUERY_IP_NUMBER 446
# define F_QUERY_LOAD_AVERAGE 447
# define F_QUERY_SNOOP 448
# define F_QUERY_VERB 449
# define F_RANDOM 450
# define F_READ_BYTES 451
# define F_READ_FILE 452
# define F_RECEIVE 453
# define F_REGEXP 454
# define F_REMOVE_ACTION 455
# define F_REMOVE_CALL_OUT 456
# define F_RENAME 457
# define F_REPLACE_STRING 458
# define F_RESTORE_OBJECT 459
# define F_RM 460
# define F_RMDIR 461
# define F_RUSAGE 462
# define F_SAVE_OBJECT 463
# define F_SET_AUTHOR 464
# define F_SET_BIT 465
# define F_SET_DEBUG_LEVEL 466
# define F_SET_HEART_BEAT 467
# define F_SET_HIDE 468
# define F_SET_LIGHT 469
# define F_SET_LIVING_NAME 470
# define F_SETEUID 471
# define F_SHADOW 472
# define F_SHUTDOWN 473
# define F_SIZEOF 474
# define F_SNOOP 475
# define F_SOCKET_ACCEPT 476
# define F_SOCKET_ACQUIRE 477
# define F_SOCKET_ADDRESS 478
# define F_SOCKET_BIND 479
# define F_SOCKET_CLOSE 480
# define F_SOCKET_CONNECT 481
# define F_SOCKET_CREATE 482
# define F_SOCKET_ERROR 483
# define F_SOCKET_LISTEN 484
# define F_SOCKET_RELEASE 485
# define F_SOCKET_WRITE 486
# define F_SORT_ARRAY 487
# define F_SPRINTF 488
# define F_STAT 489
# define F_STRCMP 490
# define F_STRINGP 491
# define F_STRLEN 492
# define F_SWAP 493
# define F_TAIL 494
# define F_TELL_OBJECT 495
# define F_TEST_BIT 496
# define F_THIS_OBJECT 497
# define F_THIS_PLAYER 498
# define F_THROW 499
# define F_TIME 500
# define F_TRACE 501
# define F_TRACEPREFIX 502
# define F_UNDEFINEDP 503
# define F_UNIQUE_ARRAY 504
# define F_UPTIME 505
# define F_USERP 506
# define F_USERS 507
# define F_VALUES 508
# define F_VERSION 509
# define F_WIZARDP 510
# define F_WRITE_BYTES 511
# define F_WRITE_FILE 512
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 2277 "lang.y"


void yyerror(str)
char *str;
{
    extern int num_parse_error;

    if (num_parse_error > 5)
	return;
    (void)fprintf(stderr, "%s: %s line %d\n", current_file, str,
		  current_line);
    fflush(stderr);
    smart_log(current_file, current_line, str,0);
    add_errors_for_file (current_file, 1);
    num_parse_error++;
}

static int check_declared(str)
    char *str;
{
    struct variable *vp;
    int offset;

    for (offset=0;
	 offset < mem_block[A_VARIABLES].current_size;
	 offset += sizeof (struct variable)) {
	vp = (struct variable *)&mem_block[A_VARIABLES].block[offset];
	if (vp->flags & NAME_HIDDEN)
	    continue;
	if (strcmp(vp->name, str) == 0)
	    return offset / sizeof (struct variable);
    }
    return -1;
}

static int verify_declared(str)
    char *str;
{
    int r;

    r = check_declared(str);
    if (r < 0) {
	char buff[100];
        (void)sprintf(buff, "Variable %s not declared !", str);
        yyerror(buff);
	return -1;
    }
    return r;
}

void free_all_local_names()
{
    int i;

    for (i=0; i<current_number_of_locals; i++) {
	FREE(local_names[i]);
	local_names[i] = 0;
    }
    current_number_of_locals = 0;
    current_break_stack_need = 0;
    max_break_stack_need = 0;
}

void add_local_name(str, type)
    char *str;
    int type;
{
    if (current_number_of_locals == MAX_LOCAL)
	yyerror("Too many local variables");
    else {
	type_of_locals[current_number_of_locals] = type;
	local_names[current_number_of_locals++] = str;
    }
}

static void copy_inherits(from, type, var_index)
    struct program *from;
    int type, var_index;
{
    int i;

    for (i = 0; i < from->p.i.num_inherited; i++) {
      struct inherit inh;

      inh = from->p.i.inherit[i]; /* Make a copy */
      /* Adjust the info */
      inh.type |= type;
      if (inh.type & TYPE_MOD_PUBLIC)
        inh.type &= ~TYPE_MOD_PRIVATE;
      inh.variable_index_offset += var_index;
      add_to_mem_block (A_INHERITS, (char *)&inh, sizeof inh);
    }
} /* copy_inherits() */

void look_for_heart_beat (prog)
    struct program *prog;
{
    char *heart_beat_name;
    extern char *findstring PROT((char *));
    heart_beat_name = findstring ("heart_beat");
    if (heart_beat_name && search_for_function (heart_beat_name, prog)) {
      prog->p.i.heart_beat_index = function_index_found;
      if (function_inherit_found == prog->p.i.num_inherited)
        prog->p.i.heart_beat_prog = -1;
      else
        prog->p.i.heart_beat_prog = function_inherit_found;
      }
    else {
      prog->p.i.heart_beat_index = -1;
      prog->p.i.heart_beat_prog = -1;
    }
} /* look_for_heart_beat() */

void hash_functions (size, from, to)
    int size;
    struct function *from, *to;
{
    int first_hole; /* Keep chained lists through the hash table */
    int first_collision;
    int last_collision;
    int i, probe;

    /* Prepare the lists */
    for (first_hole = 0, first_collision = -1, i = 0; i < size; i++) {
      to[i].next_hashed_function = i+1; /* forward */
      to[i].num_arg = i-1; /* back */
      to[i].name = 0; /* free entry */
      }
    to[size-1].next_hashed_function = -1;
    /* Hash all non-collisions and mark collisions */
    for (i = 0, last_collision = -1; i < size; i++) {
      probe = PTR_HASH(from[i].name, size);
      if (!to[probe].name) {   /* free */
        if (to[probe].num_arg == -1)
          first_hole = to[probe].next_hashed_function;
        else
          to[to[probe].num_arg].next_hashed_function =
            to[probe].next_hashed_function;
        if (to[probe].next_hashed_function != -1)
          to[to[probe].next_hashed_function].num_arg = to[probe].num_arg;
        to[probe] = from[i];
        to[probe].next_hashed_function = -1;
        }
      else { /* collision */
        if (first_collision == -1) 
          last_collision = first_collision = i; 
        else {
          from[last_collision].next_hashed_function = i;
          last_collision = i;
	  }
        from[i].next_hashed_function = -1;
        }
      }
    /* Plug collisions into the holes */
    for ( ; first_collision != -1;
          first_collision = from[first_collision].next_hashed_function) {
      i = first_hole;
      first_hole = to[i].next_hashed_function;
      to[i] = from[first_collision];
      to[i].next_hashed_function = -1;
      for (probe = PTR_HASH (to[i].name, size);
           to[probe].next_hashed_function != -1;
           probe = to[probe].next_hashed_function)
        ;
      to[probe].next_hashed_function = i;
      }
    /* Finished */
}
    
/*
 * Copy all variable names from the object that is inherited from.
 * It is very important that they are stored in the same order with the
 * Copy all variabel names from the object that is inherited from.
 * It is very important that they are stored in the same order with the
 * same index.
 */
static void copy_variables(from, type)
    struct program *from;
    int type;
{
    int i;

    for (i=0; (unsigned)i<from->p.i.num_variables; i++) {
	int new_type = type;
	int n = check_declared(from->p.i.variable_names[i].name);

	if (n != -1 && (VARIABLE(n)->type & TYPE_MOD_NO_MASK)) {
	    char p[2048];

	    sprintf(p, "Illegal to redefine 'nomask' variable \"%s\"",
		    VARIABLE(n)->name);
	    yyerror(p);
	}
	/*
	 * 'public' variables should not become private when inherited
	 * 'private'.
	 */
	if (from->p.i.variable_names[i].type & TYPE_MOD_PUBLIC)
	    new_type &= ~TYPE_MOD_PRIVATE;
	define_variable(from->p.i.variable_names[i].name,
			from->p.i.variable_names[i].type | new_type,
			from->p.i.variable_names[i].type & TYPE_MOD_PRIVATE ?
			    NAME_HIDDEN : 0);
    }
}

/*
 * This function is called from lex.c for every new line read from the
 * "top" file (means not included files). Some new lines are missed,
 * as with #include statements, so it is compensated for.
 */
void store_line_number_info()
{
    unsigned short offset = mem_block[A_PROGRAM].current_size;

    while(mem_block[A_LINENUMBERS].current_size / sizeof (short) <
	  current_line)
    {
	add_to_mem_block(A_LINENUMBERS, (char *)&offset, sizeof offset);
    }
}

static char *get_type_name(type)
    int type;
{
    static char buff[100];
    static char *type_name[] = { "unknown", "int", "string",
				     "void", "object", "mapping", "mixed", };
    int pointer = 0;

    buff[0] = 0;
    if (type & TYPE_MOD_STATIC)
	strcat(buff, "static ");
    if (type & TYPE_MOD_NO_MASK)
	strcat(buff, "nomask ");
    if (type & TYPE_MOD_PRIVATE)
	strcat(buff, "private ");
    if (type & TYPE_MOD_PROTECTED)
	strcat(buff, "protected ");
    if (type & TYPE_MOD_PUBLIC)
	strcat(buff, "public ");
    if (type & TYPE_MOD_VARARGS)
	strcat(buff, "varargs ");
    type &= TYPE_MOD_MASK;
    if (type & TYPE_MOD_POINTER) {
	pointer = 1;
	type &= ~TYPE_MOD_POINTER;
    }
    if (type >= sizeof type_name / sizeof type_name[0])
	fatal("Bad type\n");
    strcat(buff, type_name[type]);
    strcat(buff," ");
    if (pointer)
	strcat(buff, "* ");
    return buff;
}

void type_error(str, type)
    char *str;
    int type;
{
    static char buff[100];
    char *p;
    p = get_type_name(type);
    if (strlen(str) + strlen(p) + 5 >= sizeof buff) {
	yyerror(str);
    } else {
	strcpy(buff, str);
	strcat(buff, ": \"");
	strcat(buff, p);
	strcat(buff, "\"");
	yyerror(buff);
    }
}

/*
 * Compile an LPC file.
 */
void compile_file() {
    int yyparse();

    prolog();
    yyparse();
    epilog();
}

static char *get_two_types(type1, type2)
    int type1, type2;
{
    static char buff[100];

    strcpy(buff, "( ");
    strcat(buff, get_type_name(type1));
    strcat(buff, "vs ");
    strcat(buff, get_type_name(type2));
    strcat(buff, ")");
    return buff;
}

int remove_undefined_prototypes (num_functions, functions) 
  int num_functions;
  struct function *functions;
  {
  int i;
  for (i = 0; i < num_functions; ) {
    if (functions[i].flags & NAME_PROTOTYPE) 
      functions[i] = functions[--num_functions];
    else i++;
    }
  return num_functions;
  }

/*
 * The program has been compiled. Prepare a 'struct program' to be returned.
 */
void epilog() {
    int size, i;
    char *p;
    struct function *funp;
    static int current_id_number = 1;
    /* Functions left after removing dangling prototypes */
    int functions_left;

#ifdef DEBUG
    if (num_parse_error == 0 && type_of_arguments.current_size != 0)
	fatal("Failed to deallocate argument type stack\n");
#endif
    /*
     * Define the __INIT function, but only if there was any code
     * to initialize.
     */
    if (first_last_initializer_end != last_initializer_end) {
	define_new_function("__INIT", 0, 0, 0, 0, 0);
	/*
	 * Change the last jump after the last initializer into a
	 * return(1) statement.
	 */
	mem_block[A_PROGRAM].block[last_initializer_end-1] =
	    F_CONST1 - F_OFFSET;
	mem_block[A_PROGRAM].block[last_initializer_end-0] =
	    F_RETURN - F_OFFSET;
    }

    if (num_parse_error > 0) {
	prog = 0;
	for (i=0; i<NUMAREAS; i++)
	    FREE(mem_block[i].block);
	smart_log (NULL,0,NULL,1);
	return;
    }
    functions_left = remove_undefined_prototypes (
              mem_block[A_FUNCTIONS].current_size / sizeof (struct function), 
              mem_block[A_FUNCTIONS].block);
    /* I don't like doing this, but I see no other way. |D| */
    mem_block[A_FUNCTIONS].current_size = 
        functions_left * sizeof (struct function);
    size = align(sizeof (struct program));
    for (i=0; i<NUMPAREAS; i++)
	size += align(mem_block[i].current_size);
    p = (char *)XALLOC(size);
    prog = (struct program *)p;
    *prog = NULL_program;
    prog->p.i.total_size = size;
    prog->p.i.ref = 0;
    prog->name = string_copy(current_file);
    prog->p.i.id_number = current_id_number++;
    total_prog_block_size += prog->p.i.total_size;
    total_num_prog_blocks += 1;

    p += align(sizeof (struct program));
    prog->p.i.program = p;
    if (mem_block[A_PROGRAM].current_size)
	memcpy(p, mem_block[A_PROGRAM].block,
	       mem_block[A_PROGRAM].current_size);
    prog->p.i.program_size = mem_block[A_PROGRAM].current_size;

    p += align(mem_block[A_PROGRAM].current_size);
    prog->p.i.line_numbers = (unsigned short *)p;
    if (mem_block[A_LINENUMBERS].current_size)
	memcpy(p, mem_block[A_LINENUMBERS].block,
	       mem_block[A_LINENUMBERS].current_size);

    p += align(mem_block[A_LINENUMBERS].current_size);
    prog->p.i.functions = (struct function *)p;
    prog->p.i.num_functions = mem_block[A_FUNCTIONS].current_size /
	sizeof (struct function);
    if (mem_block[A_FUNCTIONS].current_size)
        hash_functions (prog->p.i.num_functions, 
			(struct function *)mem_block[A_FUNCTIONS].block, 
			prog->p.i.functions);

    p += align(mem_block[A_FUNCTIONS].current_size);
    prog->p.i.strings = (char **)p;
    prog->p.i.num_strings = mem_block[A_STRINGS].current_size /
	sizeof (char *);
    if (mem_block[A_STRINGS].current_size)
	memcpy(p, mem_block[A_STRINGS].block,
	       mem_block[A_STRINGS].current_size);

    p += align(mem_block[A_STRINGS].current_size);
    prog->p.i.variable_names = (struct variable *)p;
    prog->p.i.num_variables = mem_block[A_VARIABLES].current_size /
	sizeof (struct variable);
    if (mem_block[A_VARIABLES].current_size)
	memcpy(p, mem_block[A_VARIABLES].block,
	       mem_block[A_VARIABLES].current_size);

    p += align(mem_block[A_VARIABLES].current_size);
    prog->p.i.num_inherited = mem_block[A_INHERITS].current_size /
	sizeof (struct inherit);
    if (prog->p.i.num_inherited) {
	memcpy(p, mem_block[A_INHERITS].block,
	       mem_block[A_INHERITS].current_size);
	prog->p.i.inherit = (struct inherit *)p;
    } else
	prog->p.i.inherit = 0;
   
/* NOTE: Don't forget to hash argument types along with function names */
 
    prog->p.i.argument_types = 0;	/* For now. Will be fixed someday */

    prog->p.i.type_start = 0;
    for (i=0; i<NUMAREAS; i++)
        FREE((char *)mem_block[i].block);

    look_for_heart_beat (prog); 

    /*  marion
	Do referencing here - avoid multiple referencing when an object
	inherits more than one object and one of the inherited is already
	loaded and not the last inherited
    */
    reference_prog (prog, "epilog");
    for (i = 0; (unsigned)i < prog->p.i.num_inherited; i++) {
	reference_prog (prog->p.i.inherit[i].prog, "inheritance");
    }
}

/*
 * Initialize the environment that the compiler needs.
 */
static void prolog() {
    int i;

    switches = 0;
    switch_sptr = 0;
    if (type_of_arguments.block == 0) {
	type_of_arguments.max_size = 100;
	type_of_arguments.block = XALLOC(type_of_arguments.max_size);
    }
    type_of_arguments.current_size = 0;
/* DDT. Carpet tiles.  Three please, none in puce.
    approved_object = 0;
 */
    last_push_indexed = -1;
    last_push_local = -1;
    last_push_identifier = -1;
    prog = 0;		/* 0 means fail to load. */
    comp_stackp = 0;	/* Local temp stack used by compiler */
    current_continue_address = 0;
    current_break_address = 0;
    num_parse_error = 0;
    free_all_local_names();	/* In case of earlier error */
    /* Initialize memory blocks where the result of the compilation
     * will be stored.
     */
    for (i=0; i < NUMAREAS; i++) {
	mem_block[i].block = XALLOC(START_BLOCK_SIZE);
	mem_block[i].current_size = 0;
	mem_block[i].max_size = START_BLOCK_SIZE;
    }
    add_new_init_jump();
    first_last_initializer_end = last_initializer_end;
}

/*
 * Add a trailing jump after the last initialization code.
 */
void add_new_init_jump() {
    /*
     * Add a new jump.
     */
    ins_f_byte(F_JUMP);
    last_initializer_end = mem_block[A_PROGRAM].current_size;
    ins_short(0);
}

/*
   push_switches should be called at the beginning of the parsing of
   any loop construct.
*/

void push_switches()
{
#ifdef DEBUG
    if (switch_sptr == SWITCH_STACK_SIZE) {
        fatal("switch_stack overflow\n");
    }
#endif
    switch_stack[switch_sptr++] = switches;
    switches = 0;
}

/*
   pop_switches should be called at the end of the parsing of any loop
   construct.
*/

void pop_switches()
{
#ifdef DEBUG
    if (switch_sptr == 0) {
       fatal("switch_stack underflow\n");
    }
#endif
    switches = switch_stack[--switch_sptr];
}

char *
the_file_name(name)
char *name;
{
	char *tmp;
	int len;

	len = strlen(name);
	if (len < 3) {
		return string_copy(name);
	}
	tmp = (char *)XALLOC(len);
	if (!tmp) {
		return string_copy(name);
	}
	strcpy(tmp, "/");
	strncpy(tmp + 1, name, len - 2);
	tmp[len - 1] = '\0';
	return tmp;
}
int yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 2,
	0, 1,
	-2, 29,
-1, 31,
	40, 12,
	61, 45,
	-2, 44,
-1, 42,
	41, 19,
	-2, 29,
-1, 44,
	61, 45,
	-2, 44,
-1, 56,
	40, 221,
	-2, 209,
-1, 118,
	93, 146,
	-2, 0,
-1, 119,
	125, 141,
	-2, 0,
-1, 124,
	41, 141,
	-2, 0,
-1, 148,
	325, 194,
	91, 194,
	-2, 185,
-1, 150,
	325, 194,
	91, 194,
	-2, 186,
-1, 209,
	93, 148,
	-2, 0,
-1, 212,
	41, 143,
	125, 143,
	-2, 0,
-1, 218,
	308, 69,
	311, 72,
	125, 53,
	-2, 0,
-1, 221,
	41, 141,
	-2, 0,
-1, 233,
	308, 69,
	311, 72,
	125, 53,
	-2, 0,
-1, 248,
	308, 69,
	311, 72,
	-2, 0,
-1, 255,
	59, 139,
	-2, 0,
-1, 275,
	308, 69,
	311, 72,
	-2, 0,
-1, 297,
	44, 119,
	-2, 140,
-1, 302,
	41, 207,
	-2, 194,
-1, 310,
	59, 79,
	-2, 0,
-1, 339,
	308, 69,
	311, 72,
	-2, 0,
-1, 343,
	44, 119,
	-2, 80,
-1, 369,
	308, 69,
	311, 72,
	-2, 0,
-1, 371,
	308, 69,
	311, 72,
	-2, 0,
-1, 373,
	59, 79,
	-2, 0,
-1, 380,
	41, 79,
	-2, 0,
-1, 383,
	308, 69,
	311, 72,
	-2, 0,
	};
# define YYNPROD 230
# define YYLAST 849
int yyact[]={

   235,   233,   342,   113,     9,    23,   288,   289,   291,    23,
   323,   130,   324,   293,   138,    24,   139,    63,   287,    24,
   112,   285,    91,   339,   278,    10,     8,    63,    13,    11,
    12,    18,    91,   281,   284,   341,   245,   275,   291,   274,
    25,   264,   333,   293,    25,   283,    20,   317,   332,   331,
   318,    45,   279,    97,    98,    35,   123,   115,   306,   260,
   182,   163,   160,   156,   117,    27,    26,    28,    29,    27,
    26,    28,    29,   321,   129,   319,    44,    31,   118,   312,
   231,   203,    55,   244,    58,    81,    77,    59,   170,    82,
   263,    78,   211,   362,   295,   314,   199,    84,   315,   128,
   200,   172,    48,    90,    74,   258,   286,    52,   208,   111,
   119,    22,    43,    90,   161,    63,    41,   379,   266,   378,
    91,   368,   304,    16,   295,   272,   271,   270,   269,    32,
    15,   110,    40,   261,   245,   370,   296,   257,   210,   328,
   143,   325,    41,   326,   327,   142,    18,   382,   117,   329,
   144,   140,    39,   141,   305,   262,   217,   216,   212,   209,
   200,   207,    63,    93,   376,    33,   366,    91,    94,   364,
   363,   344,   149,   149,   301,   116,   314,   300,   228,   215,
   136,   245,   134,   145,   226,    96,   223,   151,   152,   153,
   214,   206,   167,    92,   367,   308,   155,   298,   200,   277,
   276,    90,   221,   220,   178,   127,   126,   124,    42,    63,
   316,   122,   131,   282,    91,   181,   338,    49,   158,    17,
     4,   307,    63,   183,   190,   191,   177,    91,   176,   248,
   192,   193,   204,   234,    34,   165,    50,   114,   198,     6,
   194,   195,   196,    54,    30,   200,   125,   205,    90,    66,
   273,   253,   254,    38,   256,    56,   297,   292,    23,   255,
    69,    87,    88,   213,   202,    56,    70,   201,    24,    63,
    69,    87,    88,   118,    91,    65,    70,   309,   311,    64,
   175,    35,   164,   219,   159,   166,   246,   292,   369,   251,
   252,    72,    73,    25,    57,    90,   247,   313,   224,   334,
    75,    72,    73,   383,    57,   119,    95,   147,    90,   340,
    75,   343,   320,   322,   267,   294,    71,   147,    27,   120,
    28,    29,    91,   259,    89,   380,    71,   373,   310,    76,
   232,    54,   357,   358,    89,   359,   360,   361,   345,   349,
   350,   365,   355,   356,   330,   294,   149,   337,   346,   253,
   254,   348,   256,    56,    99,    90,   250,   255,    69,    87,
    88,   347,   335,   336,    70,   299,   371,   249,   372,   243,
   242,   374,   241,   375,   343,   240,   377,   239,   234,   238,
   237,   343,   236,   381,   246,   384,   265,   251,   252,    72,
    73,   268,    57,    99,   247,   218,   253,   254,    75,   256,
    56,    80,    37,    90,   255,    69,    87,    88,    19,   154,
    36,    70,     5,    99,    71,    14,     3,    76,   132,   135,
   137,   133,    89,     2,     1,    54,   351,   352,   353,   354,
    62,   246,    86,    83,   251,   252,    72,    73,    54,    57,
    79,   247,    67,    85,    61,    75,    60,    56,   162,    68,
   280,    47,    69,    87,    88,   138,   197,   139,    70,    21,
    56,    71,     7,    23,    76,    69,    87,    88,   169,    89,
   168,    70,   173,    24,    46,   290,     0,     0,     0,     0,
     0,     0,     0,    72,    73,     0,    57,     0,     0,     0,
     0,     0,    75,     0,     0,     0,    72,    73,    25,    57,
     0,     0,     0,     0,     0,    75,     0,    56,    71,     0,
     0,    76,    69,    87,    88,     0,    89,     0,    70,     0,
     0,    71,     0,    27,   120,    28,    29,     0,     0,    89,
     0,     0,     0,     0,   184,   185,   186,   187,   188,   189,
     0,     0,     0,    72,    73,    56,    57,     0,     0,     0,
    69,     0,    75,     0,     0,    56,    70,     0,     0,     0,
    69,    87,    88,     0,     0,     0,    70,     0,    71,     0,
     0,    76,     0,     0,   121,     0,    89,    97,    98,     0,
     0,    72,    73,     0,    57,     0,    53,     0,     0,     0,
    75,    72,    73,     0,    57,   105,   106,   109,   107,     0,
    75,     0,     0,     0,     0,     0,    71,     0,     0,    76,
     0,     0,     0,     0,   108,     0,    71,     0,    51,    76,
     0,     0,     0,   100,    89,   101,     0,   102,     0,   103,
     0,   104,     0,     0,   105,   106,   109,   107,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,   108,   105,   106,   109,   107,     0,     0,
     0,     0,   100,     0,   101,     0,   102,     0,   103,     0,
   104,   157,   146,   108,   148,   150,   146,   146,   146,     0,
     0,     0,   100,     0,   101,     0,   102,     0,   103,     0,
   104,     0,     0,   171,   174,     0,     0,   146,     0,   174,
     0,   179,   180,     0,     0,     0,     0,     0,     0,     0,
   146,     0,     0,     0,     0,   146,     0,     0,   146,   146,
   146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
   146,   146,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,   146,     0,     0,     0,
     0,   146,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   146,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,   222,     0,   171,   225,     0,   227,     0,     0,
     0,   229,   230,     0,     0,     0,   174,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   303,     0,     0,
     0,     0,     0,     0,   146,     0,     0,     0,   302 };
int yypact[]={

 -1000, -1000,  -320,    71,   104, -1000,  -276,  -320, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000,  -201,    70, -1000,   121,
  -228, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000,   104,    73, -1000,   168,    51, -1000,  -202,
 -1000,  -232,  -320,   169, -1000, -1000,   152,   119, -1000,   -11,
  -272, -1000, -1000,   293,   352,    46,  -303, -1000,   -88,  -223,
 -1000, -1000, -1000,   182, -1000, -1000, -1000,   -68,   167, -1000,
 -1000, -1000,   166,   165,     5,  -204,  -312,   174, -1000, -1000,
   120,   108,   103, -1000, -1000,   277, -1000,   267,   267,   277,
   277,   277, -1000,  -320,  -215, -1000,   169, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
   229, -1000,  -216,   169,  -217, -1000,   104,   151,   169,   169,
  -312, -1000, -1000,   229,   169,   164,   169,   169,   229, -1000,
  -218,   229,   229,   229,   229,   229,   229,   229,   229,   229,
   229,   229,   277,   277,   277, -1000,  -231,   -13, -1000,   -88,
 -1000, -1000, -1000, -1000,    37, -1000, -1000, -1000, -1000,   229,
 -1000,   -12, -1000, -1000,   229,   150,   117, -1000,    15,   115,
 -1000,    80,   -33,   114, -1000,   229,     5,   149,   169,   113,
   112,   174, -1000, -1000,  -321,  -321,  -321,  -321,  -321,  -321,
   108,   108,   103,   103, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000,   169, -1000,   162, -1000, -1000,   169,   145,   169,
   169,   143,   169, -1000, -1000,   137,   169,   169,   -23,    79,
    12,   169, -1000, -1000, -1000, -1000, -1000, -1000, -1000,    89,
   111,   -35,   104,   122,   332,    69, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000,    68, -1000, -1000,    67,    66,    75,  -269,
  -274,   160,   159,    -2,    78,   169,   157,   229, -1000,   136,
   133,   267,   169, -1000,    63,   110,  -220, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000,   155,    75, -1000,   169,    21,   -29,
 -1000,     4,    99,   172,  -251,    13,  -325,    98,   102, -1000,
 -1000,   -32, -1000,  -240,  -241,  -247, -1000, -1000,   169, -1000,
 -1000, -1000,    89,    89, -1000,   104, -1000,  -295,   169,  -273,
   169,   130, -1000,    -2,   -32,   -32,   -32,   -32,   -32,   -32,
   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,
    52, -1000, -1000, -1000,   129, -1000,   128, -1000, -1000,    75,
   125,   154,    62, -1000, -1000,    77,     4,   172,  -251,    13,
    13,  -325,  -325,  -325,  -325,    98,    98,   102,   102, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000,   169, -1000,    75,
 -1000,    75,   123,   169, -1000, -1000,    60,    58, -1000, -1000,
   169,   106, -1000,    75, -1000 };
int yypgo[]={

     0,   131,    52,   101,    33,    45,    34,    21,   106,    18,
     6,     7,   475,    59,   474,   217,   111,   118,   472,   470,
   468,    88,   462,   236,   459,   456,   451,   213,   450,   449,
   448,    24,   446,   586,   444,   443,   107,     0,     2,    87,
   442,    82,   104,    86,    91,   440,    85,    89,   433,   401,
    97,   432,    84,   430,   574,   424,   423,   416,   415,   412,
    83,   410,   409,   219,   102,   408,   402,   395,    80,    41,
   386,     1,   382,   380,   379,   377,   375,   372,   370,   369,
   367,   366,   356,   328,   327,   325,   303,   288,   285,   284,
   283,   282,   280,   279,   275,   249,   246,   232,   229,   221,
   216 };
int yyr1[]={

     0,    55,    56,    56,    58,    58,    59,    53,    17,    17,
    25,    25,    61,    62,    57,    57,    57,    64,    64,    14,
    14,    26,    26,    22,    22,    22,    22,    22,    22,    23,
    23,    15,    35,    24,    24,    16,    16,    16,    16,    16,
    16,    16,    63,    63,    65,    66,    65,    60,    67,    67,
    70,    69,    69,    68,    68,    68,    71,    71,    71,    71,
    71,    71,    71,    71,    71,    71,    71,    71,    71,    80,
    81,    73,    82,    74,    83,    84,    85,    86,    75,    38,
    38,    87,    76,    77,    77,    31,    31,     2,     2,     4,
     4,     5,     5,     6,     6,     6,     7,     7,     7,     7,
     7,     8,     8,     8,     9,     9,     9,    10,    10,    10,
    10,    11,    11,    12,    12,    12,    12,    78,    37,    88,
    37,    54,    54,    54,    36,    89,    90,    36,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     1,    79,
    79,     3,     3,     3,    18,    18,    19,    19,    19,    20,
    20,    21,    41,    91,    41,    39,    92,    39,    40,    40,
    42,    42,    43,    43,    44,    44,    44,    45,    45,    45,
    45,    45,    49,    49,    49,    46,    46,    46,    47,    47,
    47,    47,    48,    48,    50,    50,    50,    50,    50,    50,
    51,    51,    51,    52,    52,    52,    52,    52,    52,    52,
    52,    52,    52,    96,    93,    94,    95,    13,    13,    33,
    33,    33,    33,    34,    28,    27,    27,    32,    97,    32,
    30,    29,    29,    29,    29,    99,    72,    98,   100,   100 };
int yyr2[]={

     0,     2,     6,     0,     0,     3,     9,     3,     1,     3,
     3,     3,     1,     1,    19,     7,     2,     7,     5,     1,
     2,     3,     7,     3,     3,     3,     3,     3,     3,     1,
     5,     5,     9,     2,     1,     3,     3,     3,     3,     3,
     3,     3,     2,     6,     5,     1,    11,     9,     0,     8,
     5,     2,     6,     0,     4,     4,     5,     2,     2,     2,
     2,     2,     2,     2,     4,     2,     2,     5,     5,     1,
     1,    15,     1,    17,     1,     1,     1,     1,    27,     1,
     2,     1,    13,     7,    11,     3,     3,     2,     7,     2,
     7,     2,     7,     2,     7,     7,     2,     7,     7,     7,
     7,     2,     7,     7,     2,     7,     7,     2,     7,     7,
     7,     2,     7,     2,     5,     5,     5,     5,     3,     1,
     9,     2,     7,     7,     3,     1,     1,    15,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     5,     1,     3,     5,     3,     7,     1,     3,     5,     3,
     7,     7,     3,     1,     9,     3,     1,     9,     2,     7,
     2,     7,     2,     7,     2,     7,     7,     2,     7,     7,
     7,     7,     2,     7,     7,     2,     7,     7,     2,     7,
     7,     7,     2,     5,     2,     5,     5,     5,     5,     5,
     2,     5,     5,     2,     3,     2,     2,     7,     3,     3,
     3,    11,    11,     1,    11,    15,    19,     1,     7,     3,
     3,    13,     9,     3,     3,     2,     7,     9,     1,    15,
     2,     2,     5,     7,     7,     1,     9,     9,     0,     4 };
int yychk[]={

 -1000,   -55,   -56,   -57,   -15,   -59,   -23,   -22,   346,   324,
   345,   349,   350,   348,   -58,    59,   -17,   -63,    42,   -65,
   322,   -24,   -16,   281,   291,   316,   342,   341,   343,   344,
   -23,   278,    59,    44,   -27,   283,   -61,   -66,   -63,   -17,
    59,    43,    40,    61,   278,   283,   -14,   -26,   -64,   -15,
   -23,   -54,   -36,   -33,   256,   -41,   278,   317,   -52,   -39,
   -32,   -34,   -53,    40,   -93,   -94,   -95,   -40,   -29,   283,
   289,   339,   314,   315,   -42,   323,   342,   -43,   -44,   -45,
   -49,   -46,   -47,   -48,   -50,   -35,   -51,   284,   285,   347,
   126,    45,    41,    44,   -17,   317,    -1,   284,   285,    61,
   330,   332,   334,   336,   338,   302,   303,   305,   321,   304,
    -1,    63,   323,    91,   325,   280,   -16,   -37,    91,   123,
   342,   -54,   279,   124,    40,   -96,    40,    40,    94,   278,
   323,    38,   298,   301,    62,   299,    60,   300,   335,   337,
    43,    45,    42,    37,    47,   -50,   -33,    40,   -33,   -52,
   -33,   -50,   -50,   -50,   -62,   -64,   278,   -54,   -36,   -89,
   278,   -37,   -30,   278,   -91,   -17,   -88,    41,   -19,   -20,
   -21,   -54,    -3,   -18,   -54,   -92,   -42,    -3,    40,   -54,
   -54,   -43,   278,   -44,   -49,   -49,   -49,   -49,   -49,   -49,
   -46,   -46,   -47,   -47,   -50,   -50,   -50,   -25,   -60,    59,
   123,   -36,   276,    93,   -97,   -41,    41,    44,    93,    44,
    58,   125,    44,   -39,    41,   -37,    44,    44,   -67,   -90,
   -37,    40,   -54,    41,   -21,   -54,    41,   -54,    41,   -54,
   -54,   -68,   -16,   -71,   256,   -37,   -72,   -73,   -74,   -75,
   -76,   -77,   -78,   -79,   -60,    59,   309,   319,   -98,   -80,
   -82,   312,   313,   274,   275,   282,   277,    58,    93,    -3,
   -13,    44,    44,   125,   -69,   -70,   -17,   -68,    59,    59,
    59,    59,    59,   -71,   308,   311,    40,    40,   -31,    -2,
   -28,    -4,   -27,    -5,    -6,    -7,    -8,    -9,   -10,   -11,
   -12,    40,   289,    45,   347,   126,    58,   -37,    40,   -36,
    41,    41,   -33,   -54,    59,    44,   278,   -99,    40,   -71,
   -83,   -37,    58,   276,   124,    94,    38,   298,   301,    62,
   299,    60,   300,   335,   337,    43,    45,    42,    37,    47,
    -2,   289,   289,   289,   -37,   -13,   -13,   -69,  -100,   318,
   -37,   308,   -38,   -37,    41,   -31,    -4,    -5,    -6,    -7,
    -7,    -8,    -8,    -8,    -8,    -9,    -9,   -10,   -10,   -11,
   -11,   -11,    41,    41,    41,   -71,    41,    40,    59,   -87,
    58,   -81,   -37,   -84,   -71,   -71,    41,   -38,    59,    59,
   -85,   -38,    41,   -86,   -71 };
int yydef[]={

     3,    -2,    -2,     4,     8,    16,    34,    29,    23,    24,
    25,    26,    27,    28,     2,     5,     0,     0,     9,    42,
     0,    31,    33,    35,    36,    37,    38,    39,    40,    41,
    30,    -2,    15,     8,     0,   215,     0,     0,    43,     0,
     6,     0,    -2,     0,    -2,   216,     0,    20,    21,     8,
    34,    46,   121,   194,     0,   124,    -2,   210,   190,   152,
   193,   195,   196,     0,   198,   199,   200,   155,     0,   213,
     7,   203,     0,     0,   158,     0,     0,   160,   162,   164,
   167,   172,   175,   178,   182,     0,   184,     0,     0,     0,
     0,     0,    13,    29,     0,    18,     0,   191,   192,   128,
   129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     0,   125,     0,     0,     0,   153,     8,   119,    -2,    -2,
    38,   118,   156,     0,    -2,     0,     0,     0,     0,   222,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   183,   194,     0,    -2,     0,
    -2,   187,   188,   189,     0,    22,    17,   122,   123,     0,
   224,   119,   218,   220,     0,     0,     0,   197,     0,   147,
   149,     0,     0,   142,   144,     0,   159,     0,     0,     0,
     0,   161,   223,   163,   165,   166,   168,   169,   170,   171,
   173,   174,   176,   177,   179,   180,   181,    14,    10,    11,
    48,   126,     0,   212,     0,   154,    32,     0,     0,    -2,
     0,     0,    -2,   157,   217,   119,     0,     0,    -2,     0,
   119,    -2,   120,   201,   150,   151,   202,   145,   204,   207,
     0,     0,     8,    -2,     0,   119,    57,    58,    59,    60,
    61,    62,    63,     0,    65,    66,     0,     0,    -2,     0,
     0,     0,     0,     0,     0,    -2,     0,     0,   211,     0,
     0,     0,     0,    47,     0,    51,     0,    54,    55,    56,
    64,    67,    68,   225,     0,    -2,    74,     0,     0,    85,
    86,    87,   214,    89,    91,    93,    96,   101,   104,   107,
   111,     0,   113,     0,     0,     0,   117,    -2,     0,   127,
   219,   205,    -2,   207,    49,     8,    50,   228,     0,     0,
    -2,   119,    83,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   114,   115,   116,   119,   208,     0,    52,   226,    -2,
   119,     0,     0,    -2,    81,     0,    88,    90,    92,    94,
    95,    97,    98,    99,   100,   102,   103,   105,   106,   108,
   109,   110,   112,   227,   206,   229,    70,     0,    75,    -2,
    84,    -2,   119,    -2,    82,    71,     0,     0,    73,    76,
    -2,     0,    77,    -2,    78 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"F_JUMP",	257,
	"F_JUMP_WHEN_ZERO",	258,
	"F_JUMP_WHEN_NON_ZERO",	259,
	"F_POP_VALUE",	260,
	"F_DUP",	261,
	"F_STORE",	262,
	"F_CALL_DOWN",	263,
	"F_CALL_SELF",	264,
	"F_PUSH_IDENTIFIER_LVALUE",	265,
	"F_PUSH_LOCAL_VARIABLE_LVALUE",	266,
	"F_PUSH_INDEXED_LVALUE",	267,
	"F_INDIRECT",	268,
	"F_INDEX",	269,
	"F_CONST0",	270,
	"F_CONST1",	271,
	"F_CLASS_MEMBER_LVALUE",	272,
	"F_CALL_EFUN",	273,
	"F_CASE",	274,
	"F_DEFAULT",	275,
	"F_RANGE",	276,
	"F_IF",	277,
	"F_IDENTIFIER",	278,
	"F_LAND",	279,
	"F_LOR",	280,
	"F_STATUS",	281,
	"F_RETURN",	282,
	"F_STRING",	283,
	"F_INC",	284,
	"F_DEC",	285,
	"F_POST_INC",	286,
	"F_POST_DEC",	287,
	"F_COMMA",	288,
	"F_NUMBER",	289,
	"F_ASSIGN",	290,
	"F_INT",	291,
	"F_ADD",	292,
	"F_SUBTRACT",	293,
	"F_MULTIPLY",	294,
	"F_DIVIDE",	295,
	"F_LT",	296,
	"F_GT",	297,
	"F_EQ",	298,
	"F_GE",	299,
	"F_LE",	300,
	"F_NE",	301,
	"F_ADD_EQ",	302,
	"F_SUB_EQ",	303,
	"F_DIV_EQ",	304,
	"F_MULT_EQ",	305,
	"F_NEGATE",	306,
	"F_SUBSCRIPT",	307,
	"F_WHILE",	308,
	"F_BREAK",	309,
	"F_POP_BREAK",	310,
	"F_DO",	311,
	"F_FOR",	312,
	"F_SWITCH",	313,
	"F_SSCANF",	314,
	"F_PARSE_COMMAND",	315,
	"F_STRING_DECL",	316,
	"F_LOCAL_NAME",	317,
	"F_ELSE",	318,
	"F_CONTINUE",	319,
	"F_MOD",	320,
	"F_MOD_EQ",	321,
	"F_INHERIT",	322,
	"F_COLON_COLON",	323,
	"F_STATIC",	324,
	"F_ARROW",	325,
	"F_AGGREGATE",	326,
	"F_AGGREGATE_ASSOC",	327,
	"F_COMPL",	328,
	"F_AND",	329,
	"F_AND_EQ",	330,
	"F_OR",	331,
	"F_OR_EQ",	332,
	"F_XOR",	333,
	"F_XOR_EQ",	334,
	"F_LSH",	335,
	"F_LSH_EQ",	336,
	"F_RSH",	337,
	"F_RSH_EQ",	338,
	"F_CATCH",	339,
	"F_END_CATCH",	340,
	"F_MAPPING",	341,
	"F_OBJECT",	342,
	"F_VOID",	343,
	"F_MIXED",	344,
	"F_PRIVATE",	345,
	"F_NO_MASK",	346,
	"F_NOT",	347,
	"F_PROTECTED",	348,
	"F_PUBLIC",	349,
	"F_VARARGS",	350,
	"F_ACTIONS_DEFINED",	351,
	"F_ADD_ACTION",	352,
	"F_ADD_WORTH",	353,
	"F_ALL_INVENTORY",	354,
	"F_ALLOCATE",	355,
	"F_ALLOCATE_MAPPING",	356,
	"F_AUTHOR_STATS",	357,
	"F_BREAK_POINT",	358,
	"F_CACHE_STATS",	359,
	"F_CALL_OTHER",	360,
	"F_CALL_OUT",	361,
	"F_CALL_OUT_INFO",	362,
	"F_CAPITALIZE",	363,
	"F_CAT",	364,
	"F_CHILDREN",	365,
	"F_CLEAR_BIT",	366,
	"F_CLONE_OBJECT",	367,
	"F_CLONEP",	368,
	"F_COMMAND",	369,
	"F_CP",	370,
	"F_CRYPT",	371,
	"F_CTIME",	372,
	"F_DEBUG_INFO",	373,
	"F_DEEP_INHERIT_LIST",	374,
	"F_DEEP_INVENTORY",	375,
	"F_DESTRUCT",	376,
	"F_DISABLE_COMMANDS",	377,
	"F_DOMAIN_STATS",	378,
	"F_DUMP_FILE_DESCRIPTORS",	379,
	"F_DUMP_SOCKET_STATUS",	380,
	"F_DUMPALLOBJ",	381,
	"F_EACH",	382,
	"F_ED",	383,
	"F_ENABLE_COMMANDS",	384,
	"F_ENABLE_WIZARD",	385,
	"F_ENVIRONMENT",	386,
	"F_EVENT",	387,
	"F_EXEC",	388,
	"F_EXPLODE",	389,
	"F_EXPORT_UID",	390,
	"F_EXTRACT",	391,
	"F_FILE_LENGTH",	392,
	"F_FILE_NAME",	393,
	"F_FILE_SIZE",	394,
	"F_FILTER_ARRAY",	395,
	"F_FIND_CALL_OUT",	396,
	"F_FIND_LIVING",	397,
	"F_FIND_OBJECT",	398,
	"F_FIND_PLAYER",	399,
	"F_FIRST_INVENTORY",	400,
	"F_FUNCTION_EXISTS",	401,
	"F_GET_CHAR",	402,
	"F_GET_DIR",	403,
	"F_GETEUID",	404,
	"F_GETUID",	405,
	"F_IMPLODE",	406,
	"F_IN_EDIT",	407,
	"F_IN_INPUT",	408,
	"F_INHERIT_LIST",	409,
	"F_INPUT_TO",	410,
	"F_INTERACTIVE",	411,
	"F_INTP",	412,
	"F_KEYS",	413,
	"F_LINK",	414,
	"F_LIVING",	415,
	"F_LOCALTIME",	416,
	"F_LOG_FILE",	417,
	"F_LOWER_CASE",	418,
	"F_MALLOC_STATUS",	419,
	"F_MAP_ARRAY",	420,
	"F_MAP_DELETE",	421,
	"F_MAPP",	422,
	"F_MASTER",	423,
	"F_MEMBER_ARRAY",	424,
	"F_MESSAGE",	425,
	"F_MKDIR",	426,
	"F_MOVE_OBJECT",	427,
	"F_MUD_NAME",	428,
	"F_MUD_STATUS",	429,
	"F_NEW",	430,
	"F_NEXT_INVENTORY",	431,
	"F_NEXT_LIVING",	432,
	"F_NOTIFY_FAIL",	433,
	"F_NULLP",	434,
	"F_OBJECTP",	435,
	"F_OPCPROF",	436,
	"F_PLURALIZE",	437,
	"F_POINTERP",	438,
	"F_PRESENT",	439,
	"F_PREVIOUS_OBJECT",	440,
	"F_PRINTF",	441,
	"F_PROCESS_STRING",	442,
	"F_QUERY_HOST_NAME",	443,
	"F_QUERY_IDLE",	444,
	"F_QUERY_IP_NAME",	445,
	"F_QUERY_IP_NUMBER",	446,
	"F_QUERY_LOAD_AVERAGE",	447,
	"F_QUERY_SNOOP",	448,
	"F_QUERY_VERB",	449,
	"F_RANDOM",	450,
	"F_READ_BYTES",	451,
	"F_READ_FILE",	452,
	"F_RECEIVE",	453,
	"F_REGEXP",	454,
	"F_REMOVE_ACTION",	455,
	"F_REMOVE_CALL_OUT",	456,
	"F_RENAME",	457,
	"F_REPLACE_STRING",	458,
	"F_RESTORE_OBJECT",	459,
	"F_RM",	460,
	"F_RMDIR",	461,
	"F_RUSAGE",	462,
	"F_SAVE_OBJECT",	463,
	"F_SET_AUTHOR",	464,
	"F_SET_BIT",	465,
	"F_SET_DEBUG_LEVEL",	466,
	"F_SET_HEART_BEAT",	467,
	"F_SET_HIDE",	468,
	"F_SET_LIGHT",	469,
	"F_SET_LIVING_NAME",	470,
	"F_SETEUID",	471,
	"F_SHADOW",	472,
	"F_SHUTDOWN",	473,
	"F_SIZEOF",	474,
	"F_SNOOP",	475,
	"F_SOCKET_ACCEPT",	476,
	"F_SOCKET_ACQUIRE",	477,
	"F_SOCKET_ADDRESS",	478,
	"F_SOCKET_BIND",	479,
	"F_SOCKET_CLOSE",	480,
	"F_SOCKET_CONNECT",	481,
	"F_SOCKET_CREATE",	482,
	"F_SOCKET_ERROR",	483,
	"F_SOCKET_LISTEN",	484,
	"F_SOCKET_RELEASE",	485,
	"F_SOCKET_WRITE",	486,
	"F_SORT_ARRAY",	487,
	"F_SPRINTF",	488,
	"F_STAT",	489,
	"F_STRCMP",	490,
	"F_STRINGP",	491,
	"F_STRLEN",	492,
	"F_SWAP",	493,
	"F_TAIL",	494,
	"F_TELL_OBJECT",	495,
	"F_TEST_BIT",	496,
	"F_THIS_OBJECT",	497,
	"F_THIS_PLAYER",	498,
	"F_THROW",	499,
	"F_TIME",	500,
	"F_TRACE",	501,
	"F_TRACEPREFIX",	502,
	"F_UNDEFINEDP",	503,
	"F_UNIQUE_ARRAY",	504,
	"F_UPTIME",	505,
	"F_USERP",	506,
	"F_USERS",	507,
	"F_VALUES",	508,
	"F_VERSION",	509,
	"F_WIZARDP",	510,
	"F_WRITE_BYTES",	511,
	"F_WRITE_FILE",	512,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"all : program",
	"program : program def possible_semi_colon",
	"program : /* empty */",
	"possible_semi_colon : /* empty */",
	"possible_semi_colon : ';'",
	"inheritance : type_modifier_list F_INHERIT string_con1 ';'",
	"number : F_NUMBER",
	"optional_star : /* empty */",
	"optional_star : '*'",
	"block_or_semi : block",
	"block_or_semi : ';'",
	"def : type optional_star F_IDENTIFIER",
	"def : type optional_star F_IDENTIFIER '(' argument ')'",
	"def : type optional_star F_IDENTIFIER '(' argument ')' block_or_semi",
	"def : type name_list ';'",
	"def : inheritance",
	"new_arg_name : type optional_star F_IDENTIFIER",
	"new_arg_name : type F_LOCAL_NAME",
	"argument : /* empty */",
	"argument : argument_list",
	"argument_list : new_arg_name",
	"argument_list : argument_list ',' new_arg_name",
	"type_modifier : F_NO_MASK",
	"type_modifier : F_STATIC",
	"type_modifier : F_PRIVATE",
	"type_modifier : F_PUBLIC",
	"type_modifier : F_VARARGS",
	"type_modifier : F_PROTECTED",
	"type_modifier_list : /* empty */",
	"type_modifier_list : type_modifier type_modifier_list",
	"type : type_modifier_list opt_basic_type",
	"cast : '(' basic_type optional_star ')'",
	"opt_basic_type : basic_type",
	"opt_basic_type : /* empty */",
	"basic_type : F_STATUS",
	"basic_type : F_INT",
	"basic_type : F_STRING_DECL",
	"basic_type : F_OBJECT",
	"basic_type : F_MAPPING",
	"basic_type : F_VOID",
	"basic_type : F_MIXED",
	"name_list : new_name",
	"name_list : new_name ',' name_list",
	"new_name : optional_star F_IDENTIFIER",
	"new_name : optional_star F_IDENTIFIER",
	"new_name : optional_star F_IDENTIFIER '=' expr0",
	"block : '{' local_declarations statements '}'",
	"local_declarations : /* empty */",
	"local_declarations : local_declarations basic_type local_name_list ';'",
	"new_local_name : optional_star F_IDENTIFIER",
	"local_name_list : new_local_name",
	"local_name_list : new_local_name ',' local_name_list",
	"statements : /* empty */",
	"statements : statement statements",
	"statements : error ';'",
	"statement : comma_expr ';'",
	"statement : cond",
	"statement : while",
	"statement : do",
	"statement : for",
	"statement : switch",
	"statement : case",
	"statement : default",
	"statement : return ';'",
	"statement : block",
	"statement : ';'",
	"statement : F_BREAK ';'",
	"statement : F_CONTINUE ';'",
	"while : /* empty */",
	"while : F_WHILE '(' comma_expr ')'",
	"while : F_WHILE '(' comma_expr ')' statement",
	"do : /* empty */",
	"do : F_DO statement F_WHILE '(' comma_expr ')' ';'",
	"for : F_FOR '('",
	"for : F_FOR '(' for_expr ';'",
	"for : F_FOR '(' for_expr ';' for_expr ';'",
	"for : F_FOR '(' for_expr ';' for_expr ';' for_expr ')'",
	"for : F_FOR '(' for_expr ';' for_expr ';' for_expr ')' statement",
	"for_expr : /* empty */",
	"for_expr : comma_expr",
	"switch : F_SWITCH '(' comma_expr ')'",
	"switch : F_SWITCH '(' comma_expr ')' statement",
	"case : F_CASE case_label ':'",
	"case : F_CASE case_label F_RANGE case_label ':'",
	"case_label : constant",
	"case_label : string_constant",
	"constant : const1",
	"constant : constant '|' const1",
	"const1 : const2",
	"const1 : const1 '^' const2",
	"const2 : const3",
	"const2 : const2 '&' const3",
	"const3 : const4",
	"const3 : const3 F_EQ const4",
	"const3 : const3 F_NE const4",
	"const4 : const5",
	"const4 : const4 '>' const5",
	"const4 : const4 F_GE const5",
	"const4 : const4 '<' const5",
	"const4 : const4 F_LE const5",
	"const5 : const6",
	"const5 : const5 F_LSH const6",
	"const5 : const5 F_RSH const6",
	"const6 : const7",
	"const6 : const6 '+' const7",
	"const6 : const6 '-' const7",
	"const7 : const8",
	"const7 : const7 '*' const8",
	"const7 : const7 '%' const8",
	"const7 : const7 '/' const8",
	"const8 : const9",
	"const8 : '(' constant ')'",
	"const9 : F_NUMBER",
	"const9 : '-' F_NUMBER",
	"const9 : F_NOT F_NUMBER",
	"const9 : '~' F_NUMBER",
	"default : F_DEFAULT ':'",
	"comma_expr : expr0",
	"comma_expr : comma_expr",
	"comma_expr : comma_expr ',' expr0",
	"expr0 : expr01",
	"expr0 : lvalue assign expr0",
	"expr0 : error assign expr01",
	"expr01 : expr1",
	"expr01 : expr1 '?'",
	"expr01 : expr1 '?' expr01",
	"expr01 : expr1 '?' expr01 ':' expr01",
	"assign : '='",
	"assign : F_AND_EQ",
	"assign : F_OR_EQ",
	"assign : F_XOR_EQ",
	"assign : F_LSH_EQ",
	"assign : F_RSH_EQ",
	"assign : F_ADD_EQ",
	"assign : F_SUB_EQ",
	"assign : F_MULT_EQ",
	"assign : F_MOD_EQ",
	"assign : F_DIV_EQ",
	"return : F_RETURN",
	"return : F_RETURN comma_expr",
	"expr_list : /* empty */",
	"expr_list : expr_list2",
	"expr_list : expr_list2 ','",
	"expr_list2 : expr0",
	"expr_list2 : expr_list2 ',' expr0",
	"expr_list3 : /* empty */",
	"expr_list3 : expr_list4",
	"expr_list3 : expr_list4 ','",
	"expr_list4 : assoc_pair",
	"expr_list4 : expr_list4 ',' assoc_pair",
	"assoc_pair : expr0 ':' expr0",
	"expr1 : expr2",
	"expr1 : expr2 F_LOR",
	"expr1 : expr2 F_LOR expr1",
	"expr2 : expr211",
	"expr2 : expr211 F_LAND",
	"expr2 : expr211 F_LAND expr2",
	"expr211 : expr212",
	"expr211 : expr211 '|' expr212",
	"expr212 : expr213",
	"expr212 : expr212 '^' expr213",
	"expr213 : expr22",
	"expr213 : expr213 '&' expr22",
	"expr22 : expr23",
	"expr22 : expr24 F_EQ expr24",
	"expr22 : expr24 F_NE expr24",
	"expr23 : expr24",
	"expr23 : expr24 '>' expr24",
	"expr23 : expr24 F_GE expr24",
	"expr23 : expr24 '<' expr24",
	"expr23 : expr24 F_LE expr24",
	"expr24 : expr25",
	"expr24 : expr24 F_LSH expr25",
	"expr24 : expr24 F_RSH expr25",
	"expr25 : expr27",
	"expr25 : expr25 '+' expr27",
	"expr25 : expr25 '-' expr27",
	"expr27 : expr28",
	"expr27 : expr27 '*' expr3",
	"expr27 : expr27 '%' expr3",
	"expr27 : expr27 '/' expr3",
	"expr28 : expr3",
	"expr28 : cast expr3",
	"expr3 : expr31",
	"expr3 : F_INC lvalue",
	"expr3 : F_DEC lvalue",
	"expr3 : F_NOT expr3",
	"expr3 : '~' expr3",
	"expr3 : '-' expr3",
	"expr31 : expr4",
	"expr31 : lvalue F_INC",
	"expr31 : lvalue F_DEC",
	"expr4 : function_call",
	"expr4 : lvalue",
	"expr4 : string",
	"expr4 : number",
	"expr4 : '(' comma_expr ')'",
	"expr4 : catch",
	"expr4 : sscanf",
	"expr4 : parse_command",
	"expr4 : '(' '[' expr_list3 ']' ')'",
	"expr4 : '(' '{' expr_list '}' ')'",
	"catch : F_CATCH",
	"catch : F_CATCH '(' comma_expr ')'",
	"sscanf : F_SSCANF '(' expr0 ',' expr0 lvalue_list ')'",
	"parse_command : F_PARSE_COMMAND '(' expr0 ',' expr0 ',' expr0 lvalue_list ')'",
	"lvalue_list : /* empty */",
	"lvalue_list : ',' lvalue lvalue_list",
	"lvalue : F_IDENTIFIER",
	"lvalue : F_LOCAL_NAME",
	"lvalue : expr4 '[' comma_expr F_RANGE comma_expr ']'",
	"lvalue : expr4 '[' comma_expr ']'",
	"string : F_STRING",
	"string_constant : string_con1",
	"string_con1 : F_STRING",
	"string_con1 : string_con1 '+' F_STRING",
	"function_call : function_name '(' expr_list ')'",
	"function_call : expr4 F_ARROW function_ident",
	"function_call : expr4 F_ARROW function_ident '(' expr_list ')'",
	"function_ident : F_IDENTIFIER",
	"function_name : F_IDENTIFIER",
	"function_name : F_COLON_COLON F_IDENTIFIER",
	"function_name : F_OBJECT F_COLON_COLON F_IDENTIFIER",
	"function_name : F_IDENTIFIER F_COLON_COLON F_IDENTIFIER",
	"cond : condStart statement",
	"cond : condStart statement optional_else_part",
	"condStart : F_IF '(' comma_expr ')'",
	"optional_else_part : /* empty */",
	"optional_else_part : F_ELSE statement",
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
		
case 5:
# line 940 "lang.y"
{ yyerror("Extra ';'. Ignored."); } break;
case 6:
# line 943 "lang.y"
{
                    char *initializer_name;
		    struct object *ob;
		    struct inherit inherit;
		    int initializer;
		    extern char *findstring PROT((char *));

		    ob = find_object2(yypvt[-1].string);
		    if (ob == 0) {
			inherit_file = yypvt[-1].string;
			/* Return back to load_object() */
			YYACCEPT;
		    }
		    FREE(yypvt[-1].string);
		    inherit.prog = ob->prog;
/* Hmmm...  well, since we dont actually seem to use this... 
		    if (ob->flags & O_APPROVED)
			approved_object = 1;
 */
		    check_nomask_functions (ob->prog);
		    inherit.prog = ob->prog;
                    inherit.type = yypvt[-3].number;
		    inherit.variable_index_offset =
			mem_block[A_VARIABLES].current_size /
			    sizeof (struct variable);
                    copy_inherits (ob->prog, inherit.type,
                                   inherit.variable_index_offset);
		    add_to_mem_block(A_INHERITS, &inherit, sizeof inherit);
		    copy_variables(ob->prog, yypvt[-3].number);
                    initializer_name = findstring ("__INIT");
                    if (initializer_name &&
                        search_for_function (initializer_name, ob->prog)) {
                      transfer_init_control();
                      ins_f_byte (F_CALL_DOWN);
                        /* indices found by search_for_function */
                      ins_byte (function_inherit_found +
                                mem_block[A_INHERITS].current_size /
                                sizeof(struct inherit) - 1 -
                                ob->prog->p.i.num_inherited);
                      ins_byte (function_index_found);
                      ins_byte (0);  /* Actual number of arguments */
                      ins_f_byte (F_POP_VALUE);
                      add_new_init_jump();
                      }
		} break;
case 7:
# line 990 "lang.y"
{
	    if ( yypvt[-0].number == 0 ) {
		ins_f_byte(F_CONST0); yyval.type = TYPE_ANY;
	    } else if ( yypvt[-0].number == 1 ) {
		ins_f_byte(F_CONST1); yyval.type = TYPE_NUMBER;
	    } else {
		ins_f_byte(F_NUMBER); ins_long(yypvt[-0].number); yyval.type = TYPE_NUMBER;
	    }
	} break;
case 8:
# line 1000 "lang.y"
{ yyval.number = 0; } break;
case 9:
# line 1000 "lang.y"
{ yyval.number = TYPE_MOD_POINTER; } break;
case 10:
# line 1002 "lang.y"
{ yyval.number = 0; } break;
case 11:
# line 1002 "lang.y"
{ yyval.number = ';'; } break;
case 12:
# line 1005 "lang.y"
{
	    /* Save start of function. */
	    push_explicit(mem_block[A_PROGRAM].current_size);

	    if (yypvt[-2].number & TYPE_MOD_MASK) {
		exact_types = yypvt[-2].number | yypvt[-1].number;
	    } else {
		if (pragma_strict_types)
		    yyerror("\"#pragma strict_types\" requires type of function");
		/* force strict types - no more compat mode */
#ifdef STRICT_TYPE_CHECKING
		exact_types = TYPE_ANY; /* default to return type of mixed */
#else
		exact_types = 0;
#endif
	    }
	} break;
case 13:
# line 1023 "lang.y"
{
	    /*
	     * Define a prototype. If it is a real function, then the
	     * prototype will be replaced below.
	     */
	    define_new_function(yypvt[-4].string, yypvt[-1].number, 0, 0,
                                NAME_PROTOTYPE, yypvt[-6].number | yypvt[-5].number);
	} break;
case 14:
# line 1032 "lang.y"
{
	    /* Either a prototype or a block */
	    if (yypvt[-0].number == ';') {
		(void)pop_address(); /* Not used here */
	    } else {
		define_new_function(yypvt[-6].string, yypvt[-3].number, current_number_of_locals - yypvt[-3].number+
			( max_break_stack_need -1 ) / sizeof(struct svalue) +1,
			pop_address(), 0, yypvt[-8].number | yypvt[-7].number);
		ins_f_byte(F_CONST0); ins_f_byte(F_RETURN);
	    }
	    free_all_local_names();
	    FREE(yypvt[-6].string);		/* Value was copied above */
	} break;
case 15:
# line 1045 "lang.y"
{ if (yypvt[-2].number == 0) yyerror("Missing type"); } break;
case 17:
# line 1049 "lang.y"
{
	    if (exact_types && yypvt[-2].number == 0) {
		yyerror("Missing type for argument");
		add_local_name(yypvt[-0].string, TYPE_ANY);	/* Supress more errors */
	    } else {
		add_local_name(yypvt[-0].string, yypvt[-2].number | yypvt[-1].number);
	    }
	} break;
case 18:
# line 1058 "lang.y"
{yyerror("Illegal to redeclare local name"); } break;
case 19:
# line 1060 "lang.y"
{ yyval.number = 0; } break;
case 21:
# line 1063 "lang.y"
{ yyval.number = 1; } break;
case 22:
# line 1064 "lang.y"
{ yyval.number = yypvt[-2].number + 1; } break;
case 23:
# line 1066 "lang.y"
{ yyval.number = TYPE_MOD_NO_MASK; } break;
case 24:
# line 1067 "lang.y"
{ yyval.number = TYPE_MOD_STATIC; } break;
case 25:
# line 1068 "lang.y"
{ yyval.number = TYPE_MOD_PRIVATE; } break;
case 26:
# line 1069 "lang.y"
{ yyval.number = TYPE_MOD_PUBLIC; } break;
case 27:
# line 1070 "lang.y"
{ yyval.number = TYPE_MOD_VARARGS; } break;
case 28:
# line 1071 "lang.y"
{ yyval.number = TYPE_MOD_PROTECTED; } break;
case 29:
# line 1073 "lang.y"
{ yyval.number = 0; } break;
case 30:
# line 1074 "lang.y"
{ yyval.number = yypvt[-1].number | yypvt[-0].number; } break;
case 31:
# line 1076 "lang.y"
{ yyval.number = yypvt[-1].number | yypvt[-0].number; current_type = yyval.number; } break;
case 32:
# line 1079 "lang.y"
{
	    yyval.type = yypvt[-2].number | yypvt[-1].number;
	} break;
case 34:
# line 1083 "lang.y"
{ yyval.number = TYPE_UNKNOWN; } break;
case 35:
# line 1085 "lang.y"
{ yyval.number = TYPE_NUMBER; current_type = yyval.number; } break;
case 36:
# line 1086 "lang.y"
{ yyval.number = TYPE_NUMBER; current_type = yyval.number; } break;
case 37:
# line 1087 "lang.y"
{ yyval.number = TYPE_STRING; current_type = yyval.number; } break;
case 38:
# line 1088 "lang.y"
{ yyval.number = TYPE_OBJECT; current_type = yyval.number; } break;
case 39:
# line 1089 "lang.y"
{ yyval.number = TYPE_MAPPING; current_type = yyval.number; } break;
case 40:
# line 1090 "lang.y"
{yyval.number = TYPE_VOID; current_type = yyval.number; } break;
case 41:
# line 1091 "lang.y"
{ yyval.number = TYPE_ANY; current_type = yyval.number; } break;
case 44:
# line 1097 "lang.y"
{
	    define_variable(yypvt[-0].string, current_type | yypvt[-1].number, 0);
	    FREE(yypvt[-0].string);
	} break;
case 45:
# line 1102 "lang.y"
{
	    int var_num;
	    define_variable(yypvt[-0].string, current_type | yypvt[-1].number, 0);
	    var_num = verify_declared(yypvt[-0].string);
	    transfer_init_control();
	    ins_f_byte(F_PUSH_IDENTIFIER_LVALUE);
	    ins_byte(var_num);
	} break;
case 46:
# line 1111 "lang.y"
{
	    if (!compatible_types((current_type | yypvt[-4].number) & TYPE_MOD_MASK, yypvt[-0].type)){
		char buff[100];
		sprintf(buff, "Type mismatch %s when initializing %s",
			get_two_types(current_type | yypvt[-4].number, yypvt[-0].type), yypvt[-3].string);
		yyerror(buff);
	    }
	    ins_f_byte(F_ASSIGN);
	    ins_f_byte(F_POP_VALUE);
	    add_new_init_jump();
	    FREE(yypvt[-3].string);
	} break;
case 47:
# line 1124 "lang.y"
{ ; } break;
case 50:
# line 1130 "lang.y"
{
	    add_local_name(yypvt[-0].string, current_type | yypvt[-1].number);
	} break;
case 56:
# line 1142 "lang.y"
{
	    ins_f_byte(F_POP_VALUE);
	    if (d_flag)
		ins_f_byte(F_BREAK_POINT);
	    /* if (exact_types && !TYPE($1,TYPE_VOID))
		yyerror("Value thrown away"); */
	} break;
case 67:
# line 1153 "lang.y"
{
		    if (current_break_address == 0)
			yyerror("break statement outside loop");
		    if (current_break_address & BREAK_ON_STACK) {
			ins_f_byte(F_BREAK);
		    } else {
		        ins_f_byte(F_JUMP); ins_short(current_break_address);
		    }
		} break;
case 68:
# line 1163 "lang.y"
{
		    if (current_continue_address == 0)
			yyerror("continue statement outside loop");
                    if (switches) {
                         ins_f_byte(F_POP_BREAK); ins_short(switches);
                    }
		    ins_f_byte(F_JUMP); ins_short(current_continue_address);
		} break;
case 69:
# line 1173 "lang.y"
{   push_explicit(current_continue_address);
	    push_explicit(current_break_address);
            push_switches();
	    current_continue_address = mem_block[A_PROGRAM].current_size;
	} break;
case 70:
# line 1178 "lang.y"
{
	    ins_f_byte(F_JUMP_WHEN_NON_ZERO); ins_short(0);	/* to block */
	    current_break_address = mem_block[A_PROGRAM].current_size;
	    ins_f_byte(F_JUMP); ins_short(0);	/* Exit loop */
	    upd_short(current_break_address-2,
		      mem_block[A_PROGRAM].current_size);
	} break;
case 71:
# line 1186 "lang.y"
{
	  ins_f_byte(F_JUMP); ins_short(current_continue_address);
	  upd_short(current_break_address+1,
		    mem_block[A_PROGRAM].current_size);
	  current_break_address = pop_address();
	  current_continue_address = pop_address();
          pop_switches();
        } break;
case 72:
# line 1195 "lang.y"
{
        int tmp_save;

        push_switches();
        push_explicit(current_continue_address);
	push_explicit(current_break_address);
	/* Jump to start of loop. */
	ins_f_byte(F_JUMP); tmp_save = mem_block[A_PROGRAM].current_size;
	ins_short(0);
	current_break_address = mem_block[A_PROGRAM].current_size;
	/* Jump to end of loop. All breaks go through this one. */
	ins_f_byte(F_JUMP); push_address(); ins_short(0);
	current_continue_address = mem_block[A_PROGRAM].current_size;
	upd_short(tmp_save, current_continue_address);
        push_address();
	
    } break;
case 73:
# line 1212 "lang.y"
{
	ins_f_byte(F_JUMP_WHEN_NON_ZERO); ins_short(pop_address());
	/* Fill in the break jump address in the beginning of the loop. */
	upd_short(pop_address(), mem_block[A_PROGRAM].current_size);
	current_break_address = pop_address();
	current_continue_address = pop_address();
        pop_switches();
    } break;
case 74:
# line 1221 "lang.y"
{ push_explicit(current_continue_address);
		    push_explicit(current_break_address);
                    push_switches(); } break;
case 75:
# line 1224 "lang.y"
{   ins_f_byte(F_POP_VALUE);
		      push_address();
		  } break;
case 76:
# line 1227 "lang.y"
{
		    ins_f_byte(F_JUMP_WHEN_NON_ZERO);
		    ins_short(0);	/* Jump to block of block */
		    current_break_address = mem_block[A_PROGRAM].current_size;
		    ins_f_byte(F_JUMP); ins_short(0);	/* Out of loop */
	 	    current_continue_address =
			mem_block[A_PROGRAM].current_size;
		  } break;
case 77:
# line 1235 "lang.y"
{
	 	    ins_f_byte(F_POP_VALUE);
		    ins_f_byte(F_JUMP); ins_short(pop_address());
		    /* Here starts the block. */
		    upd_short(current_break_address-2,
			      mem_block[A_PROGRAM].current_size);
		  } break;
case 78:
# line 1243 "lang.y"
{
       ins_f_byte(F_JUMP); ins_short(current_continue_address);
       /* Now, the address of the end of the block is known. */
       upd_short(current_break_address+1, mem_block[A_PROGRAM].current_size);
       current_break_address = pop_address();
       current_continue_address = pop_address();
       pop_switches();
   } break;
case 79:
# line 1252 "lang.y"
{ ins_f_byte(F_CONST1); } break;
case 81:
# line 1256 "lang.y"
{
	switches++;
        current_break_stack_need += sizeof(short);
        if ( current_break_stack_need > max_break_stack_need )
            max_break_stack_need = current_break_stack_need;
	push_explicit(current_case_number_heap);
	push_explicit(current_case_string_heap);
	push_explicit(zero_case_label);
	push_explicit(current_break_address);
	ins_f_byte(F_SWITCH);
	ins_byte(0xff); /* kind of table */
	current_case_number_heap = mem_block[A_CASE_NUMBERS].current_size;
	current_case_string_heap = mem_block[A_CASE_STRINGS].current_size;
	zero_case_label = NO_STRING_CASE_LABELS;
	ins_short(0); /* address of table */
	current_break_address = mem_block[A_PROGRAM].current_size |
				BREAK_ON_STACK | BREAK_FROM_CASE ;
	ins_short(0); /* break address to push, table is entered before */
	ins_short(0); /* default address */
    } break;
case 82:
# line 1277 "lang.y"
{
	char *heap_start;
	int heap_end_offs;
	int i,o;
	int current_key,last_key;
	/* int size_without_table; */
	int block_index;
	int current_case_heap;
	int lookup_start;
	int lookup_start_key;

	current_break_address &= ~(BREAK_ON_STACK|BREAK_FROM_CASE);

	if ( !read_short(current_break_address+2 ) )
	    upd_short(current_break_address+2,     /* no default given ->  */
	      mem_block[A_PROGRAM].current_size);  /* create one           */

	/* it isn't unusual that the last case/default has no break */
	ins_f_byte(F_BREAK);
	if(zero_case_label & (NO_STRING_CASE_LABELS|SOME_NUMERIC_CASE_LABELS)){
	    block_index = A_CASE_NUMBERS;
	    current_case_heap = current_case_number_heap;
	} else {
	    block_index = A_CASE_STRINGS;
	    current_case_heap = current_case_string_heap;
	    if (zero_case_label&0xffff) {
		struct case_heap_entry temp;

		temp.key = (int)ZERO_AS_STR_CASE_LABEL;
		temp.addr = zero_case_label;
		temp.line = 0; /* if this is accessed later, something is
				* really wrong				  */
		add_to_case_heap(A_CASE_STRINGS,&temp);
	    }
	}
	heap_start = mem_block[block_index].block + current_case_heap ;
	heap_end_offs = mem_block[block_index].current_size -current_case_heap;
	if (!heap_end_offs) yyerror("switch without case not supported");

        /* add a dummy entry so that we can always
        * assume we have no or two childs
        */
        add_to_mem_block(block_index, "\0\0\0\0\0\0\0\0",
            sizeof(struct case_heap_entry) );

        /* read out the heap and build a sorted table */
	/* the table could be optimized better, but let's first see
	* how much switch is used at all when it is full-featured...
	*/
	mem_block[A_CASE_LABELS].current_size = 0;
	lookup_start = 0;
	lookup_start_key = ((struct case_heap_entry*)heap_start)->key;
        for( ; ((struct case_heap_entry*)heap_start)->addr; )
        {
            int offset;
	    int curr_line,last_line;
	    unsigned short current_addr,last_addr = 0xffff;
	    int range_start;

            current_key = ((struct case_heap_entry*)heap_start)->key ;
            curr_line = ((struct case_heap_entry*)heap_start)->line ;
            current_addr = ((struct case_heap_entry*)heap_start)->addr ;
            if ( current_key == last_key &&
              mem_block[A_CASE_LABELS].current_size )
            {
                char buf[90];

                sprintf(buf,"Duplicate case in line %d and %d",
		    last_line, curr_line);
                yyerror(buf);
            }
	    if (curr_line) {
		if (last_addr == 1) {
                    char buf[120];
    
		    sprintf(buf,
"Discontinued case label list range, line %d by line %d",
		      last_line, curr_line);
                    yyerror(buf);
		}
		  else if (current_key == last_key + 1
		    && current_addr == last_addr) {
		    if (mem_block[A_CASE_LABELS].current_size
		      != range_start + 6) {
		      *(short*)(mem_block[A_CASE_LABELS].block+range_start+4)
			=1;
		      mem_block[A_CASE_LABELS].current_size = range_start + 6;
		    }
		} else {
		    range_start = mem_block[A_CASE_LABELS].current_size;
		}
	    }
            last_key = current_key;
	    last_line = curr_line;
	    last_addr = current_addr;
	    add_to_mem_block(A_CASE_LABELS,
                (char *)&current_key, sizeof(long) );
	    add_to_mem_block(A_CASE_LABELS,
		(char *)&current_addr, sizeof(short) );
            for ( offset = 0; ; )
            {

                int child1,child2;

                child1 = ( offset << 1 ) + sizeof(struct case_heap_entry);
                child2 = child1 + sizeof(struct case_heap_entry);
                if ( child1 >= heap_end_offs ) break;
                if ( ((struct case_heap_entry*)(heap_start+child1))->addr &&
                  ( !((struct case_heap_entry*)(heap_start+child2))->addr ||
                   ((struct case_heap_entry*)(heap_start+child1))->key <=
                   ((struct case_heap_entry*)(heap_start+child2))->key  ) )
                {
                    *(struct case_heap_entry*)(heap_start+offset) =
                    *(struct case_heap_entry*)(heap_start+child1);
                    offset = child1;
                } else
                    if (((struct case_heap_entry*)(heap_start+child2))->addr ) {
                        *(struct case_heap_entry*)(heap_start+offset) =
                        *(struct case_heap_entry*)(heap_start+child2);
                        offset = child2;
                    } else break;
            }
            ((struct case_heap_entry*)(heap_start+offset))->addr = 0;
        }

	/* write start of table */
        upd_short(current_break_address-2,
            mem_block[A_PROGRAM].current_size);

	add_to_mem_block(A_PROGRAM, mem_block[A_CASE_LABELS].block,
            mem_block[A_CASE_LABELS].current_size );
        /* calculate starting index for itarative search at execution time */
        for(i=0xf0,o=6; o<<1 <= mem_block[A_CASE_LABELS].current_size; )
            i++,o<<=1;
        if (block_index == A_CASE_STRINGS) i = ( i << 4 ) | 0xf;
        /* and store it */
        mem_block[A_PROGRAM].block[current_break_address-3] &= i;
#if 0  /* neither the code for ordinary switch is fully debugged now,
	* nor is the code for packed switch tables complete */
	d = ((struct case_heap_entry*)heap_start)->key;
	if ( (r-d)*sizeof(short) < heap_end_offs ) {
	    mem_block[A_PROGRAM].block[current_break_address-3] &= 0xfe;
            upd_short(current_break_address-2, mem_block[A_PROGRAM].current_size);
            size_without_table = mem_block[A_PROGRAM].current_size;
	    r = heap_end_offs / sizeof(struct case_heap_entry);
	    add_to_mem_block(A_PROGRAM,mem_block[A_PROGRAM]->block,
		r * sizeof(short) );
	    memset(mem_block[A_PROGRAM]->block+size_without_table,
		'\0',r * sizeof(short) );
	    ins_long( d );
	    for(; --r; heap_start += sizeof(struct case_heap_entry) )
	    {
		upd_short(size_without_table + sizeof(short)*
                    ( ((struct case_heap_entry*)heap_start)->key - d )
		  , ((struct case_heap_entry*)heap_start)->addr );
	    }
        }
#endif /* 0 */
	upd_short(current_break_address, mem_block[A_PROGRAM].current_size);
	
	mem_block[A_CASE_NUMBERS].current_size = current_case_number_heap;
	mem_block[A_CASE_STRINGS].current_size = current_case_string_heap;
    	current_break_address = pop_address();
	zero_case_label = pop_address();
    	current_case_string_heap = pop_address();
    	current_case_number_heap = pop_address();
        current_break_stack_need -= sizeof(short);
        switches--;
    } break;
case 83:
# line 1448 "lang.y"
{
	struct case_heap_entry temp;

	if ( !( current_break_address & BREAK_FROM_CASE ) ) {
	    yyerror("Case outside switch");
	    break;
	}
	temp.key = yypvt[-1].case_label.key;
	temp.addr = mem_block[A_PROGRAM].current_size;
	temp.line = current_line;
	add_to_case_heap(yypvt[-1].case_label.block,&temp);
    } break;
case 84:
# line 1461 "lang.y"
{
	struct case_heap_entry temp;

	if ( yypvt[-3].case_label.block != A_CASE_NUMBERS || yypvt[-1].case_label.block != A_CASE_NUMBERS )
	    yyerror("String case labels not allowed as range bounds");
	if (yypvt[-3].case_label.key > yypvt[-1].case_label.key) break;
	temp.key = yypvt[-3].case_label.key;
	temp.addr = 1;
	temp.line = current_line;
	add_to_case_heap(A_CASE_NUMBERS,&temp);
	temp.key = yypvt[-1].case_label.key;
	temp.addr = mem_block[A_PROGRAM].current_size;
	temp.line = 0;
	add_to_case_heap(A_CASE_NUMBERS,&temp);
    } break;
case 85:
# line 1478 "lang.y"
{
	    if ( !(zero_case_label & NO_STRING_CASE_LABELS) )
		yyerror("Mixed case label list not allowed");
	    if ( yyval.case_label.key = yypvt[-0].number )
	        zero_case_label |= SOME_NUMERIC_CASE_LABELS;
	    else
		zero_case_label |= mem_block[A_PROGRAM].current_size;
	    yyval.case_label.block = A_CASE_NUMBERS;
	} break;
case 86:
# line 1488 "lang.y"
{
	    if ( zero_case_label & SOME_NUMERIC_CASE_LABELS )
		yyerror("Mixed case label list not allowed");
	    zero_case_label &= ~NO_STRING_CASE_LABELS;
            store_prog_string(yypvt[-0].string);
            yyval.case_label.key = (int)yypvt[-0].string;
	    yyval.case_label.block = A_CASE_STRINGS;
        } break;
case 88:
# line 1499 "lang.y"
{ yyval.number = yypvt[-2].number | yypvt[-0].number; } break;
case 90:
# line 1502 "lang.y"
{ yyval.number = yypvt[-2].number ^ yypvt[-0].number; } break;
case 92:
# line 1505 "lang.y"
{ yyval.number = yypvt[-2].number & yypvt[-0].number; } break;
case 94:
# line 1508 "lang.y"
{ yyval.number = yypvt[-2].number == yypvt[-0].number; } break;
case 95:
# line 1509 "lang.y"
{ yyval.number = yypvt[-2].number != yypvt[-0].number; } break;
case 97:
# line 1512 "lang.y"
{ yyval.number = yypvt[-2].number >  yypvt[-0].number; } break;
case 98:
# line 1513 "lang.y"
{ yyval.number = yypvt[-2].number >= yypvt[-0].number; } break;
case 99:
# line 1514 "lang.y"
{ yyval.number = yypvt[-2].number <  yypvt[-0].number; } break;
case 100:
# line 1515 "lang.y"
{ yyval.number = yypvt[-2].number <= yypvt[-0].number; } break;
case 102:
# line 1518 "lang.y"
{ yyval.number = yypvt[-2].number << yypvt[-0].number; } break;
case 103:
# line 1519 "lang.y"
{ yyval.number = yypvt[-2].number >> yypvt[-0].number; } break;
case 105:
# line 1522 "lang.y"
{ yyval.number = yypvt[-2].number + yypvt[-0].number; } break;
case 106:
# line 1523 "lang.y"
{ yyval.number = yypvt[-2].number - yypvt[-0].number; } break;
case 108:
# line 1526 "lang.y"
{ yyval.number = yypvt[-2].number * yypvt[-0].number; } break;
case 109:
# line 1527 "lang.y"
{ yyval.number = yypvt[-2].number % yypvt[-0].number; } break;
case 110:
# line 1528 "lang.y"
{ yyval.number = yypvt[-2].number / yypvt[-0].number; } break;
case 112:
# line 1531 "lang.y"
{ yyval.number = yypvt[-1].number; } break;
case 114:
# line 1534 "lang.y"
{ yyval.number = -yypvt[-0].number; } break;
case 115:
# line 1535 "lang.y"
{ yyval.number = !yypvt[-0].number; } break;
case 116:
# line 1536 "lang.y"
{ yyval.number = ~yypvt[-0].number; } break;
case 117:
# line 1539 "lang.y"
{
	if ( !( current_break_address & BREAK_FROM_CASE ) ) {
	    yyerror("Default outside switch");
	    break;
	}
	current_break_address &= ~(BREAK_ON_STACK|BREAK_FROM_CASE);
	if ( read_short(current_break_address+2 ) )
	    yyerror("Duplicate default");
	upd_short(current_break_address+2, mem_block[A_PROGRAM].current_size);
	current_break_address |= (BREAK_ON_STACK|BREAK_FROM_CASE);
    } break;
case 118:
# line 1552 "lang.y"
{ yyval.type = yypvt[-0].type; } break;
case 119:
# line 1553 "lang.y"
{ ins_f_byte(F_POP_VALUE); } break;
case 120:
# line 1555 "lang.y"
{ yyval.type = yypvt[-0].type; } break;
case 122:
# line 1559 "lang.y"
{
	    if (exact_types && !compatible_types(yypvt[-2].type, yypvt[-0].type) &&
		!(yypvt[-2].type == TYPE_STRING && yypvt[-0].type == TYPE_NUMBER && yypvt[-1].number == F_ADD_EQ))
	    {
		type_error("Bad assignment. Rhs", yypvt[-0].type);
	    }
	    ins_f_byte(yypvt[-1].number);
	    yyval.type = yypvt[-0].type;
	} break;
case 123:
# line 1568 "lang.y"
{ yyerror("Illegal LHS");  yyval.type = TYPE_ANY; } break;
case 124:
# line 1570 "lang.y"
{ yyval.type = yypvt[-0].type; } break;
case 125:
# line 1572 "lang.y"
{
	    ins_f_byte(F_JUMP_WHEN_ZERO);
	    push_address();
	    ins_short(0);
	} break;
case 126:
# line 1578 "lang.y"
{
	    int i;
	    i = pop_address();
	    ins_f_byte(F_JUMP); push_address(); ins_short(0);
	    upd_short(i, mem_block[A_PROGRAM].current_size);
	} break;
case 127:
# line 1585 "lang.y"
{
	    upd_short(pop_address(), mem_block[A_PROGRAM].current_size);
	    if (exact_types && !compatible_types(yypvt[-3].type, yypvt[-0].type)) {
		type_error("Different types in ?: expr", yypvt[-3].type);
		type_error("                      and ", yypvt[-0].type);
	    }
	    if (yypvt[-3].type == TYPE_ANY) yyval.type = yypvt[-0].type;
	    else if (yypvt[-0].type == TYPE_ANY) yyval.type = yypvt[-3].type;
	    else if (TYPE(yypvt[-3].type, TYPE_MOD_POINTER|TYPE_ANY)) yyval.type = yypvt[-0].type;
	    else if (TYPE(yypvt[-0].type, TYPE_MOD_POINTER|TYPE_ANY)) yyval.type = yypvt[-3].type;
	    else yyval.type = yypvt[-3].type;
	} break;
case 128:
# line 1598 "lang.y"
{ yyval.number = F_ASSIGN; } break;
case 129:
# line 1599 "lang.y"
{ yyval.number = F_AND_EQ; } break;
case 130:
# line 1600 "lang.y"
{ yyval.number = F_OR_EQ; } break;
case 131:
# line 1601 "lang.y"
{ yyval.number = F_XOR_EQ; } break;
case 132:
# line 1602 "lang.y"
{ yyval.number = F_LSH_EQ; } break;
case 133:
# line 1603 "lang.y"
{ yyval.number = F_RSH_EQ; } break;
case 134:
# line 1604 "lang.y"
{ yyval.number = F_ADD_EQ; } break;
case 135:
# line 1605 "lang.y"
{ yyval.number = F_SUB_EQ; } break;
case 136:
# line 1606 "lang.y"
{ yyval.number = F_MULT_EQ; } break;
case 137:
# line 1607 "lang.y"
{ yyval.number = F_MOD_EQ; } break;
case 138:
# line 1608 "lang.y"
{ yyval.number = F_DIV_EQ; } break;
case 139:
# line 1611 "lang.y"
{
	    if (exact_types && !TYPE(exact_types, TYPE_VOID))
		type_error("Must return a value for a function declared",
			   exact_types);
	    ins_f_byte(F_CONST0);
	    ins_f_byte(F_RETURN);
	} break;
case 140:
# line 1619 "lang.y"
{
	    if (exact_types && !TYPE(yypvt[-0].type, exact_types & TYPE_MOD_MASK))
		type_error("Return type not matching", exact_types);
	    ins_f_byte(F_RETURN);
	} break;
case 141:
# line 1625 "lang.y"
{ yyval.number = 0; } break;
case 142:
# line 1626 "lang.y"
{ yyval.number = yypvt[-0].number; } break;
case 143:
# line 1627 "lang.y"
{ yyval.number = yypvt[-1].number; } break;
case 144:
# line 1629 "lang.y"
{ yyval.number = 1; add_arg_type(yypvt[-0].type); } break;
case 145:
# line 1630 "lang.y"
{ yyval.number = yypvt[-2].number + 1; add_arg_type(yypvt[-0].type); } break;
case 146:
# line 1632 "lang.y"
{ yyval.number = 0; } break;
case 147:
# line 1633 "lang.y"
{ yyval.number = yypvt[-0].number; } break;
case 148:
# line 1634 "lang.y"
{ yyval.number = yypvt[-1].number; } break;
case 149:
# line 1636 "lang.y"
{ yyval.number = yypvt[-0].number; } break;
case 150:
# line 1637 "lang.y"
{ yyval.number = yypvt[-2].number + 2; } break;
case 151:
# line 1639 "lang.y"
{ yyval.number = 2; } break;
case 152:
# line 1642 "lang.y"
{ yyval.type = yypvt[-0].type; } break;
case 153:
# line 1644 "lang.y"
{
	    ins_f_byte(F_DUP); ins_f_byte(F_JUMP_WHEN_NON_ZERO);
	    push_address();
	    ins_short(0);
	    ins_f_byte(F_POP_VALUE);
	} break;
case 154:
# line 1651 "lang.y"
{
	    upd_short(pop_address(), mem_block[A_PROGRAM].current_size);
	    if (yypvt[-3].type == yypvt[-0].type)
		yyval.type = yypvt[-3].type;
	    else
		yyval.type = TYPE_ANY;	/* Return type can't be known */
	} break;
case 155:
# line 1659 "lang.y"
{ yyval.type = yypvt[-0].type; } break;
case 156:
# line 1661 "lang.y"
{
	    ins_f_byte(F_DUP); ins_f_byte(F_JUMP_WHEN_ZERO);
	    push_address();
	    ins_short(0);
	    ins_f_byte(F_POP_VALUE);
	} break;
case 157:
# line 1668 "lang.y"
{
	    upd_short(pop_address(), mem_block[A_PROGRAM].current_size);
	    if (yypvt[-3].type == yypvt[-0].type)
		yyval.type = yypvt[-3].type;
	    else
		yyval.type = TYPE_ANY;	/* Return type can't be known */
	} break;
case 159:
# line 1678 "lang.y"
{
	      if (exact_types && !TYPE(yypvt[-2].type,TYPE_NUMBER))
		  type_error("Bad argument 1 to |", yypvt[-2].type);
	      if (exact_types && !TYPE(yypvt[-0].type,TYPE_NUMBER))
		  type_error("Bad argument 2 to |", yypvt[-0].type);
	      yyval.type = TYPE_NUMBER;
	      ins_f_byte(F_OR);
	  } break;
case 161:
# line 1689 "lang.y"
{
	      if (exact_types && !TYPE(yypvt[-2].type,TYPE_NUMBER))
		  type_error("Bad argument 1 to ^", yypvt[-2].type);
	      if (exact_types && !TYPE(yypvt[-0].type,TYPE_NUMBER))
		  type_error("Bad argument 2 to ^", yypvt[-0].type);
	      yyval.type = TYPE_NUMBER;
	      ins_f_byte(F_XOR);
	  } break;
case 163:
# line 1700 "lang.y"
{
	      ins_f_byte(F_AND);
	      if ( !TYPE(yypvt[-2].type,TYPE_MOD_POINTER) || !TYPE(yypvt[-0].type,TYPE_MOD_POINTER) ) {
	          if (exact_types && !TYPE(yypvt[-2].type,TYPE_NUMBER))
		      type_error("Bad argument 1 to &", yypvt[-2].type);
	          if (exact_types && !TYPE(yypvt[-0].type,TYPE_NUMBER))
		      type_error("Bad argument 2 to &", yypvt[-0].type);
	      }
	      yyval.type = TYPE_NUMBER;
	  } break;
case 165:
# line 1713 "lang.y"
{
	    int t1 = yypvt[-2].type & TYPE_MOD_MASK, t2 = yypvt[-0].type & TYPE_MOD_MASK;
	    if (exact_types && t1 != t2 && t1 != TYPE_ANY && t2 != TYPE_ANY) {
		type_error("== always false because of different types", yypvt[-2].type);
		type_error("                               compared to", yypvt[-0].type);
	    }
	    ins_f_byte(F_EQ);
	    yyval.type = TYPE_NUMBER;
	} break;
case 166:
# line 1723 "lang.y"
{
	    int t1 = yypvt[-2].type & TYPE_MOD_MASK, t2 = yypvt[-0].type & TYPE_MOD_MASK;
	    if (exact_types && t1 != t2 && t1 != TYPE_ANY && t2 != TYPE_ANY) {
		type_error("!= always true because of different types", yypvt[-2].type);
		type_error("                               compared to", yypvt[-0].type);
	    }
	    ins_f_byte(F_NE);
	    yyval.type = TYPE_NUMBER;
	} break;
case 168:
# line 1735 "lang.y"
{ yyval.type = TYPE_NUMBER; ins_f_byte(F_GT); } break;
case 169:
# line 1737 "lang.y"
{ yyval.type = TYPE_NUMBER; ins_f_byte(F_GE); } break;
case 170:
# line 1739 "lang.y"
{ yyval.type = TYPE_NUMBER; ins_f_byte(F_LT); } break;
case 171:
# line 1741 "lang.y"
{ yyval.type = TYPE_NUMBER; ins_f_byte(F_LE); } break;
case 173:
# line 1745 "lang.y"
{
	    ins_f_byte(F_LSH);
	    yyval.type = TYPE_NUMBER;
	    if (exact_types && !TYPE(yypvt[-2].type, TYPE_NUMBER))
		type_error("Bad argument number 1 to '<<'", yypvt[-2].type);
	    if (exact_types && !TYPE(yypvt[-0].type, TYPE_NUMBER))
		type_error("Bad argument number 2 to '<<'", yypvt[-0].type);
	} break;
case 174:
# line 1754 "lang.y"
{
	    ins_f_byte(F_RSH);
	    yyval.type = TYPE_NUMBER;
	    if (exact_types && !TYPE(yypvt[-2].type, TYPE_NUMBER))
		type_error("Bad argument number 1 to '>>'", yypvt[-2].type);
	    if (exact_types && !TYPE(yypvt[-0].type, TYPE_NUMBER))
		type_error("Bad argument number 2 to '>>'", yypvt[-0].type);
	} break;
case 176:
# line 1765 "lang.y"
{	ins_f_byte(F_ADD);
		if (yypvt[-2].type == yypvt[-0].type)
			yyval.type = yypvt[-2].type;
		else
			yyval.type = TYPE_ANY;
	} break;
case 177:
# line 1772 "lang.y"
{
	    int bad_arg = 0;

	    if (exact_types) {
		if (!TYPE(yypvt[-2].type, TYPE_NUMBER) && !(yypvt[-2].type & TYPE_MOD_POINTER) ) {
                    type_error("Bad argument number 1 to '-'", yypvt[-2].type);
		    bad_arg++;
		}
		if (!TYPE(yypvt[-0].type, TYPE_NUMBER) && !(yypvt[-0].type & TYPE_MOD_POINTER) ) {
                    type_error("Bad argument number 2 to '-'", yypvt[-0].type);
		    bad_arg++;
		}
	    }
	    yyval.type = TYPE_ANY;
	    if ((yypvt[-2].type & TYPE_MOD_POINTER) || (yypvt[-0].type & TYPE_MOD_POINTER))
		yyval.type = TYPE_MOD_POINTER | TYPE_ANY;
	    if (!(yypvt[-2].type & TYPE_MOD_POINTER) || !(yypvt[-0].type & TYPE_MOD_POINTER)) {
		if (exact_types && yyval.type != TYPE_ANY && !bad_arg)
		    yyerror("Arguments to '-' don't match");
		yyval.type = TYPE_NUMBER;
	    }
	    ins_f_byte(F_SUBTRACT);
	} break;
case 179:
# line 1798 "lang.y"
{
		if ((yypvt[-2].type != TYPE_MAPPING) || (yypvt[-0].type != TYPE_MAPPING)) {
			if (exact_types && !TYPE(yypvt[-2].type, TYPE_NUMBER))
				type_error("Bad argument number 1 to '*'", yypvt[-2].type);
			if (exact_types && !TYPE(yypvt[-0].type, TYPE_NUMBER))
				type_error("Bad argument number 2 to '*'", yypvt[-0].type);
			yyval.type = TYPE_NUMBER;
		}
		else
			yyval.type = TYPE_MAPPING;
		ins_f_byte(F_MULTIPLY);
	} break;
case 180:
# line 1811 "lang.y"
{
	    if (exact_types && !TYPE(yypvt[-2].type, TYPE_NUMBER))
		type_error("Bad argument number 1 to '%'", yypvt[-2].type);
	    if (exact_types && !TYPE(yypvt[-0].type, TYPE_NUMBER))
		type_error("Bad argument number 2 to '%'", yypvt[-0].type);
	    ins_f_byte(F_MOD);
	    yyval.type = TYPE_NUMBER;
	} break;
case 181:
# line 1820 "lang.y"
{
	    if (exact_types && !TYPE(yypvt[-2].type, TYPE_NUMBER))
		type_error("Bad argument number 1 to '/'", yypvt[-2].type);
	    if (exact_types && !TYPE(yypvt[-0].type, TYPE_NUMBER))
		type_error("Bad argument number 2 to '/'", yypvt[-0].type);
	    ins_f_byte(F_DIVIDE);
	    yyval.type = TYPE_NUMBER;
	} break;
case 183:
# line 1831 "lang.y"
{
		  yyval.type = yypvt[-1].type;
		  if (exact_types && yypvt[-0].type != TYPE_ANY && yypvt[-0].type != TYPE_UNKNOWN &&
		      yypvt[-1].type != TYPE_VOID)
		      type_error("Casts are only legal for type mixed, or when unknown", yypvt[-0].type);
	      } break;
case 185:
# line 1840 "lang.y"
{
	    ins_f_byte(F_INC);
	    if (exact_types && !TYPE(yypvt[-0].type, TYPE_NUMBER))
		type_error("Bad argument to ++", yypvt[-0].type);
	    yyval.type = TYPE_NUMBER;
	} break;
case 186:
# line 1847 "lang.y"
{
	    ins_f_byte(F_DEC);
	    if (exact_types && !TYPE(yypvt[-0].type, TYPE_NUMBER))
		type_error("Bad argument to --", yypvt[-0].type);
	    yyval.type = TYPE_NUMBER;
	} break;
case 187:
# line 1854 "lang.y"
{
	    ins_f_byte(F_NOT);	/* Any type is valid here. */
	    yyval.type = TYPE_NUMBER;
	} break;
case 188:
# line 1859 "lang.y"
{
	    ins_f_byte(F_COMPL);
	    if (exact_types && !TYPE(yypvt[-0].type, TYPE_NUMBER))
		type_error("Bad argument to ~", yypvt[-0].type);
	    yyval.type = TYPE_NUMBER;
	} break;
case 189:
# line 1866 "lang.y"
{
	    ins_f_byte(F_NEGATE);
	    if (exact_types && !TYPE(yypvt[-0].type, TYPE_NUMBER))
		type_error("Bad argument to unary '-'", yypvt[-0].type);
	    yyval.type = TYPE_NUMBER;
	} break;
case 191:
# line 1875 "lang.y"
{
	     ins_f_byte(F_POST_INC);
	     if (exact_types && !TYPE(yypvt[-1].type, TYPE_NUMBER))
		 type_error("Bad argument to ++", yypvt[-1].type);
	     yyval.type = TYPE_NUMBER;
	 } break;
case 192:
# line 1882 "lang.y"
{
	     ins_f_byte(F_POST_DEC);
	     if (exact_types && !TYPE(yypvt[-1].type, TYPE_NUMBER))
		 type_error("Bad argument to --", yypvt[-1].type);
	     yyval.type = TYPE_NUMBER;
	 } break;
case 194:
# line 1891 "lang.y"
{
	    int pos = mem_block[A_PROGRAM].current_size;
	    /* Some optimization. Replace the push-lvalue with push-value */
	    if (last_push_identifier == pos-2)
		mem_block[A_PROGRAM].block[last_push_identifier] =
		    F_IDENTIFIER - F_OFFSET;
	    else if (last_push_local == pos-2)
		mem_block[A_PROGRAM].block[last_push_local] =
		    F_LOCAL_NAME - F_OFFSET;
	    else if (last_push_indexed == pos-1)
		mem_block[A_PROGRAM].block[last_push_indexed] =
		    F_INDEX - F_OFFSET;
	    else if (last_push_indexed != 0)
		fatal("Should be a push at this point !\n");
	    yyval.type = yypvt[-0].type;
	} break;
case 197:
# line 1908 "lang.y"
{ yyval.type = yypvt[-1].type; } break;
case 198:
# line 1909 "lang.y"
{ yyval.type = TYPE_ANY; } break;
case 199:
# line 1910 "lang.y"
{ yyval.type = TYPE_NUMBER; } break;
case 200:
# line 1911 "lang.y"
{ yyval.type = TYPE_NUMBER; } break;
case 201:
# line 1913 "lang.y"
{ 
#if USE_BUGGY_CODE
/* this is an error - since expr_list3 never adds anything to the arg_stack */
             pop_arg_stack(yypvt[-2].number);
#endif
             ins_f_byte(F_AGGREGATE_ASSOC);
             ins_short(yypvt[-2].number);
             yyval.type = TYPE_MAPPING;
         } break;
case 202:
# line 1923 "lang.y"
{
	   pop_arg_stack(yypvt[-2].number);		/* We don't care about these types */
	   ins_f_byte(F_AGGREGATE);
	   ins_short(yypvt[-2].number);
	   yyval.type = TYPE_MOD_POINTER | TYPE_ANY;
       } break;
case 203:
# line 1930 "lang.y"
{ ins_f_byte(F_CATCH); push_address(); ins_short(0);} break;
case 204:
# line 1932 "lang.y"
{
	       ins_f_byte(F_END_CATCH);
		   upd_short(pop_address(),
			     mem_block[A_PROGRAM].current_size);
	       } break;
case 205:
# line 1939 "lang.y"
{
	    ins_f_byte(F_SSCANF); ins_byte(yypvt[-1].number + 2);
	} break;
case 206:
# line 1944 "lang.y"
{
	    ins_f_byte(F_PARSE_COMMAND); ins_byte(yypvt[-1].number + 3);
	} break;
case 207:
# line 1948 "lang.y"
{ yyval.number = 0; } break;
case 208:
# line 1949 "lang.y"
{ yyval.number = 1 + yypvt[-0].number; } break;
case 209:
# line 1952 "lang.y"
{
	    int i = verify_declared(yypvt[-0].string);
	    last_push_identifier = mem_block[A_PROGRAM].current_size;
	    ins_f_byte(F_PUSH_IDENTIFIER_LVALUE);
	    ins_byte(i);
	    FREE(yypvt[-0].string);
	    if (i == -1)
		yyval.type = TYPE_ANY;
	    else
		yyval.type = VARIABLE(i)->type & TYPE_MOD_MASK;
	} break;
case 210:
# line 1964 "lang.y"
{
	    last_push_local = mem_block[A_PROGRAM].current_size;
	    ins_f_byte(F_PUSH_LOCAL_VARIABLE_LVALUE);
	    ins_byte(yypvt[-0].number);
	    yyval.type = type_of_locals[yypvt[-0].number];
	} break;
case 211:
# line 1971 "lang.y"
{
	      ins_f_byte(F_RANGE);
	      last_push_indexed = 0;
	      if (exact_types) {
		  if ((yypvt[-5].type & TYPE_MOD_POINTER) == 0 && !TYPE(yypvt[-5].type, TYPE_STRING))
		      type_error("Bad type to indexed value", yypvt[-5].type);
		  if (!TYPE(yypvt[-3].type, TYPE_NUMBER))
		      type_error("Bad type of index", yypvt[-3].type);
		  if (!TYPE(yypvt[-1].type, TYPE_NUMBER))
		      type_error("Bad type of index", yypvt[-1].type);
	      }
	      if (yypvt[-5].type == TYPE_ANY)
		  yyval.type = TYPE_ANY;
	      else if (TYPE(yypvt[-5].type, TYPE_STRING))
		  yyval.type = TYPE_STRING;
	      else if (yypvt[-5].type & TYPE_MOD_POINTER)
		  yyval.type = yypvt[-5].type;
	      else if (exact_types)
		  type_error("Bad type of argument used for range", yypvt[-5].type);
	  } break;
case 212:
# line 1992 "lang.y"
{ 
               last_push_indexed = mem_block[A_PROGRAM].current_size;
               if (TYPE(yypvt[-3].type, TYPE_MAPPING)) {
                  ins_f_byte(F_PUSH_INDEXED_LVALUE);
                  yyval.type = TYPE_ANY;
               } else {
                ins_f_byte(F_PUSH_INDEXED_LVALUE);
                if (exact_types) {
                      if (!(yypvt[-3].type & TYPE_MOD_POINTER) && !TYPE(yypvt[-3].type, TYPE_STRING))
                        type_error("Bad type to indexed value", yypvt[-3].type);
                      if (!TYPE(yypvt[-1].type, TYPE_NUMBER))
                        type_error("Bad type of index", yypvt[-1].type);
                }
                if (yypvt[-3].type == TYPE_ANY)
                    yyval.type = TYPE_ANY;
                else if (TYPE(yypvt[-3].type, TYPE_STRING))
                    yyval.type = TYPE_NUMBER;
                else
                    yyval.type = yypvt[-3].type & TYPE_MOD_MASK & ~TYPE_MOD_POINTER;
              }
           } break;
case 213:
# line 2015 "lang.y"
{
	    ins_f_byte(F_STRING);
	    ins_short(store_prog_string(yypvt[-0].string));
	    FREE(yypvt[-0].string);
	    yyval.type = TYPE_STRING;
	} break;
case 214:
# line 2023 "lang.y"
{
            char *p = make_shared_string(yypvt[-0].string);
            FREE(yypvt[-0].string);
            yyval.string = p;
        } break;
case 216:
# line 2031 "lang.y"
{
	    yyval.string = XALLOC( strlen(yypvt[-2].string) + strlen(yypvt[-0].string) + 1 );
	    strcpy(yyval.string, yypvt[-2].string);
	    strcat(yyval.string, yypvt[-0].string);
	    FREE(yypvt[-2].string);
	    FREE(yypvt[-0].string);
	} break;
case 217:
# line 2040 "lang.y"
{ 
         /* Note that this code could be made a lot cleaner if some 
          * support functions were added. (efun_call() is a good one) */
        int inherited_override = !!strchr(yypvt[-3].string, ':');
        int efun_override = inherited_override && strncmp(yypvt[-3].string,"efun::", 6) == 0;
        int is_sfun;
	int f;

        if (inherited_override && !efun_override) {
          struct function *funp;
          /* Find the right function to call */
          if (find_inherited_function(yypvt[-3].string)) {
            funp = &function_prog_found->p.i.functions[function_index_found];
            yyval.type = funp->type & TYPE_MOD_MASK;
            ins_f_byte(F_CALL_DOWN);
            ins_byte(function_inherit_found);
            ins_byte(function_index_found);
	    ins_byte(yypvt[-1].number); /* Number of arguments */
            }
          else {
            char buff[100];
            sprintf(buff, "Function %.50s undefined", yypvt[-3].string);
            yyerror(buff);
            }
          }
          /* All simul_efuns are considered defined. */
        else if (!efun_override && 
                 (defined_function (yypvt[-3].string) || is_simul_efun (yypvt[-3].string)) ) {
	  struct function *funp;
	  unsigned short *arg_indices;
	  unsigned short *arg_types; 

          if (function_prog_found) {
            funp = &(function_prog_found->p.i.functions[function_index_found]);
	    arg_indices = function_prog_found->p.i.type_start;
	    arg_types = function_prog_found->p.i.argument_types;
	    }
          else {
            funp = FUNCTION(function_index_found);
		/* Beware... these are pointers to volatile structures */
	    arg_indices = (unsigned short *)mem_block[A_ARGUMENT_INDEX].block;
	    arg_types = (unsigned short *)mem_block[A_ARGUMENT_TYPES].block;
	     /* Assumption: exact_types implies correct values for arg_types */
	    }
	  /* Private functions in inherited programs may not be called. */
          if (exact_types && function_prog_found && 
              function_type_mod_found & TYPE_MOD_PRIVATE) {
            char buff[100];
            sprintf (buff, "Function %.44s marked 'private'", funp->name);
            yyerror (buff);
            }
          yyval.type = funp->type & TYPE_MOD_MASK;
          ins_f_byte (F_CALL_SELF);
          /* Insert function name into string list and code its index */
          ins_short (store_prog_string (yypvt[-3].string));
          ins_byte (yypvt[-1].number);
	  /*
	   * Check number of arguments.
	   */
	  if (funp->num_arg != yypvt[-1].number && !(funp->type & TYPE_MOD_VARARGS) &&
              (funp->flags & NAME_STRICT_TYPES) && exact_types) {
	    char buff[100];
 	    sprintf(buff, "Wrong number of arguments to %.60s", yypvt[-3].string);
	    yyerror(buff);
	    }
	  /*
	   * Check the argument types.
	   */
	  if (exact_types && arg_indices && 
	      arg_indices[function_index_found] != INDEX_START_NONE) {
	    int i, first;
		
	    first = arg_indices[function_index_found];
	    for (i=0; i < funp->num_arg && i < yypvt[-1].number; i++) {
	      int tmp = get_argument_type(i, yypvt[-1].number);
	      if (!TYPE(tmp, arg_types[first + i])) {
		char buff[100];
		sprintf(buff, "Bad type for argument %d %s", i+1,
			get_two_types(arg_types[first+i], tmp));
		yyerror(buff);
		}
	      }
	    }
          }
           /* I didn't change any code here, I just reindented. */
	else if (efun_override || (f = lookup_predef(yypvt[-3].string)) != -1) {
	  int min, max, def, *argp;
	  extern int efun_arg_types[];

	  if (efun_override) 
	    f = lookup_predef(yypvt[-3].string+6);
	  if (f == -1) {	/* Only possible for efun_override */
	    char buff[100];
	    sprintf(buff, "Unknown efun: %s", yypvt[-3].string+6);
	    yyerror(buff);
	    } 
          else {
	    min = instrs[f-F_OFFSET].min_arg;
	    max = instrs[f-F_OFFSET].max_arg;
	    def = instrs[f-F_OFFSET].Default;
	    yyval.type = instrs[f-F_OFFSET].ret_type;
	    argp = &efun_arg_types[instrs[f-F_OFFSET].arg_index];
	    if (def && yypvt[-1].number == min-1) {
	      ins_f_byte(def);
	      max--;
	      min--;
	      } 
	    else if (yypvt[-1].number < min) {
	      char bff[100];
	      sprintf(bff, "Too few arguments to %s", instrs[f-F_OFFSET].name);
	      yyerror(bff);
	      } 
	    else if (yypvt[-1].number > max && max != -1) {
	      char bff[100];
	      sprintf(bff, "Too many arguments to %s",instrs[f-F_OFFSET].name);
  	      yyerror(bff);
	      } 
	    else if (max != -1 && exact_types) {
	      /*
	       * Now check all types of the arguments to efuns.
	       */
	      int i, argn;
	      char buff[100];
	      for (argn=0; argn < yypvt[-1].number; argn++) {
		int tmp = get_argument_type(argn, yypvt[-1].number);
		for(i=0; !TYPE(argp[i], tmp) && argp[i] != 0; i++)
		  ;
		if (argp[i] == 0) {
		  sprintf(buff, "Bad argument %d type to efun %s()",
				argn+1, instrs[f-F_OFFSET].name);
		  yyerror(buff);
		  }
		while(argp[i] != 0)
	          i++;
		argp += i + 1;
		}
	      }
	    ins_f_byte(f);
	    /* Only store number of arguments for instructions
	     * that allowed a variable number.
	     */
	    if (max != min)
	      ins_byte(yypvt[-1].number);/* Number of actual arguments */
	    }
	  } 
        else {
  	  if (exact_types) {
            char buff[200];
            sprintf (buff, "Undefined function: %.50s", yypvt[-3].string);
            yyerror (buff);
            }
          /* Function not found */
          ins_f_byte(F_CALL_SELF);
          ins_short(store_prog_string(yypvt[-3].string));
          ins_byte(yypvt[-1].number); /* Number of arguments */
          yyval.type = TYPE_ANY; /* just a guess */
          }
	free(yypvt[-3].string);
	pop_arg_stack(yypvt[-1].number);	/* Argument types not needed more */
    } break;
case 218:
# line 2202 "lang.y"
{
	ins_f_byte(F_STRING);
	ins_short(store_prog_string(yypvt[-0].string));
	FREE(yypvt[-0].string);
    } break;
case 219:
# line 2208 "lang.y"
{
	ins_f_byte(F_CALL_OTHER);
	ins_byte(yypvt[-1].number + 2);
	yyval.type = TYPE_UNKNOWN;
	pop_arg_stack(yypvt[-1].number);	/* No good need of these arguments */
    } break;
case 222:
# line 2219 "lang.y"
{
		    char *p = XALLOC(strlen(yypvt[-0].string) + 3);
		    strcpy(p, "::"); strcat(p, yypvt[-0].string); FREE(yypvt[-0].string);
		    yyval.string = p;
		} break;
case 223:
# line 2225 "lang.y"
{
		    char *p = XALLOC(strlen(yypvt[-0].string)+3+strlen("object"));
		    strcpy(p, "object::"); strcat(p, yypvt[-0].string); FREE(yypvt[-0].string);
		    yyval.string = p;
		} break;
case 224:
# line 2231 "lang.y"
{
		    char *p;
			struct svalue *res;
			extern struct object *master_ob;
			int invalid = 0;

			if (master_ob && (strcmp(yypvt[-2].string, "efun") == 0)) {
				push_malloced_string(the_file_name(current_file));
				push_constant_string(yypvt[-0].string);
				res = safe_apply_master_ob("valid_override", 2);
				if (IS_ZERO(res)) {
					yyerror("Invalid simulated efunction override");
					invalid = 1;
				}
			}
			if (invalid) {
				FREE(yypvt[-2].string);
				yyval.string = yypvt[-0].string;
			} else {
				p = XALLOC(strlen(yypvt[-2].string) + strlen(yypvt[-0].string) + 3);
				strcpy(p, yypvt[-2].string); strcat(p, "::"); strcat(p, yypvt[-0].string);
				FREE(yypvt[-2].string); FREE(yypvt[-0].string);
				yyval.string = p;
			}
		} break;
case 225:
# line 2259 "lang.y"
{
	    int i;
	    i = pop_address();
	    ins_f_byte(F_JUMP); push_address(); ins_short(0);
	    upd_short(i, mem_block[A_PROGRAM].current_size);
	} break;
case 226:
# line 2266 "lang.y"
{ upd_short(pop_address(), mem_block[A_PROGRAM].current_size); } break;
case 227:
# line 2269 "lang.y"
{
	    ins_f_byte(F_JUMP_WHEN_ZERO);
	    push_address();
	    ins_short(0);
	} break;
	}
	goto yystack;		/* reset registers in driver code */
}
