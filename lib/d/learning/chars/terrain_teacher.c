/* -*- LPC -*- */
#include <armoury.h>

#include "path.h"

inherit "/obj/monster";

#define TERRAIN_CONTROL "/d/admin/room/terrain"
#define PROPERTY "terrain lesson stage"
#define CROSS_IMPATIENCE 100
#define TAPS_IMPATIENCE 200
#define MAX_IMPATIENCE 300
#define COSTUME_NORMAL 0
#define COSTUME_DESERT 1
#define COSTUME_WET 2
#define COSTUME_COLD 3

int costume_normal();
int costume_desert();
int costume_wet();
int costume_cold();
protected void to_terrain();
protected void from_terrain(string to);

object hq;
object pupil;
int impatience;
int costume;
int lesson_step;
int speeching;

mixed *lessons =
({
   /* For each lesson, we give:
    * string for exit name from foyer
    * integer for costume
    * array of commands for lesson text.  These commands can be strings
    *    which will be fed to init_command(), function pointers which will
    *    be fed to call_out(), or 0 (which makes the teacher pause).
    * string for exit name back to foyer
    */
   /* Lesson #0: floating vs. fixed rooms, the set_terrain() function */
   ({ "grassyfield", COSTUME_NORMAL,
      ({
         "emote sniffs the air.",
         "smile",
         "'This is a very simple terrain.  Take a little time to look around "
            "at it, wander around, read some of the code.  There isn't a whole "
            "lot to it.  When you feel you know what's here, come back and tell "
            "me you're ready.  I'll wait until you say \"ok\" or "
            "\"ready\" where I can hear you.  If you accidentally wander back to "
            "the foyer, type 'grassyfield' to return here.",
         0,
         "'You should have noticed that there are two different rooms in this "
            "area: the room in which we stand now, with this quaint cottage "
            "behind me, and the rooms of the field.",
         "'All the rooms are very simple.  You should have read the code for "
            "them.  If you haven't, do it now.  I'll wait until you're ready.",
         0,
         "'I assume that you have written rooms, both indoors and outdoors, "
            "before.  You should have noticed two strange features to the files "
            "that make up this area: first that they both have a call to a "
            "function called set_terrain(), and second that they both are "
            "remarkably lacking in add_exit() calls!",
         "'In fact, excepting this one 'cottage' exit behind me, the files "
            "have no exits at all.  Nevertheless, you were able to walk around "
            "quite freely.",
         "'That is the primary purpose to the terrain handler: it manages your "
            "exits for you.",
         "'The call to set_terrain(), then is simply the means by which you "
            "can tell the terrain handler that it needs to manage this room.",
         0,
         "'There are two types of rooms that the terrain handler can manage: "
            "fixed rooms and floating rooms.  The code for both looks the same, "
            "but the terrain handler treats them very differently.",
         "'In order for the terrain handler to know how to handle each room, "
            "you have to tell it.  That's done in the terrain handler control "
            "room: /d/admin/room/terrain.  You can get there from the commonroom "
            "by walking one east then one southeast.  Personally, I prefer to "
            "use an alias.",
         "'We'll go to the terrain control room in a moment, so you can see "
            "how to use it.  For the moment, suffice to say that every room "
            "that has a call to set_terrain() also needs to be registered in "
            "the terrain control room.",
         0,
         "'In this terrain, we have the doorstep (a fixed room) and all the "
            "rooms of the field (floating).  The primary difference between "
            "floating and fixed rooms is that you can only have one of each "
            "fixed room in a terrain, but you can have any number of floating "
            "rooms.",
         "'In fact, you probably noticed that this area is a 3x3 grid.  We're "
            "in the center of the southern edge of that grid right now, at the "
            "doorstep.",
         "'When I created this terrain, I just said that the field should "
            "cover the entire 3x3 area.  Then I plopped the fixed room down here, "
            "and it automatically took the place of the field room that would "
            "have been here.",
         "'This is an important concept: floating rooms cover predefined "
            "regions, while fixed rooms are in one place.  When a particular "
            "location is in the region of a floating room _and_ is the location "
            "of a fixed room, the fixed room wins.",
         "'This makes it so you can define huge sweeping areas of the floating "
            "rooms, and just plop down your fixed rooms in strategic areas.",
         0,
         "'Now, when I say 'location', what I'm referring to is the coordinates "
            "of the room.  As a creator, you can look at a room, and the "
            "coordinates (or \"unset\") appear just before the description.  "
            "As you work on terrains, you'll find yourself looking at those "
            "coordinates a lot.",
         "'Walk around a bit, and see how the coordinates change as you move "
            "in the different directions.  You will see the first number "
            "increase when you walk north, and decrease when you walk south.  "
            "The second increases when you walk east, and the third increases "
            "whenever you go up.",
         "'So that's ( north, east, up ).",
         0,
         "'Ok, let's show you the terrain control room now.",
         (: this_object()->to_terrain() :),
         "'Ok, here we are in the terrain control room.",
         "'Feel free to look at the room here before I explain everything "
            "to you.  Well, not quite everything: that green box is an advanced "
            "device for a later lesson.",
         0,
         "'The first thing you need to do before using the terrain room is "
            "always, always, always, set the terrain name.  Do that with the "
            "\"terrain\" command.",
         "'Type \"terrain tutorial_grassy_field\".  Be very careful about "
            "the spelling, you have to get it exactly right.",
         "'If you're worried, just type \"terrain\" and the room will "
            "show you the current terrain",
         0,
         "'Now that you've set the terrain, you can list the rooms that I "
            "have set here.  Use \"list floating\" to see all the floating "
            "rooms, and \"list fixed\" to see all the fixed rooms.",
         "'As I've mentioned before, this terrain only has one fixed and "
            "one floating room.",
         0,
         "'When it lists the floating room, it tells you the name of the "
            "file, the coordinates of two opposite corners of the region, and "
            "one final number.  That last number is a priority.  We'll talk "
            "about that in the second lesson.  For now, ignore it.",
         "'In this terrain, we have the field room in the whole area where "
            "the first coordinate is between -100 and 100 (inclusive), the "
            "second coordinate is between -100 and 100 (inclusive), and "
            "the third coordinate is equal to 800000.",
         "'The only reason for that strange third coordinate was that I wanted "
            "to keep these rooms far away from the player areas.",
         "shrug",
         "'That number before the file name is just an index in the list of "
            "floating rooms.  Since we only have one floating room, it isn't "
            "very useful to us.",
         0,
         "'Both of our rooms inherit /std/room/outside.  That makes the rooms 100 "
            "units wide in each direction.  So in that range of coordinates, we "
            "can have 3 rooms: -100, 0, and 100 for each of the first two "
            "coordinates.  So the whole terrain is a 3x3 block.",
         0,
         "'Now looking at the fixed room, it gives you the name of the file "
            "and _one_ set of coordinates.  That's it.  There's only the one set "
            "of coordinates, because each fixed room can only be in one location.",
         "'If you try to add another fixed room with the same file, the "
            "terrain control room will gripe at you and refuse to do it.  You'll "
            "have to remove the old room first.  You do that with the \"remove\" "
            "command, but we'll get to that later.",
         0,
         "'Notice that the fixed room is at (-100, 0, 800000).  That is inside "
            "the range of coordinates that we have set for the floating rooms: "
            "the first and second coordinates are between -100 and 100, and the "
            "last coordinate is 800000.  But this is a fixed room, so it wins the "
            "fight for that location.",
         "'This is what I was talking about earlier, when I mentioned putting "
            "the doorstep in the middle of the southern edge of the square: -100 "
            "is the southernmost edge of the square, while 0 is in the middle of "
            "that edge.",
         0,
         (: this_object()->from_terrain(TERRAIN_TUTORIAL "grassystep") :),
         "'And here we are back at the field.",
         "'That's it for the basic terrain lesson.  Wander around a bit more "
            "to look at the things I have mentioned, and come back here when "
            "you're ready to finish this lesson up.",
         "'There are more lessons available to you, so if you're up to it, "
            "talk to me when we get back to the foyer, and I'll continue to teach "
            "you.",
         "smile",
         "'You've been an excellent pupil.",
         0,
         "'Good",
      }),
      "cottage"
   }),
   /* Lesson #1: Multiple levels of floating rooms.   The 'nothing' room */
   ({ "desert", COSTUME_DESERT,
      ({
         "stretch",
         "emote starts to sweat",
         "'Ah, the desert!",
         "'As in the field, take a moment to wander around here.  Use what you "
            "learned in the first lesson, and look a bit more closely at the rooms "
            "in this area.",
         0,
         "'You should have noticed that there are three different areas in this "
            "terrain: this spot here next to the tent, the desert, and an oasis.  "
            "If you didn't notice, look at it now.",
         0,
         "'Once again, we have a largely square area, this time 6 squares east "
            "and west, and 5 squares north and south.  As you might have guessed, "
            "I created this area by covering the whole square with the desert "
            "room, and then plopped down this room in the corner.  But the oasis "
            "squares are interesting.  Let's go to the terrain control room and "
            "see.",
         (: this_object()->to_terrain() :),
         0,
         "'Ok, set the terrain to \"tutorial_desert\".  If you've forgotten, "
            "that's \"terrain tutorial_desert\".",
         "'When you've done that, list the floating rooms to see some nifty "
            "stuff.",
         0,
         "'Notice how I've got two rooms here: sanddunes and sandoasis.",
         "'Look carefully at the coordinates for the rooms, and you'll see that "
            "the sandoasis rooms overlap the sanddunes rooms.\n",
         "'The important difference is that last number.  Sanddunes says that "
            "it is \"level 0\", while sandoasis says it is \"level 1\".",
         "'The level is a kind of priority: when two or more floating rooms are "
            "at the same coordinates, the one with the highest level wins, and "
            "will be the room that the terrain handler actually puts down.",
         "'So in this terrain, the far northwest corner (that's at "
            "200, 1000, 800000) is in both the sanddunes and the sandoasis "
            "areas.  But since sandoasis has a higher level, that location "
            "is an oasis.",
         0,
         "'This little wrinkle lets you overlay lots of different rooms, "
            "stacking them up, to get very complex shapes without having to "
            "plop down rooms one by one.",
         "'For example, suppose one room covers (-200, -200, 0) to "
            "(200, 200, 0), at level 0.  Then we put a second room from "
            "(-100, -100, 0) to (100, 100, 0) at level 1.  The first room "
            "makes a 5 by 5 ring around the second area, which is 3 by 3.",
         "'We can make that more complex by putting more of the first "
            "room down, from (0, 0, 0) to (0, 100, 0) at level 2.  Now the "
            "center area, instead of being a 3 by 3 square, is a kind of "
            "'C' shape.  It might be helpful to draw that out on a piece "
            "of graph paper, so you can see it better.",
         0,
         "'Using these levels, you can even make it so that a terrain has "
            "holes in it.  That's convenient when you want to force people "
            "to walk around something.  Just add a floating room with a high "
            "level, and use the special file name \"nothing\".",
         0,
         "'Well, that's it for this lesson.",
         (: this_object()->from_terrain(TERRAIN_TUTORIAL "sandtent") :),
         "'Come back again, and we'll talk about some of the pitfalls "
            "involved in designing terrains.",
         0,
      }),
      "tent"
   }),
   /* Lesson #2: Making sure the coordinates line up.  set_room_size() */
   ({ "mountaintop", COSTUME_COLD,
      ({ "rub hands",
         "shiver",
         "'Sorry about the cold, I never got around to putting a door on "
            "this cabin.",
         "frown",
         "shrug",
         "'Oh well.",
         "'This time, I don't want you to wander freely around the terrain: "
            "it's much more complex than the others you've explored, and I want "
            "to do this one one step at a time.",
         0,
         "'This room right here is a fixed room.  We have one more fixed room "
            "upstairs, and both are connected by the staircase behind me.  That "
            "exit was added automatically by the terrain handler, just as in "
            "the case of floating rooms, but here we have two fixed rooms whose "
            "exits are being managed.",
         "'We'll go to the terrain control room in a moment, but first I want "
            "to show you a few things.",
         "'Look at the coordinates for this room.  If you didn't see the "
            "coordinates when you came in to this room, 'look' now.",
         0,
         "up",
         "'Now compare the coordinates of this room to the room we were just "
            "in.",
         0,
         "'Previously, whenever you moved from one room to another, the "
            "coordinates changed by 100.  This time, they only changed by 20.",
         "'Previously, all our rooms were 100 units across.  These two "
            "rooms, however, are only 20.  That's because our previous rooms "
            "all inherited /std/room/outside, while these two cabin rooms inherit "
            "/std/room/basic_room.  The code assumes that indoor rooms tend to be smaller "
            "than the great outdoors.",
         "'One side effect of that is that you have to be careful when "
            "lining up rooms in the terrain control room.  Be sure to check "
            "that the positions you give your rooms line up right, so the "
            "terrain handler can generate the correct exits.",
         "'In this case, I had to make sure that I planted this attic "
            "exactly 20 units above the other room.",
         0,
         "'This gets a little bit hairy when you have an indoor room "
            "meeting an outdoor room.  Let's look at that now.",
         "down",
         "'Here's an indoor room",
         0,
         "west",
         "lsay And here's an outdoor room",
         0,
         "lsay This time the coordinates changed by 60.",
         "lsay \"60?!  Where'd 60 come from?\" I bet you ask?",
         "lsay I've already said that outside rooms are 100 units across, and "
            "indoor rooms are 20 units across.  So from the center of an outside "
            "room, the walls are all 50 units from you.  From the center of an "
            "indoor room, the walls are 10 units from you.  Normally, when you go "
            "from the center of one outdoor room to another, that's 50 units in "
            "the first room, and another 50 units in the destination room: 100 "
            "units total",
         "lsay Likewise, in an indoor room, you go 10 units in the first room, "
            "and another 10 units in the second room: 20 units total.",
         "lsay So the 60 units we just moved is 10 units in the indoor room "
            "where we started, plus another 50 units in the outdoor room where "
            "we ended up.",
         "lsay Of course, I had to take that into account in the terrain "
            "control room when I lined up these rooms.",
         0,
         "lsay Now, there's one more thing that I want to show you before "
           "we go to the terrain room.",
         "sw",
         "s",
         "lsay Here's a tree that I planted a few years ago.  We can climb "
            "it.",
         "up",
         0,
         "lsay Did you notice the change in coordinates this time?  70.",
         0,
         "lsay Now this tree that we're hanging from is an outdoor room, as "
            "you can tell by all the weather we're having.  However, it's not "
            "the usual size.",
         "lsay Do 'find -d setup() here', to read the setup function for this "
            "room.  Notice how I have a call to set_room_size() just before "
            "the call to set_terrain().  That set_room_size() controls how far "
            "the walls are from the center of the room.  So this room is twice "
            "that width: 40 units across and 40 units tall.",
         "lsay That should account for the coordinates changing by 70 when "
            "we climbed the tree: 50 as we left the lower room, and 20 coming in "
            "to this room.",
         0,
         "lsay You can change the size of any room you want this way, but "
            "make absolutely sure that you call set_room_size() _before_ "
            "you call set_terrain().  That is critically important, and all "
            "sorts of subtle bugs can happen, generally involving the wrong "
            "exits showing up, if you get that wrong.",
         "lsay Also, set_room_size() can take an array of 3 coordinates to "
            "control the size of the room separately in all three dimensions.",
         "lsay Unfortunately, the terrain handler doesn't understand that "
            "level of complexity, so don't ever do it with rooms you expect to "
            "have managed by the terrain handler.",
         0,
         "lsay ok, let's go to the terrain control room, and you can look "
            "at my work.",
         (: this_object()->to_terrain() :),
         "lsay Here we...",
         "blush",
         "' Heh.  Got so used to yelling over the wind...",
         "smile",
         "'Here we are in the terrain room.  Set the terrain to "
            "tutorial_mountain and list out the fixed rooms.",
         0,
         "'Notice the last coordinates on both.  That's what I mentioned "
            "before, that I had set the two 20 units apart.",
         "'Now list the floating rooms.",
         0,
         "'Here I've done something different.  The mountainsnows room "
            "makes a big square, 5 by 5.  In the very center of that square, "
            "there is a 'nothing' room.  You should remember from the last "
            "lesson, that when the filename of a floating room is 'nothing', "
            "the terrain handler puts no room at that location.  So here we "
            "have the floating terrain with a hole in the middle.",
         "'Inside that hole, I plopped down the cabin.  How it's not "
            "centered in that hole, it's actually to the west of the center.",
         "'That's because I needed to ensure that, with the differente "
            "sizes of the outside vs. inside rooms, things would still line up.",
         "'So when you walk out of the cabin, you walk west.  When you walk "
            "around it, the terrain handler doesn't give you any extra entrances "
            "to the cabin.",
         0,
         "'There's one extra thing I had to do to get the cabin to work right, "
            "and I'll show you that soon, but first notice the mountaintree "
            "rooms.  There shouldn't be any suprises with that, just remember "
            "that the tree rooms are a different size.",
         0,
         "'Ok, back to the cabin.",
         (: this_object()->from_terrain(TERRAIN_TUTORIAL "mountaincabin") :),
         "'Here we are, back in the ground floor of the mountain cabin.",
         "'Notice how we have 3 exits here: up, down, and west.  Obviously, "
            "there's no north, east, or south exits, because we have that "
            "'nothing' room right here.",
         "lsay Wrong!",
         "grin",
         "'That 'nothing' trick only works with floating rooms.  In order to "
            "ensure that this room doesn't have the north, east, and south exits, "
            "I had to add a special function to the room.",
         "'Type 'find -d query_exit_type() here'",
         0,
         "'In any room that is managed by the terrain handler, whether than "
            "room is a floating room or a fixed room, the terrain handler checks "
            "a function called query_exit_type() to see what type of exit it "
            "should create in each direction.",
         "'The arguments are a string for the direction of the exit (north, "
            "west, south, northeast, etc.) and a string containing the file name "
            "of the destination room.  These are both just like you would use "
            "in a call to add_exit().",
         "'The function returns a string for an exit type, just like the "
            "third argument to add_exit().",
         "'There is, of course, a wrinkle: if query_exit_type() returns "
            "\"none\", then that exit isn't created at all.",
         0,
         "'If the room doesn't have a function called query_exit_type(), "
            "then some default values are used.  Those defaults are in an array "
            "at the beginning of /obj/handlers/terrain_handler.  It's real "
            "easy to find.",
         "'There are a lot of things that can be done to modify the exits "
            "that the terrain handler builds, but that'll be the subject for the "
            "next lesson.",
         "'For now, wander around if you feel like it, and come back to me "
            "when you're ready to finish this up.",
         0,
      }),
      "down"
   }),
   /* Lesson #3: Changing the exit types. */
   ({ "grassyfield", COSTUME_NORMAL,
      ({ "laugh",
         "'In spite of dragging you out here like this, Sin hasn't actually "
            "scripted this lesson yet.  Sorry.  This would've been the fourth "
            "lesson, in which I talk about how you can modify the exits that "
            "the terrain handler makes for you.",
         "shrug",
      }),
      "cottage"
   }),
});

