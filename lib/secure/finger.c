/*  -*- LPC -*-  */
/**
 * The finger object, this handles all the finger and refer information
 * for the players.
 * @author Pinkfish
 * @started way back in 1991 sometime
 */
/*
 * $Locker:  $
 * $Id: finger.c,v 1.114 2003/07/16 18:40:28 pinkfish Exp $
 * 
 */
#include <config.h>
#include <mail.h>
#include <finger.h>
#include <player_handler.h>
#include <player.h>
#include <access.h>
#include <login.h>
#include <clubs.h>
#include <alias.h>
#include <library.h>
#include <nomic_system.h>

inherit "/std/basic/club_listing";
inherit "/global/family";

// 7776000 == 90 days.
#define NO_EMAIL_TIME 7776000
#define WWW_REPLACEMENTS (["%^BOLD%^" : "<b>", \
                           "%^EBOLD%^" : "</b>", \
                           "%^RESET%^" : "", \
                           "\n" : "<br>\n" ])
#define REPLACEMENTS (["%^EBOLD%^" : "%^RESET%^", \
                       "<br>" : ""])

void create() {
   seteuid("Root");
}                               /* create() */

/**
 * This method fixes up the output for html.
 * @param str the string to check
 * @return the htmlised string
 */
string htmlise(string str) {
   return replace_string(str, "<", "&lt;");
} /* htmlise() */

/**
 * This method returns the infomation printed when a banished person is
 * fingered;
 * @param name the name of the banished person
 * @return the banished information
 */
string banish_finger(string name, int caller) {
  string retval;
  string *file;
  
  file = explode(read_file("/banish/" + name[0..0] + "/" + name + ".o"), "\n");
  retval = sprintf("%35-s%35-s\n", "Login name : " + name,
                   "Real name : Banished");
  retval += "Not really many seconds old.\n";
  retval += "No mail ever.\n";
  retval += "No plan.\nNo future.\n";
  if (caller) {
    if (sizeof(file) == 3) {
      retval += implode(file, "\n") + "\n";
    } else {
      retval += "Banish info not in the correct format.\n";
    }
  }
  
  if (CLUB_HANDLER->is_club(name)) {
    retval += "\nThere is also a club of this name, try: 'refer club " +
      name + "' for information on the club.\n";
  }
  
  return replace(retval, "@@", "@ @ ");
}

/**
 * This method turns a time elapsed into a string.  Like: ten minutes
 * and one second
 * @param time_elapsed the amount of time elapsed
 * @return a nice time elapsed string
 */
string time_elapsed_string(int time_elapsed) {
   return query_time_string(time_elapsed, -1);
}                               /* time_elapsed_string() */

/**
 * This method returns additional information about the player.  This is
 * stuff like, are they a playtester, are they a trustee etc.
 * @param name the name of the player to get addional information about
 * @param user_colour if it should user colour codes
 * @param caller the grade of the caller
 *        (player, creator, director : 0, 1, 2)
 * @return the additional information
 */
