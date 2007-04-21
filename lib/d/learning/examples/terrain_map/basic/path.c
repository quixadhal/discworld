#include <terrain_map.h>

inherit TERRAIN_MAP_OUTSIDE_BASE;

// Important that we return the pathname to our handler.
string query_handler_path() { return __DIR__ + "field_handler"; }

// A standard room setup.
void setup()

{
  set_short("trampled path");
  set_long("This path meanders through a large grassy field.\n");

  add_item("path", "Trampled down by the hooves of time.");
  add_item("field", "Large and grassy.");
  add_item("grass", "It's green, which should come as no surprise at all.");
}
