/**
 * This is the handler for all things clubby, a club being a group of
 * players.  Each club must have a unique name.  It also handles elections
 * for various club positions.
 * <p>
 * This is broken up into this bit because LPC deals with classes and
 * inheritance in extremely odd ways.
 * @author Pinkfish
 * @started Sun Sep 27 03:35:42 EDT 1998
 */

inherit "/obj/handlers/inherit/club_discuss";

#define __CLUBS_H_NO_CLASSES
#define __MAIL_H_NO_CLASSES

#include <mail.h>
#include <clubs.h>
#include <player_handler.h>
#include <broadcaster.h>
#include <player.h>
#include <top_ten_tables.h>
#include <library.h>
#include <player_handler.h>

// To deal with the check_clubs() callout
private nosave int _callout_id;
private nosave int _test_callout_id;
private string _current_test_club;
private int _last_test_time;

protected void check_extra_information(string name, string person, int startup);
void check_clubs(int startup);

class top_ten_tables {
   int num;
   int qp;
   int age;
   int start_time;
   int gender;
   int single_gender;
   int rels;
   int no_pks;
   mapping guild;
}

/**
 * This method will do the callout to the check_clubs function
 * @ignore yes
 */
protected void setup_call_check_clubs(int time, int startup) {
   remove_call_out(_callout_id);
   _callout_id = call_out("check_clubs", time, startup);
} /* setup_call_check_clubs() */

/**
 * This method sends mail to the nessessary members of the club when the
 * balance is low.
 * @param name the name of the club to send the messages too
 */
private void send_low_balance_mail(string club) {
   string *to;
   string bit;

   switch (query_club_type(club)) {
      case CLUB_FAMILY :
         // For a family everyone gets the mail.
         to = query_members(club);
         bit = "family";
         break;
      case CLUB_PERSONAL :
         // Personal club only the initiator gets the mail.
         to = ({ query_founder(club) });
         bit = "club";
         to -= ({ "unknown" });
         if (!sizeof(to)) {
            to = query_members(club);
         }
         break;
      case CLUB_ELECTED :
         // Elected club, the committee gets it.
         to = query_committee(club) - ({ CLUB_UNKNOWN_MEMBER });
         if (!sizeof(to)) {
            to = ({ query_members(club) });
         }
         bit = "club";
         break;
   }

   to = filter(to, (: PLAYER_HANDLER->test_user($1) :));

   if (sizeof(to)) {
   // Ok, now send the mail out...
      MAILER->do_mail_message(implode(to, ","),
                           "Patricians office",
                           "The " + bit + " has fees due.",
                           "",
                           sprintf("%-=80s\n", "Morning,\n\n"
                           "This is a reminder from the Patrician's Office "
                           "the " + bit + " '" + query_club_name(club) +
                           "' is currently in remission and "
                           "its balance will need to be updated before "
                           "the next "
                           "pay period or your " + bit + " will be "
                           "disbanded.\n\nYours happily,\nThe Patrician."));
   }
} /* send_low_balance_mail() */

/**
 * This method sends mail to the nessessary members of the club when the
 * club is being disbanded.
 * @param name the name of the club to send the messages too
 */
private void send_disband_mail(string club) {
   string *to;
   string bit;

   switch (query_club_type(club)) {
      case CLUB_FAMILY :
         // For a family everyone gets the mail.
         to = query_members(club);
         bit = "family";
         break;
      case CLUB_PERSONAL :
         // Personal club only the initiator gets the mail.
         to = ({ query_founder(club) });
         bit = "club";
         break;
      case CLUB_ELECTED :
         // Elected club, the committee gets it.
         to = query_committee(club) - ({ CLUB_UNKNOWN_MEMBER });
         if (!sizeof(to)) {
            to = ({ query_members(club) });
         }
         bit = "club";
         break;
   }

   // Ok, now send the mail out...
   MAILER->do_mail_message(implode(to, ","),
                           "Patricians office",
                           "The " + bit + " has been disbanded.",
                           "",
                           sprintf("%-=80s\n", "Morning,\n\n"
                           "This is a message from the patricians "
                           "office to you that your " + bit +
                           " '" + query_club_name(club) +
                           "' has been disbanded for having its fees "
                           "in remission."
                           "\n\nYours happily,\nThe Patrician."));

} /* send_disband_mail() */

/**
 * This method checks the extra information for the club.  Please note
 * this does *not* save the information.  You need to do this yourself.
 * @param name the name of the club to check
 * @see check_clubs()
 * @see remove_member()
 */