private string get_additional_info(string name, int no_colour,
                                   int user_colour, int width, int caller) {
  string retval, tmp, tmp_leader;
   string *bing;
   int i, used_colour, need_new, domain_leader, domain_deputy,
     domain_developer;
   object ob;

   retval = "%^BOLD%^";

   if (PLAYER_HANDLER->test_creator(name)) {
      if (master()->query_trustee(name))
         retval += capitalize(name) + " is a Trustee.\n";
      else if (master()->query_director(name))
         retval += capitalize(name) + " is a Director.\n";
      else if (master()->query_senior(name))
         retval += capitalize(name) + " is a Senior Creator.\n";
      else
         retval += capitalize(name) + " is a Creator.\n";

      bing = "/secure/master"->query_domains();
      
      tmp = "";
      tmp_leader = "";
      for (i = 0; i < sizeof(bing); i++) {
         if ((string) ("/d/" + bing[i] + "/master")->query_lord() == name) {          
            bing[i] = capitalize(bing[i]);
            tmp_leader += "Leader of the " + bing[i] + " domain.\n";
            domain_leader = 1;
            continue;
         } else if (("/d/" + bing[i] + "/master")->query_deputy(name)) {
            tmp += "Deputy of the " + capitalize(bing[i]) + " domain.\n";
            domain_deputy = 1;
         } else if (!("/d/" + bing[i] + "/master")->query_member(name)) {
           if(bing[i] != "fluffy" && bing[i] != "liaison")
             domain_developer = 1;
           continue;
         }
         tmp += "Project in the " + capitalize(bing[i]) + " domain: " + 
           ("/d/" + bing[i] + "/master")->query_project(name) + "\n";
         bing[i] = capitalize(bing[i]);
      }
      
      tmp = tmp_leader + tmp;
      
      if (!sizeof(bing) && !domain_leader && !domain_deputy &&
          !domain_developer)
          tmp += "Not a member of any domain.\n";

      bing = ({ });
      if("/d/admin/master"->query_member(name))
        bing += ({ "Administrator" });
      if(domain_leader)
        bing += ({ "Domain Leader" });
      if(domain_deputy)
        bing += ({ "Domain Deputy" });
      
      if("/d/liaison/master"->query_member(name))
        bing += ({ "Liaison" });
      else if(domain_developer)
        bing += ({ "Developer" });
      
      if(sizeof(bing))
        retval += "Roles : " + query_multiple_short(bing) + ".\n";

      if(tmp)
        retval += sprintf("Domains : %-=*s", width - 10, tmp);
      
   } else if (PLAYTESTER_HAND->query_playtester(name)) {
      bing = ({"Playtester"});
      if (PLAYTESTER_HAND->query_pt_exec (name)) {
        bing += ({"Playtesting Executive"});        
      }
      
      if (PLAYTESTER_HAND->query_senior_playtester(name)) {
         retval += capitalize(name) + " is a Senior Playtester.\n";
      } else {
         retval += capitalize(name) + " is a Playtester.\n";
      }
      
        retval += "Roles : " + query_multiple_short(bing) + ".\n";
   }
   retval += "%^EBOLD%^";
   
   i = PLAYER_HANDLER->test_start_time(name);
   if (i)
      retval += "First logged on " + ctime(i) + ".\n";

   retval += capitalize(time_elapsed_string(-PLAYER_HANDLER->test_age(name))) + " old.\n";

   // On since or last logged on info.
   if (find_player(name) &&
       (caller >= find_player(name)->query_invis() ||
        (this_player() &&
         reference_allowed(find_player(name), this_player())))) {
      retval += "On since " + ctime(PLAYER_HANDLER->test_last(name)) + " (" +
         capitalize(time_elapsed_string(time() - PLAYER_HANDLER->test_last(name))) +
         ").\n";
   } else {
      /* Should be a nice number.... */
      i = time() - PLAYER_HANDLER->test_last(name);
      retval += "Last logged off " + time_elapsed_string(i) + " ago.\n";
   }

   if (caller > 1) {
      if (!find_player(name)) {
         retval += PLAYER_HANDLER->test_last_on_from(name) + "  ";
         need_new = 1;
      } else {
         retval += query_ip_name(find_player(name)) + " (" +
            query_ip_number(find_player(name)) + ")\n";
      }
   }

   
   ob = find_player(name);
   if (ob && (caller >= ob->query_invis() ||
                      (this_player() &&
                       reference_allowed (ob, this_player())))) {
      if (interactive(ob)) {
         retval += "Idle for " + time_elapsed_string(query_idle(ob)) + ".\n";
         need_new = 0;
      } else {
         retval += "Net dead.\n";
         need_new = 0;
      }
   }

   if (need_new)
      retval += "\n";

   retval += (string) MAILER->finger_mail(name);
   tmp = PLAYER_HANDLER->query_project(name);
   if(no_colour || !user_colour) {
     tmp = strip_colours(tmp);
     tmp = replace(tmp, "%^", " ");
   }
   
   if (tmp && tmp != "") {
      if (strsrch(tmp, "%^") != -1)
         used_colour = 1;
      retval += "%^BOLD%^Project:%^EBOLD%^\n" + tmp + "\n%^RESET%^";
   } else
      retval += "No project.\n";

   tmp = PLAYER_HANDLER->query_plan(name);
   if(no_colour || !user_colour) {
     tmp = strip_colours(tmp);
     tmp = replace(tmp, "%^", " ");
   }
   
   if (tmp && tmp != "") {
      if (strsrch(tmp, "%^") != -1)
         used_colour = 1;

      retval += "%^BOLD%^Plan:%^EBOLD%^\n" + tmp + "\n%^RESET%^";
   } else
      retval += "No plan.\n";

   if (used_colour)
      retval += "%^RESET%^%^BOLD%^---  End of finger ---%^EBOLD%^\n";

   return retval;
}

