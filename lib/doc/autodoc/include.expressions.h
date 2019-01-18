.DT
expressions.h
Disk World autodoc help
expressions.h

.SH Description
.SP 5 5

The include file to go with the expression handler.  You may need to use
the define EXPRESSION_NO_CLASSES to stop clashes occuring from includeing
this file into something which inherits the basic expressions file.
.EP
.SP 10 5


Written by Pinkfish

Started Mon May  8 17:01:14 PDT 2000

.EP



.SH See also
.SP 5 5
/std/basic/expresssions.c
.EP
.SH Classes

.SI 3
* %^BOLD%^parse_node%^RESET%^
class parse_node {
int type;
mixed value;
mixed * tree;
}

.EI
.SP 7 5

The class used to hold information about both expression nodes and
evaluated data.  The type is one of the types above and the value is
dependant on the type.
.EP


.SH Defines
.SI 3
* %^BOLD%^EXPRESSION_TYPE_ARRAY%^RESET%^
.EI
.SP 7 5
An opaque array type, cannot get at the internal bits. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_ARRAY_NULL%^RESET%^
.EI
.SP 7 5
A null array.  This is typeless, sort of. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_ARRAY_OFFSET%^RESET%^
.EI
.SP 7 5
This is the amount added to a type to get the array equivilant of it. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_BOOLEAN%^RESET%^
.EI
.SP 7 5
A boolean type. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_ERROR%^RESET%^
.EI
.SP 7 5
The error type. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_FLOAT%^RESET%^
.EI
.SP 7 5
A float type. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_FUNCTION%^RESET%^
.EI
.SP 7 5
A functionm type. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_FUNCTION_VARIABLE%^RESET%^
.EI
.SP 7 5
A function arguement type. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_INTEGER%^RESET%^
.EI
.SP 7 5
An integer type. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_MAPPING%^RESET%^
.EI
.SP 7 5
A mapping type. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_MONEY%^RESET%^
.EI
.SP 7 5
A money type. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_OBJECT%^RESET%^
.EI
.SP 7 5
An object type. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_OPERATOR%^RESET%^
.EI
.SP 7 5
An operator type. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_SPECIAL%^RESET%^
.EI
.SP 7 5

A special type, this is type (and above this) that should be used by
the inheriables if they wish to define special types of controls.

.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_STRING%^RESET%^
.EI
.SP 7 5
A string type. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_USER_FUNCTION%^RESET%^
.EI
.SP 7 5
A user defined function. 
.EP

.SI 3
* %^BOLD%^EXPRESSION_TYPE_VARIABLE%^RESET%^
.EI
.SP 7 5
A variable type. 
.EP

