inherit "/std/object";

void setup_route(string *ss, string c, string r);
string *notes = ({});

void setup() {

  set_name( "pole" );

  set_short("wooden pole");
  add_adjective( "wooden" );
  
  set_long("This is a wooden pole.  It's fairly unexciting.\n");
  set_weight( 10000 );

  add_property( "there", ( ({"casually placed in a corner",
                             "here",
                             "standing on the side of the road",
                             "trying to be noticed"})
                          [random(4)]));
  add_extra_look(this_object());

  reset_get();

} /* setup() */


void setup_route( string *stops, string colour, string route) {

  object note;
  string language; // Part of a Bad, Temp Solution
  string pathofhere;

  if(member_array(colour + " note", notes) != -1) {
    return;
  }

  notes += ({ colour + " note" });

  pathofhere = file_name(environment(this_object()));

  /* Right. The following is a horrible hack, it'd be much nicer if
   * there was a way to find out the correct language for an area,
   * preferrably through some sort of query() somewhere. :)
   *
   * - Danbala, Aug 2002
   */
  if(strsrch(pathofhere, "/d/am/") != -1) 
    language = "morporkian";
  if(strsrch(pathofhere, "/d/cwc/") != -1) 
    language = "agatean";
  if(strsrch(pathofhere, "/d/klatch/") != -1) 
    language = "djelian";
  if(strsrch(pathofhere, "/d/ram/") != -1) 
    language = "morporkian";
  if(strsrch(pathofhere, "/d/ram/Uberwald/") != -1) 
    language = "uberwaldean"; 
  if(strsrch(pathofhere, "/d/sur/") != -1) 
    language = "morporkian";

  note = clone_object(__DIR__ + "carriage_note"); 
  note->setup_route(stops, colour, route, language);
  environment(this_object())->add_hidden_object(note);
  environment(this_object())->add_property("carriage pole",
    this_object());   

  if(sizeof(notes)) {
    set_short("wooden pole with " + (query_multiple_short(map( notes,
    (: add_a($1) :)))) + " attached to it") ;
  }

} /*setup_route()*/

string extra_look() {

  if(sizeof(notes)) {
    return capitalize((query_multiple_short(map( notes, (: add_a($1) :)))) + 
      ((sizeof(notes) > 1) ? " are" : " is") + " attached to the pole.\n") ;
  }

  return "";

} /* extra look */