/**
 * This method returns the intermud finger information.
 * @param name the name to get the intermud info on
 * @return the intermud information
 */
public mixed *remote_finger(string name) {
   mixed *retval;
   object ob;
   string cname;

   if (!strlen(name) || !PLAYER_HANDLER->test_user(name))
      return 0;

   ob = find_player(name);

   cname = PLAYER_HANDLER->query_cap_name(name);
   if (!cname) {
      cname = capitalize(name);
   }
   retval = allocate(9);
   retval[0] = cname;
   retval[1] = cname;
   retval[0] = cname;
   retval[1] = cname;
   retval[2] = (PLAYER_HANDLER->test_real_name(name) ?
                PLAYER_HANDLER->test_real_name(name) : "???");
   retval[3] = "0";
   retval[4] = (ob && !ob->query_invis() ? ctime(ob->query_last_log_on()) :
                capitalize(time_elapsed_string(time() -
                                    PLAYER_HANDLER->test_last(name))));
   retval[5] = (ob && !ob->query_invis() ? query_idle(ob) : -1);
   retval[6] = 0;
   retval[7] = 0;
   retval[8] = get_additional_info(name, 1, 0, 80, 0);

   return retval;
}                               /* remote_finger() */


/* Wraps the name with a link to its www finger info. */
private string link_name(string name) {
   return "<a href=\"finger.c?player="+ name +"\">"+ capitalize(name) + "</a>";
}


/**
 * This method returns the information used to do a domain finger of
 * the domain.
 * @param name the name of the domain to finger
 * @param www 1 if called from www_finger_info()
 * @return the information about the domain
 */
string domain_finger(string name,
                     int width,
                     int www)
{
   string ret;
   string master;
   string *members;
   string *deputies;
   string idle_str;
   int i;
   string projects;

   master = "/d/" + name + "/master";

   if (!stringp(ret = master->generate_string())) {

      ret = "%^BOLD%^The domain of " + capitalize(name) + ".\n";
      ret += "The leader for this domain is " +
         (www?
          link_name((string) master->query_lord()) + ".\n":
          capitalize((string) master->query_lord()) + ".\n");
      members = master->query_members();
      deputies = master->query_deputies();
      if (deputies && sizeof(deputies)) {
         if (sizeof(deputies) > 1) {
            ret += "The deputies for the domain are ";
         } else {
            ret += "The deputy for the domain is ";
         }
         if(www) {
            ret +=
               query_multiple_short(map(deputies, (: link_name($1) :)));
         } else {
            ret +=
               query_multiple_short(map(deputies, (: capitalize($1) :)));
         }
         ret += ".\n";
      }

      ret += "%^EBOLD%^";
      ret += "The current members of this domain are :\n";
      projects = "";
      if(www) {
         // Begin table
         ret +=
            "<table cols=\"2\" rules=\"none\""
            "border=\"0\" width=\"100%\">";
      }
      for (i = 0; i < sizeof(members); i++) {
         if (master->query_deputy(members[i])) {
            projects = "[deputy] " + master->query_project(members[i]) + "\n";
         } else {
            projects = master->query_project(members[i]) + "\n";
         }
         if(www) {
            ret += "<tr> <td>"+ link_name(members[i]) +
               "</td> <td>"+ projects +"</td> <tr>";
         } else {
            ret += sprintf("%-=12s %-=*s",
                           capitalize(members[i]), width - 14, projects);
         }
      }
      if(www) {
         // Close the table
         ret += "</table>";
      }
   }

   if (master->query_info()) {
      ret += sprintf("%-=*s", width, (string) master->query_info());
   }

   if ( ( idle_str = master->query_idle_mess() ) ) {
       ret += idle_str;
   }
   else { 
       ret += "It hasn't been idle, it just hasn't had a lunch break in "
        "years.\n";
   }

   return ret;
}

