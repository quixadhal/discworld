/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: describe.c,v 1.10 2003/07/16 18:26:24 pinkfish Exp $
 * $Log: describe.c,v $
 * Revision 1.10  2003/07/16 18:26:24  pinkfish
 * Fix up the message.
 *
 * Revision 1.9  2003/07/16 18:19:52  pinkfish
 * Fix up some describe things.
 *
 * Revision 1.8  2003/03/03 05:35:09  pinkfish
 * Fix up the sentances.
 *
 * Revision 1.7  2003/03/02 20:15:59  pinkfish
 * Make it so you have to put in 'main' to set the main description.
 *
 * Revision 1.6  2003/03/01 21:17:32  pinkfish
 * Fix up the spelling of sentence.
 *
 * Revision 1.5  2003/02/21 02:21:16  pinkfish
 * Allow people to set zone based descriptions.
 *
 * Revision 1.4  2003/02/19 08:43:05  ceres
 * Fixed runtime caused by colour codes
 *
 * Revision 1.3  2000/10/30 02:13:50  presto
 * Added some code to prevent the command object from being destructed
 * while someone is still editing their description.
 *
 * Revision 1.2  2000/05/31 01:27:41  presto
 * Added a query_patterns function so that 'syntax describe' works.  Took out
 * the "Usage" notify_fails
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
#include <player.h>
#include <clothing.h>
inherit "/cmds/base";

#define TP this_player()

int use_count = 0;

private string _zone_str;

void create() {
   string zone;

   _zone_str = "face|hair";
   foreach (zone in keys(CLOTHING_HANDLER->query_all_clothing_zones())) {
      _zone_str += "|" + zone;
   }
}

int cmd(string str) {
   string desc;
   string zone;

   desc = TP->query_desc();
   if (!str) {
      if (desc) {
         write("Description reads:\n" + TP->query_cap_name() + " " +
               desc + "\n");
      } else {
         write("No description set.\n");
      }
      if (sizeof(TP->query_zone_desc_names())) {
         foreach (zone in TP->query_zone_desc_names()) {
            write("$I$5=$C$" + zone + ": " + TP->query_zone_desc(zone) + "\n");
         }
         if (!desc) {
            write("%^YELLOW%^Warning! Your zone descriptions will not be "
                  "displayed unless you also have a main description.%^RESET%^\n");
         } else {
            write("Each zone description is a sentence, or multiple "
                  "sentences.  If you do not do this then the descriptions "
                  "will look wrong.\n");
         }
      } else {
         write("No zone descriptions setup.\n");
      }
      return 1;
   }
   else if (str == "clear") {
      write("Description cleared.\n");
      TP->set_desc(0);
      return 1;
   }
   else if (str == "edit") {
      /*
       * The cap name is stuck on so they can see what their desc will look
       * like.
       */
     if (desc) {
         this_player()->do_edit(sprintf("%-=*s", this_player()->query_cols(),
                                        this_player()->query_cap_name() + " " +
                                        replace(strip_colours(desc), "\n",
                                                "\n\n") + "\n"),
                                "finish_desc_edit");
     } else
       this_player()->do_edit("", "finish_desc_edit");
     ++use_count;
     return 1;
   }
   else if (strlen(str) > MAX_DESC) {
      notify_fail("Description unchanged.  Given description was too long.  "
                  "Maximum allowed length is " + MAX_DESC+ ".\n");
      return 0;
   }
   TP->set_desc(strip_colours(str));
   write("Description set.\n");
   return 1;
}

void finish_desc_edit(string str) {
   string cname;
   
   --use_count;
   if (!str) {
      write("Description unchanged.\n");
     return;
   }
   /* Save them some trouble if they started off with their name... */
   cname = this_player()->query_cap_name();
   if (str[0 .. strlen(cname)] == cname + " ")
      str = str[strlen(cname) + 1 .. 10000];
   str = strip_colours(str);
   str = replace(str, ({ "\n\n", "$NEW_LINE$", "\n", " " }));
   str = replace(str, "$NEW_LINE$", "\n");
   if (strlen(str) > MAX_DESC) {
      write("Description unchanged.  Given description was too long.  "
            "Maximum allowed length is " + MAX_DESC+ ".\n");
      return;
   }
   TP->set_desc(str);
   write("Description set.\n");
   return;
}

int cmd_zone(string zone, string desc) {
   this_player()->set_zone_desc(zone, desc);
   write("Set the description for the zone " + zone + " as \"" + desc + "\".\n");
   return 1;
}

int cmd_zone_clear(string zone) {
   this_player()->set_zone_desc(zone, 0);
   write("Cleared the description for the zone " + zone + ".\n");
   return 1;
}

mixed *query_patterns()  {
   return ({ "", (: cmd(0) :),
             "main <string'description'>", (: cmd($4[0]) :),
             "edit", (: cmd("edit") :),
             "clear", (: cmd("clear") :),
             "zone {" + _zone_str + "} <string'description'>",
                  (: cmd_zone($4[0], $4[1]) :),
             "zone {" + _zone_str + "} clear", 
                  (: cmd_zone_clear($4[0]) :),
               });
}

