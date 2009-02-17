/*  -*- LPC -*-  */
/*
 * $Id: report_base.c,v 1.44 2003/07/08 07:12:18 pinkfish Exp $ 
 */
/**
 * This is the base object for creating typo, bug, idea etc reporting
 * commands from.  You should set the error type of the
 * object in its create function.  The use_last_error flag should
 * be set for those error report types which will need to use the last
 * runtime error on the player object.
 * @example
 * inherit "/cmds/report_base";
 * 
 * void create() {
 *    ::create();
 *    set_error_type("TYPO");
 * } /\* create() *\/
 */

#include <creator.h>
#include <log.h>
#include <command.h>
#include <spells.h>
#include <user_parser.h>
#include <soul.h>
#include <error_handler.h>

inherit "/cmds/base";
inherit "/cmds/bug_replies";

class errors {
   int type;
   string file;
   string error;
   string extra;
}
#define ROOM_BUG    1
#define OBJECT_BUG  2
#define RITUAL_BUG  3
#define SPELL_BUG   4
#define HELP_BUG    5
#define COMMAND_BUG 6
#define GENERAL_BUG 7
#define WEB_BUG     8
#define SYNONYMS "/doc/SYNONYMS"
int bug_command(string str);
int bug_spell(string str);
int bug_ritual(string str);
int bug_object(object * obj,
               string str);
int bug_help(string str);
int bug_man(string str);
int bug_room();
int bug_general();
int bug_soul(string str);

private nosave mapping _globals;
private nosave string _error_type;
private nosave int _use_last_error;

void create()
{
   bug_replies::create();
   _globals = ([ ]);
}                               /* create() */

/**
 * This sets the error type name.  The error type should be one
 * of "TYPO", "BUG", "IDEA".
 * @param type the type to set
 * @see query_verb()
 */
void set_error_type(string type)
{
   _error_type = type;
}                               /* set_error_type() */

/**
 * This sets the use_last_error flag.  If this flag is set to a non-zero
 * value then the last runtime error stored on the player object will
 * be attached to the error report.
 * @param error the new value of the last error flag
 */
void set_use_last_error(int error)
{
   _use_last_error = error;
}                               /* set_use_last_error() */

/**
 * This method returns the currently set value of the last error flag.
 * @return the current value of the last error flag
 */
int query_use_last_error()
{
   return _use_last_error;
}                               /* query_use_last_error() */

/** @ignore yes */
int bug_room()
{
   if (environment(this_player())) {
 _globals[this_player()] = new (class errors, type: ROOM_BUG, error: "ROOM " + _error_type, file:file_name(environment
             (this_player
              ())));
  } else {
 _globals[this_player()] = new (class errors, type: ROOM_BUG, error: "ROOM " + _error_type, file:"/d/mudlib/void");
   }
   this_player()->do_edit(0, "end_of_edit");
   return 1;
}                               /* bug_room() */

int bug_special(string which) {
  _globals[this_player()] = new (class errors,
                                 type: GENERAL_BUG,
                                 error: "GENERAL " + _error_type,
                                 file: "/d/special/"+which+"/BugReports");
  this_player()->do_edit(0, "end_of_edit");
  return 1;
}

int bug_misc(string which) {
  _globals[this_player()] = new (class errors,
                                 type: GENERAL_BUG,
                                 error: "GENERAL " + _error_type,
                                 file: "/obj/"+which+"/BugReports");
  this_player()->do_edit(0, "end_of_edit");
  return 1;
}

/** @ignore yes */
int bug_general()
{
   string dir;
   string *bits;
   string file;

   //
   // For a general bug put it in the domains base directory.
   //
   if (!environment(this_player())) {
      dir = "/d/am/fluff";
   } else {
      dir = file_name(environment(this_player()));
   }
   bits = explode(dir, "/");
   if (bits[0] == "d") {
      file = implode(bits[0..1], "/") + "/general";
   } else {
      file = file_name(environment(this_player()));
   }
 _globals[this_player()] = new (class errors, type: GENERAL_BUG, error: "GENERAL " + _error_type, file:file);
   this_player()->do_edit(0, "end_of_edit");
   return 1;
}                               /* bug_general() */