void setup() {
   object obj;
   object backpack;
   
   set_name("terrain teacher");
   add_alias("teacher");
   set_short("terrain teacher");
   add_property("determinate", "the ");
   add_property("unique", 1);
   set_long("This man is middle aged, and apparently human.  That's about "
            "all you can really see, because he's soaking wet, his hair is caked "
            "with leaves and mud, his clothes are torn, and his boots are caked "
            "with dirt.\n");
   set_race("human");
   set_guild("monk");
   set_level(50);
   set_gender(1);
   
   add_effect("/std/effects/other/wetness", 10000);

   set_respond_to_with( ({
      ({ "@say", "teach", "please" }), "#new_pupil",
      ({ "@say", "please", "teach" }), "#new_pupil",
      ({ "@say", "teach" }), "#new_rude_pupil",
      ({ "@say", ({ "ok", "ready" }) }), "#next_speech",
   }) );
   
   backpack = ARMOURY->request_item("large backpack", 80);
   if (!backpack) backpack = ARMOURY->request_item("black leather backpack", 80);
   if (!backpack) backpack = ARMOURY->request_item("small backpack", 80);
   if (backpack) backpack->move(this_object());
   
   obj = ARMOURY->request_item("large leather boots", 20);
   if (obj) obj->move(this_object());
   
   obj = ARMOURY->request_item("thermal underwear", 10);
   if (obj) obj->move(this_object());
   
   obj = ARMOURY->request_item("fawn cotton trousers", 50);
   if (obj) obj->move(this_object());
   
   obj = ARMOURY->request_item("leather belt", 50);
   if (obj) obj->move(this_object());
   
   obj = ARMOURY->request_item("black wool socks", 40);
   if (obj) obj->move(this_object());
   
   obj = ARMOURY->request_item("gaudy shirt", 35);
   if (obj) obj->move(this_object());
   
   obj = ARMOURY->request_item("brown felt hat", 10);
   if (obj) obj->move(this_object());
   
   obj = ARMOURY->request_item("white cotton toga", 80);
   if (obj) obj->move(backpack);
   
   obj = ARMOURY->request_item("yellow raincoat", 60);
   if (obj) obj->move(backpack);
   
   obj = ARMOURY->request_item("old black cloak", 72);
   if (obj) obj->move(backpack);
   
   obj = ARMOURY->request_item("amusing earmuffs", 100);
   if (obj) obj->move(backpack);
   
   init_equip();
   
   costume = COSTUME_NORMAL;
   hq = find_object(TERRAIN_TUTORIAL "foyer");
   impatience = 0;
   lesson_step = 0;
   speeching = 0;
} /* setup() */

