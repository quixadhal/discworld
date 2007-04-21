
inherit "/std/object";

void setup_route(string* a, string b, string c, string d);

void setup() {

  set_name( "note" );

  set_short(  "Test note" ) ;

  set_long( "This note is probably broken, please contact a "
    "creator. :)\n" ); 
  reset_get();

  add_alias( "table" );

} /* setup() */

void setup_route(string* stops, string colour, string route, string language) {

  string prettystops = "";
  int i;

  set_short(route + " table");
  add_adjective(map(explode(route, " "), (: lower_case($1) :))); 
  add_adjective(colour);
  add_alias("table");
  add_alias("cardboard");
  add_adjective(({"piece", "of", colour}));

  set_long("This is the table for the " + route + " route.  It is "
    "printed on a piece of sturdy " + colour + " cardboard, and stuck "
    "firmly to the pole.\n");   

  for(i = 0; i < sizeof(stops) - 1; i++) {
    prettystops += capitalize(stops[i]) + "\n";
  }
  prettystops += "and back to " + stops[sizeof(stops) - 1];

  add_read_mess("\nThe " + route + " route:\n" +
    prettystops + ".\n",
    "bold letters", language);

} /* setup_route() */


