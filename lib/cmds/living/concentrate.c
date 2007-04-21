inherit "/cmds/base";

int concentrate_list( ) {
   object conc;

   conc = this_player()->query_concentrating();
   if ( !conc ) {
      write("You are not currently concentrating on "
                  "any particular opponent.\n" );
   } else {
      write("You are currently concentrating on "+
            conc->the_short() + ".\n");
   }
   return 1;
} /* concentrate_list() */

int concentrate(object *things ) {
   if ( sizeof( things ) > 1 ) {
      return notify_fail( "You can only concentrate on one thing at once.\n" );
   }
   if (!sizeof(this_player()->query_attacker_list())) {
      add_failed_mess("You are not fighting anyone.\n");
      return 0;
   }
   if ( things[0] == this_player() ) {
      return notify_fail( "It would be silly to concentrate on yourself.\n" );
   }
   if (member_array(things[0], this_player()->query_attacker_list()) == -1) {
     return notify_fail("You can only concentrate on people you are "
                        "fighting.\n");
   }
   if (this_player()->set_concentrating(things[0])) {
        add_succeeded_mess(({"You are now concentrating on $I.\n", "" }),
                           things);
   } else {
      add_succeeded_mess(({"Unable to concentrate on $I.\n", "" }), things);
   }
   return 1;
} /* concentrate() */

mixed *query_patterns() {
   return ({ "[on] <indirect:living>", (: concentrate($1) :),
             "", (: concentrate_list() :) });
} /* query_patterns() */