int check_anyone_here() {
   if (pupil) return 1;
   else return ::check_anyone_here();
} /* check_anyone_here() */

void heart_beat() {
   ::heart_beat();
   
   if (!pupil) return;
   ++impatience;
   if (impatience == CROSS_IMPATIENCE) {
      init_command("emote is beginning to look a bit impatient.", 1);
   } else if (impatience == TAPS_IMPATIENCE) {
      init_command("emote begins tapping his foot.", 1);
   } else if (impatience == MAX_IMPATIENCE) {
      init_command("'That's it, you clearly don't really want to learn.  "
                   "I give up on you, " + pupil->short(), 1);
      call_out("come", 3 + costume_normal(), hq);
      pupil = 0;
      impatience = 0;
   }
} /* heart_beat() */

int add_follower( object who ) {
   if (who == pupil) {
      int lesson;
      
      impatience = 0;
      lesson = pupil->query_property(PROPERTY);
      if (lesson < 0 || lesson >= sizeof(lessons)) {
         init_command("think", 1);
         init_command("'Odd, you seem to have done more lessons than I know.", 2);
         init_command("'I'm afraid I have nothing to teach you.", 3);
         pupil->remove_property(PROPERTY);
         pupil = 0;
         return 0;
      } else if (::add_follower(who)) {
         int delay;
         
         switch (lessons[lesson][1]) {
          case COSTUME_NORMAL: delay = costume_normal(); break;
          case COSTUME_DESERT: delay = costume_desert(); break;
          case COSTUME_WET: delay = costume_wet(); break;
          case COSTUME_COLD: delay = costume_cold(); break;
         }
         init_command(lessons[lesson][0], delay + 2);
         lesson_step = 0;
         call_out("speech", delay + 10);
         return 1;
      } else {
         return 0;
      }
   } else {
      init_command("'For what reason do you want to follow me, " +
                   who->short() + "?  I'm not teaching you.", 0);
      return 0;
   }
} /* add_follower() */

