#include <terrain_map.h>

inherit TERRAIN_MAP_AIR_BASE;

void setup() {
   set_long("Flying up high in the air.");
   set_short("air room");
}

void terrain_setup() {
   int* coords;
   int* ground_coords;
   string ground_room;
   string ground_base_str;

   coords = query_terrain_coords();

   ground_room = query_terrain_handler()->query_ground_room(coords[0], coords[1]);
   ground_coords = ground_room->query_terrain_coords();

   switch (coords[2] - ground_coords[2]) {
   case 0..100 :
      ground_base_str = ground_room->query_long();
      set_long("Down below you see: " + ground_base_str);
      break;
   case 101..200 :
      set_long("Flying up high above " + ground_room->query_short() + ".");
      break;
   default :
      set_long("High up in the sky.");
      break;
   }

}

void event_enter(object ob) {
   // Flying or floating.
   if (ob->query_property("flying")) {
      return ;
   }
   call_out("plumet_down", 2, ob);
}

void plumet_down(object ob) {
   int* coords;
   int* ground_coords;
   class coord coords_real;
   class coord ground_coords_real;
   int z;
   int damage;
   int dist;
   object room;

   if (environment(ob) != this_object() || ob->query_property("flying")) {
      return ;
   }
   coords = query_terrain_coords();
   ground_coords = query_terrain_handler()->query_ground_room_co_ords(coords[0], coords[1]);
   coords_real = query_terrain_handler()->query_real_from_terrain_coord(coords[0], coords[1], coords[2]);
   ground_coords_real = query_terrain_handler()->query_real_from_terrain_coord(ground_coords[0], ground_coords[1], ground_coords[2]);
   dist = coords_real->z - ground_coords_real->z;
   for (z = coords[2] - 1; z > ground_coords[2]; z--) {
      room = query_terrain_handler()->query_room_at(coords[0], coords[1], z);
      if (room) {
         tell_room(room, ob->the_short() + " plumets by on the way to the "
                   "ground.\n");
      }
   }
   // Hit the ground :)
   room = query_terrain_handler()->load_room_at(coords[0], coords[1], ground_coords[2]);
   tell_object(ob, "%^BOLD%^%^RED%^You find it hard to float and make a quick trip " +
                   query_terrain_handler()->query_distance_str(dist) +
                   " downwards.%^RESET%^\n");
   // Figure out the damage.
   ob->move_with_look(room, "$N plumets down out of the sky.",
                  "$N plumets down out of the sky.");
   damage = 2 * dist;
   damage -= ob->query_ac( "blunt", damage );
   ob->adjust_hp(damage, this_object(), this_object());
   tell_object(ob, "%^BOLD%^%^RED%^Ouch, that hurt.%^RESET%^\n");
}

string query_death_message() {
   return "from forgetting they cannot fly";
}

int query_theft_command() {
   return 1;
}

int can_enter_from(string key, string direc) {
   return 0;
}

int can_exit_to(string key, string direc) {
   return 0;
}