/** @ignore yes */
int bug_command(string str)
{
   int i;
   string dir;
   string file;
   string *bits;
   mixed *junk,
    *coms;
   class command cmd;
   class errors bing;
   class command_data cmd_data;
   class command_class cmd_class;
   mapping temp;


   bing = new (class errors);
   coms = ({ });
   junk = actions_defined(this_player(), 0, 12);
   for (i = 0; i < sizeof(junk); i += 2) {
      if (junk[i] == str) {
         coms += ({ junk[i], junk[i + 1][0], junk[i + 1][1] });
      }
   }
   if (sizeof(coms) > 3) {
      return notify_fail("More than one commands with the name \"" + str +
                         "\" found.  Please be more specific.\n");
   }
   if (sizeof(coms)) {
      bing->file = function_exists((string) coms[2], (object) coms[1]);
      if (!(bing->file)) {
         bing->file = base_name((object) coms[1]);
         str += " (protected method, so it might not be in this file)";
      }
   } else {
    cmd = new (class command, verb:str);
      if (CMD_D->HandleStars(cmd) &&
          sizeof((coms = (mixed *) CMD_D->GetPaths(cmd->verb) &
                  (mixed *) this_player()->GetSearchPath()))) {
         bing->file = coms[0] + "/" + cmd->verb;
      } else {
         if ((cmd_class = this_player()->query_parse_command(str))) {
//tell_creator("presto", "%O\n", coms);
            temp = cmd_class->patterns;
            cmd_data = temp[keys(temp)[0]];
//tell_creator("presto", "%O\n", cmd_data->calls[0]);
//tell_creator("presto", "%O\n", values(coms->patterns));
            bing->file = base_name(cmd_data->calls[0]);
//            bing->file = base_name((values(coms->patterns))[0]->calls[0]);
//            bing->file = base_name(coms[0][OBJS]);
         } else {
            if ((coms = SOUL_OBJECT->query_soul_command(str))) {
               bing->file = "/soul/" + str[0..0] + "/" + str;
            } else if (_error_type == "IDEA") {
               //bing->file = "/soul/" + str[0..0] + "/" + str;
               dir = file_name(environment(this_player()));
               bits = explode(dir, "/");
               if (bits[0] == "d") {
                  file = implode(bits[0..1], "/") + "/general";
               } else {
                  file = file_name(environment(this_player()));
               }
               bing->file = file;
            } else {
               return notify_fail("Command " + str + " not found.\n");
            }
         }
      }
   }
   bing->error = "COMMAND " + _error_type + " " + str;
   bing->type = COMMAND_BUG;
   _globals[this_player()] = bing;
   this_player()->do_edit(0, "end_of_edit");
   return 1;
}                               /* bug_command() */

/** @ignore yes */
int bug_help(string str)
{
   mixed *stuff;
   class errors bing;
   string tmp;

   bing = new (class errors);
   tmp = "/cmds/player/help"->query_synonym(str);
   if (strlen(tmp))
      str = tmp;

   if (str && str != "") {
      stuff = "/cmds/player/help"->query_help_on(str);
      if (!sizeof(stuff)) {
         // See if it is a soul command.
         if (!SOUL_OBJECT->query_soul_command(str)) {
            notify_fail("Could not find the help file '" + str +
                        "'.  If you wish "
                        "to suggest a new command use 'idea help'\n");
            return 0;
         } else {
            bing->file = "/soul/" + str;
         }
      } else {
         sscanf(stuff[0][0], "%*s (%s)", bing->file);
      }
   } else {
     bing->file = "/cmds/player/help";
   }

   bing->error = "HELP " + _error_type + " " + str;
   bing->type = HELP_BUG;
   _globals[this_player()] = bing;
   this_player()->do_edit(0, "end_of_edit");
   return 1;
}                               /* bug_help() */

/** @ignore yes */
int bug_soul(string str)
{
   class errors bing;
   string tmp;

   bing = new (class errors);
   if (str)  {
      tmp = sprintf("/soul/%c/%s.s", str[0], str);
      if (file_size(tmp) < 1)  {
         notify_fail("No such soul command \"" + str + "\".\n");
         return 0;
      }
      else return bug_command(str);
   }
   bing->file = "/soul/ideas";
   bing->error = "COMMAND " + _error_type + " " + str;
   bing->type = COMMAND_BUG;
   _globals[this_player()] = bing;
   this_player()->do_edit(0, "end_of_edit");
   return 1;
}                               /* bug_help() */


