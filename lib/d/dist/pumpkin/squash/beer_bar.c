#include "path.h"
#include <armoury.h>
#include <shops/pub_shop.h>

inherit "/std/shops/pub_shop";

object *drunkards;
object bartender, barmaid;

int do_sit(string str);
int ref_to_serve(object player, int type);
void setup() {
   set_short("beer room");
   set_room_size(5);
   add_property( "place", "Pumpkin");
   set_light(60);
   set_theft_handler( HOSPITAL );
   set_zone("Flaming Cabbage");
   set_open_function( (: ref_to_serve :) );

   set_long("This is a beer bar.  Hence, all the bottles, signs and glasses "
      "are related to beer.  Maybe it could be possible to get something "
      "else, but most likely, beer (and of course the sort of healthy, "
      "nutritious food that goes along with beer) is all they sell here.  "
      "The customers, sitting on chairs around the tables, are all drinking "
      "beer and chatting merrily.  At one of the more secluded tables, "
      "some seedy-looking characters are whispering less merrily.  Of course, "
      "they too are enjoying a tasty beer.  A menu is tacked to the "
      "wall behind the counter.\n");

   add_item("floor", "The floor is sticky, and it seems like it'd be hard "
      "to walk out of here.  That could explain why some of the clientele "
      "here seem to have been drinking for days.");
   add_item("sign","There are a number of signs in this bar.  Most of "
      "them are tacky, sticky old cardboard signs pointing out the "
      "special benefits of drinking beer X or ale Y.");
   add_item("bottle","There are a vast amount of bottles on the wall "
      "behind the counter.  They all seem to be the same sort, and quite "
      "a few of them seem to be empty.  At least they catch the seedy "
      "light in a very pretty way.");
   add_item("glass","Beer glasses (the empty kind which are not overly dirty "
      "on the inside) are flooding the bar behind the counter.  Considering "
      "how empty, used glasses seem to disappear altogether, it is probably "
      "necessary to have quite a few glasses around."); 
   add_item(({"customer","clientele"}), "The people in here seem to be "
      "mainly concerned with the intake of beer.  They all look rather red "
      "around their noses and, for some reason, they all look rather "
      "happy.  All except those who wouldn't be caught dead doing something "
      "as silly as looking happy, of course.");
   add_item(({"seedy-looking character","character","corner"}), "In a "
      "corner of the bar, some people (who seem to melt in extraordinarily "
      "well into the dark shadows) are quietly drinking some beer, keeping "
      "a watchful eye on everything that happens");
   add_item(({"bar","counter"}), "The counter runs along one wall of the "
      "room.  All empty glasses put on it seem to disappear to some other "
      "dimension.  Only Dogbolter, the God of Empty Beer Pints, knows "
      "where that is.");
   add_item("wall", "The walls are sticky with dried in beer.  Somehow it "
      "has ended up on the walls, but there is nothing here to explain how.  "
      "On one of the walls there's a sign - it appears to be stuck there "
      "without the aid of a pin or anything...");
   add_item("ceiling", "The ceiling is sticky.  They must have had quite a "
      "party here recently.  Or maybe they had a party a long time ago and "
      "just haven't washed the ceiling since?");
   add_item( "wooden table", ({"long", "This is a rather nice wooden "
      "table.  At least it could be, somewhere underneath all that "
      "sticky beer.",
      "position","the wooden table"}));
   add_item( "small table", ({"long", "This table is made of wood, but a "
      "lot smaller than the other tables here.",
      "position","the small table"}));
   add_item( "sticky table", ({"long", "This large table is a lot stickier " 
      "than anything else in here, and that means it's pretty darn sticky!",
      "position","the sticky table"}));
   add_item( "old chair",
         ({ "long", "The chairs around the wooden table are old and worn.",
            "position","the wooden table"
         }) );   
   add_item( "small chair",
         ({ "long", "The chairs around the wooden table are old and worn.",
            "position","a chair at the wooden table"
         }) );   
   add_item("sticky beer", "It's just about everywhere.");

   add_menu_item("Spicy sausage", PUB_APPETISER, 396, "spicy sausage");
   add_menu_alias("sausage", "Spicy sausage");
   add_menu_item("Salad", PUB_MAINCOURSE, 796, "salad",
                 PUB_STD_SIDEPLATE);
   add_menu_item("Fish'n'chips", PUB_MAINCOURSE, 1196, "fish'n'chips",
                 PUB_STD_DINNERPLATE);
   add_menu_item("Ribs", PUB_MAINCOURSE, 1304, "ribs",
                 PUB_STD_DINNERPLATE);
   add_menu_item("Hot chicken sandwich", PUB_MAINCOURSE, 796, 
      "chicken sandwich");
   add_menu_alias("chicken sandwich", "Hot chicken sandwich");
   add_menu_item("Ham'n'cheese sandwich", PUB_MAINCOURSE, 796, "ham sandwich");
   add_menu_alias("ham sandwich", "Ham'n'cheese sandwich");
   add_menu_item("Unnameable liquid", PUB_ALCOHOL, 1704, "unnameable",
                 PUB_STD_PINT, 0, 1);
   add_menu_alias("unnameable", "Unnameable liquid");
   add_menu_item("CMOTD's Finest Brew", PUB_ALCOHOL, 1704, "finest brew",
                 PUB_STD_PINT, 0, 1);
   add_menu_alias("brew", "CMOTD's Finest Brew");
   add_menu_alias("cmotd", "CMOTD's Finest Brew");
   add_menu_item("Pumpkine Ale", PUB_ALCOHOL, 1838, "/obj/food/ale.food",
                 PUB_STD_PINT, 0, 1);
   add_menu_alias("ale", "Pumpkine Ale");
   add_menu_item("Troll Beer", PUB_ALCOHOL, 1838, "troll beer",
                 PUB_STD_PINT, 0, 3);
   add_menu_alias("beer", "Troll Beer");
   add_menu_item("Schlorg Beer", PUB_ALCOHOL, 1838, "schlorg beer",
                 "schlorg bottle", 0, 2);
   add_menu_alias("schlorg", "Schlorg Beer");

   add_exit("west", PATH + "squash9", "corridor");

} /* setup() */

