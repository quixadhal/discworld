#include "language.h"
inherit "/std/object";

object *writing_on;
int ink_amt,
    size;
object ink_ob;

void create() {
  ::create();
  set_name("quill");
  set_long("A nice long feathered quill.\n");
  writing_on = ({ });
  set_weight(10);
  size = 10;
}

mixed *query_init_data()
{
   return ::query_init_data() +
   ({
      "name", 0, 0,
      "long", 0, 0,
      "short", 0, 0,
      "plurals", 0, 0,
      "adjectives", 0, 0,
      "read_mess", 0, 0,
      "main_plural", 0, 0,
      "labels", 0, 0,
      "alias", 0, 0,
      "weight", 0, 0
   });
}

void init() {
  ::init();
  this_player()->add_command("dip", this_object(), "%D %p %I");
  this_player()->add_command("write", this_object(), "%p %I 'with' %D");
  this_player()->add_command("write", this_object(), "'with' %D %p %I");
} 

int do_dip(object *obs) {
  int i;
  int *coord;
  string *effects;
 
  for (i=0;i<sizeof(obs);i++) {
     coord = (int *)obs[i]->query_ps_coord();
     if ((int)obs[i]->query_volume() && pointerp(coord)) {
        effects = (string *)"/obj/handlers/potion_space"->query_effect_at(coord);
        if (sizeof(effects) && effects[0]->query_ink()) {
           this_player()->add_succeeded(obs[i]);
           ink_ob = obs[i];
           return 1;
        }
     }
  }
  ink_ob = 0;
  return 0;
}
 
int do_write(object *obs) {
  int i;
  string lang;

  lang = (string)this_player()->query_current_language();
  if (!LANGUAGE_HAND->query_language_written(lang)) {
    write("You are not using a written language.\n");
    return 0;
  }
  if (sizeof(writing_on))  /* Look!  We are already writing on it...  sheeze. */
    return 0;
  if (!ink_ob) {
    write("You dont have any ink on "+short(0)+"!\n");
    return 0;
  }
  for (i=0;i<sizeof(obs);i++)
    if (obs[i]->query_property("writeable"))
      writing_on += ({ obs[i] });
  if (!sizeof(writing_on))
    return 0;
  this_player()->add_succeeded(writing_on[0]);
  write("Message to write on "+writing_on[0]->short()+"\n");
  say(this_player()->query_cap_name()+" starts writing on "+
      writing_on[0]->short()+".\n");
  this_player()->do_edit(0, "finished_editing");
  this_player()->supresse_add_command(); /* Supress the message printing. */
  return 1;
}

int finished_editing(string str) {
    string lang;
  lang = (string)this_player()->query_current_language();
  if (!str || str == "") {
    write("Not writing anything on "+writing_on[0]->short()+".\n");
  } else if (!LANGUAGE_HAND->query_language_written(lang)) {
    write("You seem to have changed to using a non written language.\n");
  } else {
/* Ok... now do the writing itself... */
    if (member_array(ink_ob, (object *)environment()->find_inv_match()) == -1 &&
        member_array(ink_ob, (object *)this_player()->find_inv_match()) == -1) {
      write("You have lost your ink.\n");
      return 1;
    }
    ink_amt = strlen(str)/10;
/* I dont think we need this....
    str = replace(str, ({ "\n\n", "\n", "\n", " " }) );
 */
    str = (string)writing_on[0]->add_read_mess(str,
                                    ink_ob->query_liquid_name(), lang, size);
    if (ink_ob->query_volume() < ink_amt) {
      str = str[0..(int)ink_ob->query_volume()*10];
      write("Due to a shortage of ink you only write '"+str+"' on "+
            writing_on[0]->short()+".\n");
    } else
      write("You write '"+str+"' on "+writing_on[0]->short()+".\n");
    ink_ob->remove_volume(strlen(str)/10);
    if (!strlen(str))
      write("Your ink ran out and you could not write on "+
            writing_on[0]->short()+".\n");
    say(this_player()->query_cap_name()+" writes on "+
        writing_on[0]->short()+" with "+short(0)+".\n");
  }
  writing_on = writing_on[1..1000];
  if (sizeof(writing_on)) {
    write("Writing on "+writing_on[0]->short()+".\n");
    this_player()->do_edit(0, "finished_editing");
    return 1;
  }
  return 1;
}

void heart_beat() {
  ink_amt -= 2;
  if (ink_amt <=0 ) {
    ink_amt = 0;
    set_heart_beat(0);
  }
}
