/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: finger.c,v 1.14 2003/07/16 18:21:53 pinkfish Exp $
 * $Log: finger.c,v $
 * Revision 1.14  2003/07/16 18:21:53  pinkfish
 * Make $NEW_LINE$ force a new line in a desc.
 *
 * Revision 1.13  2003/07/13 09:50:24  pinkfish
 * Fix up the email querying for the options command.
 *
 * Revision 1.12  2003/04/03 19:12:49  ceres
 * Fixed to handle ramdisk
 *
 * Revision 1.11  2003/03/19 21:35:09  ceres
 * Allowed /secure/nlogin to alter the password
 *
 * Revision 1.10  2003/03/01 21:18:33  pinkfish
 * Put in only one space.
 *
 * Revision 1.9  2003/02/21 02:10:24  pinkfish
 * Add in stuff to allow zone based descriptions.
 *
 * Revision 1.8  2001/12/28 02:35:13  presto
 * When clearing the description, remove the mapping entry.  Also, if
 * player_info["desc"] is undefined, return "" for the description
 *
 * Revision 1.7  2001/12/17 02:13:32  presto
 * Add a RESET to the end of the description in query_desc()
 *
 * Revision 1.6  2001/03/12 01:36:40  ceres
 * Protected email addresses.
 * &&
 *
 * Revision 1.5  2000/06/26 23:14:44  ceres
 * Made password require 6 char passwords
 *
 * Revision 1.4  1999/08/30 01:15:31  pinkfish
 * Modify a few things and make the birthday use the UNKNOWN_BIRTHDAY
 * define.
 *
 * Revision 1.3  1999/08/30 00:58:39  ceres
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.2  1998/04/13 12:17:00  pinkfish
 * Adding documentaion and making the code a bit neater.
 *
 * Revision 1.1  1998/01/06 04:54:05  ceres
 * Initial revision
 * 
 */

/**
 * This file contains all of the information related to a player,
 * their password, real name, birthday, location etc.
 * @author Pinkfish
 */
#include <player.h>
#include <mail.h>
#include <clothing.h>

private mapping player_info;

private string password;
private nosave string tmppassword;

string query_name();
private int change_password();
private int change_password2(mixed pass);
private int change_password3(string pass);
private int change_password4(string pass);
int add_command(string command, object ob, mixed format, function func);

/**
 * The commands related to the fingering of a player.
 * Just the change password command at the moment.
 */
void finger_commands() {
   add_command("password", this_object(), "", (: change_password() :));
   add_command("passwd", this_object(), "", (: change_password() :));
} /* finger_commands() */

/**
 * This method sets the description of the player.  The
 * description is the little bit of text seen in the long.
 * @param str the new description of the player
 * @see query_desc()
 */
void set_desc(string str) {
   if (!player_info) {
      player_info = ([ ]);
   }
   if (!str) {
      map_delete(player_info, "desc");
   } else {
      player_info["desc"] = str;
   }
}

/**
 * This method returns the current description of the player.
 * @see set_desc()
 * @return the current description
 */
string query_desc() {
   if (!player_info  ||  undefinedp(player_info["desc"])) {
      return "";
   }
   return player_info["desc"] + "%^RESET%^";
} /* query_desc() */

/**
 * This method sets the description for the specific area of the body.
 * @param zone the zone to set the description for
 * @param desc the description for that zone
 */
void set_zone_desc(string zone, string desc) {
   if (!player_info["zone desc"]) {
      player_info["zone desc"] = ([ ]);
   }
   if (!desc) {
      map_delete(player_info["zone desc"], zone);
   } else {
      player_info["zone desc"][zone] = desc;
   }
}

/**
 * This method returns the zone description for the specified zone.
 * @param zone the zone to get the description for
 * @return the zone description
 */
string query_zone_desc(string zone) {
   if (!player_info["zone desc"]) {
      return 0;
   }
   return player_info["zone desc"][zone];
}

/**
 * This method returns all the zones there are currently descriptions for.
 * @return the zones there are descriptions for
 */
string* query_zone_desc_names() {
   if (!player_info["zone desc"]) {
      return ({ });
   }
   return keys(player_info["zone desc"]);
}

/**
 * This method returns the main zone description to use in the
 * long of the player.
 * @return the main zone description
 */
string query_main_zone_desc(object* wearing) {
   object bing;
   string zone;
   string type;
   string eq_type;
   string str;
   mixed types;
   mapping covered;

   if (!sizeof(player_info["zone desc"])) {
      return "";
   }

   covered = ([ ]);
   foreach (bing in wearing) {
      types = bing->query_type();
      if (!arrayp(types)) {
         types = ({ types });
      }
      foreach (type in types) {
         eq_type = CLOTHING_HANDLER->query_equivilant_type(type);
         if (eq_type) {
            foreach (zone in CLOTHING_HANDLER->query_zone_names(eq_type)) {
               covered[zone] = 1;
            }
         } else {
            foreach (zone in CLOTHING_HANDLER->query_zone_names(type)) {
               covered[zone] = 1;
            }
         }
      }
   }

   str = "";
   foreach (zone in query_zone_desc_names()) {
      if (!covered[zone]) {
         str += " " + replace_string(query_zone_desc(zone), "$NEW_LINE$", "\n");
      }
   }
   return str;
}

/**
 * This method changes the password of the player.  It can only
 * be called by the login object at startup.
 * @param pass the new password
 */
void set_password(string pass) {
   if (file_name(previous_object())[0..12] == "/secure/login" ||
       file_name(previous_object())[0..13] == "/secure/nlogin") {
      password = pass;
   }
} /* set_password() */

/**
 * This method is called when the password is attempted to be changed.
 * @return 1 if successful, 0 on failure
 */