object create_item( string word ) {
   object thing, thang;

   switch ( word ) {
 
   case "spicy sausage" :
      thing = clone_object( "/obj/food" );        
      thing->set_weight_per_bite(1);
      thing->set_name("sausage");
      thing->set_short("spicy sausage");
      thing->set_long( "The sausage, of a very questionable origin, seems "
         "to be designed to increase the thirst of whomever eats it.\n" );
      thing->set_value( 396 );
      thing->set_weight(5);
      /* thing->add_eat_effect( "SOMETHING THAT MAKES YOU THIRSTY", 120 ); *
       * so... I'd have to make a thirst effect :P */
      return thing;
   case "unnameable liquid" :
      thang = clone_object( "/obj/reagents/generic_liquid.ob" );
      thang->set_medium_alias("beer");          
      thang->set_name("unnameable");
      thang->set_short( "unnameable liquid" );
      thang->set_long( "This sludge is definitely better off without a "
           "name.  If someone was to try to give it a name, it'd have to be "
           "something along the lines of \"Stinking Mud\", \"Utterly "
           "Disgusting and Undrinkable Slush\" or maybe just \"Yuck!\".\n" );
      thang->add_eat_effect( "/std/effects/ingested/drunk", 40 );
      thang->add_eat_effect("/std/effects/ingested/nausea", 100);       
      thang->set_amount( 2000 );
      return thang;
   case "finest brew" :
      thang = clone_object( "/obj/reagents/generic_liquid.ob" );
      thang->set_name("brew");
      thang->set_medium_alias("beer");        
      thang->set_short( "CMOTD's Finest Brew" );
      thang->add_alias(({"finest brew","CMOTD's finest","CMOTD's brew"}));
      thang->set_long( "CMOT Dibbler has produced an amazing beer from "
           "Pumpkin, weird spices and possibly some more things.  Being "
           "Dibbler, he has of course made sure that the pint is rather "
           "a small one.\n" );
      thang->add_eat_effect( "/std/effects/ingested/drunk", 50 );
      thang->set_amount( 980 );
      return thang;
   case "troll beer" :
      thang = clone_object( "/obj/reagents/generic_liquid.ob" );
      thang->add_alias("beer");        
      thang->set_name("beer");
      thang->set_short("troll beer");
      thang->add_adjective("pint");
      thang->set_long("This is trollish beer.  You suspect a human "
            "shouldn't really be drinking this stuff.\n" );
      thang->add_alias( "trollbeer" );
      thang->add_adjective("troll");
      thang->add_eat_effect("/std/effects/ingested/drunk", 70);
      thang->set_amount( 2000 );
      return thang;
   case "schlorg" :
      /* the stuff that goes in the bottle */
      thang = clone_object( "/obj/reagents/generic_liquid.ob" );
      thang->set_name("schlorg");
      thang->set_short("Schlorg");
      thang->set_medium_alias("beer");
      thang->set_long("This beer must be \"forn\".  It has none of the "
           "characteristic muddiness that all beer brewed from Pumpkin water "
           "possesses.\n" );
      thang->add_eat_effect("/std/effects/ingested/drunk", 180);
      thang->set_amount(3900);
      thang->move(thing);
      thing->do_close();
      return thing;
      break;
   case "salad" :
      thing = clone_object( "/obj/food" );
      thing->set_name("salad");
      thing->add_property("food", 1);
      thing->set_short( "green salad" );
      thing->set_long( "This green salad is at least partially green, "
         "with interesting bits and thingies in it.\n" );
      thing->add_eat_effect("/std/effects/ingested/nausea", 10);        
      thing->set_value( 796 );
      thing->set_amount( 2000 );
      return thing;
   case "fish'n'chips" :
      thing = clone_object( "/obj/food" );
      thing->add_alias(({"fish","chips"}));
      thing->add_property("food", 1);          
      thing->set_name("cod");
      thing->set_short( "fish'n'chips" );
      thing->set_long( "It's hard to tell what is the fish and which of "
         "the small, sloppy pieces that are supposed to be the chips.  "
         "They all taste the same anyway.\n" );
      thing->add_eat_effect("/std/effects/ingested/nausea", 10);        
      thing->set_value( 1196 );
      thing->set_amount( 2000 );
      return thing;
   case "ribs" :
      thing = clone_object( "/obj/food" );
      thing->set_name("ribs");
      thing->add_property("food", 1);
      thing->set_short( "ribs" );
      thing->set_main_plural("ribs");
      thing->set_long( "These are ribs.  What animal(s) they come from is "
         "not of any interest, rest assured...  They are quite a lot smaller "
         "than the Real Pork Ribs that you can get in the posh restaurants "
         "in any case.\n" );
      thing->set_value( 1304 );
      thing->set_amount( 2000 );
      return thing;
   case "chicken sandwich" :
      thing = clone_object( "/obj/food" );
      thing->add_alias("sandwich");          
      thing->add_property("food", 1);
      thing->set_name("chicken");
      thing->set_short( "hot chicken sandwich" );
      thing->set_long( "The chicken on this sandwich is of the green sort.  "
         "It smells funny, moves funny and even sounds a bit funny.  The "
         "only nice thing to say about this sandwich is that it is indeed "
         "hot.  It should probably be eaten with caution, though.\n" );
      thing->add_eat_effect("/std/effects/ingested/nausea", 40);        
      thing->set_value( 796 );
      thing->set_amount( 2000 );
      return thing;
   case "ham sandwich" :
      thing = clone_object( "/obj/food" );
      thing->add_alias("sandwich");
      thing->add_property("food", 1);
      thing->set_name("ham");
      thing->set_short( "ham'n'cheese sandwich" );
      thing->set_long( "This sandwich has some ham and some cheese on it, "
         "which is why it is called a ham and cheese sandwich.  It has been "
         "baked in an oven.  A long time ago.  It is rather cold an clammy "
         "now.\n" );
      thing->set_value( 796 );
      thing->set_amount( 2000 );
      return thing;
   }

} /* create_object() */

