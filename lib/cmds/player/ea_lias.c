#include <alias.h>
inherit "/cmds/base";

mapping editing_alias;

void create() {
   editing_alias = ([ ]);
} /* create() */

/**
 * The method used by the 'ealias' command.  
 * @param str the alias name to edit
 * @return 1 on success, 0 on failure
 * @see cmd::ealias
 * @see finish_alias_edit()
 */
protected int edit_alias(string str, int new_flag) {
   string al;

   if (editing_alias[this_player()]) {
      add_failed_mess("You are already editing the '" + 
                  editing_alias[this_player()] + "' alias.\n");
      return 0;
   }
   if (!this_player()->is_alias(str))  {
      if (!new_flag)  {
         add_failed_mess("No alias '"+str+"' defined.\n");
         return 0;
      }
   } else if (new_flag)  {
      add_failed_mess("You already have an alias called \"" + str + "\".");
      return 0;
   }
   editing_alias[this_player()] = str;
   if (new_flag)  al = "";
   else al = replace(ALIAS_CMD->alias_string(this_player()->
      query_player_alias(str)), ";", "\n");
   this_player()->do_edit(al, "finish_alias_edit", 0);
   return 1;
} /* edit_alias() */

/**
 * This method used to copy an alias to another one and then edit the
 * result.
 * @param alias the alias to copy
 * @param new_alias the alias to copy to
 * @return 1 on success, 0 on failure
 * @see cmd::ealias
 * @see finish_alias_edit()
 */
protected int edit_alias_copy(string alias, string new_alias) {
   string al;

   if (!this_player()->is_alias(alias)) {
      add_failed_mess("No alias '"+alias+"' defined.\n");
      return 0;
   }
   if (this_player()->is_alias(new_alias)) {
      add_failed_mess("The alias '"+new_alias+"' already exists.\n");
      return 0;
   }
   new_alias = replace_string(new_alias, "END_ALIAS", "NO YOU DONT");
   editing_alias[this_player()] = new_alias;
   al = replace(ALIAS_CMD->alias_string(this_player()->query_player_alias(alias)), ";", "\n");
   this_player()->do_edit(al, "finish_alias_edit", 0);
   return 1;
} /* edit_alias_copy() */

/**
 * The method called when the editing of the alias has been completed.
 * @param str the new alias value
 * @return 1 on success, 0 on failure
 */
void finish_alias_edit(string str) {
   string alias;

   if (!editing_alias[this_player()]) {
      write("Oh dear.  We got terribly confused and forgot "
            "what you were trying to do.  Terribly sorry.\n");
      return ;
   }
   if (!str  ||  str == "") {
      write("Can't save an empty alias, sorry.\n");
      editing_alias[this_player()] = 0;
      return ;
   }
   str = replace(str, "\n", ";");
   /*
     str = replace(str, "@@", "@ @ ");
   */
   alias = editing_alias[this_player()];
   if (!this_player()->is_alias(alias))
      printf("Added alias '%s'.\n", alias);
   else
      printf("Changed alias '%s'\n", alias);
   this_player()->add_player_alias(alias, ALIAS_CMD->compile_alias(str));
   map_delete(editing_alias, this_player());
   return ;
} /* finish_alias_edit() */

mixed *query_patterns() {
  return ({
    "<word'alias'>", (: edit_alias($4[0], 0) :),
    "new <word'alias'>", (: edit_alias($4[0], 1) :),
    "copy <word'alias'> to <word'new alias'>", (: edit_alias_copy($4[0], $4[1]) :),
      });
} /* query_patterns() */


/* These functions are here so the command object isn't destructed
 * while someone is still editing. */
int clean_up()  {
   if (sizeof(editing_alias) == 0)
      ::clean_up();
   return 1;
}


void reset()  {
   if (sizeof(editing_alias) == 0)
      ::reset();
   return;
}
