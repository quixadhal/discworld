/* this thingie spits out monsters for you, given its name.
 * please use it for when you want several identical monsters in different room
 * Please avoid giving monsters their own files, and just clone /obj/monster
 * from a room if you don't want to use this thing.
 * -- the Memory Monster...
 */

#define MONSTER "/obj/monster"
#define ARMOURY "obj/handlers/armoury"
#define HOSPITAL "/d/am/hospital"
#define THIEVES_SHOP "/d/am/thief/shop"

inherit "std/room";

int *city_chance, city_tot_chance;
string *city_monsters;


object get_monster(string type) {
  object ob, ob2;
  int temp, rand, loop;

  if(!city_monsters) {
    city_monsters = ({
      "merchant", "mercenary", "rat", "beggar", "sailor",
      "officer", "wizard", "merchant", "warrior", "thief",
      "assassin", "person" });
  }
  if(!city_chance) {
    city_chance = ({
      1, 1, 5, 20, 5,
      3, 10, 10, 20, 5,
      5, 15 });
    city_tot_chance = 100;
  }
  switch(type) {
    case "shades":
    case "city":
      rand = random(city_tot_chance - city_chance[sizeof(city_chance) - 1]);
      temp = 0;
      loop = 0;
      while(rand > temp)
        temp += city_chance[loop++];
      ob2 = get_monster(city_monsters[loop]);
      ob2->add_move_zone("ankh morpork");
      ob2->add_property("monster_type", type);
/* Remember to comment this out after the move... */
      return ob2;
    case "under water" :
      ob2 = get_monster(({ "fish" })[random(1)]);
      return ob2;
    case "docks" :
      ob2 = get_monster(({ "sailor", "thief", "beggar",
                           "merchant" })[random(4)]);
      ob2->add_move_zone("ankh morpork");
      ob2->add_property("monster type", type);
/* Remember to comment this out after the move... */
      ob2->set_move_after(0,0);
      return ob2;
    case "wizard":
      ob = clone_object(MONSTER);
      ob->set_name("wizard");
      ob->set_short("Wizard");
      ob->set_main_plural("wizards");
      ob->set_race("human");
      ob->set_class("priest");
      ob->set_level(5+random(30));
      ob->set_gender("male");
      ob->add_move_zone("pub");
      ob->set_long(
"This is a wizard of the Unseen University.  He is a powerful wizard "+
"by the looks of his uniform.  You'd best leave him alone.\n");
      ob->adjust_money(random(80), "copper");
      ob->load_chat(2, ({
        1, "The wizard shuffles away from you.\n",
        1, "'Not now please, I'm very busy.\n",
        1, "'What did you say?\n",
        1, "'Have you seen a pipe?  One of the senior wizards " +
        "has lost his.\n",
        1, "@sigh",
      }));
      ob->set_wimpy(90);
      ob->load_a_chat(20, ({
        1, "'Die feeble mortal.\n",
        1, "'Do you know the powers you are messing with.\n",
        1, ":mutters some strange incantation.\n",
        1, "The crowd flee in terror.\n",
        1, ":misses you and a spectator vanishes.\n"
      }));
      ob->add_move_zone("unseen university");
      ob->add_property("monster_type", type);
      return ob;
    case "cityguard":
      ob = clone_object(MONSTER);
      ob->add_property("monster_type", type);
      ob->set_name("guard");
      ob->add_adjective("city");
      ob->set_short("city guard");
      ob->set_long("This is a city guard. He is supposed to protect the " +
        "city from undesirables. Sadly, Anhk-Morpork isn't very fussy.\n");
      ob->set_race("human");
      ob->set_gender("male");
      ob->set_guild("fighter");
      ob->set_level(8);
      ob->set_al(50);
      ob->give_money(150, 100);
      ob->load_chat(10, ({
          3, "@grumble bitterly",
          1, "@daydream",
          1, "@sigh heavily"
          }) );
      ob->load_a_chat(50, ({
          1, "'Ouch!",
          1, "'Come on! I don't need this.",
          1, "'Be on your way!",
          }) );
      ARMOURY->request_weapon("long sword", 40+random(40))->move(ob);
      if(random(2))
        ARMOURY->request_armour("hard leather cap", 70 + random(30))->move(ob);
      ob->init_equip();
      return ob;
    case "thief" :
      ob = clone_object(MONSTER);
      ob->set_name("man");
      ob->set_main_plural("men");
      ob->add_move_zone("pub");
      ob->add_move_zone("guild of thieves");
      ob->set_gender(random(2) + 1);
      ob->set_long(
"A small shifty looking man wearing dark coloured clothes.  He looks "+
"suspiciously around the room.\n");
      ob->adjust_money(30 + random(30),"copper");
      ob->add_triggered_action("bing", "pub_brawl", HOSPITAL, "pub_brawl");
      ob->add_triggered_action("bing2", "event_death", HOSPITAL, "death");
      ob->add_enter_commands(({ "get all", "drop corpses", "hide" }));
      ob->set_race("human");
      ob->set_class("thief");
      ob->set_level(1+random(50));
      ob->load_chat(2, ({
        1, "'Have you paid all your robbery tax this year?\n",
        1, "'Can you show me your reciepts?\n",
        1, ":wanders over to someone and threatens them.\n",
        1, "@peer sus at $lname$",
      }));
      ob->load_a_chat(30, ({
        1, "'Wait till my boss hears about this!\n",
        1, "'I'm not giving you a reciept for this one.\n",
        1, "'If you leave now, I might forget this incident.\n",
        1, ":tries to hide in the crowd, but fails.\n"
      }));
      ob->add_property("monster_type", type);
      ARMOURY->request_weapon("dagger",80+random(20))->move(ob);
      ARMOURY->request_armour("leather",80+random(20))->move(ob);
      ob->init_equip();
      return ob;
    case "merchant":
      ob = clone_object(MONSTER);
      ob->set_name("merchant");
      ob->set_short("Merchant");
      ob->set_race("human");
      ob->set_class("merchant");
      ob->set_level(5+random(4));
      ob->set_gender(random(2) + 1);
      ob->add_alias("trader");
      ob->add_move_zone("docks");
      ob->set_long(
"This is a merchant. "+capitalize((string)ob->query_pronoun())+
" comes to Ankh Morpork to sell "+ob->query_possessive()+" goods.\n");
      ob->adjust_money(10+random(20),"copper");
      ob->load_chat(5, ({
        1, "'My goods are cheapest.",
        1, "'Don't buy his, they are defective.",
        1, "'Don't buy her's, they are defective.",
        1, "'Alas, It'll be the death of me, but you can have it cheap.",
        1, "'What?  No you can't possibly have it cheaper.",
        1, ":shuffles over to you and mutters about what "+
           ob->query_pronoun()+" is selling.",
        1, ":wanders over to an unsuspecting victim and tries to " +
           "sell to her.",
        1, ":wanders over to an unsuspecting victim and tries to " +
           "sell to him."
      }));
      ob->load_a_chat(60, ({
        1, "'But I paid the thieves guild already.",
        1, "'The patrician will hear about this.",
        1, "'Why me?",
        1, "'Help, Officer save me.",
        1, ":hides under a table that you quickly destroy."
      }));
      if(random(4))
        ARMOURY->request_weapon("knife",40+random(40))->move(ob);
      ARMOURY->request_armour("cloth robe",40+random(40))->move(ob);
      ob->add_property("monster_type", type);
      ob->init_equip();
      return ob;
    case "assassin":
      ob = clone_object(MONSTER);
      ob->set_name("mercenary");
      ob->add_adjective("mean");
     ob->set_al(-350);
      ob->set_race("human");
      ob->set_class("assassin");
      ob->set_level(8+random(16));
      ob->set_gender(random(2) + 1);
      ob->set_short("Mean Mercenary");
      ob->set_main_plural("mean mercenaries");
      ob->add_plural("mercenaries");
      ob->add_triggered_action("bing", "pub_brawl", HOSPITAL, "pub_brawl");
      ob->add_triggered_action("bing2", "event_death", HOSPITAL, "death");
      ob->set_long(
"This is a mean mercenary.  "+capitalize((string)ob->query_pronoun())+
" is wearing a stylish robe.  From "+ob->query_possessive()+" "+
"looks you get the impression "+ob->query_pronoun()+
" is not going to like being bothered.\n");
      ob->load_chat(5, ({
        1, ":stares at you in a strange way.",
        1, ":seems to be watching you."
      }) );
      ob->load_a_chat(30, ({
        1, "'you'll be sorry for this.",
        1, "'Just wait till I tell Zlorf.",
        1, "'Ouch!"
      }) );
      if(random(2))
        ARMOURY->request_weapon("dagger",60+random(40))->move(ob);
      ARMOURY->request_armour("cloth robe", 90+random(10))->move(ob);
      ob->add_property("monster_type", type);
      ob->init_equip();
      return ob;
    case "mercenary":
      ob = clone_object(MONSTER);
      ob->set_name("mercenary");
      ob->add_adjective("tough");
      ob->set_race("human");
      ob->set_class("fighter");
      ob->set_level(50+random(100));
      ob->set_gender(random(2) + 1);
      ob->add_triggered_action("bing", "pub_brawl", HOSPITAL, "pub_brawl");
      ob->set_long(
"A tough battled scarred mercenary.  There are better ways of filling in " +
"an afternoon that messing with the likes of "+ob->query_objective()+".\n");
      ob->adjust_money(100 + random(100), "copper");
      ob->load_chat(3, ({
        1, "'Anyone you don't like?",
        1, "'I'll do anything if the price is right?",
        1, "'Don't get me angry.  You won't like me when I'm angry."
      }));
      ob->load_a_chat(30, ({
        1, "'I'll show you whose boss.",
        1, "'You think your so tough.",
        1, "The crowd run in terror as the Mercenary kills someone by " +
           "accident."
      }));
      if(random(10))
        ARMOURY->request_weapon("bastard sword", 40+random(60))->move(ob);
      else
        ARMOURY->request_weapon("long sword", 40+random(60))->move(ob);
      ARMOURY->request_armour("chainmail", 40+random(60))->move(ob);
      ob->add_property("monster_type", type);
      ob->init_equip();
      return ob;
    case "rat":
      ob = clone_object(MONSTER);
      ob->set_name("rat");
      ob->set_short("rat");
      ob->set_race("rat");
      ob->set_level(-5);
      ob->add_alias("city rat");
      ob->add_adjective("dirty");
      ob->add_adjective("city");
      ob->set_long(
"This is a large rat.  It appears to have done well living in the city.\n"+
"The pollution must not affect it to much.\n");
      ob->load_chat(5,({1, "The rat squeaks.\n"}));
      ob->load_a_chat(50,({1, "The rat squeels in pain.\n"}));
      ob->add_triggered_action("regen", "death", HOSPITAL, "regen_after_death");
      ob->add_property("monster_type", type);
      return ob;
    case "fish" :
      ob = clone_object(MONSTER);
      ob->set_name("fish");
      ob->set_race("fish");
      ob->set_level(-2);
      ob->set_long(
"A nice fish.  It is silvery and sleek.  Looks very cute.\n");
      ob->load_chat(50,({ 1, "The fish swims around a bit.",
                          1, "The fish flaps a fin.",
                          1, "The fish scales glint." }));
      return ob;
    case "beggar":
      ob = clone_object(MONSTER);
      ob->set_name("beggar");
      ob->set_short("Poor beggar");
      ob->set_main_plural("Poor Beggars");
      ob->add_alias("beggar");
      ob->set_race("human");
      ob->set_level(-3+random(3));
      ob->set_gender(1+random(2));
      ob->add_move_zone("docks");
      ob->add_move_zone("underdocks");
      set_long(
"This is a poor beggar.  Perhaps you should give "+ob->query_objective()+
" some money.\n");
      ob->adjust_money(random(300), "brass");
      ob->load_chat(10, ({
1, "'Money for the poor.",
1, "'Please give me some money.",
1, ":pulls on your leg.",
1, ":rattles "+ob->query_possessive()+" tin.",
1, ":mutters something about a wife and 7 children.",
1, ":mutters something about a husband and 7 children.",
1, ":looks at you with big brown eyes.",
1, "'who will help me?",
1, ":looks down at the ground and sobs."
      }));
      ob->add_property("monster_type", type);
      return ob;
    case "sailor":
      ob = clone_object(MONSTER);
      ob->set_name("sailor");
      ob->add_alias("seaman");
      ob->add_adjective("drunken");
      ob->set_race("human");
      ob->set_level(1+random(20));
      ob->set_gender(1+random(2));
      ob->set_short("Sailor");
      ob->add_move_zone("pub");
      ob->add_move_zone("docks");
      ob->add_move_zone("underdocks");
      ob->add_triggered_action("bing", "pub_brawl", HOSPITAL, "pub_brawl");
      set_long(
"This is a sailor from one of the many countries that trade here.\n"+
capitalize((string)ob->query_pronoun())+" is roaring drunk.\n");
      ob->adjust_money(random(10), "copper");
      ob->load_chat(5, ({
1, "'You aven't sh'een a pub go pash't ave you?",
1, "'It wash' here a minute ago.",
1, "'Why are there two of you?",
1, ":staggers backwards and forwards.",
1, "@burp",
1, "@trip",
1, "@puke",
3, "@stagger",
      }));
      ob->load_a_chat(20, ({
1, "'What I do?, What I do?",
1, "'Help me guysh!  He'sh picking on me.",
1, "'Boy are you in trouble now.",
1, ":swings his fists wildly.",
1, ":trips and runs into a post."
      }));
      ARMOURY->request_weapon("dagger",20+random(80))->move(ob);
      ob->add_property("monster_type", type);
      ob->init_equip();
      return ob;
    case "officer":
      ob = clone_object(MONSTER);
      ob->set_name("officer");
      ob->add_adjective("watch");
      ob->set_race("human");
      ob->set_class("fighter");
      ob->set_level(50+random(100));
      ob->set_gender("male");
      ob->set_short("Officer of the Watch");
      ob->set_main_plural("Officers of the Watch");
      ob->set_long(
"This is an Officer of the Watch.  They try to patrol Anhk Morpork and "+
"keep it safe.  They don't do very well.\n");
      ob->load_chat(2, ({
1, "'Move along, Move along.",
1, "'Lets not be loitering then.",
1, "'You look suspicious.  Move on and I'll forget I saw you.",
1, ":twirls "+ob->query_possessive()+" baton.",
1, "@eye $lname$",
      }));
      ob->load_a_chat(20, ({
1, "'I'm sorry.  I forgot you paid me yesterday.",
1, "'Please let me go.  I'm prepared to forget everything.",
1, "'Mummy!",
1, "'I'll speak to the Patrician about this.  I paid already."
      }));
      ob->add_move_zone("watch office");
      ob->add_property("monster_type", type);
      ARMOURY->request_weapon("long sword",70+random(30))->move(ob);
      ARMOURY->request_armour("leather",60+random(40))->move(ob);
      ob->init_equip();
      return ob;
    case "warrior":
      ob = clone_object(MONSTER);
      ob->set_name("warrior");
      ob->set_short("Warrior");
      ob->set_main_plural("Warriors");
      ob->add_triggered_action("bing", "pub_brawl", HOSPITAL, "pub_brawl");
      ob->set_gender(random(2) + 1);
      ob->set_long(
"This is a warrior who is doing "+ob->query_possessive()+
" hardest to make Discworld a safe place "+
"to live.\n");
      ob->set_race("human");
      ob->set_class("fighter");
      ob->set_level(6+random(70));
      ob->adjust_money(100+random(400), "copper");
      ob->load_chat(5, ({ 1, ":grunts.\n",
      1, "@grunt",
      1, "'Are you trying to provoke me?",
      1, "'What do you mean?",
      1, "'Ug."
      }));
      ob->load_a_chat(30, ({
      1, "'YOU'RE GONNA DIE FOR THIS.",
      1, "'Try to make the world a safe place and this happens.",
      1, "'Do you know who I am?",
      1, "'Hrun the Barbarian is a close friend of mine you know."
      }));
      ob->add_move_zone("smithy");
      if(random(2))
        ARMOURY->request_armour("ringmail",50+random(50))->move(ob);
      else
        ARMOURY->request_armour("studded leather", 70+random(30))->move(ob);
      if(random(4))
        ARMOURY->request_armour("medium wooden shield",50+random(50))->move(ob);
      if(random(10))
        ARMOURY->request_armour("hard leather cap", 70+random(30))->move(ob);
      if(random(2))
        ARMOURY->request_weapon("dagger", 50+random(50))->move(ob);
      else
        ARMOURY->request_weapon("knife", 70+random(30))->move(ob);
      ob->add_property("monster_type", type);
      ob->init_equip();
      return ob;
    case "person":
      ob = clone_object(MONSTER);
      ob->set_race("human");
      ob->set_long("This is one of the inhabitants of Ankh Morpork.\n");
      switch(random(3)) {
        case 0:
          ob->set_name("man");
          ob->set_main_plural("men");
          ob->set_gender(1);
          ob->set_level(-1+random(3));
          break;
        case 1:
          ob->set_name("lady");
          ob->set_main_plural("ladies");
          ob->set_gender(2);
          ob->set_level(-1+random(3));
          break;
        case 2:
          ob->set_name("child");
          ob->set_short("child");
          ob->set_main_plural("children");
          ob->set_gender(1 + random(2));
          ob->set_level(-5+random(5));
      }
      ob->add_property("monster_type", type);
      return ob;
    case "ysabell":
      return clone_object("/d/sur/death/chars/ysabell");
      ob = clone_object(MONSTER);
      ob->set_name("ysabell");
      ob->set_race("human");
      ob->set_level(2);
      ob->set_gender("female");
      ob->add_alias("girl");
      ob->set_al(500);
      ob->set_wimpy(60);
      ob->adjust_money(100,"copper");
      ob->set_short("Ysabell");
      ob->set_long(
"This is Ysabell, death's daughter (adopted).  She is a little on the " +
"podgy side.  And she looks as if she probably knows how to throw " +
"it around.\n");
      ob->add_property("monster_type", type);
      return ob;
    case "mort":
    case "albert":
    case "stren":
      ob = clone_object(MONSTER);
      ob->set_name("stren");
      ob->add_move_zone("ankh morpork");
      ob->set_move_after(0, 0);
      ob->set_name("stren");
      ob->set_race("human");
      ob->set_class("fighter");
      ob->set_level(100);
      ob->set_short("Stren Withel");
      ob->set_main_plural("Stren Withels");
      ob->set_long(
"This is Stren Withel, renowned cut hroat and bad guy.  He hangs around "+
"in the Mended Drum, because he likes it and will be one of the first "+
"to join any fight that might start here.\n");
       ob->set_join_fights("Stren exclaims loudly: PUB BRAWL!\n");
       ob->add_alias("swordsman");
       ob->add_alias("cut throat");
       ob->adjust_money(random(100), "copper");
     ob->set_al(-500);       
       ob->set_gender(1);
       ob->join_fights("Stren Withel shouts: PUB BRAWL!\n");
       ob->set_attack_everyone(1);
       ob->load_chat(2, ({
                1, "@eye $lname$",
                1, ":seems to be sizing $lcname$ up as an opponent.",
                1, "'Another beer please."
       }));
       ob->load_a_chat(30, ({
                1, "'PUB BRAWL!",
                1, "'Join in the Brawl.",
                1, ":smashes a chair over $acname$ head."
       }));
      ob->add_triggered_action("bing", "pub_brawl", HOSPITAL, "pub_brawl");
      ob->add_triggered_action("bing2", "event_death", HOSPITAL, "death");
      ob->add_property("monster_type", type);
      ob->add_ac("bing", "blunt", ({ 10, 5, 5 }) );
      ob->add_ac("bing2", "sharp", ({ 10, 4, 5 }) );
      ob->add_ac("bing2", "pierce", ({ 10, 2, 5 }) );
      ARMOURY->request_weapon("bastard sword",40+random(40))->move(ob);
      ARMOURY->request_armour("chainmail",40+random(40))->move(ob);
      ARMOURY->request_armour("leather gloves",40+random(40))->move(ob);
      ob->init_equip();
      return ob;
    case "hrun":
      ob = clone_object(MONSTER);
      ob->add_move_zone("ankh morpork");
      ob->set_move_after(0, 0);
      ob->set_name("hrun");
      ob->set_main_plural("hruns");
      ob->set_race("human");
      ob->set_join_fights("Hrun exclaims loudly: PUB BRAWL!\n");
      ob->set_class("fighter");
      ob->set_level(70);
      ob->set_gender("male");
      ob->set_short("Hrun");
      ob->add_triggered_action("bing", "pub_brawl", HOSPITAL, "pub_brawl");
      ob->set_long(
"This is not really Hrun the barbarian.  He was once pointed out as being "+
"Hrun the Barbarian to a tourist, by a failed wizard.  Not wanting to let "+
"the truth get in the way of some fame and forture, he has maintained this "+
"identity.  He loves a great pub brawl.\n");
      ob->adjust_money(random(100), "copper");
      ob->set_al(-150);
      ob->join_fights("Hrun shouts: PUB BRAWL!\n");
      ob->set_attack_everyone(1);
      ob->load_chat(3, ({
1, "@burp",
1, "'More scumble please.",
1, "@stumble",
      }));
      ob->load_a_chat(30, ({
1, "'PUB BRAWL!",
1, "'I love a good Brawl.",
1, ":smashes you with a club."
      }));
      ob->add_property("monster_type", type);
      ob2 = (object)ARMOURY->request_weapon("club",40+random(40));
      ob2->add_attack("sharp", 100, 0, ({ 20, 130 }), 0, 40, "sharp");
      ob2->set_short("spiked club");
      ob2->add_adjective("spiked");
      ob2->set_value((int)ob2->query_value()*150/100);
      ob2->move(ob);
      ob->add_ac("bing", "blunt", ({ 10, 5, 5 }) );
      ob->add_ac("bing2", "sharp", ({ 10, 4, 5 }) );
      ob->add_ac("bing2", "pierce", ({ 10, 2, 5 }) );
      ARMOURY->request_armour("leather",40+random(40))->move(ob);
      ob->init_equip();
      return ob;
    default:
      ob = clone_object(MONSTER);
      ob->set_name("failure");
      ob->set_alias("d/am/am/hospital#"+type+file_name(previous_object()));
      ob->set_short("failure");
      ob->set_long("Please inform a creator of this object.\n");
      ob->add_property("monster_type", type);
      return ob;
  }
}

