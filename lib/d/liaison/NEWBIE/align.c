/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: align.c,v 1.10 2002/09/11 04:25:30 pinkfish Exp $
 *
 *
 */
/* Added no teleport property- 26-06-98- Shalla */
/* New room to allow new players to set their alignment when they start the
   game. Ceres
 */

#include "path.h"
inherit "/std/room/basic_room";
inherit PATH+"common";

object _wlever;
object _blever;
object _glever;
int do_pull(object lever);

void setup() {
  object sign;

  set_zone("Newbie");
  set_short("alignment room" );
  set_light( 75 );
  add_property("no teleport", 1);
  set_long( "This is a small, bare room that is quite empty apart from "
            "a lever stuck onto each of three walls and a small wooden "
            "sign placed in the centre of the room.\n"
            "A door to the east leads back to the foyer.\n"); 

  add_item(({"west wall", "south wall", "east wall"}),
           "A plain, flat wall with a lever sticking out of it.");
  
  sign=add_sign("A sign marked 'alignment'.\n",
                FOYER->format_sign(
           "Alignment is how good or evil you are.  This changes gradually "
           "depending on the things you do.  Because it changes so slowly "
           "you can choose to start out with a good, neutral or evil "
           "alignment.  To become evil, pull the black lever, to become "
           "good pull the white lever, and to stay neutral don't pull "
           "either lever, or pull the grey lever if you have already pulled "
           "one of the other ones.\n"),0,"sign","general");
  sign->set_short("sign");
  sign->add_adjective("wooden");

  /* Make it so you cannot godmother out of here */
  add_property("no godmother", 1);

  add_exit( "foyer", PATH + "foyer", "door");
  add_alias( "east", "foyer");

  room_chat(({10, 40, ({"A voice which seems to come from the black lever "
                        "says: Don't be a goody-goody, pull me!",
                        "A voice which seems to come from the white lever "
                        "says: You should be nice, pull me!",
                        "A voice which seems to come from the grey lever "
                        "says: It's all about balance in the end."})}));
}

void init() {
  if (_wlever) {
     this_player()->add_command("pull", _wlever, "<direct:object>",
                              (: do_pull(_wlever) :));
  }
  if (_blever) {
     this_player()->add_command("pull", _blever, "<direct:object>",
                              (: do_pull(_blever) :));
  }
  if (_glever) {
     this_player()->add_command("pull", _glever, "<direct:object>",
                              (: do_pull(_glever) :));
  }
  ::init();
}

void reset() {
  if(!_wlever) {
    _wlever = clone_object("/std/object");
    _wlever->set_name("lever");
      _wlever->add_adjective("white");
    _wlever->set_short("white lever");
    _wlever->set_long("This is a lever which is set into the wall.  It "
                     "radiates a soft white glow.  The closer you stand "
                     "to it the more peaceful you feel.\n");
    _wlever->reset_get();
    _wlever->add_property("there", "on the east wall");
    _wlever->move(this_object());
  }
  if(!_blever) {
    _blever = clone_object("/std/object");
    _blever->set_name("lever");
    _blever->add_adjective("black");
    _blever->set_short("black lever");
    _blever->set_long("This is a lever which is set into the wall.  It "
                     "is pure black and emanates a deep, blood-red glow.  "
                     "The closer you get to it the more angry and hateful "
                     "you feel.\n");
    _blever->reset_get();
    _blever->add_property("there", "on the west wall");
    _blever->move(this_object());
  }
  if(!_glever) {
    _glever = clone_object("/std/object");
    _glever->set_name("lever");
    _glever->add_adjective("grey");
    _glever->set_short("grey lever");
    _glever->set_long("This is a lever which is set into the wall.  It "
                     "is a dull grey and very boring.\n");
    _glever->reset_get();
    _glever->add_property("there", "on the south wall");
    _glever->move(this_object());
  }
}

int do_pull(object lever) {
  int align;
  
  if(lever == _glever) {
    add_succeeded_mess(({ "$N $V $I and feel an overwhelming urge to do "
          "absolutely nothing about the world's problems.\n",
          "$N $V $I and suddenly looks very apathetic.\n" }),
          ({ _glever }));
    this_player()->set_al(0);
    return 1;
  }
  
  align = 300 + random(400);

  if(lever == _wlever) {
    align = -align;
  }

  this_player()->set_al(align);

  if(lever == _wlever) {
    add_succeeded_mess(({ "$N $V $I and suddenly feel$s like hugging "
          "everyone.\n",
          "$N $V $I and looks bright and cheery.\n" }),
          ({ _wlever }));
  } else {
    add_succeeded_mess(({ "$N $V $I and get a sudden urge to pull the "
          "wings off butterflies and torture small furry creatures.\n",
          "$N $V $I and a dark scowl crosses $p face.\n" }),
          ({ _blever }));
  }
  return 1;
}
