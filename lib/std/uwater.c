/* The under water standard thingy. */
inherit "/std/room";

#define ADD_ARR(ob, arr) if (member_array(ob, arr) == -1) arr += ({ ob });
static object *decrease;

int check_breath(object ob);

void create() {
  decrease = ({ });
  ::create();
}

int query_underwater() { return 1; }

/*
 * check for things like whether they can live when they enter the room.
 */
void event_enter(object ob, string mess, object from) {
  if (!living(ob))
    return ;
  set_heart_beat(1);
  if (from && !from->query_underwater()) {
    if (ob->query_water_breathing()) /* Spell or something */
      return ;
/* Init the time the player can stay under water, 1-2 minutes */
    ob->add_property("water breathing", 30+random(30));
/* Check for breathing aperatus. */
    if (sizeof(filter_array(all_inventory(ob), "check_init_breath",
                             this_object())))
      return ;
    ADD_ARR(ob, decrease)
    return ;
  } else {
    if (ob->query_water_breathing()) /* Spell or something */
      return ;
/* Check for breathing aperatus. */
    if (sizeof(filter_array(all_inventory(ob), "check_breath",
                             this_object())))
      return ;
/* Init the time the player can stay under water, 1-2 minutes */
    if (check_breath(ob))
      return ;
  }
  tell_object(ob, "You are chokeing!  You cannot breath underwater!\n");
  this_object()->player_drowning(ob);
}

void event_exit(object ob) {
  decrease = decrease - all_inventory(ob);
}

/*
 * Default make them leave up.
 */
void player_drowning(object ob) {
  this_object()->do_exit_command("up", "up", ob);
}

/*
 * Ok, two sorts of breathing thingys.  Ones that are infinite and ones
 * that last for definite periods of time.  Will turn on heart_beat and
 * decrease the ones that need decreasing every heart_beat.
 */
int check_breath(object ob) {
  if (ob->query_property("water breathing")) {
    ADD_ARR(ob, decrease)
    set_heart_beat(1);
    return 1;
  }
  return 0;
}

int check_init_breath(object ob) {
  int i;

  if (!ob->init_water_breathing())
    if (!(i=(int)ob->query_property("time for water breathing")))
      return 0;
    else
      ob->add_property("water breathing", i);
  ADD_ARR(ob, decrease)
  set_heart_beat(1);
  return 1;
}

int check_living(object ob) {
  return living(ob);
}

void heart_beat() {
  int i,
      num;
  object *liv;

  if (!sizeof(liv = filter_array(all_inventory(), "check_living",
                                 this_object())))
    set_heart_beat(0);
  decrease = decrease - ({ 0 });
  for (i=0;i<sizeof(decrease);i++)
    decrease[i]->add_property("water breathing",
                    (int)decrease[i]->query_property("water breathing")-1);
  for (i=0;i<sizeof(liv);i++)
    event_enter(liv[i], 0, this_object());
}

object *query_decrease() { return decrease; }
