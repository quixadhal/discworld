#include <weapon.h>
#include <armoury.h>

inherit "/obj/monster";

object *greeters = ({ });

#define GREETINGS ({ \
   "Welcome to Combat Boot Camp, $name$.",\
   "This here course is set up to teach you the basics of fighting.",\
   "You can attack this here dummy and I'll tell ya how well ya done.",\
   "Arr!  You have the look of a fine fighter, $name$." \
})

void setup() {
  //setup_nationality( "/std/nationality/morpork", "Ankh-Morpork" );
  set_language( "general" );
  basic_setup( "human", "warrior", 100 );
  set_gender( 1 );
  
  ARMOURY->request_item("leather jerkin", 30)->move( this_object() );
  ARMOURY->request_item("leather breeches", 30)->move( this_object() );
  ARMOURY->request_item("hard leather boots", 30)->move( this_object() ); 
   
  init_equip();

  
} /* setup() */

void whichone(string name) {
  switch(name) {
  case "xrazzicaz":
    set_name( "xrazzicaz" );
    add_alias( "xraz" );
    add_property( "determinate", "" );
    set_short( "Xrazzicaz" );
    set_long( "This is the former Warriors' Guild dummy.  After being replaced by "
              "the newer mechanical dummy, Xrazzicaz decided to stay on as an "
              "instructor.\n");
    break;
  case "zazzifraz":
  set_name( "zazzifraz" );
  add_alias( "zazz" );
  add_property( "determinate", "" );
  set_short( "Zazzifraz" );
  set_long( "This is Xrazzicaz' brother, a former Warriors' Guild dummy.  After "
            "being replaced by the newer mechanical dummy, Zazzifraz, like "
            "his brother, decided to stay on as an instructor.\n");
  break;
  case "john":
    set_name( "john" );
    add_property( "determinate", "" );
    set_short( "John" );
    set_long( "This is Xrazzicaz' other brother, John.  He was the Warriors' "
              "Guild dummy in Ohulan Cutash until he was replaced by a "
              "mechanical dummy.  Like Xrazzicaz and Zazzifraz he got a "
              "job teaching after retirement.\n");
  }
}

int attack_by(object thing) {
  write(this_object()->the_short() + " glares at you before swiftly knocking "
    "you to the ground.\n");
  this_object()->stop_fight(thing);
  thing->stop_fight(this_object());
  return 0;
} /* attack_by() */

int attack_ob(object thing) {
  this_object()->stop_fight(thing);
  thing->stop_fight(this_object());
  return 0;
} /* attack_ob() */

void init() {
  if(!this_player()) {
    return;
  }
  if(environment() == find_object("/room/rubbish")) {
    return;
  }
  if(this_player()->query_property("dead")) {
    return;
  }
  if(!this_player()->query_visible(this_object())) {
    return;
  }
  if(!userp(this_player())) {
    return;
  }
  if(this_player()->query_property("player")) {
    greeters += ({this_player()});
    call_out("do_greeting", 1, this_player());
  }
} /* init() */

void do_greeting() {
  string str;
  
  if(!sizeof(greeters)) {
    return;
  }
  greeters = filter(greeters, 
    (: $1 && environment($1) == environment(this_object()) &&
     $1->query_visible(this_object()) :));
  uniq_array(greeters);
  foreach(str in GREETINGS) {
    str = replace(str, "$name$", query_multiple_short(greeters));
    do_command("'" + str);
  }
  greeters = ({ });
} /* do_greeting() */

void stop_them( object attacker, object dummy ) {
  tell_object( attacker, one_short() +" stops you attacking "+
               (string)dummy->the_short() +".\n" );
  tell_room( environment(), one_short() +" stops "+
             (string)attacker->the_short() +" attacking "+
             (string)dummy->the_short() +".\n", attacker );
  attacker->stop_fight( dummy );
  dummy->stop_all_fight();
} /* stop_them() */
