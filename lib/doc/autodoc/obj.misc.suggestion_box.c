.DT
suggestion_box.c
Disk World autodoc help
suggestion_box.c

.SH Description
.SP 5 5

.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_fill%^RESET%^
.EI
.SI 5
int do_fill()
.EI
.SP 7 5

.EP

.SI 3
* %^BOLD%^end_suggestion%^RESET%^
.EI
.SI 5
void end_suggestion(string text)
.EI
.SP 7 5

.EP

.SI 3
* %^BOLD%^query_save_dir%^RESET%^
.EI
.SI 5
string query_save_dir()
.EI
.SP 7 5

This function can be used to query what save directory is in use for the
suggestion box.  Either a save file *or* a save directory is required.
or your suggestions will not save.  If both are set then the save file
will be used.  If a save dir is used, the file name will be in the form of
save_dir/player_name-time.txt
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the full path to the file where the suggestions are kept
.EP

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI
.SP 7 5

This function can be used to query what save file is in use for the
suggestion box.  Either a save file *or* a save directory is required.
or your suggestions will not save.  If both are set then the save file
will be used.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the full path to the file where the suggestions are kept
.EP

.SI 3
* %^BOLD%^set_save_dir%^RESET%^
.EI
.SI 5
void set_save_dir(string dir)
.EI
.SP 7 5

This function gives the suggestion box a directory to write its 
suggestions to. Either a save file *or* a save directory is required.
or your suggestions will not save.  If both are set then the save file
will be used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
string - dir The path of the directory to write suggestions to 
it had better not end in / or you will be sorry.
.EP

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string file)
.EI
.SP 7 5

This function gives the suggestion box a file to write its suggestions to
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
string - file The path of the directory to write suggestions to 
it had better end in / or you will be sorry.

.EP


