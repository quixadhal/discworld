/* this is the room handler... it keeps track of all sorts of silly
 * things...
 */


mapping exit_types;
mapping door_types;
mapping opposite;

/* ({ mess, obv, size, func }) */
void create() {
  exit_types = ([
   "standard" :({ 0, 1,  10, 0 }),
   "corridor" :({ 0, 1,  10, 0 }),
   "plain"    :({ 0, 1, 100, 0 }), /* very large */
   "door"     :({ 0, 1,   5, 0 }),
   "stair"    :({ 0, 1,   5, 0 }), /* going up? */
   "hidden"   :({ 0, 0,   5, 0 }), /* hidden non door exit */
   "secret"   :({ 0, 0,   5, 0 }), /* secret door */
   "gate"     :({ 0, 1,  15, 0 }),
   "road"     :({ 0, 1,  30, 0 }),
   "path"     :({ 0, 1,  20, 0 }),
  ]);
  door_types = ([
    "door"    :({ 0, 0, "generic_key", 100 }),
    "secret"  :({ 0, 0, "generic_key", 100 }),
    "gate"    :({ 1, 0, "generic_key", 100 }),
  ]);
  opposite = ([ "north":({0,"the south"}), "south":({0,"the north"}),
                "east":({0,"the west"}),  "west":({0,"the east"}),
                "northeast":({0,"the southwest"}),
                "northwest":({0,"the southeast"}),
                "southeast":({0,"the northwest"}),
                "southwest":({0,"the northeast"}),
                "up":({0,"below"}), "down":({0,"above"}),
                "out":({0,"inside"}), "in":({0,"outside"}),
                "exit":({0,"inside"}), "enter":({0,"outside"}) ]);
}

int add_exit_type(string type, mixed message, mixed obvious, int size,
                  mixed func) {
  if (exit_types[type])
    return 0;

  exit_types[type] = ({ message, obvious, size, func });
  return 1;
}

int remove_exit_type(string type) {
  exit_types = m_delete(exit_types, type);
  return 1;
}

mixed *query_door_type(string type, string direc, string dest) {
  int i;
  mixed s;

  if (!door_types[type])
    return 0;

/* If there isnt a door on the other side.  We don't join. */
  call_out("check_door", 0, ({ previous_object(), direc, dest }));
  return door_types[type] + ({ 0 });
}

void check_door(mixed bing) {
  mixed frog;

  if (!find_object(bing[2]))
    if (catch(call_other(bing[2], "??"))) { /* room failed to load. */
      bing[0]->modify_exit(bing[1], ({ "undoor", 0 }));
      return ;
    }
  frog = (mixed)bing[2]->query_door(bing[0]);
  if (!frog)
    bing[0]->modify_exit(bing[1], ({ "undoor", 0 }));
  else
    bing[0]->modify_exit(bing[1], ({ "other", frog }));
}

mixed *query_exit_type(string type, string dir) {
  int i;
  mixed s;

  if (!(s = opposite[dir])) {
/* Commented out...
    log_file("BAD_DIRS", "Type: " + type + ", Dir: " + dir + ", Caller: " +
      file_name(previous_object()) + ".\n\n");
 */
    s = ({ 0, "elsewhere" });
  }
  if (!exit_types[type])
    return exit_types["standard"] + ({ s });
  return exit_types[type] + ({ s });
}
