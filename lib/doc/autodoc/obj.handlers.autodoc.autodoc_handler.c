.DT
autodoc_handler.c
Disk World autodoc help
autodoc_handler.c

.SH Description
.SP 5 5

Documentation system for removing documentation from source files.


The documentation will be created in the directory /doc/autodoc. Extra documentation should be placed into the other help directories and will be accessed via the help system. The html documentation will be placed into the /www/autodoc directory. The files in theses directories will be named with the '/'s replaced with '.'s. ie: /obj/handlers/room_handler.c would be in the file /doc/autodoc/obj.handlers.room_handler.c and /www/autodoc/obj.handlers.room_handler.c.html. An index of the methods and object names will be generated from the input files and linked into the help system. An alphabetical index will also be produced that is linked to the html stuff. 

The first comment which follows the comment format below and occurs right before a function will be used as the documentation for that function. The comment must start with two '*'s to distinguish it from a normal comment. A special tag at the start of one of the first comments in the file will signify that it is a comment for the class. 

Only comments before a real function declaration will be used, comments before a predeclaration will be ignored. 

Any function which is not declared as private and does not have the @ignore tag in its description will be placed into the resulting documentation file. This will be done even if no comment was found. 

In the text part of the comment the main section is a free form documentation, it is merely a description of the whole function. After this several tags can be added to the comment which allows other things to be documented. All the text after the tag is taken as part of that tags information. The main tag should never actually be used, it is implied as being the tag associated with the first section of comment. 

HTML tags will be used in the comments to allow certain things to happen. The tags 'p', 'b', 'i', 'code', 'ul', 'ol', 'dl', 'dd', 'dt', 'pre' and 'li' will be supported. 

The sequences '/', '\' and '\>' will be processed by the comment
handling code so that they do not get placed into the output code.


White space is ignored... Both html and dw nroff format files will be created from the documentation maker. 

The supported tags are: 
.EP
.SP 5 5
@param 
.EP
.SP 8 5
Specify the parameter and what it does. This should be one short sentance only. There needs to be one of these for every parameter, the first word of the sentance is the name of the paramater and the rest is the descrption. There should be no full stop on the end of the sentance. 
.EP
.SP 5 5
@member 
.EP
.SP 8 5
This is used with classes and defines the members of the class. The definition is the same as for the @param element above. 
.EP
.SP 5 5
@return 
.EP
.SP 8 5
Specify what the return value is. This should be one short sentance only. Theres should only be one of these in every function. There should be no full stop on the end of the sentance. 
.EP
.SP 5 5
@see 
.EP
.SP 8 5
Specify an object or function to see. The format must be one of: 
.EP
.SO 9 2 -13

	*	object_name
.EO
.SO 9 2 -13
		%^CYAN%^@see /global/new_parse%^RESET%^
	*	function_name()
.EO
.SO 9 2 -13
		%^CYAN%^@see frog()%^RESET%^
	*	object_name->function_name()
.EO
.SO 9 2 -13
		%^CYAN%^@see /global/new_parse->add_command()%^RESET%^
	*	efun::name()
.EO
.SO 9 2 -13
		%^CYAN%^@see efun::sort_array()%^RESET%^
	*	help::name
.EO
.SO 9 2 -13
		@see help::effects 
.EO
.SP 5 5

.EP
.SP 5 5
@ignore 
.EP
.SP 8 5
Do not document this function, the whole comment is ignored. 
.EP
.SP 5 5
@main 
.EP
.SP 8 5
The main documentation section. 
.EP
.SP 5 5
@classification 
.EP
.SP 8 5
Used in the effects files to classifiy the effect. So a tree of the classifications can be derived. 
.EP
.SP 5 5
@index 
.EP
.SP 8 5
Adds in a new index refernece on this name. This should be one word only. 
.EP
.SP 5 5
@change 
.EP
.SP 8 5
Placed in the class documentation to reference a change list. First line after the tag is the date and the author and then follows a description of the change. 
.EP
.SP 5 5
@example 
.EP
.SP 8 5
If you wish to embed an example in the code. 
.EP
.SP 5 5
@started 
.EP
.SP 8 5
When the file was started. 
.EP
.SP 5 5
@author 
.EP
.SP 8 5
The author of the file. 
.EP
.SP 5 5
The output format will look something like the API documentation for java. If you have not seen this before, go to http://www.javasoft.com and look in the JDK and then the API section. 

