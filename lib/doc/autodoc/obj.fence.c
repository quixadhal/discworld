.DT
fence.c
DW_wibble autodoc help
fence.c

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/monster.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/money.h, /include/armoury.h, /include/move_failures.h and /include/shops/bank.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^hist_offer%^RESET%^
class hist_offer {
                   object ob;
                   int old_offer;
                   int value;
}

.EI

.SI 3
* %^BOLD%^offer%^RESET%^
class offer {
              object who;
              int amount;
              int offer_time;
              object * objects;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^attack_by%^RESET%^
.EI
.SI 5
int attack_by(object ob)
.EI

.SI 3
* %^BOLD%^busy%^RESET%^
.EI
.SI 5
int busy()
.EI

.SI 3
* %^BOLD%^calc_place%^RESET%^
.EI
.SI 5
string calc_place()
.EI

.SI 3
* %^BOLD%^check_cont%^RESET%^
.EI
.SI 5
void check_cont()
.EI

.SI 3
* %^BOLD%^cost_str%^RESET%^
.EI
.SI 5
string cost_str(int amt)
.EI

.SI 3
* %^BOLD%^cost_string%^RESET%^
.EI
.SI 5
string cost_string(object thing, string place, object buyer)
.EI

.SI 3
* %^BOLD%^do_fence%^RESET%^
.EI
.SI 5
int do_fence(object * in_dir, string direct, string indirect, mixed args, string format)
.EI

.SI 3
* %^BOLD%^do_no%^RESET%^
.EI
.SI 5
void do_no(object player)
.EI

.SI 3
* %^BOLD%^do_yes%^RESET%^
.EI
.SI 5
void do_yes(object person)
.EI

.SI 3
* %^BOLD%^event_exit%^RESET%^
.EI
.SI 5
void event_exit(object ob, string message, object to)
.EI

.SI 3
* %^BOLD%^give_back%^RESET%^
.EI
.SI 5
void give_back()
.EI

.SI 3
* %^BOLD%^judge_value%^RESET%^
.EI
.SI 5
int judge_value(object ob, string type)
.EI

.SI 3
* %^BOLD%^query_cont%^RESET%^
.EI
.SI 5
object query_cont()
.EI

.SI 3
* %^BOLD%^query_cost%^RESET%^
.EI
.SI 5
int query_cost(object thing, object buyer)
.EI

.SI 3
* %^BOLD%^query_current_offer%^RESET%^
.EI
.SI 5
string query_current_offer()
.EI

.SI 3
* %^BOLD%^query_item_type%^RESET%^
.EI
.SI 5
string query_item_type(object ob)
.EI

.SI 3
* %^BOLD%^scaled_value%^RESET%^
.EI
.SI 5
int scaled_value(int n)
.EI

.SI 3
* %^BOLD%^set_fence_type%^RESET%^
.EI
.SI 5
void set_fence_type(string str)
.EI

.SI 3
* %^BOLD%^whisper%^RESET%^
.EI
.SI 5
void whisper(object ob, string message)
.EI