void pub_brawl(object ob, mixed where, object me) {
  mixed *arr;
  int i;

  arr = (mixed *)previous_object()->query_current_room()->query_dest_dir();
  if (!arr)
    return ;
  if ((i = member_array(where, arr)) == -1) {
    if (!objectp(where))
      where = find_object(where);
    else
      where = file_name(where);
    if ((i = member_array(where, arr)) == -1)
      return ; /* we cant get there (sniff) */
  }
  previous_object()->init_command(arr[i-1]);
}

void regen_after_death() {
  object ob, dest;
  string nam;

  nam = (string)previous_object()->query_property("monster_type");
  dest = (object)previous_object()->query_property("start location");
  if (!dest)
    return ;
  ob = get_monster(nam);
  dest->add_monster(previous_object(), ob);
  call_out("do_move", 10, ({ ob, dest }) );
}

void do_move(mixed *junk) {
  junk[0]->move(junk[1]);
}

void death() {
  call_out("do_pick_up", 0, previous_object());
}

void do_pick_up(object ob) {
  ob->init_command("'Thank you for the nice booty!\n");
  ob->init_command("get all from corpses");
  call_out("do_run", 2, ob);
  call_out("selling_time", 4, ob);
}

int check_bing(object ob) {
  return !ob->query_money() && !ob->query_weilded() && !ob->query_worn();
}

