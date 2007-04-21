/**
 * This file is the inheritable for a meeting room.  It handles the floor
 * and a few other things neatly.
 * @author Pinkfish 
 * @started 25th of June
 */

private object _floor;
private string _leader;

int do_bang();
int do_recover();
int do_start();
int do_finish();
object create_floor();
void add_property(string name, mixed data);

/** @ignore yes */
void create() {
   _floor = create_floor();
   _floor->move(this_object());
} /* create() */

/** @ignore yes */
void init() {
   add_command("bang", "gavel", (: do_bang() :));
   add_command("recover", "floor", (: do_recover() :));
   add_command("start", "meeting", (: do_start() :));
   add_command("finish", "meeting", (: do_finish() :));
} /* init() */

/**
 * This method creates the floor for the room.
 * @return the floor for the room
 */
object create_floor() {
   object floor;
   
   floor = clone_object("/std/object");
   floor->set_name("floor");
   floor->set_short( "floor" );
   floor->add_property( "determinate", "the " );
   floor->add_property( "no burial", 1);
   floor->set_long("This is the floor of the meeting.  It gives you the "
                   "right to speak during the meeting.  Please give this "
                   "to the next person to speak after you have finished.\n");
   return floor;
} /* create_floor() */

/**
 * Called to start a meeting.
 * @return 1 on success, 0 on failure
 */
int do_start() {
   if (_leader &&
       find_player(_leader) &&
       environment(find_player(_leader)) == this_object()) {
      add_failed_mess("There is already a meeting started with " + 
                      find_player(_leader)->the_short() +
                      " as the leader.\n");
      return 0;
   }

   _leader = this_player()->query_name();
   add_succeeded_mess("$N $V the meeting.\n");
   return 1;
} /* do_start() */

/**
 * This method is called when the gavel is going to banged.
 * @return 1 on success, 0 on failure
 */
int do_bang() {
   if (this_player()->query_name() != _leader) {
      add_failed_mess("Only the leader can bang the gavel!\n");
      return 0;
   }

   add_succeeded_mess("$N $V the gavel.\n\nSTOP STOP!\n\n");
   return 1;
} /* do_bang() */

/**
 * This method is called when the floor is recovered by the leader.
 * @return 1 on success, 0 on failure
 */
int do_recover() {
   if (this_player()->query_name() != _leader) {
      add_failed_mess("Only the leader can recover the floor.\n");
      return 0;
   }

   _floor->move(this_player());
   add_succeeded_mess("$N recovers the floor.\n");
   return 1;
} /* do_recover() */

/**
 * This method is called to finish the meeting.
 * @return 1 on success, 0 on failure
 */
int do_finish() {
   if (this_player()->query_name() != _leader) {
      add_failed_mess("Only the leader can stop the meeting!\n");
      return 0;
   }

   _leader = 0;
   add_succeeded_mess("$N $V the meeting.\n");
   return 1;
} /* do_finish() */

/** @ignore yes */
void event_exit(object ob,
                string mess,
                object to) {
   object *inv;

   if (living(ob)) {
      if (userp(ob) &&
          ob->query_name() == _leader) {
         tell_room(this_object(),
                   "The meeting finishes abruptly as the leader leaves.\n");
      }
      inv = deep_inventory(ob);
      if (member_array(_floor, inv) != -1) {
         _floor->move(this_object());
         tell_room(this_object(),
                      "The floor zips out of " + ob->the_short() +
                      "'s hands and flops onto the floor.\n", ({ ob }));
         tell_object(ob, "The floor zips out of your hands.\n");
      }
   }
} /* event_exit() */

/** @ignore yes */
void event_dest_me(object ob) {
   object *inv;

   if (living(ob)) {
      if (userp(ob) &&
          ob->query_name() == _leader) {
         tell_room(this_object(),
                   "The meeting finishes abruptly as the leader leaves.\n");
      }
      inv = deep_inventory(ob);
      if (member_array(_floor, inv) != -1) {
         _floor->move(this_object());
         tell_room(this_object(),
                      "The floor zips out of " + ob->the_short() +
                      " hands into flops onto the floor.\n", ({ ob }));
         tell_object(ob, "The floor zips out of your hands.\n");
      }
   }
} /* event_dest_me() */

/** @ignore yes */
void event_quit(object ob) {
   object *inv;

   if (living(ob)) {
      if (userp(ob) &&
          ob->query_name() == _leader) {
         tell_room(this_object(),
                   "The meeting finishes abruptly as the leader leaves.\n");
      }
      inv = deep_inventory(ob);
      if (member_array(_floor, inv) != -1) {
         _floor->move(this_object());
         tell_room(this_object(),
                      "The floor zips out of " + ob->the_short() +
                      " hands into flops onto the floor.\n", ({ ob }));
         tell_object(ob, "The floor zips out of your hands.\n");
      }
   }
} /* event_quit() */
