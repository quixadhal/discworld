/**
 * The include file to go with the expression handler.  You may need to use
 * the define EXPRESSION_NO_CLASSES to stop clashes occuring from includeing
 * this file into something which inherits the basic expressions file.
 * @see /std/basic/expresssions.c
 * @author Pinkfish
 * @started Mon May  8 17:01:14 PDT 2000
 */
#ifndef __EXPRESSIONS_H
/** @ignore yes */
#define __EXPRESSIONS_H

/** An integer type. */
#define EXPRESSION_TYPE_INTEGER  1
/** A string type. */
#define EXPRESSION_TYPE_STRING   2
/** An opaque array type, cannot get at the internal bits. */
#define EXPRESSION_TYPE_ARRAY    3
/** A mapping type. */
#define EXPRESSION_TYPE_MAPPING  4
/** A float type. */
#define EXPRESSION_TYPE_FLOAT    5
/** An operator type. */
#define EXPRESSION_TYPE_OPERATOR 6
/** A boolean type. */
#define EXPRESSION_TYPE_BOOLEAN  7
/** A variable type. */
#define EXPRESSION_TYPE_VARIABLE 8
/** A money type. */
#define EXPRESSION_TYPE_MONEY    9
/** A functionm type. */
#define EXPRESSION_TYPE_FUNCTION 10
/**
 * A special type, this is type (and above this) that should be used by
 * the inheriables if they wish to define special types of controls.
 */
#define EXPRESSION_TYPE_SPECIAL  11
/** A function arguement type. */
#define EXPRESSION_TYPE_FUNCTION_VARIABLE 12
/** A user defined function. */
#define EXPRESSION_TYPE_USER_FUNCTION 13
/** An object type. */
#define EXPRESSION_TYPE_OBJECT   14
/** A null array.  This is typeless, sort of. */
#define EXPRESSION_TYPE_ARRAY_NULL   15
/** This is the amount added to a type to get the array equivilant of it. */
#define EXPRESSION_TYPE_ARRAY_OFFSET  20
/** The error type. */
#define EXPRESSION_TYPE_ERROR    -1

#ifndef EXPRESSION_NO_CLASSES
/**
 * The class used to hold information about both expression nodes and
 * evaluated data.  The type is one of the types above and the value is
 * dependant on the type.
 * @element type the type of the data
 * @element value the value of the data
 * @element tree if we need to make a tree for short circuit evaluation
 */
class parse_node {
   int type;
   mixed value;
   mixed* tree;
}
#endif

#endif
