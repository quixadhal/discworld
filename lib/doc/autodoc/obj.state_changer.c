.DT
state_changer.c
Dead DW2 autodoc help
state_changer.c

.SH Description
.SP 5 5

State Changer Inheritable!
You can use this to write a device which will change the states of
things.
.EP
.SP 10 5


Written by Terano

Started 30/9/02

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/vessel.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/effect.h and /include/tasks.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^beginning%^RESET%^
.EI
.SI 5
object beginning(object apparatus, object tp, int id)
.EI
.SP 7 5

Some effect stuff to save on call outs.

.EP

.SI 3
* %^BOLD%^do_action%^RESET%^
.EI
.SI 5
int do_action(object * obs)
.EI

.SI 3
* %^BOLD%^do_empty%^RESET%^
.EI
.SI 5
int do_empty(object * dest, string me, string him, string prep)
.EI

.SI 3
* %^BOLD%^do_fill%^RESET%^
.EI
.SI 5
int do_fill(object * to, mixed * args_b, mixed * args_a, mixed * args)
.EI

.SI 3
* %^BOLD%^end%^RESET%^
.EI
.SI 5
object end(object apparatus, object tp, int id)
.EI

.SI 3
* %^BOLD%^extra_look%^RESET%^
.EI
.SI 5
string extra_look(object ob)
.EI
.SP 7 5

Something to mask!

.EP

.SI 3
* %^BOLD%^long%^RESET%^
.EI
.SI 5
string long(string word, int dark)
.EI

.SI 3
* %^BOLD%^parse_message%^RESET%^
.EI
.SI 5
string parse_message(string message)
.EI

.SI 3
* %^BOLD%^query_busy%^RESET%^
.EI
.SI 5
int query_busy()
.EI

.SI 3
* %^BOLD%^query_classification%^RESET%^
.EI
.SI 5
string query_classification()
.EI

.SI 3
* %^BOLD%^set_abort_message%^RESET%^
.EI
.SI 5
void set_abort_message(string message)
.EI

.SI 3
* %^BOLD%^set_command%^RESET%^
.EI
.SI 5
void set_command(string _command)
.EI

.SI 3
* %^BOLD%^set_command_mess%^RESET%^
.EI
.SI 5
void set_command_mess(string message)
.EI

.SI 3
* %^BOLD%^set_duration%^RESET%^
.EI
.SI 5
void set_duration(int _duration)
.EI

.SI 3
* %^BOLD%^set_end_func%^RESET%^
.EI
.SI 5
void set_end_func(function f)
.EI

.SI 3
* %^BOLD%^set_end_message%^RESET%^
.EI
.SI 5
void set_end_message(string message)
.EI

.SI 3
* %^BOLD%^set_fail_func%^RESET%^
.EI
.SI 5
void set_fail_func(function f)
.EI

.SI 3
* %^BOLD%^set_fail_message%^RESET%^
.EI
.SI 5
void set_fail_message(string message)
.EI

.SI 3
* %^BOLD%^set_fail_process%^RESET%^
.EI
.SI 5
void set_fail_process(string _process)
.EI

.SI 3
* %^BOLD%^set_gp_cost%^RESET%^
.EI
.SI 5
void set_gp_cost(int gp)
.EI

.SI 3
* %^BOLD%^set_gp_type%^RESET%^
.EI
.SI 5
void set_gp_type(string type)
.EI

.SI 3
* %^BOLD%^set_interim_message%^RESET%^
.EI
.SI 5
void set_interim_message(string message)
.EI

.SI 3
* %^BOLD%^set_process%^RESET%^
.EI
.SI 5
void set_process(string _process)
.EI

.SI 3
* %^BOLD%^set_running_inv_mess%^RESET%^
.EI
.SI 5
void set_running_inv_mess(string mess)
.EI

.SI 3
* %^BOLD%^set_skill%^RESET%^
.EI
.SI 5
void set_skill(string _skill, int _base_bonus)
.EI

.SI 3
* %^BOLD%^set_start_func%^RESET%^
.EI
.SI 5
void set_start_func(function f)
.EI

.SI 3
* %^BOLD%^set_start_message%^RESET%^
.EI
.SI 5
void set_start_message(string message)
.EI

.SI 3
* %^BOLD%^show_message%^RESET%^
.EI
.SI 5
object show_message(object apparatus, object tp, int id)
.EI

.SI 3
* %^BOLD%^test_add%^RESET%^
.EI
.SI 5
int test_add(object ob, int flag)
.EI

.SI 3
* %^BOLD%^test_remove%^RESET%^
.EI
.SI 5
int test_remove(object ob, int flag, mixed dest)
.EI