object create_container(string name) {
   object thing;

   switch (name) {
   case "schlorg" :
     /* the bottle */
      thing = clone_object( "/obj/bottle" );
      thing->make_bottle("green", 1000);
      thing->set_value(1596);
      thing->set_long( (string)thing->query_long() + "This bottle has a "
               "slightly odd-looking porcelain cap, and no label at all, "
               "in the ordinary fashion of Schlorg-bottles discwide.\n");
      return thing;
   }
} /* create_container() */

void reset() {
   int i, j, x, y, z;

   string *kinds = ({"jolly","burly","fat","happy","drunk","very drunk", 
      "silly"});

   j = 5 + random(2);

   if (sizeof(drunkards) < 3) {
      drunkards = allocate(j);
   }
   for (i=0;i<(sizeof(drunkards));i++) {
      if (!drunkards[i]) {
         x = random(sizeof(kinds));
         y = random(4);
         drunkards[i] = clone_object("/obj/monster");
         drunkards[i]->set_name("drunkard");
         drunkards[i]->add_alias(kinds[x]+"drunkard");
         drunkards[i]->add_alias(kinds[x]+" drunkard");
         drunkards[i]->add_property("emote",1);
         drunkards[i]->add_move_zone("Flaming Cabbage");
         drunkards[i]->set_short(kinds[x]+" drunkard");
         drunkards[i]->set_main_plural(kinds[x]+" drunkards");
         drunkards[i]->add_plural("drunkards");
         drunkards[i]->add_plural(kinds[x]+" drunkards");
         drunkards[i]->set_gender(1+random(1));

         drunkards[i]->set_long("This "+kinds[x]+" drunkard seems very "
            "content with sitting here, chugging down beer after beer.  Who "
            "can blame " + drunkards[i]->query_objective() + ", really?\n");

         drunkards[i]->set_race("human");
         drunkards[i]->set_al(-300 + random(600));
         drunkards[i]->set_guild("fighters");
         drunkards[i]->set_class("fighter");
         drunkards[i]->adjust_con(10);
         drunkards[i]->adjust_str(5);
         drunkards[i]->adjust_dex(-1);
         drunkards[i]->set_level(75 + random(25));
         drunkards[i]->add_skill_level( "other.health", 50 );
         drunkards[i]->add_skill_level("fighting.combat.melee",
                                       20+random(20));
         drunkards[i]->add_skill_level("fighting.combat.special",30);
         drunkards[i]->adjust_money(0 + random(1),"Pumpkin dollar");
         drunkards[i]->adjust_money(10+ random(20),"Pumpkin pence");
         switch(y) {
            case 0:
            ((object)ARMOURY->request_item("dagger",30 +
                    random(30)))->move(drunkards[i]);
/*
            ((object)ARMOURY->request_item("cloth robe",30 +
                    random(30)))->move(drunkards[i]);
 */
            drunkards[i]->init_equip();
            drunkards[i]->do_command("tactics response parry");
            drunkards[i]->add_skill_level("fighting.combat.parry.held",
                                          50+random(100));
            break;
            case 1:
               ((object)ARMOURY->request_item("knife",70 +
                                              random(20)))->move(drunkards[i]);
               ((object)ARMOURY->request_item("green pants",70 +
                                              random(20)))->move(drunkards[i]);
               drunkards[i]->init_equip();
               drunkards[i]->do_command("tactics response parry");
               drunkards[i]->add_skill_level("fighting.combat.parry.held",
                                             50+random(20));
               break;
            default:
               ((object)ARMOURY->request_item("knife",30 +
                                           random(30)))->move(drunkards[i]);
               ((object)ARMOURY->request_item("green pants",70 +
                                              random(20)))->move(drunkards[i]);
               drunkards[i]->init_equip();
               drunkards[i]->add_property("emote", 1);
               drunkards[i]->do_command("tactics response parry");
               drunkards[i]->add_skill_level("fighting.combat.parry.held",
                                             20+random(20));
               drunkards[i]->load_chat(2,({
                  1, "'I need another beer.",
                  1, "'Bring me some beer, will you?",
                  1, "'I jolly well need beer!  Hurry up!"
                    }));
         }
         drunkards[i]->move("/room/void");
         y = random(4);
         z = 40 + random(90);
         call_out("make_drunk", z, drunkards[i], y, kinds[x]);
      }
   }
   if (!bartender) {
      bartender = clone_object( "/obj/monster" );
      bartender->set_name("bartender");
      bartender->set_short("sympathetic bartender");
      bartender->add_property("determinate","a ");
      bartender->set_long("This bartender looks just like you would "
         "expect in a place like this.  He has a huge red beard and seems "
         "to be laughing and smiling in a friendly manner at everyone all "
         "the time.\n");
      bartender->set_race("human");
      bartender->set_gender( 1 );
      bartender->set_al( -350 );
      bartender->set_class("fighter");
      bartender->set_level(100);
      bartender->do_command("tactics attitude offensive");
      bartender->do_command("tactics response parry");
      bartender->adjust_con(6);
      bartender->adjust_str(6);
      bartender->adjust_dex(3);
      bartender->add_skill_level( "other.health",55);
      bartender->add_skill_level("fighting.combat.melee.sharp",100);
      bartender->add_skill_level("fighting.combat.special",50); 
      ((object)ARMOURY->request_item("dagger",100))->move(bartender);
      ((object)ARMOURY->request_item("dagger",85))->move(bartender);
      ((object)ARMOURY->request_item("leather undershirt",80))->
         move(bartender);
      ((object)ARMOURY->request_item("leather breeches",80))->move(bartender);
      ((object)ARMOURY->request_item("white apron",80))->move(bartender);
      bartender->add_respond_to_with( ({({"@spank"}),"bartender"}),
         "thank $hname$ profusely");
      bartender->load_chat(25,({
                  1, ":brings some beer out to his thirsty customers.",
                  1, "@wink barmaid",
                  1, "@beam brightly",
                  1, "@hum",
                  1, "@laugh",
                  1, ":laughs pleasantly.",
                  1, "'Well, well - what have we here?",
                  1, "'Bring you another beer, shall I?"
                    }));
      bartender->init_equip();
      
      barmaid = clone_object( "/obj/monster" );
      barmaid->set_name("barmaid");
      barmaid->set_short("lovely barmaid");
      barmaid->set_long("This barmaid looks happy with her work.  She smiles "
         "a lot and has a noticeable tendency to flutter her eye-lashes at "
         "the bartender.\n");
      barmaid->set_race("human");
      barmaid->set_class("fighter");
      barmaid->set_level(50);
      barmaid->set_gender(2);
      ((object)ARMOURY->request_item("white linen skirt",80))->move(barmaid);
      ((object)ARMOURY->request_item("white apron",80))->move(barmaid);
      barmaid->init_equip();

      call_out("make_bart", 20, bartender, barmaid);
   }

} /* reset */

