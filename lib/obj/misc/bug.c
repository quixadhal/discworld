inherit "/std/object";

int no_shots;

void setup() {
   no_shots = 10;

   set_name( "bug");
   set_long( "It sits there, happier than a clam, its long black silvery "
             "legs sticking out the side.  The legs look fixed in place and "
             "there are some silvery words carved onto the top.  When you "
             "shake it, it sounds like it has something rattling around "
             "inside.  There appears to be a spray nozzle at one end.\n");
   add_read_mess("68000\n", "silver writing", "general", 0);
} /* setup() */

void init() {
  add_command("shake", "<direct:object>");
  add_command("spray", "<indirect:object> with <direct:object>");
} /* init() */

int do_shake() {
   this_player()->add_succeeded_mess(this_object(),
      "$N $V $D and you can hear something rattling around inside.\n", ({ }));
   return 1;
} /* do_shake() */

int do_spray(object *obs) {
   int i;
   int done;

   if (no_shots <= 0) {
      return 0;
   }

   done = 0;
   for (i = 0; i < sizeof(obs); i++) {
      if (no_shots > 0 && obs[i] != this_object()) {
         obs[i]->add_effect("/std/effects/object/bug_effect", 0);
         no_shots--;
         this_player()->add_succeeded(obs[i]);
         done = 1;
      }
   }
   return done;
} /* do_spray() */

mapping query_dynamic_auto_load() {
   mapping map;

   map = ([ "::" : ::query_dynamic_auto_load(),
            "shots" : no_shots ]);
   return map;
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping map, object foo) {
   ::init_dynamic_arg(map["::"]);
   no_shots = map["shots"];
} /* init_dynamic_arg() */