void coming( object where ) {
   tell_room( where,
      "%^CYAN%^Someone says: One moment!  I'll be right down!%^RESET%^\n",
             ({ this_object() }) );
} /* coming() */

void annoyed_coming( object where ) {
  tell_room( where,
    "%^CYAN%^An annoyed someone says: I said I'm coming!  "
    "Jeez!%^RESET%^\n",
    ({ this_object() }) );
} /* annoyed_coming() */

void come( object where ) {
   hq = where;
   if (where != environment())
      move( where, this_object()->a_short() + " arrives",
           this_object()->a_short() + " stomps away" );
   init_command( "emote looks around", 1 );
   init_command( "sigh", 4 );
   init_command( "get sign", 7 );
   init_command( "emote looks at the sign", 9 );
   init_command( "sit on bench", 10 );
   init_command( "'What can I do for you?", 12 );
} /* come() */

void done() {
   int delay = costume_normal();
   int lesson;
   
   lesson = pupil->query_property(PROPERTY) + 1;
   if (lesson == sizeof(lessons)) {
      init_command("'congratulations, " + pupil->short() + ", you have "
                   "completed all the terrain handler lessons.", ++delay);
      pupil->remove_property(PROPERTY);
   } else
      pupil->add_property(PROPERTY, lesson);
   
   init_command("emote looks around", delay + 1);
   init_command("sigh", delay + 4);
   init_command("get sign", delay + 7);
   init_command("emote looks at the sign", delay + 9);
   init_command("sit on bench", delay + 10);
   
   pupil = 0;
   impatience = 0;
   speeching = 0;
} /* done() */

