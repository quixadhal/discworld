/**
 * This command will allow the player to control their various options.
 * @author Pinkfish
 * @started Thu Jun  8 18:10:16 PDT 2000
 */

inherit "/cmds/base";
#include <cmds/options.h>

int cmd_set(string option, string value);

int cmd_display(string option) {
   string *bits;
   string *groups;
   string bing;

   if (sscanf(option, "%s=%s", option, bing) == 2) {
      return cmd_set(option, bing);
   }

   if (OPTION_HANDLER->is_option(this_player(), option)) {
     write(sprintf("%-20s = %s (%s)\n", option,
                   OPTION_HANDLER->query_option_value(this_player(), option),
                   OPTION_HANDLER->query_option_help(this_player(),option)));
      return 1;
   } else if (OPTION_HANDLER->is_option_group(this_player(), option)) {
      bits = OPTION_HANDLER->query_sub_options(this_player(), option);
      if (option == "") {
         write("Top level:\n");
      } else {
         write(option + ":\n");
      }
      bits = sort_array(bits, (: strcmp :));
      groups = filter(bits, (: OPTION_HANDLER->is_option_group(this_player(),
                                               $2 + " " + $1) :), option);
      bits -= groups;
      foreach (bing in groups) {
         write(sprintf("   %-17s <more options>\n", bing));
      }
      foreach (bing in bits) {
         if (OPTION_HANDLER->is_option(this_player(), option + " " + bing)) {
            write(sprintf("$I$30=   %-17s = %-10s <%s>\n", bing,
                    OPTION_HANDLER->query_option_value(this_player(),
                                                     option + " " + bing),
                    implode(OPTION_HANDLER->query_option_values(this_player(),
                                        option + " " + bing), "|")));
            //              write(sprintf("$I$30=   %-70s\n", OPTION_HANDLER->
            //query_option_help(this_player(),option + " " +
            //bing)));
         }
      }
      return 1;
   } else {
      add_failed_mess("There is no option " + option + ".\n");
      return 0;
   }
} /* cmd_display() */

int cmd_set_all(string option, string value) {
   string* stuff;
   string* ok;
   string* bad;
   string opt;

   if (!OPTION_HANDLER->is_option_group(this_player(), option)) {
      add_failed_mess("The option must be an option group to use the 'all' "
                      "keywork.\n");
      return 0;
   }

   stuff = OPTION_HANDLER->query_sub_options(this_player(), option);
   option += " ";
   ok = ({ });
   bad = ({ });
   foreach (opt in stuff) {
      if (OPTION_HANDLER->is_option(this_player(), option + opt)) {
         if (OPTION_HANDLER->set_option_value(this_player(), option + opt,
                                              value)) {
            ok += ({ option + opt });
         } else {
            bad += ({ option + opt });
         }
      } else {
         bad += ({ option + opt });
      }
   }

   if (sizeof(ok)) {
      add_succeeded_mess("Set " + query_multiple_short(ok) + " to " +
                         value + ".\n");
      return 1;
   }

   add_failed_mess("Unable to set " + query_multiple_short(bad) + " to " +
                   value + ".\n");
   return 0;
} /* cmd_set_all() */

int cmd_set(string option, string value) {
   string *bits;

   //
   // if the last bit is "all" then we allow option groups.
   //
   if (!option  ||  option == "")
      return 0;
   bits = explode(option, " ");
   if(!sizeof(bits))
     return 0;
   
   if (bits[<1] == "all") {
      if (sizeof(bits) > 1) {
         return cmd_set_all(implode(bits[0..<2], " "), value);
      }
      add_failed_mess("Cannot use 'all' as an option by itself.\n");
      return 0;
   }

   if (OPTION_HANDLER->is_option(this_player(), option)) {
      if (OPTION_HANDLER->set_option_value(this_player(), option, value)) {
         write("Set option " + option + " to " +
                OPTION_HANDLER->query_option_value(this_player(), option) +
                ".\n");
         return 1;
      } else {
         add_failed_mess("Unable to set option " + option + " to " +
                value +
                ".\n");
         return 0;
      }
   } else {
      add_failed_mess("There is no option " + option + ".\n");
      return 0;
   }
} /* cmd_set() */

int cmd_edit(string option) {
   string value;

   //
   // if the last bit is "all" then we allow option groups.
   //
   if (OPTION_HANDLER->is_option(this_player(), option)) {
      value = OPTION_HANDLER->query_option_value(this_player(), option);
      write("Editing the option " + option + ".\n");
      this_player()->do_edit(value, "finish_edit", this_object(),
                             0, option);
      return 1;
   } else {
      add_failed_mess("There is no option " + option + ".\n");
      return 0;
   }
} /* cmd_set() */

void finish_edit(string value, string option) {
   if (!value) {
      write("Aborting.\n");
      return ;
   }

   if (OPTION_HANDLER->set_option_value(this_player(), option, value)) {
      write("Set option " + option + " to " +
                OPTION_HANDLER->query_option_value(this_player(), option) +
                ".\n");
   } else {
      write("Unable to set the option " + option + ".\n");
   }
} /* finish_edit() */

mixed *query_patterns() {
   return ({ "<string'option'>",
               (: cmd_display($4[0]) :),
             "",
               (: cmd_display("") :),
             "edit <string'option'>",
                (: cmd_edit($4[0]) :),
             "<string'option'> = <word'value'>",
                (: cmd_set($4[0], $4[1]) :),
             "<string'option'> all = <word'value'>",
                (: cmd_set_all($4[0], $4[1]) :),
             "set <string'option'> all <string:quoted'value'>",
                (: cmd_set_all($4[0], $4[1]) :),
             });
} /* query_patterns() */