Comment format: 
.EP
.SP 5 5
/**
.EP
.SP 5 5
 *  <text>
.EP
.SP 5 5
 *    ..
.EP
.SP 5 5
 *  <text>
.EP
.SP 5 5
 *
.EP
.SP 5 5
 */
.EP
.SP 5 5
ie: it starts with a /** and ends with a */ on a line by themselves and every intervening line has a * at the start. By using a \ you can make characters escaped. This means they are not processed by the internal system, so to make a end comment appear inside a the text you go : */ 
.EP
.SP 10 5


Written by Pinkfish

Started Tue Oct 28 13:25:09 EST 1997


.EP



.SH See also
.SP 5 5
/obj/handlers/autodoc/autodoc_file, /obj/handlers/autodoc/autodoc_nroff and /obj/handlers/autodoc/autodoc_html
.EP
.SH Example
.SI 5
inherit "/std/object";

/**
 *   This class is a nice shade of blue and allows all other shades of
 *   green and yellow to mix well.
 * @see yellow
 * @see blue
 * @change 12/3/97 Pinkfish
 *    Pushed some of the yellow stuff down a hole and ate a bagel.
 */

/**
 * This method mixes the green in with our blue.
 * @param  green   the green object to mix.
 */
void mixGreen(object green) {
  /* Something wild and exciting! */
} /* mixGreen() */

/**
 * This method mixes the yellow in with our blue.
 * @param  yellow the yellow object to mix.
 * @return returns a TRUE or FALSE depending on if the mix succeeded.
 * @example
 *   mixYellow(12);
 */
int mixYellow(object yellow) {
} /* mixYellow() */

.EI
.SH Includes
.SP 5 5
This class includes the following files /include/autodoc.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_file%^RESET%^
.EI
.SI 5
int add_file(string fname)
.EI
.SP 7 5

Adds a file into the list of files to process for autodocumentation.
This file will be processed every time around in the autodocumentation
loop to check to see if it has been changed and the results will be
placed into the index.  It automaticly calls recreate_documentation
in random(60) seconds.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the name of the file to add

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the add succeeds, 0 otherwise.

.EP

.SI 3
* %^BOLD%^is_autodoc_file%^RESET%^
.EI
.SI 5
int is_autodoc_file(string name)
.EI
.SP 7 5

This method tells us if the file is currently in the autodoc set.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the file is found, 0 if not

.EP

.SI 3
* %^BOLD%^query_files%^RESET%^
.EI
.SI 5
string * query_files()
.EI
.SP 7 5

Returns the list of files we are current processing.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of strings being the file names

.EP

.SI 3
* %^BOLD%^query_help_map%^RESET%^
.EI
.SI 5
mapping query_help_map()
.EI
.SP 7 5

Returns the help mapping.  This is the mapping from function names to
files.  Each element in the mapping referes to an array of file
names which contain the function or define.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping of arrays of files

.EP

.SI 3
* %^BOLD%^query_help_on%^RESET%^
.EI
.SI 5
string query_help_on(string file, string func)
.EI
.SP 7 5

This method returns the file name of the help for the specified
function in the specified file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
file - the file name to find the help in
.EP
.SP 9 5
func - the function to look for help on in the file
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the full path to the help file, 0 if it does not exist

.EP

.SI 3
* %^BOLD%^query_summary_map%^RESET%^
.EI
.SI 5
mapping query_summary_map()
.EI
.SP 7 5

Retuirns the mapping of file names to a summary.  This is only
valid during the index creation cycle of the documentation generation
system.  It is used to create a more useful index page.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of file names to summarys

.EP

.SI 3
* %^BOLD%^recreate_documentation%^RESET%^
.EI
.SI 5
int recreate_documentation(string fname)
.EI
.SP 7 5

Recreate documentation for one fiel immediately.  This does not update
the index, it merely creates the base files for this file.  It does it
at a random delay of up to 30 seconds.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the file name to update

.EP

.SI 3
* %^BOLD%^recreate_indexes%^RESET%^
.EI
.SI 5
void recreate_indexes()
.EI
.SP 7 5

Regenerate the index files.

.EP

.SI 3
* %^BOLD%^remove_file%^RESET%^
.EI
.SI 5
int remove_file(string fname)
.EI
.SP 7 5

Removes a file from the list of files to be processed for autodocumentation.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the name of the file to remove

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the remove succeeds, 0 otherwise.

.EP