protected void check_extra_information(string name,
                                       string member,
                                       int startup) {
   if (is_elected_club(name)) {
      check_elected_information(name, member, startup);
   } else if (is_family(name)) {
      check_family_information(name, member, startup);
   }
} /* check_extra_information() */

/**
 * This method checks the members of the club and makes sure they have
 * not been up to anything naughty, like getting deleted.  It also
 * updates the top ten information on the top ten handler.
 * @param club_name the name of the club
 * @param members the members left to check
 * @param top_info the top ten information
 */
protected void check_club_members(string club_name,
                                string* members,
                                class top_ten_tables top_info) {
   int i;
   int tmp;
   string member;
   string str;
   string flip;
   class relationship_data *rels;
   class relationship_data frog;

   for (i = 0; i < 1 && i < sizeof(members); i++) {
      member = members[i];
      if (!PLAYER_HANDLER->test_user(member)) {
         log_file("CLUB", ctime(time()) + ": removed " + member +
                          " from " + club_name + " for not existing.\n");
         remove_member(club_name, member);
      } else if (!PLAYER_HANDLER->test_creator(member) &&
                 !PLAYER_HANDLER->test_property(member, "no score")) {
         //
         // Generate information for the top ten handler.
         //
         top_info->num++;
         top_info->qp += LIBRARY->query_quest_points(member);
         tmp = -PLAYER_HANDLER->test_age(member) / (60 * 60);
         top_info->rels += sizeof(CLUB_HANDLER->query_relationships(club_name, member));
         top_info->age += tmp;
         top_info->start_time += PLAYER_HANDLER->test_start_time(member) / (60 * 60);
         str = PLAYER_HANDLER->test_guild(member);
         if (str &&
             str != "") {
            if (top_info->guild[str]) {
               top_info->guild[str]++;
            } else {
               if (file_size(str + ".c") > 0) {
                  flip = str->query_real_guild_object();
                  if (flip) {
                     str = flip;
                  }
                  top_info->guild[str]++;
               }
            }
         }
         if (PLAYER_HANDLER->test_player_killer(member)) {
            top_info->no_pks++;
         }
         if (!top_info->gender) {
            top_info->gender = PLAYER_HANDLER->test_gender(member);
         }
         if (top_info->single_gender != -1 &&
             PLAYER_HANDLER->test_gender(member) != top_info->gender) {
            top_info->single_gender = -1;
         }
      }
      if (is_family(club_name)) {
         check_family_information(club_name, member, 0);
         rels = CLUB_HANDLER->query_relationships(club_name, member);
         foreach (frog in rels) {
            if (!PLAYER_HANDLER->test_family(frog->related_to)) {
/*
               remove_relationship(club_name,
                                   member,
                                   0,
                                   frog->related_to,
                                   frog->relationship);
 */
               log_file("CLUB", ctime(time()) + ": removed relationship " +
                          member +
                          " <-> " + frog->related_to + " (" +
                          frog->relationship + ") for loosing their "
                          "family.\n");
            }
         }
      }
   }

   //
   // Check to see if we have checked all the members...
   //
   members = members[1..];
   if (!sizeof(members)) {
      if (!sizeof(query_members(club_name))) {
         disband_club(club_name);
      } else if (top_info->num > 3) {
         //
         // Tell the club handler about this club/family...
         //
         TOP_TEN_HANDLER->inform_of_club(club_name,
                                         is_family(club_name),
                                         TOP_TEN_LARGEST_FAMILY,
                                         top_info->num);
         TOP_TEN_HANDLER->inform_of_club(club_name,
                                         is_family(club_name),
                                         TOP_TEN_OLDEST_FAMILY,
                                         top_info->age / top_info->num);
         TOP_TEN_HANDLER->inform_of_club(club_name,
                                         is_family(club_name),
                                         TOP_TEN_MOST_QUEST_POINTS,
                                         top_info->qp / top_info->num);
         TOP_TEN_HANDLER->inform_of_club(club_name,
                                         is_family(club_name),
                                         TOP_TEN_MOST_GUILD,
                                         top_info->guild);
         TOP_TEN_HANDLER->inform_of_club(club_name,
                                         is_family(club_name),
                                         TOP_TEN_MOST_PKS,
                                         top_info->no_pks);
         if (top_info->single_gender != -1) {
            TOP_TEN_HANDLER->inform_of_club(club_name,
                                            is_family(club_name),
                                            TOP_TEN_SINGLE_GENDER,
                                            top_info->num);
         } else {
            TOP_TEN_HANDLER->inform_of_club(club_name,
                                            is_family(club_name),
                                            TOP_TEN_SINGLE_GENDER,
                                            0);
         }
         TOP_TEN_HANDLER->inform_of_club(club_name,
                                         is_family(club_name),
                                         TOP_TEN_OLDEST_LOGONS,
                                         top_info->start_time / top_info->num);
      }

      top_info = new(class top_ten_tables);
      top_info->guild = ([ ]);
      i = member_array(_current_test_club, query_clubs());
      if (i < sizeof(query_clubs())) {
         _current_test_club = query_clubs()[i+1];
         _test_callout_id = call_out("check_club_members", 60, _current_test_club, query_members(_current_test_club), top_info);
      } else {
         _current_test_club = 0;
         _test_callout_id = 0;
      }
      save_main();
   } else {
      _test_callout_id = call_out("check_club_members", 60, club_name, members, top_info);
   }
} /* check_club_members() */

