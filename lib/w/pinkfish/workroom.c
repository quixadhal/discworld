#define CREATOR "pinkfish"
inherit "/std/room";

object button;

void setup() {
/* the short description is what you get in a room in brief mode.
 */
  set_short(CREATOR+"'s workroom");
/* you set this so you can see in the room.
 * for a description of light levels and what they mean type
 * help light
 */
  set_light(100);
/* the long description is the description you get when you type look
 * and when you work into a room in verbose mode.
 */
  set_long("The workroom of "+CREATOR+".  You can see a small wooden "+
           "desk sulking in the corner of the room, it has a beautifully "+
           "carved wooden rolly chair in front of it.  It looks like this "+
           "was just newly created, there is a smell of paint and "+
           "leather in the air.\n");
/* the aliases are used for the items.  When you type look at walls
 * it will query the wall item and print that description.
 */
  add_alias("wall", "walls");
  add_alias("baskets", "basket");
  add_alias("cloud", "clouds");
/* the items are the things in the room that you can look at. */
/* try and put lots of these in the rooms you make, it makes them much
 * more interesting.
 */
  add_item("chair", "The teak rolly chair has amazing carvings of dragons and "+
                    "... other things on it.  They almost seem alive, "+
                    "you especialy don't note the way the dragon "+
                    "blinks at you");
  add_item("desk", "The desk is made of mahogany with a touch of "+
                   "mountain ash.  The top has two little basket things "+
                   "on it labeled \"in\" and \"out\".  The in basket "+
                   "is rather small");
  add_item("dragon", "I told you, you dont notice it winking at you");
  add_item("in basket", "The in basket is full of small yellow forms "+
                        "with large red letters on them saying "+
                        "\"Important read me first\" and then not "+
                        "appearing to acutally have any thing else on "+
                        "them");
  add_item("out basket", "Is completely empty");
  add_item("basket", "There are two baskets sitting on top of the desk.  "+
                     "they have the words \"in\" and \"out\" written "+
                     "on them");
  add_item("paint", "You look around to see the source of the paint "+
                    "smell and you cant find it.  The walls? of the room "+
                    "ummm seem to be well shall we just twisting in a "+
                    "confused mish mash of shapes, you are sure they are "+
                    "not threatening");
  add_item("wall", "The walls are made up of a cloudy substance, white "+
                   "in nature that is roiling around the place where "+
                   "you are standing.  Great magic must be holding it "+
                   "back, every now and then you see a demonic head "+
                   "push its way out of the cloud roll its eyes and fade "+
                   "back into the cloud");
  add_item("cloud", "The cloud is white coloured with a few flashes "+
                    "of light bursting in it");
  add_item("demon", "The demonic heads that pop out are pretty nasty "+
                    "looking, they disapear with a snarl of strain");
/* adds all the exits to the room.
 * the first paramater is the verb used to get through the exit.
 * the second is where to go once that exit is opened.
 * and the third is the type of the exit.  For a better desription of
 * this read the docs on rooms.
 */
  add_exit("west", "/w/pinkfish/me_guild", "door");
  add_exit("common", "/w/common", "door");
  add_exit("drum", "/d/am/am/mendeddrum", "door");
  add_exit("post", "/d/am/am/post", "door");
/* these two add_alias alias the exits.  Now when you type north (or n)
 * you will go to the mended drum and south to the common room
 */
  add_alias("drum", "north");
  add_alias("common", "south");
/* this adds and exit back to your workroom into the common room
 */
  "/w/common"->add_exit(CREATOR, "/w/"+CREATOR+"/workroom", "door");
}

void reset() {
/* if the button does not exist (it has been dested or it hasnt been
 * created yet the varible is set to 0)  This makes sure you dont get
 * more than one button in your room.
 */
  if (!button) {
    button = clone_object("/obj/misc/button");
    button->move(this_object());
  }
}

void dest_me() {
/* the dest_me function is called when an object is destructed.  We trap
 * this so that when the room is updated you dont end up with lots of
 * buttons, which does look pretty silly.
 */
  if (button)
    button->dest_me();
/* for a description of what the :: does go and shoot someone.
 * I dont think I can explain it in less than ten pages, and you would
 * not understand it anyway.
 */
  ::dest_me();
}

void init() {
  ::init();
  add_action("listem", "listem");
  add_action("reset_bing", "reset");
}

int reset_bing(string str) {
  return (int)"/w/furball/workroom"->reset_peoples();
}

int listem(string str) {
  return (int)"/w/furball/workroom"->list_peoples(str);
}