/**
 * This method returns the information about the specified club.
 * @param name the name of the club to finger
 * @return the club information
 */
string club_finger(string name)
{
   return club_info_string(name, 0, "gumball lord");
}                               /* club_finger() */

/**
 * This method returns the information about the specified family.
 * @param name the name of the family to finger
 * @return the family information
 */
string family_finger(string name)
{
   return family_info_string(name, "gumball lord");
}                               /* family_finger() */

/**
 * This method returns the information about the specified deity.
 * @param name the name of the deity to finger
 * @return the deity information
 */
string deity_finger(string name)
{
   return 0;
}


/** @ignore yes
 * This handles fingering of things other than players.
 * It currently supports: domains, banished names, clubs and families.
 */
private string other_finger(string name,
                            int no_colour,
                            int user_colour,
                            int width,
                            int caller,
                            int www)
{

   // Im sure I wrote a function for this.
   if (member_array(name, "/secure/master"->query_domains()) != -1) {
      return domain_finger(name, width, www);
   }

   // have to be before test for banish as deities are banished
   if (file_size("/save/deities/" + name + ".o") > 0 ||
       file_size("/save/deities/" + name + ".o.gz") > 0) {
      return deity_finger(name);
   }

   if (name[0..4] == "club ") {
      if (CLUB_HANDLER->is_club(name[5..]) &&
          !CLUB_HANDLER->is_family(name[5..])) {
         return club_finger(name[5..]);
      }
   } else if (name[0..6] == "domain ") {
      if (member_array(name[7..], "/secure/master"->query_domains()) != -1) {
         return domain_finger(name[7..], width, www);
      }
   } else if (name[0..6] == "family ") {
      if (CLUB_HANDLER->is_family(name[7..])) {
         return family_finger(name[7..]);
      }
   } else if (CLUB_HANDLER->is_club(name)) {
      if (CLUB_HANDLER->is_family(name)) {
         return family_finger(name);
      } else {
         return club_finger(name);
      }
   }

   if (file_size("/banish/" + name[0..0] + "/" + name + ".o") > 0) {
      return banish_finger(name, caller);
   }

   return 0;
}

