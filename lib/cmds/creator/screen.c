/**
 * ScreenWrite Command. 
 * Used to send blocks to text to other creators, in a more
 * readable fashion.
 * Written by Terano, on 14th September, 1998
 */

inherit "/cmds/base";

object target;

int cmd(object player)
{

   target = player;

   if (target == this_player())
      return notify_fail("You start to send text to yourself, "
                         "but think better of it.\n");

/*
   if ( !target->query_creator() && !this_player()->query_lord() ) 
        return notify_fail( "There is really no need to echo text to "
          + target->query_short() +", is there?\n" ); 
*/

   if (target->check_earmuffs("remote-spam")) {
      tell_object(target, sprintf("%s tried to screen some text to you.\n",
                                  this_player()->query_short()));

      return notify_fail(target->query_short() +
                         " currently has " + target->query_possessive() +
                         " screen earmuffs on, ask " +
                         target->query_possessive() + " to turn "
                         "them off.\n");
   }

   tell_object(target, sprintf("Receiving text from %s.\n",
                               this_player()->query_short()));

   tell_object(this_player(),
               sprintf("Sending text to %s. Use ** to stop.\n]",
                       target->query_short()));

   input_to("do_screen");
   return 1;
}

void do_screen(string spam)
{

   if (spam == "**") {
      tell_object(target,
                  sprintf("Stopped receiving text from %s.\n",
                          this_player()->query_short()));

      tell_object(this_player(),
                  sprintf("Stopped sending text to %s.\n",
                          target->query_short()));
      return;
   }

   if (spam) {
      tell_object(target, "] " + spam + "\n");
      write("] ");
   }
   input_to("do_screen");

}

int screen_toggle(string toggle)
{

   if (toggle == "on") {
      if (member_array("remote-spam",
                       this_player()->query_property("earmuffs")) == -1)
         return notify_fail("You aren't earmuffing remote-spam events!\n");

      this_player()->add_property("earmuffs",
                                  (this_player()->query_property("earmuffs") -
                                   ({ "remote-spam" })));
      write("Remote-spam earmuffs off.\n");
      return 1;
   }

   if (toggle == "off") {
      if (member_array("remote-spam",
                       this_player()->query_property("earmuffs")) != -1)
         return
            notify_fail("You are already earmuffing remote-spam events!\n");

      this_player()->add_property("earmuffs",
                                  (this_player()->query_property("earmuffs") +
                                   ({ "remote-spam" })));
      write("Remote-spam earmuffed.\n");
      if (!this_player()->query_earmuffs())
         write("Remeber to \"earmuff on\" to enable them.\n");
      return 1;
   }

}
mixed *query_patterns()
{
   return ({ "<indirect:player>", (: cmd($1[0]) :),
             "{on|off}", (: screen_toggle($4[0]) :) });
}
