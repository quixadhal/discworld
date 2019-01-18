.DT
craft_shop.h
Disk World autodoc help
craft_shop.h

.SH Description
.SP 5 5

This is the include file for the craft shop inheritable.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Sep 10 21:13:26 PDT 1998

.EP

.SH Defines
.SI 3
* %^BOLD%^CRAFT_CATEGORY_CHOICE_ABSTAIN%^RESET%^
.EI
.SP 7 5

This is the argument for a abstain vote.

.EP

.SI 3
* %^BOLD%^CRAFT_CATEGORY_CHOICE_NO%^RESET%^
.EI
.SP 7 5

This is the argument for a no vote.

.EP

.SI 3
* %^BOLD%^CRAFT_CATEGORY_CHOICE_YES%^RESET%^
.EI
.SP 7 5

This is the arguement for a yes vote.

.EP

.SI 3
* %^BOLD%^CRAFT_CATEGORY_ERROR%^RESET%^
.EI
.SP 7 5

This is the value returned as an error from methods that normally return
nice happy values.

.EP

.SI 3
* %^BOLD%^CRAFT_CATEGORY_STATE_ACCEPTED%^RESET%^
.EI
.SP 7 5

The state of the category when it has been accepted and is happy about
it.

.EP

.SI 3
* %^BOLD%^CRAFT_CATEGORY_STATE_DELETEING%^RESET%^
.EI
.SP 7 5

The state of the category when it is being setup for deletion.

.EP

.SI 3
* %^BOLD%^CRAFT_CATEGORY_STATE_NOMINATING%^RESET%^
.EI
.SP 7 5

The state of the category when it is being nominated as a new
position.

.EP

.SI 3
* %^BOLD%^CRAFT_CATEGORY_STATE_VOTING%^RESET%^
.EI
.SP 7 5

The state of the category when it is being voted into a real category
position.

.EP

.SI 3
* %^BOLD%^CRAFT_SHOP_CONTROLLER%^RESET%^
.EI
.SP 7 5

The path of the craft shop controller.

.EP

.SI 3
* %^BOLD%^CRAFT_SHOP_DATA_SAVE_FILE%^RESET%^
.EI
.SP 7 5

This means the individual shop item is being saved.
This is the control type for the save/load functions. 

.EP

.SI 3
* %^BOLD%^CRAFT_SHOP_EXPRESSION_HELP_DIR%^RESET%^
.EI
.SP 7 5

This is the directory to use to get help on the function names.

.EP

.SI 3
* %^BOLD%^CRAFT_SHOP_INHERIT%^RESET%^
.EI
.SP 7 5

The path of the craft shop inheritable.

.EP

.SI 3
* %^BOLD%^CRAFT_SHOP_MAIN_SAVE_FILE%^RESET%^
.EI
.SP 7 5

This means the main data file is being save.
This is the control type for the save/load functions. 

.EP

.SI 3
* %^BOLD%^CRAFT_SHOP_MAX_NAME_LENGTH%^RESET%^
.EI
.SP 7 5

This method defines the maximum number of letters a name in a 
craft shop can be.

.EP

.SI 3
* %^BOLD%^CRAFT_SHOP_REMOVE_DATA_SAVE_FILE%^RESET%^
.EI
.SP 7 5

This means the individual shop item is being removed because it is no
longer used.  This will only be passed to the save function.
This is the control type for the save/load functions. 

.EP