private nosave int _specific_callout_id = 0;
private nosave string *_clubs_to_check = ({ });

protected void check_specific_club(string club_name, int startup) {
    class club_info bing;

    bing = (class club_info)query_club_info(club_name);

    if (bing->last_touched < time() - CLUB_TIMEOUT) {
        disband_club(club_name);
        return;
    }

    if (!pointerp(query_members(club_name))) {
        disband_club(club_name);
        return;
    }

/*
      blue = new(class top_ten_tables);
      blue->guild = ([ ]);
      call_out("check_club_members",
               extra_space,
               club_name,
               query_members(club_name),
               blue);
 */

    if (query_time_fees_due(club_name) < time()) {
        if (query_balance(club_name, CLUB_DEFAULT_ACCOUNT_NAME) < 0) {
            // This means they must have already have been mailed.
            // So disband the club...
            send_disband_mail(club_name);
            disband_club(club_name);
            return;
        }

        if (remove_money(club_name, query_club_cost_per_period(club_name),
            CLUB_TRANSACTION_UPKEEP, "Club Dues", 
            CLUB_DEFAULT_ACCOUNT_NAME, 0)) {
            int tmp;

            bing = (class club_info)query_club_info(club_name);
            tmp = bing->last_paid;
            bing->last_paid = query_time_fees_due(club_name);
            set_club_changed(club_name);
            if (query_balance(club_name, CLUB_DEFAULT_ACCOUNT_NAME) < 0) {
               // Mail people about this.
               reset_eval_cost();
               send_low_balance_mail(club_name);
            }
        }
    }

    /*
     * This checks things like if an election needs to be called and if all
     * the people in the positions still exist in the club.
     */
    call_out( (: check_extra_information :),  1,  club_name, 0,  startup);
} /* check_specific_club() */

protected void check_specific_clubs( int startup, int pos ) {
    string club_name;
    
    if ( pos >= sizeof( _clubs_to_check ) ) {
        _clubs_to_check = ({ }); 
        _specific_callout_id = 0; 
        pos = 0; 
        return;
    }

    club_name = _clubs_to_check[ pos ]; 

    tell_creator( "taffyd", "Checking club %s (%d)\n", club_name, pos ); 
    
    check_specific_club(club_name, startup); 
    
    // Now check the next club. 
    _specific_callout_id = call_out( (: check_specific_clubs :), 10, startup, ++pos ); 
}

/**
 * This method checks all the clubs to make sure they are currently valid
 * and takes money out of the accounts of those that need to pay.  It
 * will notify those whose accounts are deliquent.
 */
void check_clubs(int startup) {
   class top_ten_tables blue;

   //remove_call_out(_callout_id);
   setup_call_check_clubs(24 * 60 * 60, 0);

   if ( !_specific_callout_id ) {
       _clubs_to_check = query_clubs();
        _specific_callout_id = call_out( (: check_specific_clubs :), 0, 
            startup, 0 ); 
   }

   // foreach (club_name in query_clubs()) {
   //   call_out("check_specific_club", extra_space - 2, club_name, extra_space,
   //            startup);
   // } 

   //
   // Start checking the members of the clubs.
   //
   remove_call_out(_test_callout_id);
   if (!_current_test_club &&
       _last_test_time + 7 * 24 * 60 * 60 < time()) {
      if (sizeof(query_clubs()) > 0) {
         _current_test_club = query_clubs()[0];
      }
      _last_test_time = 0;
      save_main();
   }
   if (_current_test_club) {
      blue = new(class top_ten_tables);
      blue->guild = ([ ]);
      _test_callout_id = call_out("check_club_members",
                                 60,
                                 _current_test_club,
                                 query_members(_current_test_club),
                                 blue);
   }
} /* check_clubs() */

#ifdef OLD_CHECK_CODE
protected void check_specific_club(string club_name, int extra_space,
                                   int startup) {
} /* check_specific_club() */
#endif