/** @ignore yes */
int bug_ritual(string str)
{
   mapping junk;
   class errors bing;
   string sname;
   mixed* data;
   string file;

   str = this_player()->expand_nickname(str);
   bing = new (class errors);
   junk = this_player()->query_spells();
   // Do a case insensative search.
   foreach (sname, data in junk) {
      if (lower_case(sname) == lower_case(str)) {
         if (sizeof(data) <= S_OBJECT) {
            // If it has a bad file name, ignore it
            file = file_name(environment(this_player()));
         } else {
            file = data[S_OBJECT];
         }
      }
   }
   if (!file) {
      notify_fail("Ritual " + str + " not found.\n");
      map_delete(_globals, this_player());
      return 0;
   }
   bing->file = file;
   bing->error = "RITUAL " + _error_type + " " + str;
   bing->type = RITUAL_BUG;
   _globals[this_player()] = bing;
   this_player()->do_edit(0, "end_of_edit");
   return 1;
}                               /* bug_ritual() */

/**
 * This entry point is used to erroring spells without specifically saying
 * which one.
 * @param file the file path to use
 * @param name the name to use
 */
int bug_web(string url)
{
   class errors bing;

   bing = new (class errors);
   bing->file = "/www/fluff";
   bing->error = "OBJECT " + _error_type + " " + url;
   bing->type = WEB_BUG;
   _globals[this_player()] = bing;
   this_player()->do_edit("Url: " + url + "\n\n", "end_of_edit");
   return 1;
}                               /* bug_spell() */

/**
 * This entry point is used to erroring spells without specifically saying
 * which one.
 * @param file the file path to use
 * @param name the name to use
 */
int bug_spell_file(string file, string name)
{
   class errors bing;

   bing = new (class errors);
   bing->file = file;
   bing->error = "SPELL " + _error_type + " " + name;
   bing->type = SPELL_BUG;
   _globals[this_player()] = bing;
   this_player()->do_edit(0, "end_of_edit");
   return 1;
}                               /* bug_spell() */

/** @ignore yes */
int bug_spell(string str)
{
   mapping junk;
   string file;
   string sname;
   mixed data;

   if (str == "wizards" || str == "witches") {
      file = "/obj/spells/" + str;
   } else {
      // Expand the nickname
      str = this_player()->expand_nickname(str);
      junk = this_player()->query_spells();
      // Do a case insensative search.
      foreach (sname, data in junk) {
         if (lower_case(sname) == lower_case(str) && arrayp(data)) {
            file = data[S_OBJECT];
         }
      }
      if (!file) {
         notify_fail("Spell " + str + " not found.\n");
         map_delete(_globals, this_player());
         return 0;
      }
   }
   bug_spell_file(file, str);
   return 1;
} /* bug_spell() */

/** @ignore yes */
int bug_object_new(string name)
{
   string info;
   class errors bing;

   bing = new (class errors);
   bing->file = base_name(environment(this_player()));
   bing->error = "OBJECT " + _error_type + " " + name;
   bing->type = OBJECT_BUG;
   bing->extra = info;
   _globals[this_player()] = bing;
   this_player()->do_edit(0, "end_of_edit");
   return 1;
}                               /* bug_object() */

/** @ignore yes */
int bug_object(object * obj,
               string str)
{
   string info;
   class errors bing;
   string name;
   string short;
   string* bits;

   bing = new (class errors);
   if (sizeof(obj) > 1) {
      notify_fail("More than one object can be identified with the name " +
                  str + "\n");
      map_delete(_globals, this_player());
      return 0;
   }
   bing->file = base_name(obj[0]);
   switch (bing->file) {
   case "/std/room/basic/item":
      bing->file = base_name(environment(this_player()));
      info = sprintf("Room item %s.\n\n", str);
      break;
   case "/std/bit" :
      bing->file = "/std/races/happy_bit";
      break;
   case "/std/book" :
   case "/obj/armour":
   case "/obj/baggage":
   case "/obj/clothing":
   case "/obj/container":
   case "/obj/food":
   case "/obj/monster":
   case "/obj/weapon":
   case "/std/object":
      if (obj[0]->query_property("virtual name")) {
         bing->file = obj[0]->query_property("virtual name");
         info = sprintf("VObject: %s, Object: %s\nName: %s, Short: %s\n\n",
                        obj[0]->query_property("virtual name"),
                        bing->file,
                        obj[0]->query_name(), obj[0]->query_short());
         break;
      } else {
         bing->file = base_name(environment(this_player()));
      }
   default:
      bits = explode(bing->file, "/");
      name = bits[0];
      if (name == "obj" || name == "std") {
         // Put it in the room by default.
         if (sizeof(bits) == 2 ||
             (bits[1] != "food" && bits[1] != "armours" &&
             bits[1] != "weapons" && bits[1] != "amulets" &&
             bits[1] != "monster" && bits[1] != "music" &&
             bits[1] != "plants" && bits[1] != "rings" &&
             bits[1] != "furnitures" && bits[1] != "jewellery" &&
             bits[1] != "wands")) {
            bing->file = base_name(environment(this_player()));
         }
      }
      if (pointerp(obj[0]->query_name()))  { /* doors */
         name = (obj[0]->query_name())[0];
      } else {
         name = obj[0]->query_name();
      }
      if (!name) {
         name = "<Bad name>";
      }
      short = obj[0]->query_short();
      if (!short) {
         short = "<Bad short>";
      }
      info = sprintf("Name: %s, Short: %s\n\n", name, short);
      break;
   }
   bing->error = "OBJECT " + _error_type + " " + str;
   bing->type = OBJECT_BUG;
   bing->extra = info;
   _globals[this_player()] = bing;
   this_player()->do_edit(0, "end_of_edit");
   return 1;
}                               /* bug_object() */

