.DT
peopler.c
Disk World autodoc help
peopler.c

.SH Description
.SP 5 5

This prints out nicely formated lists of information.  It ius used
by the people, finger etc commands.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/peopler.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_command%^RESET%^
.EI
.SI 5
int do_command(mixed * pattern, string constraint, function sort_func, int only_duplicates)
.EI
.SP 7 5

This method is the main access point to the peopler.  It prints out
the commands.  The optional sort function allows you to sort on 
somethign other than the name of the player.


The format of the pattern string is an array with every second element being the type and the other element being the width of the printable string. 
.EP
.SP 7 5
   ({ type, width,  ... })
.EP
.SP 7 5
With strings the width is the string...
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pattern - the pattern to print with
.EP
.SP 9 5
constraint - All the names should start with this, 0 fo rnone
.EP
.SP 9 5
sort_func - the function to sort with (optional)
.EP
.SP 9 5
only_duplicates - only print entries whicxh are the same (using the
        sort_func
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success
.EP

.SI 3
* %^BOLD%^get_people%^RESET%^
.EI
.SI 5
object * get_people(string str)
.EI
.SP 7 5

This method returns the list of people using the 'str' as a 
constraint.  So it only shows the people who match with the
str.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the constraint string
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of people

.EP

.SI 3
* %^BOLD%^review%^RESET%^
.EI
.SI 5
int review()
.EI
.SP 7 5

The review command.  Prints out the current settings for
the commands.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the player name restriction string
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if it failed, 1 if it succeeded
.EP

.SI 3
* %^BOLD%^set_var%^RESET%^
.EI
.SI 5
int set_var(string str)
.EI
.SP 7 5

Sets a variable.  Sets the variable to the specified value.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the - name and arg of the varible
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP


