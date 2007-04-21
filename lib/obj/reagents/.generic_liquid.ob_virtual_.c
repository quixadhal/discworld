#include <virtual.h>

void dest_me() { destruct( this_object() ); }

void create()
{
  seteuid( (string)"/secure/master"->creator_file( file_name( this_object() ) ) );
} /* create() */

object create_virtual_ob() {
   object clone;

  clone = (object)SERVER->create_virtual_object( "/obj/food.c", 1 );
  clone->add_property( VIRTUAL_NAME_PROP, "/obj/reagents/generic_liquid.ob#1" );
  call_other( clone, "add_adjective",  "some"  );
  call_other( clone, "add_alias",  "liquid"  );
  call_other( clone, "set_amount_types",  ([ "drop": ({ 1, "drops" }),              "teaspoon": ({ 20, "teaspoons" }),              "tablespoon": ({ 60, "tablespoons" }),              "ounce": ({ 120, "ounces" }),              "cup": ({ 960, "cups" }),              "pint": ({ 2400, "pints" }),                          "quart": ({ 4800, "quarts" }),                          "gallon": ({ 19200, "gallons" }) ])  );
  call_other( clone, "add_adjective",  ({ "drop", "drops", "teaspoon", "teaspoons", "tablespoon",                "tablespoons", "ounce", "ounces", "cup", "cups", "pint",                "pints", "quart", "quarts", "gallon", "gallons", "of" })  );
  call_other( clone, "set_pile_types",  ({ 50, "small", 100, "medium", "large" })  );
  call_other( clone, "set_continuous",   );
  call_other( clone, "set_liquid",   );
  call_other( clone, "add_property",  "determinate", "" );
   return clone;
} /* create_virtual_ob() */
