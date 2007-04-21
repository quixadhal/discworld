#include <config.h>

#define DOM_TITLE "Liaison Domain"
#define LORD "wyvyrn"
#define DOMAIN "liaison"
#define BOSS_ROOM "/w/wyvyrn/rooms/office"
inherit "/std/dom/cmn_mas";
object board;

void setup() {
   set_dom( DOMAIN );
   set_light( 100 );
   set_short( "Liaison Domain Common room" );
   add_property( "determinate", "the " );
   set_long("The Liaison common room looks like a comfortable place to "
            "come to after a hard day's liaising.  The large pile of stress "
            "balls sat in one corner and the drinks cabinet against a wall "
            "highlight this.  The various evil-looking devices scattered "
            "around the room seem to add character and even a slightly "
            "malicious overtone to the entire place.  There are also a "
            "few chairs scattered around the place providing welcome bum "
            "relief.  One even looks to be occupied.\n"
            "A sign is hanging on the wall.\n");
   add_alias("balls", "ball");
   add_alias("devices", "device");
   add_alias("bottles", "bottle");
   add_alias("chairs", "chair");
   add_alias("whips", "whip");
   add_alias("chains", "chain");
   add_alias("easy chair", "chair");
   add_alias("dolls", "doll");
   add_alias("armrests", "armrest");
   add_alias("cushions", "cushion");
   add_alias("strings", "string");
   add_alias("needles", "needle");
   add_item( ({"ball", "stress ball"}), "The pile of stress balls seems very "
            "inviting, albeit until you get up close and see that they are "
            "actually coated in tough plastic and tied to a pole with string - "
            "effectively ending their usefulness.  They look nice though.");
   add_item("string", "Each piece of string connects a ball to the pole, "
            "stopping the ball moving very far.  Cruel.");
   add_item("pile", "The pile of stress balls seems very inviting, albeit "
            "until you get up close and see that they are actually coated in "
            "tough plastic and tied to a pole with string - effectively "
            "ending their usefulness. They look nice though.");
   add_item("lock", "A big lock that looks completely unpickable, mainly due "
            "to it having no keyhole.  Nothing will unlock it, so you heavy "
            "drinkers are just out of luck.");
   add_item("corner", "A corner of the room. Where two walls, the ceiling "
            "and the floor meet.");
   add_item("pole", "There is a short pole hiding unobtrusively behind the "
            "stress balls and it is only through very close scruitiny that "
            "it is actually noticable at all.  The pole seems to have "
            "lots of strings coming out of it, each of which lead to a "
            "stress ball, limiting the distance it can travel.\n"
            "It is secured to the floor and cannot come out.");
   add_item("device", "Devices of differing shapes and sizes litter most "
            "of the walls - all seemingly to do with harming nasty players. "
            " From the voodoo dolls in one corner to the rack in another, "
            "not to mention the whips and chains.  Serious stuff.");
   add_item("whip", "I told you not to mention them.");
   add_item("chain", "I told you not to mention them.");
   add_item("bottle", "The bottles line the inside of the cabinet, all "
            "different shapes and sizes of bottles.  Green bottles, "
            "blue bottles, red bottles, bottles with caterpillers in, "
            "bottles with toads in, bottles with DUCKs in, bottles "
            "with labels on, bottles with warnings on, bottles with "
            "skulls and crossbones on, so basically, lots of bottles.");
   add_item("cabinet", "The drinks cabinet is leaning idly against a "
            "wall [as much as a drinks cabinet can] and seems to sneer "
            "at you [as much as a drinks cabinet can].  The polished "
            "glass of the cabinet windows show all the drinks that "
            "seem to look smug [as much as bottles can].  The large "
            "lock holding the cabinet shut makes the bottles "
            "completely out of range.\n"
            "If you are really desperate, "
            "clone /w/lucifer/misc/blasterbottle.c and go nuts.");
   add_item("glass", "It's just some glass in the cabinet.  Nothing "
            "special.");
   add_item("chair", "The easy chairs that line the walls are "
            "pretty nondescript: big "
            "deep cushions and fluffy armrests, but that's all "
            "they are - and ever will be: chairs.  "
            "As a profound sense of misery sweeps over you, you "
            "notice one of the chairs is occupied by a rather "
            "despondant looking man.");
   add_item("doll", "The voodoo dolls lie in a messy pile in a "
            "corner of the room, looking particularly morbid with "
            "a hundred different needles sticking out at various "
            "points of their doll bodies.  The faces of the dolls "
            "are hard to make out, seeming to shift from likeness to "
            "likeness as you look at them.  There is a barrier in front "
            "of the dolls, suggesting they shouldn't be touched.");
   add_item("barrier", "A thick looking piece of metal completely blocking "
            "all access to the dolls.  Most probably for the best, too.");
   add_item("needle", "The needles are -unsurprisingly- sharp things "
            "currently embedded into the dolls, no doubt causing extreme "
            "pain.  Well, at least they would if the dolls were real. "
            "Which they aren't.  Which is a good thing.");                  
   add_item("rack", "The rack is a vicious looking instrument that allows "
            "the human [or non-human] body to be stretched to completely "
            "wrong proportions.  This is something that should be used "
            "with extreme caution.  Happily, it appears not to have been "
            "used yet [there is a still plastic covering on it] and long "
            "may it stay that way.");
   add_item("covering", "Mercifully there is a covering on the rack, "
            "showing it has never been used.  Which is a good thing.");
   add_item("armrest", "It's an armrest.  You rest your arm on it.");
   add_item("wall", "It's a wall.  That's about it.");
   add_item("cushion", "It's a cushion. You rest your bum on it.");
   add_item("man", "The man seated in one of the easy chairs looks "
            "to have resigned himself to a lifetime there, and "
            "from what you can see there appears to be a deep "
            "groove in the chair from his body shape.\n"
            "He notices you looking at him and speaks slowly, like he "
            "has nearly forgotten how: 'Do you know how long Moonchild "
            "will be? I've been waiting here for over 5 years now! I "
            "missed the fall of quest XP and everything - just because "
            "I wanted my items back!'\n"
            "As the man starts to froth at the mouth, you hurriedly turn "
            "your back on him and leave him to his waiting.");
   add_sign("This is a sign.  But then you knew that, or at least we hope "
            "you did.\n",
            "Commands available here are:\n"
            "add <name>   : add <name> as one of your characters.\n"
            "remove <name>: remove <name> as one of your characters.\n"
            "list [<name>]: list characters added for you or <name>.", 0, 0, 0);
   board = clone_object("/obj/misc/board");
   board->set_datafile(DOMAIN);
   board->move(this_object());
   add_exit("office", "/w/wyvyrn/rooms/study", "door");
   add_exit("drum", CONFIG_START_LOCATION, "path");
   add_exit("corridor", "/d/liaison/cre_corridor", "door");
   add_exit("interview", "/d/liaison/utils/interview", "door");
   BOSS_ROOM->add_exit("common", "/d/"+DOMAIN+"/common", "door");
} /* setup() */