private string internal_finger_info(string name,
                                    int no_colour,
                                    int user_colour,
                                    int width,
                                    int caller,
                                    int www) // 1 if webfinger
{
   string retval,
     tmp,
     tmp2;
   int i;
   int days_left;
   mixed *suspended, *lockedout;

   // Call other_finger to handle fingering things that aren't actually players.
   if (!find_player(name) && !PLAYER_HANDLER->test_user(name)) {
   //   if ( !PLAYER_HANDLER->test_user(name) ) {
      return other_finger(name, no_colour, user_colour, width, caller, www);
   }

   // Do the first line of real & login name
   retval = (no_colour ? "" : "%^BOLD%^") +
      sprintf("%*-sReal name : %*-=s\n", (width - 2) / 2,
              "Login name : " +
              strip_colours(PLAYER_HANDLER->query_cap_name(name) + "<br>"),
              (width - 2) / 2 - 12,
              (PLAYER_HANDLER->test_real_name(name) ?
               htmlise(strip_colours(PLAYER_HANDLER->test_real_name(name)[0..30])) :
               "???"));

   // Calculate and show the email & birthday line.
   tmp = PLAYER_HANDLER->test_email(name);
   tmp2 = PLAYER_HANDLER->test_birthday(name);

   // Liaison deputies and above. 
   if ( strlen( tmp ) ) {
     if(tmp[0] == ':' || www ||
        PLAYER_HANDLER->test_last( name ) + NO_EMAIL_TIME < time()) {
       if(caller == 0) {
         tmp = "";
       }
     }
   }
   if (tmp) {
      tmp = htmlise(tmp);
      if (tmp2 && tmp2 != UNKNOWN_BIRTHDAY) {
         tmp2 = htmlise(tmp2);
         retval += sprintf("%*-sEmail : %*-=s\n", (width - 2) / 2,
                           "Birthday : " + tmp2 + "<br>", (width - 2) / 2,
                           tmp);
      } else {
         retval += sprintf("Email : %-*s\n", (width - 2) / 2 - 8, tmp[0..30]);
      }
   } else if (tmp2 && tmp2 != UNKNOWN_BIRTHDAY) {
      tmp2 = htmlise(tmp2);
      retval += sprintf("Birthday : %-*s\n", (width - 2) / 2 - 11, tmp2 +
                        "<br>");
   }

   // Indicate if they're marked for deletion.
   i = PLAYER_HANDLER->test_deleting(name);
   if (i) {
      retval += (no_colour ? "" : "%^RED%^") +
         "This character is marked to be deleted in ";
      if (time() - i > (10 * 60 * 60 * 24)) {
         retval += "1 day.\n";
      } else {
         days_left = (10 - ((time() - i) / (60 * 60 * 24)));
         if (days_left > 1) { 
            retval += days_left + " days.\n";
         } else {
           retval += days_left + " day.\n";
         }
      }
      if (!no_colour)
         retval += "%^RESET%^";
   }

   // Indicate if they're marked for deletion.
   i = PLAYER_HANDLER->test_appealing(name);
   if (i) {
      retval += (no_colour ? "" : "%^RED%^") +
         "This character is marked to be deleted in ";
      if (time() - i > (28 * 60 * 60 * 24)) {
         retval += "1 day";
      } else {
         days_left = (28 - ((time() - i) / (60 * 60 * 24)));
         if (days_left > 1) { 
            retval += days_left + " days";
         } else {
           retval += days_left + " day";
         }
      }
      retval += " pending appeal.\n";
      if (!no_colour)
        retval += "%^RESET%^";
   }
   tmp = PLAYER_HANDLER->test_home_dir(name);
   if (tmp)
      retval +=
         sprintf("%*-s", (width - 2) / 2, "Home directory : " + tmp + "<br>");

   tmp = PLAYER_HANDLER->test_guild(name);
   if (tmp) {
      if (catch(retval += "Member of the " + tmp->query_short() + ".\n"))
         retval += "Member of a very broken guild.\n";
   } else
      retval += "Member of the Adventurers' guild.\n";

   if(!no_colour)
     retval += "%^EBOLD%^";
   
   tmp = PLAYER_HANDLER->test_location(name);
   if (strlen(tmp)) {
      tmp = htmlise(tmp);
      retval += "Location : " + tmp[0..30] + "\n";
   }
   
   tmp = PLAYER_HANDLER->test_homepage(name);
   if (stringp(tmp)) {
      tmp = htmlise(tmp);
      retval += "Home Page: " + tmp + "\n";
   }

   // Show their suspension if they're suspended.
   suspended = "/secure/bastards"->query_suspended(name);
   if (suspended) {
      tmp = capitalize(name) + " has been suspended until " +
         ctime(suspended[SUSPEND_TIME]);
      if (caller) {
         tmp += " for " + suspended[SUSPEND_REASON] + ".\n";
      } else {
         tmp += ".\n";
      }
      retval += "%^YELLOW%^" + sprintf("%s%-=*s", tmp[0..10], width - 11,
                                       tmp[11..]) + "%^RESET%^";
   }
   // Show their suspension if they're suspended.
   lockedout = "/secure/bastards"->query_lockedout(name);
   if (lockedout) {
     tmp = capitalize(name) + " is locked out until " +
       ctime(lockedout[SUSPEND_TIME]);
     if (caller) {
       tmp += " because " + lockedout[SUSPEND_REASON] + ".\n";
     } else {
       tmp += ".\n";
     }
      retval += "%^YELLOW%^" + sprintf("%s%-=*s", tmp[0..10], width - 11,
                                       tmp[11..]) + "%^RESET%^";
   }

   retval += htmlise(get_additional_info(name, no_colour, user_colour, width, caller));

   return retval;
}