void speech() {
   int lesson = pupil->query_property(PROPERTY);
   int delay;
   
   delay = 0;
   speeching = 1;
   if (lesson_step == sizeof(lessons[lesson][2])) {
      init_command(lessons[lesson][3], delay + 3);
      call_out("done", delay + 4);
      call_out( (: speeching = 0 :), delay + 5);
      return;
   }
   do {
      if (functionp(lessons[lesson][2][lesson_step])) {
         call_out(lessons[lesson][2][lesson_step], ++delay);
      } else if (stringp(lessons[lesson][2][lesson_step])) {
         init_command(lessons[lesson][2][lesson_step], ++delay);
      }
      if (++lesson_step == sizeof(lessons[lesson][2])) {
         init_command(lessons[lesson][3], delay + 3);
         call_out("done", delay + 4);
         call_out( (: speeching = 0 :), delay + 5);
         return;
      }
   } while (lessons[lesson][2][lesson_step]);
   lesson_step++;
   init_command("emote waits for " + pupil->a_short(), ++delay);
   call_out( (: speeching = 0 :), ++delay);
} /* speech() */

void pupil_arrived( object where ) {
   if (pupil) return;
   if (!environment()) {
      if (find_call_out("come") != -1)
         call_out("annoyed_coming", 0, where);
      else {
         call_out("come", 10, where);
         if (find_call_out("coming") == -1)
            call_out("coming", 2, where);
      }
   } else if (environment() == where) {
      init_command("'Oh, a pupil!", 2);
   }
} /* pupil_arrived() */

