inherit "/cmds/base";
#include <playtesters.h>

varargs int cmd(string name, string comment) {
  object ob = load_object (PT_APPLICATION_HANDLER);
  string ret = "";
  int i;
  
  if (!ob) {
    this_player()->add_failed_mess (this_object(), "Horrible, horrible "
      "error!\n", ({ }));
    return 0;
  }

  if (!sizeof (comment)) {
    this_player()->do_edit(0, "comment_editor", this_object(), "", name);
  }
  else {
    i =  ob->do_comment (name, comment);

    if (i) {
      tell_object (this_player(), "Thank you for your comments.\n");
    }
    else {
      tell_object (this_player(), "No candidate of that name registered.\n");
    }      
  }
    
  return 1;
} /* cmd() */

mixed* query_patterns() {
   return ({ 
      "<word'name'> <string'comment'>", (: cmd($4[0], $4[1]) :),
      "<word'name'>", (: cmd ($4[0]) :), 
  });
  
} /* query_patterns() */

void comment_editor (string comment, string name) {
  object ob = load_object (PT_APPLICATION_HANDLER);  
  int i;
  
  if (!sizeof (comment)) {
    return;
  }
  else {
   i =  ob->do_comment (name, comment);
  }
  if (i) {
    tell_object (this_player(), "Thank you for your comments.\n");
  }
  else {
    tell_object (this_player(), "No candidate of that name registered.\n");
  }      

}

  