private string make_html(string str)
{
   int pos;

   pos = strsrch(str, "http:");
   if (pos == -1) {
      str = "http://" + str;
   } else {
      str = str[pos..];
   }
   pos = strsrch(str, " ");
   if (pos != -1) {
      str = str[0..pos - 1];
   }
   return str;
}                               /* make_html() */

/**
 * This is the main finger method.  It determines the type of finger
 * and prints out the relevant information.
 * @param name the name of the thing being fingered
 * @param no_colour remove all the colour codes
 * @param user_colour show user defined colours
 * @return the shorterned finger information
 */
string finger_info(string name,
                   int no_colour)
{
   int caller,
     width,
     user_colour;
   string retval,
     str;
   string user,
     nick;

   user = this_player()->query_name();
   user_colour = this_player()->query_property(PLAYER_ALLOW_COLOURED_SOULS);
   
   if (this_player()) {
      nick = (string) this_player()->expand_nickname(name);
      if (nick)
         name = nick;
      width = this_player()->query_cols();
   }

   if (!width) {
      width = 80;
   }

   if (master()->query_director(user)) {
      caller = 3;
   } else if (master()->is_liaison_deputy(user)) {
      caller = 2;
   } else if (PLAYER_HANDLER->test_creator(user)) {
      caller = 1;
   } else {
      caller = 0;
   }

   retval = internal_finger_info(name, no_colour, user_colour,
                                 width, caller, 0);
   if (!retval) {
      return 0;
   }

   foreach(str in keys(REPLACEMENTS)) {
      retval = replace_string(retval, str, REPLACEMENTS[str]);
   }

   return retval;
}

/**
 * This method returns the finger information to the web pages.
 * @param name the name of the thing to finger
 * @param user the user who is doing the fingering
 * @return the html code for the finger
 */
string www_finger_info(string name,
                       string user)
{
   int caller;
   string retval,
     str;

   if (!user || user == "")
      caller = 0;
   else if (master()->query_director(user))
      caller = 3;
   else if (master()->is_liaison_deputy(user))
      caller = 2;
   else if (PLAYER_HANDLER->test_creator(user))
      caller = 1;
   else
      caller = 0;

   retval = internal_finger_info(name, 0, 1, 80, caller, 1);
   if (!retval)
      return 0;

   foreach(str in keys(WWW_REPLACEMENTS))
      retval = replace_string(retval, str, WWW_REPLACEMENTS[str]);

   retval = strip_colours(retval);
   return retval;
}

/**
 * This method returns the refer information of the player.
 * @param name the name of the player to get the refer information on
 * @return the refer information
 */