protected void to_terrain() {
   move(TERRAIN_CONTROL, "The terrain teacher appears",
        "The terrain teacher snaps his fingers and vanishes");
   tell_object(pupil, "Something snags you through space\n");
   pupil->move_with_look(TERRAIN_CONTROL, pupil->short() + " appears",
                         pupil->short() + " looks surprised and disappears.");
} /* to_terrain() */

protected void from_terrain( string to ) {
   move(to, "The terrain teacher appears",
        "The terrain teacher snaps his fingers and vanishes");
   tell_object(pupil, "Something snags you through space\n");
   pupil->move_with_look(to, pupil->short() + " appears",
                         pupil->short() + " looks surprised and disappears.");
} /* from_terrain() */

protected int modesty() {
   int num;
   
   num = sizeof(filter_array(all_inventory(environment()),
            (: living($1) && $1->query_gender() != 1 :) ));
   if (num) {
      if (num > 1)
         init_command("'Ladies, would you please turn around for a "
                      "moment?", 1);
      else
         init_command("'Ma'am, would you please turn around for a "
                      "moment?", 1);
      return 10;
   } else return 0;
} /* modesty() */

int costume_normal() {
   int i;
   
   if (costume == COSTUME_NORMAL) return 0;
   i = modesty();
   init_command("remove backpack", ++i);
   init_command("remove toga, raincoat, cloak, earmuffs", ++i);
   init_command("put toga, raincoat, cloak, earmuffs in backpack", ++i);
   init_command("get trousers, belt, shirt, hat from backpack", ++i);
   init_command("equip", ++i);
   costume = COSTUME_NORMAL;
   return i;
} /* costume_normal() */

