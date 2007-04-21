/**
 * Print out a cute little map if one exists.
 * @author Pinkfish
 * @started Wed Jul 17 23:51:41 PDT 2002
 */
inherit "/cmds/base";

int do_map() {
   string map;
   int* coords;
   object env;
   string handler;

   env = environment(this_player());
   if (!env)  {
      add_failed_mess("You are somewhere not on any map.\n");
      return 0;
   }

   if (this_player()->query_blinded()) {
      add_failed_mess("You are blind and cannot see.\n");
      return 0;
   }

   switch(this_player()->check_dark(environment(this_player())->query_light()))
   {
   case -2:
   case -1:
     return add_failed_mess("It is too dark to see anything.\n");
   case 2:
   case 1:
     return add_failed_mess("It is too bright to see anything.\n");
   }
   
   coords = env->query_terrain_coords();
   handler = env->query_terrain_handler();
   if (!coords  ||  !handler) {
      add_failed_mess("You can only do this in a terrain.\n");
      return 0;
   }

   map = handler->query_player_map_template(coords[0], coords[1], coords[2],
                                   env->query_visibility(env));
   if (!map) {
      add_failed_mess("There is no map available here.\n");
      return 0;
   }
   write(map);
   return 1;
}

mixed* query_patterns() {
   return ({ "", (: do_map :) });
}
