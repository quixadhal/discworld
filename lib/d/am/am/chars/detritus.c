#include "path.h"
#include "library.h"
inherit defmonster;
 
void setup() {
 
  add_attack("hands", 0, 100, 150, 0, ({ 10 }), "blunt");
  add_attack("feet", 0, 100, 150, 0, ({ 10 }), "blunt");
   set_name("detritus");
   add_alias("detritus");
   add_alias("splatter");
   set_main_plural("detrituses");
add_alias("troll");
   set_short("detritus");
set_long(
"This is Detritus the splatter.  A splatter is sort of like a bouncer, but "+
"an improved and souped up version.  He is here to stop trouble and loves "+
"breaking up opponents involved in fights.\n");
  set_race("troll");
  set_class("fighter");
  set_gender(1);
  set_level(140);
  call_out("remove_attack",2,"standard");
set_join_fights("Detritus shouts: STOP THAT FIGHT!\n");
  set_throw_out(500,70,"$mcname$ chucks you bodily out of the room.\n",
                  "$mcname$ chucks $ocname$ out of the room.\n");
  load_chat(1,({
    1, ":eyes you over.",
1, "'Youre a strange boy.",
1, ":clenches his fist.",
1, "'lets have no tom-foolery in here."
  }));
  load_a_chat(30,({
    1, "'Stop that fight.",
    1, "'Next one to break a chair gets it.",
    1, "'Your going to be billed for this."
  }));
}

int heart_beat() {
  string s1, s2;

  ::heart_beat();
  /* right assume ::heart_beat() sorts out the attacker list */
  if (sizeof(attacker_list) == 1) {
    s1 = (string)attacker_list[0]->query_name();
    if (s1 == "hrun" || s1 == "stren") {
      attacker_list[0]->stop_fight(this_object());
      stop_fight(attacker_list[0]);
    }
  } else if(sizeof(attacker_list) == 2) {
    s1 = (string)attacker_list[0]->query_name();
    s2 = (string)attacker_list[1]->query_name();
    if ((s1 == "hrun" || s1 == "stren") && (s2 == "hrun" || s2 == "stren")) {
      attacker_list[0]->stop_fight(this_object());
      attacker_list[1]->stop_fight(this_object());
      attacker_list[0]->stop_fight(attacker_list[1]);
      attacker_list[1]->stop_fight(attacker_list[0]);
      stop_fight(attacker_list[0]);
      stop_fight(attacker_list[0]);
    }
  }
}

int second_life() {
  int loop;
  object *atts;

  atts = query_attacker_list();
  for(loop = 0; loop < sizeof(atts); loop++)
    if(query_ip_number(atts[loop]))
      LIBRARY->set_quest(atts[loop]->query_name(), "detritus");
  return 0;
}

void event_death(object dier) {
  call_out("get_from_corpse", 0);
}


void get_from_corpse() {
 command("'Ahhh! These will make good trophies for the Drum. ");
}