int costume_desert() {
   int i;
   
   if (costume == COSTUME_DESERT) return 0;
   i = modesty();
   init_command("remove backpack", ++i);
   init_command("remove raincoat, cloak, earmuffs", ++i);
   init_command("remove belt, hat, shirt", ++i);
   init_command("remove trousers", ++i);
   init_command("put trousers, belt, shirt, hat, raincoat, cloak, "
                "earmuffs in backpack", ++i);
   init_command("get toga from backpack", ++i);
   init_command("equip", ++i);
   costume = COSTUME_DESERT;
   return i;
} /* costume_desert() */

int costume_wet() {
   int i;
   
   if (costume == COSTUME_WET) return 0;
   i = modesty();
   init_command("remove backpack", ++i);
   init_command("remove toga, cloak, earmuffs", ++i);
   init_command("put toga, cloak, earmuffs in backpack", ++i);
   init_command("get trousers, belt, shirt, hat, raincoat from "
                "backpack", ++i);
   init_command("equip", ++i);
   costume = COSTUME_WET;
   return i;
} /* costume_wet() */

int costume_cold() {
  int i;
   
   if (costume == COSTUME_COLD) return 0;
   i = modesty();
   init_command("remove backpack", ++i);
   init_command("remove toga, raincoat, hat", ++i);
   init_command("put toga, raincoat, hat in backpack", ++i);
   init_command("get trousers, belt, shirt, cloak, earmuffs "
                "from backpack", ++i);
   init_command("equip", ++i);
   costume = COSTUME_COLD;
   return i;
} /* costume_cold() */