void make_drunk(object ob, int y, string str) {

   int a = random(2);
   string adj, adv;

   adv = ({ "not at all", "very", "quite", "extremely", "rather" })
     [ random(5) ];

   ob->move(this_object(), "A "+  str + " drunkard comes in, looking " + adv +
          " thirsty.");
   
   switch (y) {
   case 0:
      adj = "small";
      break;
   case 1:
   case 2:
      adj = "wooden";
      break;
   case 3:
      adj = "sticky";
      break;
   }

   if (a)
      ob->do_command("sit at "+adj+" table");

   y = 5 + random(50);
   call_out("buy_new", y, ob);

} /* make_drunk() */

void make_bart(object ob, object obette) {
   ob->move(this_object(), "The bartender pops up from behind the counter "
          "with a wide grin on his face.");
   call_out("make_barm", 5, obette);
} /* make_bart() */

void make_barm(object ob) {
   ob->move(this_object(), "The barmaid appears from behind the counter and "
          "tries to look innocent.  She desperately tries to sort out her "
          "hair which is a bit unordered.");
} /* make_barm() */

void buy_new(object ob) {
   int tim;

   if (!undefinedp(ob)) {
      if (environment(ob) == this_object()) {
         int z;
         string beer;
         beer = ({ "troll beer", "brew", "brew", 
                      "unnameable liquid" })[ random(4) ];
         ob->adjust_money(5, "Pumpkin dollar");
         ob->do_command("buy " + beer );
         ob->do_command("hold glass in left hand");
         z = 120 + random(120);
         //
         // Make them take a sip every ten seconds or so...
         /* No! Way too spammy! I'll go for > 30 seconds though... *
          *  Danbala, feb 2000                                     */
         //
         tim = 20 + random(20);
         while (tim < z) {
            call_out("sip_beer", tim, ob);
            tim += 30 + random(20);
         }
         call_out("drink_beer", z, ob);
      }
   }
} /* buy_new() */

