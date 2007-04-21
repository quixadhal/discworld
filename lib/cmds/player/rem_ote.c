/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: rem_ote.c,v 1.26 2003/03/14 22:28:21 pinkfish Exp $
 * 
*/
#include <library.h>
#include <player.h>
#include <network.h>

#define TELL_REPLY_LOCK_ID "lock fluff"

inherit "/cmds/base";

mixed cmd( object *obs, string words ) {
   object other;
   object *ok;
   object *ignoring;
   object *roleplaying;
   object *net_dead;
   object *fail;
   object *earmuffed;
   object *multiple_earmuffed;
   object *update_list;
   string* prop;
   string me_name;
   string tmp;
   string pad = " ";
   string* ok_string;
   int colour;

   if ( this_player()->query_role_playing() ) { 
      add_failed_mess( "You cannot use remotes when you are role playing.\n" );
      return -1;
   }

   /* Do not allow players with remotes ear muffed to use the command. */
   if ( this_player()->check_earmuffs( "remote" ) ) {
      if (this_player()->query_earmuffs() == PLAYER_ALLOW_FRIENDS_EARMUFF) {
         if (sizeof(filter(obs, (: !this_player()->is_friend($1->query_name()) :)))) {
            add_failed_mess("One of the people you are trying to remote is not a "
                            "friend (and you have tells earmuffed).\n");
            return -1;
         }
      } else {
         return notify_fail( "You have remote ear muffed.\n" );
      }
   }

   if ( userp( this_player() ) && !this_player()->query_creator() ) {
      if ( !LIBRARY->query_quest_done( (string)this_player()->query_name(),
            "remote" ) )
         return notify_fail( NOT_ALLOWED );
      if ( environment( this_player() )->query_property( "no remote" ) )
         return 0;
      if ( (int)this_player()->adjust_sp( -REMOTE_COST ) < 0 )
         return notify_fail( NO_POWER );
   }

   ok = ({ });
   ignoring = ({ });
   roleplaying = ({ });
   net_dead = ({ });
   earmuffed = ({ });
   multiple_earmuffed = ({ });
   update_list = ({ });

   if (sizeof(obs) == 0)  {
      prop = this_player()->query_tell_reply_list();
      if (prop)
         obs = map(prop, (: find_player($1) :));
      else  {
         add_failed_mess("No one has told you anything in the last 15 "
                         "minutes.\n");
         return 0;
      }
   }

   if (sizeof(obs) == 1 && obs[0] == this_player()) {
      add_failed_mess("Interesting way of expression emotion, try "
                      "talking to someone else.\n");
      return 0;
   }

   fail = this_player()->query_ignoring(obs);
   if ( sizeof( fail ) )  {
      write( "You are currently ignoring " +
             query_multiple_short(fail) + ", so you ought to leave " +
             (sizeof(fail) > 1 ? "them" : fail[0]->query_objective()) +
             " alone.\n" );
      obs -= fail;
   }
   ok_string = ({ this_player()->query_name() });
   foreach (other in obs) {
      if ( other && other->query_property( "ignoring" ) ) {
         if ( member_array( (string)this_player()->query_name(),
                             other->query_property( "ignoring" ) ) != -1 &&
               !this_player()->query_creator()) {
           ignoring += ({ other });
           other = 0;
         }
      }
      
      // This check needs to be done separately.
     
      if ( other && other->query_role_playing() && 
          !this_player()->query_creator() ) {
         roleplaying += ({ other });
         other = 0; 
      }

      if (other) {

         this_player()->adjust_time_left( -10 );
         if( userp(other) &&
             !interactive( other ) ) {
            net_dead += ({ other });
         } else if( other->check_earmuffs( "remote" ) ) {
            earmuffed += ({ other });
         } else if( other->check_earmuffs( "multiple-remote" ) &&
                    sizeof(obs) > 1) {
            multiple_earmuffed += ({ other });
         } else if (other != this_player()) {
           /*
            other->event_emote( this_player(), "%^MAGENTA%^$C$"+
                  (string)this_player()->query_cap_name() +" "+
                  words +"%^RESET%^\n" );
           */
            ok += ({ other });
            ok_string |= ({ other->query_name() });
            prop = other->query_tell_reply_list();
            if (prop && prop[0] == TELL_REPLY_LOCK_ID) {
               // Refresh if one of the locked people tells us.
               if (member_array(this_player()->query_name(), prop) != -1) {
                  other->set_tell_reply_list(prop);
               }
            } else {
               update_list += ({ other });
            }
         }
      }
   }

   update_list->set_tell_reply_list(ok_string);

   if( sizeof( ignoring ) ) {
      write( query_multiple_short(ignoring) +
            ( sizeof( ignoring ) > 1 ? " are":" is" ) +
            " ignoring you and will not have seen your message.\n" );
   }

   if ( sizeof( roleplaying ) ) {
      write( query_multiple_short( roleplaying ) +
            ( sizeof( roleplaying ) > 1 ? " are":" is" ) +
            " currently role playing and will not have seen your message.\n" );
   }


   if( sizeof( earmuffed ) ) {
      write( query_multiple_short( earmuffed ) +
            ( sizeof( earmuffed ) > 1 ? " have":" has" ) +
            " remotes earmuffed and will not have seen your message.\n" );
   }

   if (sizeof( multiple_earmuffed)) {
      write( query_multiple_short( multiple_earmuffed ) +
            ( sizeof( multiple_earmuffed ) > 1 ? " have":" has" ) +
            " multiple-remotes earmuffed and will not have seen your "
            "message.\n" );
   
   }

   colour = strsrch(words, "%^") != -1;
   // have a go at replacing multiple spaces with a single one. This should
   // stop anyone trying to format the emote to look as though it's an emote
   // followed by a tell or somesuch.
   words = replace(words, ({"        ", " ",
                              "       ", " ",
                              "      ", " ",
                              "     ", " ",
                              "    ", " ",
                              "   ", " ",
                              "  ", " "}));

   if (colour) {
      words += "%^RESET%^";
   }

   me_name = this_player()->query_short();
   if (this_player()->query_family_name()) {
      me_name += " " + this_player()->query_family_name();
   }
   if (words[0..0] == "'") pad = "";
   if (sizeof(net_dead) || sizeof(ok)) {
     if (sizeof(ok + net_dead) > 1) {
       foreach(other in ok)  {        
         tmp = other->colour_event("remote", "%^MAGENTA%^");
         other->event_emote(this_player(),
                         tmp +
                         "$C$Remotes to " +
                         query_multiple_short(net_dead + ok) + ": " + 
                         me_name + pad + words +
                         (colour?"\n" + tmp + "--End of remote--": "") +
                         "%^RESET%^\n");
         other->add_tell_history(me_name + " remotes ", words, 1);
       }
     } else {
       foreach(other in ok)  {
         tmp = other->colour_event("remote", "%^MAGENTA%^");
         other->event_emote(this_player(),
                       tmp + "$C$" + me_name + pad +
                       words +
                       (colour?"\n" + tmp + "--End of remote--": "") +
                       "%^RESET%^\n");
         other->add_tell_history(me_name + " ", words, 1);
       }
     }
     this_player()->add_tell_history("You remote to " +
                                      query_multiple_short(net_dead + ok, "a", 1) + 
                                     ": ",
                                     me_name + pad + words, 1);
     write( "You remote to " + query_multiple_short(net_dead + ok) + ": $C$" +
            me_name + pad + words +"%^RESET%^\n" );
   }

   if( sizeof( net_dead ) ) {
      write( query_multiple_short( net_dead ) +
            ( sizeof( net_dead ) > 1 ? " are":" is" ) +
            " netdead and may not have seen your message.\n" );
   }

   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "<indirect:player> <string>", (: cmd($1, $4[1]) :),
              "reply <string>",
              (: cmd(({ }), $4[0]) :) });
} /* query_patterns() */