void next_speech( object who, string message ) {
   if (who != pupil) return;
   if (speeching) return;
   
   impatience = 0;
   call_out("speech", 1);
} /* next_speech() */

void new_pupil( object who, string message ) {
   if ( pupil ) {
      if ( pupil == who ) {
         impatience = 0;
         init_command("pat " + who->short(), 1);
         init_command("'That's alright.  I understand you can get "
                      "excited at times like this.  Be assured that I am going "
                      "to teach you, " + who->short(), 3);
      } else {
         init_command("'sorry, " + who->short() + ", but I am teaching " +
                      pupil->short() + " right now.", 1);
      }
   } else {
      pupil = who;
      set_heart_beat(1);
      init_command("'Yes, " + pupil->short() +
                   ", I would be happy to teach you.", 1);
      init_command("drop sign", 3);
      init_command("stand", 4);
      init_command("'follow me, if you will", 6);
      impatience = 0;
   }
} /* new_pupil() */

void new_rude_pupil( object who, string message ) {
   if (!pupil) {
      init_command("emote hums quietly to himself.", 2);
      init_command("mumble rude pupils", 5);
   }
} /* new_rude_pupil() */

mixed *stats() {
   return ::stats() +
      ({
         ({ "pupil", pupil ? pupil->query_name() : "no pupil" }),
         ({ "impatience", impatience }),
         ({ "costume", (costume == COSTUME_NORMAL) ? "normal" :
            ((costume == COSTUME_DESERT) ? "desert" :
             ((costume == COSTUME_WET) ? "wet" :
              ((costume == COSTUME_COLD) ? "cold" : "???"))) }),
         ({ "lesson step", lesson_step }),
      });
} /* stats() */