string refer_info(string name) {
   string retval, nick, tmp, family, rels, area, str;
   object ob;
   int width, sex;

   retval = "";
   width = 80;

   if (this_player()) {
      nick = (string) this_player()->expand_nickname(name);
      width = this_player()->query_cols();
   }
   if (nick) {
      name = nick;
   }

   if (!PLAYER_HANDLER->test_user(name)) {
     retval = other_finger(name, 0, 0, width, 0, 0);
     if(retval)
       foreach(str in keys(REPLACEMENTS))
         retval = replace_string(retval, str, REPLACEMENTS[str]);
     return retval;
   }


   family = "";

   tmp = PLAYER_HANDLER->test_player_title(name);
   if (tmp) {
      tmp += " ";
   } else {
      tmp = "";
   }

   retval = PLAYER_HANDLER->query_cap_name(name);
   if (!retval) {
      return 0;
   }

   tmp = PLAYER_HANDLER->test_family_name(name);
   if (tmp) {
      retval += " " + tmp;
   }

   if ("/d/liaison/master"->query_member(name)) {
      retval += " (%^YELLOW%^Liaison%^RESET%^)";
   } else if (PLAYER_HANDLER->test_creator(name)) {
      if ("/secure/master"->query_trustee(name)) {
         retval += " (%^RED%^Trustee%^RESET%^)";
      } else if ("/secure/master"->query_director(name)) {
         retval += " (%^RED%^Director%^RESET%^)";
      } else if ("/secure/master"->query_senior(name)) {
         retval += " (%^RED%^Senior%^RESET%^)";
      } else {
         retval += " (%^RED%^Creator%^RESET%^)";
      }
   }

   ob = find_player(name);
   if (ob) {
      if (tmp = (string) ob->query_gtitle()) {
         retval += " " + tmp;
      } else {
         retval += " the Adventurer";
      }
      if (tmp = (string)ob->query_property("player_title")) {
         retval += ", " + tmp;
      }
      tmp = (string) ob->query_title();
      if (tmp && tmp != "") {
         retval += ", " + tmp;
      }
   }

   if (PLAYTESTER_HAND->query_playtester(name)) {
     if (PLAYTESTER_HAND->query_senior_playtester(name)) {
       retval += ", (%^BOLD%^%^CYAN%^Senior Playtester%^RESET%^)";
     } else {
       retval += ", (%^CYAN%^Playtester%^RESET%^)";
     }
   }

   if(ob) {
     retval += ".";
      if(ob->query_nationality_name()) {
        retval += "\n" + capitalize(ob->query_possessive()) +
          " nationality is " + capitalize(ob->query_nationality_name());
        if(ob->query_nationality_region())
          retval += " and " + ob->query_pronoun() + " is from "+
            capitalize(ob->query_nationality_region());
        retval +=  ".\n";
      }
   } else {
      retval += " (not currently logged in)\n";
   }

   tmp = PLAYER_HANDLER->test_desc(name);
   if (tmp && tmp != "") {
     if(!this_player()->query_property(PLAYER_ALLOW_COLOURED_SOULS))
       tmp = strip_colours(tmp);
      sex = PLAYER_HANDLER->test_gender(name);
      if (sex == 1) {
         retval += "He " + tmp + "\n";
      } else if (sex == 2) {
         retval += "She " + tmp + "\n";
      } else {
         retval += "It " + tmp + "\n";
      }
   } else {
      retval += "No description.\n";
   }

   //
   // Do some magistrate and citizen stuff.
   //
   foreach(area in NOMIC_HANDLER->query_citizenship_areas()) {
      if (NOMIC_HANDLER->is_magistrate_of(area, name)) {
         retval += "Magistrate in " + area + ".\n";
      } else if (NOMIC_HANDLER->is_citizen_of(area, name)) {
         retval += "Citizen in " + area + ".\n";
      }
   }

   rels = "/std/basic/club_listing"->player_relationships_string(name);
   if (rels != "") {
      retval += PLAYER_HANDLER->query_cap_name(name) + " " +
         PLAYER_HANDLER->test_family(name) + " is the " + rels + "\n";
   }

   if(PLAYER_HANDLER->test_player_killer(name)) {
     retval += PLAYER_HANDLER->query_cap_name(name) + " is a player killer.\n";
   }

   tmp = PLAYER_HANDLER->query_reference(name);
   if(!this_player()->query_property(PLAYER_ALLOW_COLOURED_SOULS))
     tmp = strip_colours(tmp);
      
   if (tmp && tmp != "") {
      retval += "\n" + tmp + "\n%^RESET%^";
      if (strsrch(tmp, "%^") != -1) {
         retval += "--- End of Reference --\n";
      }
   } else {
      retval += "No reference.\n";
   }

   return retval;
}

