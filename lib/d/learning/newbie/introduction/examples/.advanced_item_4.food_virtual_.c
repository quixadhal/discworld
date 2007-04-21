#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/food.c", 1 );
  clone->add_property( "virtual name", "/d/learning/newbie/introduction/examples/advanced_item_4.food#1" );
  call_other( clone, "set_name", "sandwich"  );
  call_other( clone, "set_short", "monkey sandwich"  );
  call_other( clone, "add_adjective", ({ "monkey" })  );
  call_other( clone, "set_value", 0  );
  call_other( clone, "set_weight", 1  );
  call_other( clone, "set_long", "This is a monkey sandwich.  Made from real monkey.  Mmm!\n"  );
  call_other( clone, "add_eat_effect",  "/std/effects/ingested/poison", 600 );
   return clone;
} /* create_virtual_ob() */