void sip_beer(object ob) {
   if (ob &&
       environment(ob) == this_object()) {
      ob->do_command("drink 1/3rd of glass");
   }
} /* sip_beer() */

void drink_beer(object ob) {
   if (!undefinedp(ob)) {
      ob->do_command("drink glass");
      ob->do_command("'Ha!  Nice beer, this.");
      call_out("no_litter",3,ob);
   }
} /* drink_beer() */

void no_litter(object ob) {         
   object pint, *obinv;
   int z;
   int a = random(2);

   if (!ob) {
      return ;
   } 
   obinv = deep_inventory(ob); 
   if (sizeof(obinv)) {
      foreach( pint in obinv ) {
	 if (pint->query_max_volume()) {
            switch (a) {
            case 0:
               ob->do_command("'Ah.  An empty pint.  Now what would I "
                       "want that for?");
               break;
            case 1:
               ob->do_command("'Bugger.  It's all empty.  Now I'm sad.");
               break;
            case 2:
               ob->do_command("'No more beer.  What has the world come to?");
               break;
	    }
            ob->do_command("put " + pint->query_name() + " on counter");
         }
      }
   }
   if (a) {
      z = 120 + random(240);
      call_out("buy_new", z, ob);
   } else {
      ob->do_command("north");
   }
} /* no_litter() */

int ref_to_serve(object player, int type) {
   int clock, servtime; 

   clock = (time()%(3600*24)); 

   servtime = ((clock > 35000) && (clock < 60000)); 

   /* Because we all know pubs don't really serve food when they say     *
    * they should, but at other mysterious times, when noone wants food  *
    * anyway.                                                            */

   if((!servtime) && (type == PUB_MAINCOURSE)) { 
      add_failed_mess("Unfortunately, this pub doesn't serve food at the "
         "moment.  Try to change your order, please.\n"); 
      return 2; 
   } 
} /* ref_to_serve() */ 

void event_enter(object ob, string str, object from) {

   if (ob->query_name() == "drunkard") {
      if( sizeof( match_objects_for_existence( "pint", ob ))) {
         ob->do_command("'Time for another beer for me.");
         call_out("buy_new",5 + random(50),ob);
      }
   }  
} /* event_enter() */
