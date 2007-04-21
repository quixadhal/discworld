#include "potion_attrs.h"
#include "library.h"
inherit "/std/object";
#define MY_ROOM "/d/sur/Sto_Plains/n_caravan/picnic_area"

object *demons, cauldron, coin;

void setup() {
  set_weight(10);
  set_value(5);
  set_name("shell");
  add_adjective("sea");
  add_alias("seashell");
  set_short("sea shell");
  set_long("A large conch shell.  It looks like it has been hollowed "+
           "out so that it can be played.  Perhaps if you blew into "+
           "it it might work.\n");
}

void init() {
  this_player()->add_command("blow", this_object());
}

int finish_quest();

int do_blow(object *indir, string s1, string s2, string prep) {
  if (sizeof(indir)) {
    write("Blow "+short(0)+" "+prep+" "+indir->short(0)+"????\n");
    return 0;
  }
  if (file_name(environment(this_player())) == MY_ROOM)
    return finish_quest();
  add_succeeded_mess("$N $V $D and make$s a horrible noise.\n");
  return 1;
}

int finish_quest() {
  object *obs, *obs2, *obs3, ob, fish, frisbee, ladle;
  mixed *attrs;
  string failure;
  int i, j;

  write("The "+short(0)+" blows amazing loudly.\n");
  say(this_player()->one_short()+" blows the "+short(0)+
      " amazingly loudly.\n");
  if (demons) {
    write("Demons glare at you.\n");
    say("Demons glare at "+this_player()->one_short()+".\n");
    return 1;
  }
  obs = all_inventory(environment(this_player()));
  obs3 = obs;
  obs = map_array(obs, "bingle_bit", this_object());
  if ((i=member_array("ladle", obs)) == -1) {
    failure = "no ladle";
  } else {
    ladle = obs3[i];
  }
  if ((i=member_array("frisbee", obs)) == -1) {
    failure = "no frisbee";
  } else {
    frisbee = obs3[i];
  }
  if ((i=member_array("cauldron", obs)) == -1) {
    failure = "no caldron";
  } else {
    cauldron = obs3[i];
      obs2 = match_objects_for_existence("cauldrons", environment(this_player()));
    for (i=0;i<sizeof(obs2);i++) {
      if ((int)obs2[i]->query_volume() < 100 ||
          (int)obs2[i]->query_volume() < (int)obs2[i]->query_max_volume() / 2) continue;
      attrs = (mixed *)obs2[i]->query_misc_attrs();
      if (pointerp(attrs)) attrs = attrs[POTION_NAMES];
      if (pointerp(attrs) && sizeof(attrs) && attrs[0][0] == "yucky ankh water") {
        j++;
      }
    }
    if (!j) {
      failure = "no cauldron full of anhk river water";
    }
  }
  if ((i = member_array("coin", obs)) == -1) {
    failure = "no coin";
  } else {
    coin = obs3[i];
  }
  if ((i=member_array("fish", obs)) == -1) {
    failure = "no fish";
  } else {
    fish = obs3[i];
  }
      obs2 = match_objects_for_existence("toadstools", environment(this_player()));
  obs2 = filter_array(obs2, "bongle_array", this_object());
  if (sizeof(obs2) < 4) {
    failure = "not enough toadstools to sit on";
  }
  if (failure) {
    write("A bunch of hungry looking demons show up.\n");
    write("They look around the clearing.\n");
    write("Mutter to themselves about there being "+failure+".\n");
    write("Demon glares at you.\n");
    say("A bunch of hungry looking demons show up.\n");
    say("They look around the clearing.\n");
    say("Mutter to themselves about there being "+failure+".\n");
    say("Demon glares at "+this_player()->one_short()+".\n");
    return 1;
  }
  demons = allocate(4);
  for (i=0;i<sizeof(demons);i++) {
    demons[i] = clone_object("/std/container");
    demons[i]->set_name("demon");
    demons[i]->reset_get();
    demons[i]->set_long("A small demon with a manic look in his eyes "+
                        "cooking soup.\n");
    demons[i]->move(environment(this_player()));
    obs2[i]->move(demons[i]);
  }
  coin->reset_get();
  cauldron->reset_get();
  fish->dest_me();
  ladle->move(demons[2]);
  frisbee->move(demons[1]);
  say("The demons grab the fish and stick it into the cauldron.  "+
        "They then put the frisbee on the top of the cauldron as a "+
        "lid.  One of the demons grabs the ladle and stirs the "+
        "mixture around a bit whilst the other demons blow on "+
        "cauldron to heat it up.\n");
  call_out("do_finish_it", 10);
  write("The demons grab the fish and stick it into the cauldron.  "+
        "They then put the frisbee on the top of the cauldron as a "+
        "lid.  One of the demons grabs the ladle and stirs the "+
        "mixture around a bit whilst the other demons blow on "+
        "cauldron to heat it up.\n");
  return 1;
}

void do_finish_it() {
  object *obs;
  int i, j;

  say("The soup is obviously hot now, and all the demons sit down "+
        "on their toadstools to enjoy the meal.  After finishing the "+
        "meal, they smile happily.  You hear one of the say on the "+
        "way out, \"Great way to get a meal, it works every time\".\n"+
       "The demons leave taking the table, cauldron and ladle with them.\n");
  write("The soup is obviously hot now, and all the demons sit down "+
        "on their toadstools to enjoy the meal.  After finishing the "+
        "meal, they smile happily and leave.  You hear one of the "+
        "demons say on the "+
        "way out, \"Great way to get a meal, it works every time\".\n"+
       "The demons leave taking the table, cauldron and ladle with them.\n");
  cauldron->dest_me();
  coin->dest_me();
  obs = all_inventory(environment(demons[0]));
  for (i=0;i<sizeof(obs);i++)
    if (interactive(obs[i])) {
      if (!LIBRARY->set_quest(obs[i]->query_name(), "feeding frenzy"))
        continue;
      obs[i]->adjust_xp(5000);
    }
  for (i=0;i<sizeof(demons);i++) {
    obs = all_inventory(demons[i]);
    for (j=0;j<sizeof(obs);j++)
      obs[j]->dest_me();
    demons[i]->dest_me();
  }
  demons = 0;
}

mixed bingle_bit(object ob) {
  return (mixed)ob->query_property("feeding_frenzy");
}

int bongle_array(object ob) {
  return (int)ob->query_property("feeding_frenzy") != 0;
}
