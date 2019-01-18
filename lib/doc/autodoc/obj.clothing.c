.DT
clothing.c
Disk World autodoc help
clothing.c

.SH Description
.SP 5 5

This is the clothing file.  COntains everything you need to
make some nice clothing.

.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/wearable, /std/basic/close_lock and /std/container.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/virtual.h and /include/move_failures.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_pocket%^RESET%^
.EI
.SI 5
void add_pocket(string type, int amount)
.EI
.SP 7 5

This method adds a pocket onto the clothing.


When setting the amount a pocket can hold use the following guide: 
.EP
.SP 7 5
o==================o==============o========o
.EP
.SP 7 5
| Type of Clothing | example type | amount |
.EP
.SP 7 5
o==================o==============o========o
.EP
.SP 7 5
|   Aprons         |    front     |  8-10  |
.EP
.SP 7 5
|------------------+--------------+--------|
.EP
.SP 7 5
|   Coats          |     side     |  6-7   |
.EP
.SP 7 5
|                  |    inside    |   5    |
.EP
.SP 7 5
|------------------+--------------+--------|
.EP
.SP 7 5
|   Corsets        |   cleavage   |   2    |
.EP
.SP 7 5
|------------------+--------------+--------|
.EP
.SP 7 5
|   Dresses        |     side     |  2-3   |
.EP
.SP 7 5
|------------------+--------------+--------|
.EP
.SP 7 5
|   Hats           |    inside    |  2-6   |
.EP
.SP 7 5
|------------------+--------------+--------|
.EP
.SP 7 5
|   Jackets        |     side     |  2-3   |
.EP
.SP 7 5
|                  |    inside    |  2-3   |
.EP
.SP 7 5
|------------------+--------------+--------|
.EP
.SP 7 5
|   Robes          |     side     |   5    |
.EP
.SP 7 5
|                  |    sleeve    |   2    |
.EP
.SP 7 5
|------------------+--------------+--------|
.EP
.SP 7 5
|   Shirts         |    breast    |  2-3   |
.EP
.SP 7 5
|------------------+--------------+--------|
.EP
.SP 7 5
|   Skirts         |     side     |   4    |
.EP
.SP 7 5
|------------------+--------------+--------|
.EP
.SP 7 5
|   Trousers       |     side     |   4    |
.EP
.SP 7 5
|                  |     back     |   2    |
.EP
.SP 7 5
|------------------+--------------+--------|
.EP
.SP 7 5
|   Underwear      |    front     |  1-2   |
.EP
.SP 7 5
|                  |    breast    |  1-2   |
.EP
.SP 7 5
|                  |   cleavage   |   2    |
.EP
.SP 7 5
o==================o==============o========o
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of pocket
.EP
.SP 9 5
amount - the amount the pocket can hold
.EP

.SI 3
* %^BOLD%^break_me%^RESET%^
.EI
.SI 5
void break_me()
.EI
.SP 7 5

This method causes the object to be broken.

.EP

.SI 3
* %^BOLD%^query_clothing%^RESET%^
.EI
.SI 5
int query_clothing()
.EI
.SP 7 5

This method will return true if the item is an item of clothing.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns true

.EP

.SI 3
* %^BOLD%^query_is_pair%^RESET%^
.EI
.SI 5
int query_is_pair()
.EI

.SI 3
* %^BOLD%^query_objective%^RESET%^
.EI
.SI 5
string query_objective()
.EI

.SI 3
* %^BOLD%^query_pocket_mess%^RESET%^
.EI
.SI 5
string query_pocket_mess()
.EI
.SP 7 5

This method returns the message associated with the pocket.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the message associated with the pocket
.EP

.SI 3
* %^BOLD%^query_pockets%^RESET%^
.EI
.SI 5
mixed * query_pockets()
.EI
.SP 7 5

This method returns all the pockets on the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the pockets on the object
.EP

.SI 3
* %^BOLD%^query_pronoun%^RESET%^
.EI
.SI 5
string query_pronoun()
.EI

.SI 3
* %^BOLD%^remove_pockets%^RESET%^
.EI
.SI 5
void remove_pockets()
.EI
.SP 7 5

This method removes all pockets from the clothing.
.EP

.SI 3
* %^BOLD%^replace_me%^RESET%^
.EI
.SI 5
void replace_me()
.EI

.SI 3
* %^BOLD%^set_is_pair%^RESET%^
.EI
.SI 5
void set_is_pair(int comes_in_pairs)
.EI

.SI 3
* %^BOLD%^set_pocket_mess%^RESET%^
.EI
.SI 5
void set_pocket_mess(string words)
.EI
.SP 7 5

This method sets the message associated with the pocket.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the message associated with the pocket
.EP

.SI 3
* %^BOLD%^setup_clothing%^RESET%^
.EI
.SI 5
void setup_clothing(int number)
.EI
.SP 7 5

This method sets up the condition for the clothing.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the maximum condition for the clothing

.EP


