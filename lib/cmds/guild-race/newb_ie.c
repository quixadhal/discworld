/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: newb_ie.c,v 1.14 2003/04/03 11:07:32 wyvyrn Exp $
 */
#include <player.h>
#include <language.h>
#include <cmds/options.h>
#include <newbiehelpers.h>

#define HIST "/obj/handlers/hist_handler"

#define SPAM_PROPERTY "newbie_tell"
#define TIME_OUT     30

#define TIME  0
#define WORDS 1

inherit "/cmds/guild_base";
inherit "/cmds/speech";

int newbie_history(object who);

void create() {
   ::create();
   set_nroff_file("newbie");
   set_command_name("newbie");
} /* create() */

int cmd( string words ) {
   mixed *array;

   if (words == "history") {
      return newbie_history(this_player());    
   }
   
   if ( this_player()->query_mature() &&
        !this_player()->query_creator() &&
        !this_player()->query_property( "newbie helper" ) &&
        !NEWBIEHELPERS_HANDLER->query_can_chat(this_player()) ) {
      
      write( "You are no longer a newbie, being older than " + MATURITY +
             " hours.\n" );
      this_player()->remove_known_command( "newbie" );
      return 1;
   }

   if ( array = this_player()->query_property( SPAM_PROPERTY ) ) {
     if ( array[ WORDS ] == words ) {
       write( "You only said that a few seconds ago, why not wait for a "
         "response?\n" );
       return 1;
     }

     if ( time() < array[ TIME ] ) {
       write( "You only used the newbie channel a few seconds ago, why not "
         "wait for a response?\n" );
       return 1;
     }
   }

   if (this_player()->query_property("gagged")) { //  Added by Obilix 30/3/97
      write("You are gagged!  You will not be able to use the "
            "newbie channel until a creator removes your gag.\n");
      return 1;
   }

   if(this_player()->query_property(SHORTHAND_PROP))
     words = LANGUAGES->fix_shorthand(words);
   words = strip_colours (words); 

   write( "You newbie-tell: "+ words +"\n" );
   
   user_event("newbie", this_player()->query_cap_name() + ": "+words);
   
   HIST->add_chat_history("newbie", this_player()->query_name() + 
                          " newbie-told: ", words);

   if (!this_player()->query_creator() && 
       !this_player()->query_property( "newbie helper") &&
       !NEWBIEHELPERS_HANDLER->query_can_chat(this_player()) ) {
     this_player()->add_property( SPAM_PROPERTY, ({ time() + 5, words }),
                                  TIME_OUT );
     write( "Please remember: this command is for obtaining assistance; "
            "it is not a chat channel.  If you over use it players "
            "stop listening and you don't get assistance when you "
            "really need it!\n");
   }
   
   return 1;
} /* cmd() */

int newbie_history(object who) {
    mixed *history;
    string result;
    
    history = HIST->query_chat_history("newbie");
    
    if ( !arrayp( history ) || !sizeof( history ) ) {
        tell_object(who, "Nothing has been said on the newbie channel.\n");
    }
    else {
        result = "$P$Newbie Channel$P$";
        result += implode(map(history, (: "$I$5=$C$" + $1[0] + $1[1] +
                    "%^RESET%^" :)), "\n");
        tell_object(who, result);
    }
    
    return 1;
} /* newbie_history() */

mixed *query_patterns() {
    return ({ "<string'message'>", (: cmd($4[0]) :),
              "history", (: cmd("history") :) });
} /* query_patterns() */
