
/*
** Creator Interface to the Playtesters' Palace Armoury, where PTs can request
** items which are in playtesting.
**
** @author Arienne
*/

inherit "/cmds/base";

#define PT_ARMOURY "/d/playtesters/palace_rooms/armoury"

mixed *query_patterns() {
    return ({ "info",
                  (: PT_ARMOURY->do_info() :),

              "help",
                  (: PT_ARMOURY->do_info() :),

              "list",
                  (: PT_ARMOURY->do_list_all() :),

              "list commands",
                  (: PT_ARMOURY->do_list_commands() :),

              "list objects",
                  (: PT_ARMOURY->do_list_commands() :),

              "control",
                  (: PT_ARMOURY->do_list_control_all() :),

              "control commands",
                  (: PT_ARMOURY->do_list_control_commands() :),

              "control objects",
                  (: PT_ARMOURY->do_list_control_commands() :),

              "request <string:'item'>",
                  (: PT_ARMOURY->do_request_object( lower_case( $4[ 0 ] ) ) :),
                  
              "add object <word:'path of object'> as <string:'item name'>",
                  (: PT_ARMOURY->do_add_object( $4[ 0 ],
                                                lower_case( $4[ 1 ] ) ) :),

              "remove object <string:'item name'>",
                  (: PT_ARMOURY->do_remove_object( $4[ 0 ] ) :),
                  
              "allow <word:'guilds'> to use object <string:'item name'>",
                  (: PT_ARMOURY->do_allow_object( $4[ 0 ],
                                                  lower_case( $4[ 1 ] ) ) :),
    
              "add command <string:'command name'>",
                  (: PT_ARMOURY->do_add_command( $4[ 0 ] ) :),

              "remove command <string:'command name'>",
                  (: PT_ARMOURY->do_remove_command( $4[ 0 ] ) :),

              "allow <word:'guilds'> to use command <string:'command name'>",
                  (: PT_ARMOURY->do_allow_command( $4[ 0 ], $4[ 1 ] ) :)
                  
              });
} /* query_patterns() */