object *check_loot(object ob) {
  if (!ob) return ({ });
  return filter_array(all_inventory(ob), "check_bing", this_object());
}

mixed get_co_ord(mixed ob) {
  mixed bing;
  bing = (mixed)ob->query_co_ord();
  if (bing)
    return bing;
  ob->calc_co_ord();
  return (mixed)ob->query_co_ord();
}

string change_to_name(object ob) {
  return implode((string *)ob->query_adjectives(), " ")+
         (string)ob->query_name();
}

int distance(mixed *co_ord, mixed *co_ord2) {
  int i, ret;

  if (!co_ord || !co_ord2)
    return 1000000;
  i = co_ord[0] - co_ord2[0]; 
  ret = (i<0?-i:i);
  i = co_ord[1] - co_ord2[1]; 
  ret += (i<0?-i:i);
  i = co_ord[2] - co_ord2[2]; 
  ret += (i<0?-i:i);
  return ret;
}

void selling_time(object ob) {
  mixed co_ord, t_guild, *dirs;
  object env;
  int min_dist, i, k;
  string dir, *from;
  object *obs;

  if (!sizeof(obs = check_loot(ob)))
/* We dont have any loot :( */
    return 0;
  co_ord = get_co_ord(env = (object)ob->query_current_room());
  if (!pointerp(co_ord)) /* Eeeek! */
    return ;
  t_guild = get_co_ord(THIEVES_SHOP);
  if (!pointerp(t_guild)) /* Double eek */
    return ;
  ob->add_property("goto co-ordinates", t_guild);
  ob->add_property("goto property", "thieves shop");
  ob->add_triggered_action("bing", "goto_co_ord", HOSPITAL, "the_end");
}

void the_end(int bing) {
  string str;

  previous_object()->set_move_after(10,10);
  if (!bing)
    return ;
  str = implode(map_array(check_loot(previous_object()), "change_to_name",
                          this_object()), ", ");
  previous_object()->init_command("sell "+str);
}

void do_run(object ob) {
  if(ob) ob -> run_away();
}

void fight_check(object ob, object ob1) {
  if (ob->query_property(previous_object()->query_name()))
    previous_object()->attack_ob(ob1);
}