/** @ignore yes */
void end_of_edit(string body)
{
   if (strlen(body)) {
      int ending;
      string name,
        trace = 0;
      mapping last_error;
      class errors bing;

      bing = _globals[this_player()];
      if (bing->extra) {
         body = bing->extra + body;
      }
      if (body[<1] != '\n') {
         ending = 1;
      }
      if (environment(this_player())) { // && bing->type != ROOM_BUG) {
         if (ending) {
            body += "\n";
            ending = 0;
         }
         body += sprintf("\nEnvironment: %s (%s)\n",
                         file_name(environment(this_player())),
                         environment(this_player())->short());
      }
      if (_use_last_error) {
         last_error = (mapping) this_player()->get_last_error();
         if (mapp(last_error)) {
            trace = (string) master()->standard_trace(last_error, 1);
            this_player()->set_last_error(0);
         }
      }

      if (ending) {
         body += "\n";
      }
      name = (string) this_player()->query_name();
      SMART_LOG->smart_log(bing->error, name, body, trace, bing->file);
      printf("Thank you for your %s report.\n", lower_case(_error_type));
   } else {
      printf("Not saving %s report, aborting.\n", lower_case(_error_type));
   }
   map_delete(_globals, this_player());
}                               /* end_of_edit() */

/** @ignore yes */
int clean_up()
{
   return 0;
}                               /* clean_up() */

/** @ignore yes */
void reset()
{
}                               /* reset() */

/** @ignore yes */
mixed *query_patterns()
{
   return ({ "command <string'name'>", (: bug_command($4[0]) :),
             "replies", (: bug_replies(0) :),
             "replies new", (: bug_replies(1) :),
             "web <string'url'>", (: bug_web($4[0]) :),
             "spell <string'name'>", (: bug_spell($4[0]) :),
             "spell {generic|wizards|witches}", (: bug_spell_file("/obj/spells/" + $4[0], $4[0]) :),
             "spell new", (: bug_spell_file("/obj/spells/generic", "generic") :),
             "object new",
             (: bug_object_new("new") :),
             "object name <string'name'>",
             (: bug_object_new($4[0]) :),
             "object <indirect:object:me-here'name of NPC or item'>",
             (: bug_object($1, $4[0]) :),
             "ritual <string'name'>", (: bug_ritual($4[0]) :),
             "ritual generic", (: bug_spell_file("/obj/rituals/generic", "generic") :),
             "ritual new", (: bug_spell_file("/obj/rituals/generic", "generic") :),
             "help <string'subject'>", (: bug_help($4[0]) :),
             "help", (: bug_help("") :),
             "soul <string'soul command'>", (: bug_command($4[0]) :),
             "soul new", (: bug_soul(0) :),
             "room", (: bug_room() :),
             "special {" +
               implode(filter(get_dir("/d/special/"),
                              (: file_size("/d/special/"+$1+"/BugReports") >= 0 :)),
                       "|") + "}", (: bug_special($4[0]) :),
             "misc {" +
               implode(filter(get_dir("/obj/"),
                              (:  file_size("/obj/"+$1+"/BugReports") >= 0 :)),
                       "|") + "}", (: bug_misc($4[0]) :) });
}                               /* query_patterns() */