void init() {
   this_player()->add_command( "add", this_object(), "<word> <word>" );
   this_player()->add_command( "add", this_object(), "<word>" );
   this_player()->add_command( "remove", this_object(), "<word> <word>" );
   this_player()->add_command( "remove", this_object(), "<word>" );
   this_player()->add_command( "list", this_object(), "" );
   this_player()->add_command( "list", this_object(), "<word>" );
} /* init() */

int do_add( object *indir, string indir_match, string dir_match, 
           mixed *args, string pattern  ) {
   switch( pattern ) {
    case "<word> <word>":
      if ( master()->query_lord( this_player()->query_name() ) || 
           "/d/liaison/master"->query_deputy( this_player()->query_name() ) ) {
         if ( "/d/liaison/master"->add_member_alias( args[ 0 ], 
                                                    args[ 1 ] ) ) {
            this_player()->add_succeeded_mess( this_object(), 
                args[ 1 ] +" added as alias for "+ args[ 0 ] +".\n" );
            return 1;
         } else {
            this_player()->add_failed_mess( this_object(), 
                "Something went wrong.\n" );
            return 0;
         }
      } else {
         this_player()->add_failed_mess( this_object(), 
                "Only lords and deputies can add aliases to others.\n" );
         return 0;
      }
      break;
    case "<word>":
      if ( "/d/liaison/master"->add_member_alias( this_player()->query_name(), 
                                                 args[ 0 ] ) ) {
         this_player()->add_succeeded_mess( this_object(), 
                args[ 0 ] +" added as alias for $N.\n" );
         return 1;
      } else {
         this_player()->add_failed_mess( this_object(), 
                "Couldn't add "+ args[ 0 ] +" as alias for $N.\n" );
         return 0;
      }
      break;
   }
} /* do_add */

int do_remove( object *indir, string indir_match, string dir_match, 
           mixed *args, string pattern  ) {
   switch( pattern ) {
    case "<word> <word>":
      if ( master()->query_lord( this_player()->query_name() ) || 
           "/d/liaison/master"->query_deputy( this_player()->query_name() ) ) {
         if ( "/d/liaison/master"->delete_member_alias( args[ 0 ], 
                                                       args[ 1 ] ) ) {
            this_player()->add_succeeded_mess( this_object(), 
                    args[ 1 ] +" removed as alias for "+ args[ 0 ] +".\n" );
            return 1;
         } else {
            this_player()->add_failed_mess( this_object(), 
                "Something went wrong.\n" );
            return 0;
         }
      } else {
         this_player()->add_failed_mess( this_object(), 
                "Only lords and deputies can remove aliases for others.\n" );
         return 0;
      }
      break;
    case "<word>":
      if ( "/d/liaison/master"->delete_member_alias(
                 this_player()->query_name(), args[ 0 ] ) ) {
         this_player()->add_succeeded_mess( this_object(), 
                 args[ 0 ] +" removed as alias for $N.\n" );
         return 1;
      } else {
         this_player()->add_failed_mess( this_object(), 
                 "Couldn't remove "+ args[ 0 ] +" as alias for $N.\n" );
         return 0;
      }
      break;
   }
} /* do_remove */

int do_list( object *indir, string indir_match, string dir_match, 
           mixed *args, string pattern  ) {
   string *names, name;
   int i;
   
   switch( pattern ) {
    case "<word>":
      name = args[ 0 ];
      break;
    case "":
      name = this_player()->query_name();
      break;
   }
   names = "/d/liaison/master"->member_alias( name );
   name = capitalize( name );
   i = sizeof( names );
   if ( i ) {
      if ( i > 1 ) {
         while ( i-- ) {
            names[ i ] = capitalize( names[ i ] );
         }
         this_player()->add_succeeded_mess( this_object(), 
                "Aliases for "+ name +" are "+
                query_multiple_short( names )+".\n" );
      } else {
         this_player()->add_succeeded_mess( this_object(), 
                "Alias for "+ name +" is "+
                capitalize( names[ 0 ] )+".\n" );
      }
   } else {
      this_player()->add_succeeded_mess( this_object(), 
             "No aliases known for "+ name +".\n" );
   }
   return 1;
} /* do_list */

