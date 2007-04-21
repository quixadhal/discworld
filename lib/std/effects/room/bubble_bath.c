/*  Object:         Bubblebath!
    Author:         Wobin
    Description:    Makes funtime bath! ... err... or something...
                    Uses the argument 
                    ({ <expiry time>, ({ <colour>, <fragrance> }) })
*/
                    
#include <effect.h>
#define PATH __DIR__

string query_classification() { return "room.bubblebath"; }

int query_surface_water(object room) {
    return room->query_surface() ||
     sizeof(filter(all_inventory(room), (: $1->query_baths() :)));
}

void beginning( object room, mixed *args) {
    object uproom;

   tell_room( room, "As the liquid enters the water it diffuses into " 
         "a "+ args[1][0] +" coloured cloud.\n" );
   if(query_surface_water(room))
       tell_room( room, "A gentle fragrance of " + args[1][1] + 
          " fills the room.\n");
   else if(uproom = find_object(room->query_destination(room->query_up_dir())))
       uproom->add_effect(PATH + "bubble_bath", args);

   room->submit_ee( "bubblebubble", ({10,30}), EE_CONTINUOUS );
   room->submit_ee( 0, args[0], EE_REMOVE );

   room->add_extra_look( this_object() );
   room->add_item(args[1][0] + " bubbles", "These " + args[1][0] + " coloured "
           "bubbles fill the room.");
} /* beginning() */

/** @ignore yes */
mixed *merge_effect( object room, mixed *old_arg, mixed *new_arg, int id ) {

  int index,count,copy=0;
  string type;
  object uproom;

   tell_room( room, "The bubbles surge as more bubble bath is added.\n" );
   room->submit_ee( 0, new_arg[0] + (int)room->expected_tt(), EE_REMOVE );

   count = sizeof(old_arg);
   for( index = 1; index < count; index++) {
     if( new_arg[1][0] == old_arg[index][0] ) {
       copy = 1;
       break;
     }
   }

   if(!copy) {
     old_arg += ({ new_arg[1] });
     room->add_extra_look(this_object());
     type = (room->query_surface()?"air":"water");
     room->add_item( new_arg[1][0] + " bubbles", "These " + new_arg[1][0] + 
        " coloured bubbles fill the " + type + ".");
     tell_room( room, "As the liquid enters the water it diffuses into " 
         "a "+ new_arg[1][0] +" coloured cloud.\n" );
     if(query_surface_water(room))
     tell_room( room, "A gentle fragrance of " + new_arg[1][1] + 
          " fills the air.\n");
     else if(uproom = find_object(room->query_destination(room->query_up_dir()))) {
         uproom->add_item( new_arg[1][0] + " bubbles", "These " + new_arg[1][0] + 
        " coloured bubbles fill the " + type + ".");
         uproom->add_effect(PATH + "bubble_bath", new_arg);
     }
     return old_arg;
   }

   return 0;
} /* merge_effect() */

/** @ignore yes */
void end( object room, mixed *args, int id) {
    int index, count;
    string type =(room->query_surface()?"fragrant":"colourful");
    tell_room( room, "The last of the bubbles disappear with a " + type +
             " *pop*.\n" );
    room->remove_extra_look( this_object() );
    
    count = sizeof(args);
    for (index = 1; index < count; index++ )
       room->remove_item( args[index][0] + " bubbles" );
       
} /* end() */

string extra_look( object room ) {
    
  int *enums;  /* effects list */
  int count, size;
  mixed  shades=({}); /* list of colours */
  string bubblez;  /* description to return */
  string type;

  enums = room->effects_matching( query_classification() );
  type = (room->query_surface()?"smelling":"looking"); 
  if ( !sizeof ( enums )) return "";

  /* Since there should be only one effect... */  
  shades = room->arg_of( enums[0] );

  if ( !sizeof ( shades )) return "";
    
  if ( sizeof ( shades ) == 1 )
      bubblez = "A multitude of " + shades[1][0] +" coloured bubbles fill the "
      "room, " + type +  " delicately of " + shades[1][1] + ".\n";

  if ( sizeof ( shades ) > 1 ) {
    size = sizeof ( shades );
    bubblez = "The room is filled with " + shades[1][0];
    for ( count = 2; count < size; count++ ) {

        if ( count != size-1 ) {
            bubblez += ", ";
        }
        else {
            bubblez += " and ";
        }           
            bubblez += shades[count][0];
    }
    bubblez += " coloured bubbles, floating everywhere.\n";
  }
      return bubblez;

} /* extra_look() */

void bubblebubble( object room, mixed *args) {
    int index = 0;
    string type = (query_surface_water(room)?"fragrance":"colours"); 
    string where= (query_surface_water(room)?"air":"water"); 
    /* Get a random index that isn't 0 */
    while (!index) index = random(sizeof(args));
    tell_room( room, "The gentle " + type + " of " + args[index][1] + " floats "
               "through the " + where + " as some of the " + args[index][0] + " bubbles "
               "pop.\n");
}               

