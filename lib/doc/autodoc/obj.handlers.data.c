.DT
data.c
Disk World autodoc help
data.c

.SH Description
.SP 5 5

Universal data initialization handler.
The data initializer can be used to initialize databases for use in other
handlers.  The database is defined in a user-provided text file, with
a format similar to the virtual object compiler.  The input file is
converted to a temporary object, which allows fairly complicated
expressions to be used as data values.  The initializer can handle
arrays and mappings (nested to any level (theoretically)), with a base
element of any type, including mixed and classes.


Added in some code to allow it to compile lisp like files into a mapping as well. 

To initialize a variable, assign it the value returned by compile_data() in the initializer. compile_data() takes an array of filenames as its only argument. 

%^BOLD%^File format%^RESET%^The data file uses the following keywords. Each keyword is followed by the required data. The data can be spread over multiple lines, following the same rules as for LPC code, but the keywords must be at the beginning of the line (preceded by optional whitespace). 
.EP
.SP 5 5
::data:: < array | mapping > [ of ] ... < base > 
.EP
.SP 8 5
This keyword defines the structure of the data. The word "of" is optional. "array" and "mapping" may be repeated as many times as desired. "base" is the base type of the data. For classes, it would be of the form "class <classname>". For types other than classes, the base isn't really used at this time, but something needs to be there to keep the parser in line. Some examples: %^BOLD%^::data:: array of mapping of array of int
.EP
.SP 8 5
::data:: mapping of mapping of mixed
.EP
.SP 8 5
::data:: mapping of array of class myclassThere should only be one ::data:: keyword in the list of files passed to compile_data(). Also, note that classes need to be defined before this statement. This can be done either with ::quote:: or ::#include::. 
.EP
.SP 5 5
::item <index> [,] ... :: [ value ] 
.EP
.SP 8 5
This keyword defines the value for one item of the data. <index> is repeated as often as necessary, given the structure declared in the ::data:: statement. For mappings, the index can be any valid mapping key. For arrays, the index can be either a number, or the strings i, i++, or ++i, for current index, current index (incrementing afterwards), or next index. The value can (and probably should) be omitted for classes, with the field values specified with the "->" keyword below. Examples (corresponding to the three ::data:: examples above): 
.EP
.SP 8 5
::item 0, "item 1", 2:: 42
.EP
.SP 8 5
::item "a", "b":: ({ 1, "fred", ({ 2, 3 }) })
.EP
.SP 8 5
::item "x" i++::
.EP
.SP 8 5

.EP
.SP 5 5
::-><field>:: <value> 
.EP
.SP 8 5
This allows the fields of items of type class to be assigned individually. In general, the preceding ::item:: keyword should not have been given a value. The class must have been defined previously, either with an ::#include:: directive, or with the ::quote:: keyword. Examples: 
.EP
.SP 8 5
::Quote::
.EP
.SP 8 5
class item_data {
.EP
.SP 8 5
    string *season;
.EP
.SP 8 5
    mixed  quant;
.EP
.SP 8 5
    string ob;
.EP
.SP 8 5
}
.EP
.SP 8 5

.EP
.SP 8 5
::Data:: mapping of class item_data
.EP
.SP 8 5

.EP
.SP 8 5
:item "white clover"::
.EP
.SP 8 5
::->season:: ({ "spring" })
.EP
.SP 8 5
::->quant:: ({ 3, 4 })
.EP
.SP 8 5
::->ob:: "/std/plant"
.EP
.SP 8 5
These statements set the season, quant, and ob fields of the mapping 
.EP
.SP 5 5
::quote:: <LPC code> 
.EP
.SP 8 5
This keyword allows specific LPC statements to be inserted in the object that creates the database. To use this effectively requires a little understanding of the translation process. First, all lines associated with a given keyword are folded into one line. This means that using the "//" comment delimiter in a ::quote:: will cause the remainder of the quoted statements to be ignored. Second, the prototype of the function that returns the data isn't written until the ::data:: keyword is encountered. Therefore, any "global" statements (such as class definitions) should be included or quoted before the ::data:: line. The easiest way to see what's going on is to try a few examples and look at the resulting .c file (which is the first data file name with ".c" stuck on the end). 
.EP
.SP 5 5

.EP
.SP 10 5


Written by Jeremy

.EP

.SH Example
.SI 5
mixed data;
data = "/obj/handlers/data"->compile_data(({ "/save/file1.dat",
     "/save/file2.dat" }));
// This will catenate the two files into one, translate it, and return
// the data.  Of course, someone has to create the data files also.
.EI
.SH Includes
.SP 5 5
This class includes the following files /include/function.h and /include/data.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^compile_data%^RESET%^
class compile_data {
string file_name;
function call_back;
int current_line;
int look_for;
int last_chunk_compile;
int file_len;
class queue_node * depths;
}

.EI

.SI 3
* %^BOLD%^data_node%^RESET%^
class data_node {
int type;
mixed value;
}

.EI

.SI 3
* %^BOLD%^queue_node%^RESET%^
class queue_node {
string name;
class data_node data;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^compile_data%^RESET%^
.EI
.SI 5
mixed compile_data(string * path)
.EI
.SP 7 5

Actualy compiles the files down.
See the header file for a more detailed explaination
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
path - the files to parse

.EP

.SI 3
* %^BOLD%^compile_file%^RESET%^
.EI
.SI 5
void compile_file(string fname, function call_back)
.EI
.SP 7 5

Compiles up the file into the useful soul format.   It also tells
the soul about it.


See the soul data files in /save/new_soul for details of the format for the soul files. The file has to come from the soul save directory or the call will not work.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the name of the file to compile

.EP

.SI 3
* %^BOLD%^to_diceint%^RESET%^
.EI
.SI 5
mixed to_diceint(string str)
.EI
.SP 7 5

This will return a normal integer, or a dice class.  The dice class
allows for things of the form 5d6+10 or 3d11-10.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the dice class

.EP