private int change_password() {
   if (password) {
      write("Please enter your old password : ");
      input_to((: change_password2 :),1);
      return 1;
   }
   change_password2(0);
   return 1;
} /* change_password() */

private int change_password2(mixed pass) {
   string str;
   
   if (password) {
      str = crypt(pass,password);
      if (str != password) {
         write("\nIncorrect.\n");
         return 1;
      }
   }
   write("\nEnter new Password : ");
   input_to((: change_password3 :),1);
   return 1;
} /* change_password2() */

private int change_password3(string pass) {
   tmppassword = pass;
   if(sizeof(pass) < 6) {
     write("\nPassword is too short, must be at least 6 characters.\n");
     write("Enter new Password :");
     input_to((: change_password3 :),1);
     return 1;
   }
     
   write("\nPlease enter again : ");
   input_to((: change_password4 :),1);
   return 1;
} /* change_password3() */

private int change_password4(string pass) {
   if (tmppassword != pass) {
      write("\nIncorrect.\n");
      return 1;
   }
   password = crypt(pass,password);
   write("\nOk.\n");
   return 1;
} /* change_password4() */

/**
 * Find out which IP addresses this user wants to be able to login from.
 */
string *query_rhosts() {
  if(!player_info)
    return 0;
  return player_info["allowed_ips"];
}

void set_rhosts(string *ips) {
  if(file_name(previous_object())[0..18] != "/cmds/player/access" &&
     file_name(previous_object())[0..19] != "/cmds/lord/authorise")
    return ;
  
  if(!player_info)
    player_info = ([ ]);
  
  player_info["allowed_ips"] = ips;
}

/**
 * This will return the real name of the player.
 * @return the real name of the player
 * @see set_real_name()
 */
string query_real_name() {
   if (!player_info) {
      return 0;
   }
   return player_info["real_name"];
} /* query_real_name() */

/**
 * This will set the real name of the player.
 * @see query_name()
 * @param str the new real name of the player
 */
void set_real_name(string str) {
   if (!player_info) {
      player_info = ([ ]);
   }
   player_info["real_name"] = str;
} /* set_real_name() */

/**
 * This methoid returns the location of the player.  The location
 * is the location in their funger infor, so like "Perth, oz, whatever".
 * @return their currently set location
 * @see set_where()
 */   
string query_where() {
   if (!player_info) {
      return "";
   }
   return player_info["location"];
} /* query_where() */

/**
 * This method sets the currnet location of the player.
 * @see query_location()
 * @param str the new location
 */
void set_where(string str) {
   if (!player_info) {
      player_info = ([ ]);
   }
   player_info["location"] = str;
} /* set_where() */

/**
 * This method returns the players birthday.  The birthday will be
 * of the form "4th of july".
 * @return the current birthday
 * @see set_birthday()
 * @see query_is_birthday_today()
 */
string query_birthday() {
   if (!player_info) {
      return UNKNOWN_BIRTHDAY;
   }
   if (!player_info["birthday"]) {
      return UNKNOWN_BIRTHDAY;
   }
   
   return player_info["birthday"];
} /* query_birthday() */

/**
 * This method sets the players current birthday.  The birthday
 * needs to be of the form:  "4th of July".
 * @param i the new birthday to set
 * @see query_birthday()
 * @see query_is_birthday_today()
 */
void set_birthday(string i) {
   if (!player_info) {
      player_info = ([]);
   }
   player_info["birthday"] = i;
} /* set_birthday() */

/**
 * This method checks to see if it is currently the players birthday.
 * @return 1 if it is their birthday, 0 if not
 * @see set_birthday()
 * @see query_birthday()
 */
int query_is_birthday_today() {
   string cmonth;
   string dummy;
   string bmonth;
   int cdate;
   int bdate;
    
   if (sscanf(ctime(time()), "%s %s %d %s", dummy, cmonth, cdate, dummy)!=4) {
      return 0; /* error in date */
   }
   if (sscanf(query_birthday(), "%d%s of %s", bdate, dummy, bmonth) !=3) {
      return 0; /* no bday set */
   }
   if (cmonth == bmonth[0..2] && cdate == bdate) {
      return 1;
   }
} /* query_is_birthday_today() */

/**
 * This method returns the current email address of the player.
 * @see set_email()
 * @return the current email address
 */
string query_email() {
  if(file_name(previous_object())[0..13] != "/secure/finger" &&
     file_name(previous_object())[0..27] != "/obj/handlers/player_handler" &&
     file_name(previous_object())[0..28] != "/obj/handlers/options_handler") {
    return "";
   }

   if (!player_info) {
      return "";
   }
   return player_info["email"];
}
/**
 * This method sets the current email address of the player.
 * @see query_email()
 * @param str the new email address
 */
nomask void set_email(string str) {
   if (!player_info) {
      player_info = ([]);
   }
   player_info["email"] = str;
} /* set_email() */

/**
 * This method queries the current homepage for the player.
 * @see set_homepage()
 * @return their homepage url
 */
string query_homepage() {
   if (!player_info) {
      player_info = ([ ]);
   }
   return player_info["homepage"];
} /* query_homepage() */

/**
 * This method sets the players homepage.
 * @see query_homepage()
 * @param their homepage url
 */
nomask void set_homepage(string str) {
   if (!player_info) {
      player_info = ([]);
   }
   player_info["homepage"] = str;
} /* set_homepage() */

/**
 * This method returns whether the player has set their finger info.
 * @return 1 if they have their finger information set, 0 if not
 */
int finger_set() {
   if (!player_info || 
      player_info == ([ ]) || 
      sizeof(keys(player_info)) == 1) {
      return 0;
   }
   return 1;
} /* finger_set() */
