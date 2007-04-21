/*  -*- LPC -*-  */
/**
 * This is the room in which the newbie chooses which land they wish to
 * start in.
 * @author Pinkfish
 * @started Thu May 23 11:55:41 PDT 2002
 */

#include "path.h"
#include <config.h>
#include <language.h>
#include <money.h>

inherit PATH+"inside";

#define DEBUG 1
#define LOG PATH + "save/CHOICE"

private mapping _nationalities;
private object _sign;

void setup() {
   string name;
   string nationality;
   string region;

  _nationalities = ([
       "pumpkin" : "/std/nationality/pumpkin"
     ]);
  add_property("no teleport", 1);
  add_property("determinate", "");
/* Make it so you cannot godmother out of here */
  add_property("no godmother", 1);
  set_zone( "Newbie" );
  set_short( "Discworld's realm of choice" );
  set_light( 75 );
  set_long("You are on a huge open-air platform far, far above the Disc itself.  "
           "Down below, the water tumbles and falls off the rim of the disc "
           "spilling down and out into a spray of mist over the elephants and "
           "the back of A'tuin, the great star turtle.  There are several "
           "sets of stairs leading down off the platform to the unknown and "
           "a large dial sits in the middle of the room.  A large sign "
           "shimmers and hangs in the air in the middle of the platform.\n");

  add_item(({ "disc", "Disc", "discworld"}),
           "You can see from the sharp spire of Cori "
           "Celesti at the centre, to the surrounding Circle Sea, "
           "the deserts of Klatch, the watery world of Krull, and the "
           "mysterious Counterweight Continent.  A spectacular rimbow "
           "sparkles where the seas pour over the edge of the Disc.  As "
           "you study the map's details, you are sure you saw one of the "
           "elephants move.");

  add_item("rimbow", "It's like a rainbow except that it's caused by the "
           "water pouring off the edge of the disc.");
  
  add_item("elephant", "These are the four elephants that bear the weight"
           " of the Disc.");

  add_item(({ "floor", "platform" }),
           "The floor is made of some weird black stone that is "
           "semi-transparent, allowing the Disc below you to be "
           "seen through it.");
  
  add_item(({ "stairs", "stairways" }), "Stairs leading down to the "
           "dark unknown.  They all appear to be crafted of some thin, "
           "spidery metal.");

  add_item("dial", "The dial is a huge heavy looking stone dial, with some "
                   "arrows and numbers carved into the surface.  They say "
                   "'Klatch 4000 miles', 'Ankh-Morpork 3000 miles', "
                   "'Lancre 3500 miles', 'Bes Pelargic 3000 miles'.");

   _sign = add_sign("A huge shimmering sign, the letters are transparent "
      "and it is possible to walk through it.\n", 
      FOYER->format_sign(
      "Travellers choose your destination!  Which parts of the world "
      "do you want to explore?\n\n"
      "Look carefully at each of the lands "
      "before you make your choice - it determines the language you "
      "start with and your local accent, as well as where you start.\n\n"
      "The current list of available start nationalities is: " +
      query_multiple_short(keys(_nationalities)) + ".\n\n"
      "You can get 'info' on each nationality, and each region within "
      "a nationality.\n\n"
      "You choose your nationality with 'choose <nationality> <region>'; "
      "for example 'choose morpork Ankh-Morpork'.\n\n"
      "Use 'help here' for more help on 'info' and 'choose'.\n"),
      "shimmering sign", "sign", "general");

   foreach (name, nationality in _nationalities) {
      if (file_size(nationality + ".c") > 0) {
         add_item(name, nationality->query_information());
         foreach (region in nationality->query_regions()) {
            //add_exit("choose " + name + " " + lower_case(region), nationality->query_region_start_location(region), "stairs");
            //modify_exit("choose " + name + " " + lower_case(region), ({ "function", "setup_nationality" }) );
            add_item(name + " " + lower_case(region), nationality->query_region_description(region));
         }
      } else {
         printf("Bad nationality %O\n", nationality);
      }
   }

   add_help_file(PATH + "doc/choose_land");
} /* setup() */

string find_region(string nationality, string region) {
   string tmp;

   foreach (tmp in nationality->query_regions()) {
      if (lower_case(tmp) == lower_case(region)) {
         return tmp;
      }
   }
   return 0;
}

void change_nationality(object person, string nationality, string region) {
   string old_nationality;
   int old_speak;
   int old_write;
   int new_speak;
   int new_write;
   string skill_speak;
   string skill_write;
   string lang;
   string guild;
   mixed newbie;

   // Find out what their old nationality is.
   nationality = _nationalities[nationality];
   old_nationality = this_player()->query_nationality();
#ifdef DEBUG
   debug_printf("Old nationality was %O, new nationality is %O.\n", old_nationality, nationality);
#endif
   // Move around the skills.
   lang = nationality->query_language();
   skill_speak = LANGUAGE_HAND->query_language_spoken_skill(lang);
   skill_write = LANGUAGE_HAND->query_language_written_skill(lang);
   old_speak = this_player()->query_skill(skill_speak);
   old_write = this_player()->query_skill(skill_write);
   if (old_speak > 100) {
      old_speak = 100;
   }
   this_player()->add_skill_level(skill_speak, 100 - old_speak); 
   if (old_write > 100) {
      old_write = 100;
   }
   this_player()->add_skill_level(skill_write, 100 - old_write);

   if (old_nationality &&
       old_nationality->query_language() != nationality->query_language()) {
      // Put the levels we removed into the old language. 
      lang = old_nationality->query_language();
      skill_speak = LANGUAGE_HAND->query_language_spoken_skill(lang);
      skill_write = LANGUAGE_HAND->query_language_written_skill(lang);
      new_speak = this_player()->query_skill(skill_speak);
      new_write = this_player()->query_skill(skill_write);
      if (new_speak > 100) {
         new_speak = 100;
      }
      this_player()->add_skill_level(skill_speak, old_speak - new_speak);
      if (new_write > 100) {
         new_write = 100;
      }
      this_player()->add_skill_level(skill_write, old_write - new_write);
   }
   this_player()->set_nationality(nationality);
   this_player()->set_nationality_region(find_region(nationality, region));
   lang = nationality->query_language();
   // Setup the default language correctly.
   this_player()->set_default_language(lang);
   this_player()->set_language(lang);

   guild = this_player()->query_guild_ob();
   newbie = this_player()->query_property("not real newbie");

   log_file( LOG , "%s:\n   %s the %s%s chose %s (%s)\n",
      ctime(time()),
      this_player()->query_short(), 
      guild ? capitalize(guild[12..<1]) : "Adventurer",
      newbie ? "" : " (newbie)",
      capitalize(this_player()->query_nationality_region()),
      capitalize(this_player()->query_nationality()[17..<1]));

   // mess with money.
   if (!this_player()->query_property("not real newbie")) {
      int value;
      string currency_area = "";
      mixed* array;

      value = this_player()->query_value_in("Newbie Area");
      currency_area = this_player()->query_nationality()->query_currency_area();
      // Hack to correct Sto Lat money from AM to Provincial
      if (this_player()->query_nationality_region() == "Sto Lat" ) {
         currency_area = "default";
      }
      tell_creator( "wyvyrn" ,
         "[ %s the %s%s picked %s and got money for %s ]\n",
         this_player()->query_short(), 
         guild ? capitalize(guild->query_name()[0..<2]) : "Adventurer",
         newbie ? "" : " (newbie)",
         this_player()->query_nationality_region(),
         currency_area);

      array = MONEY_HAND->create_money_array(value, currency_area);
      this_player()->set_money_array(array);
   } else {
      object ob;

      foreach (ob in all_inventory()) {
         if (!living(ob) && ob->query_short() != "shimmering sign") {
            ob->move(nationality->query_region_start_location(region));
         }
      }
      tell_creator( "wyvyrn" ,
         "[ %s the %s%s picked %s and kept their money ]\n",
         this_player()->query_short(), 
         guild ? capitalize(guild->query_name()[0..<2]) : "Adventurer",
         newbie ? "" : " (newbie)",
         this_player()->query_nationality_region());
   }
}

int setup_nationality(string direction, object person) {
   string* bits;

   bits = explode(direction, " ");
   change_nationality(person, bits[1], implode(bits[2..], " "));
   return 1;
}

int do_choose(string nat, string region) {
   string region_new;
   string nationality;
   string loc;

   nationality = lower_case(nat);
   if (!_nationalities[nationality]) {
      add_failed_mess("There is no nationality " + nationality + ".\n");
      return 0;
   }
   nat = nationality;
   nationality = _nationalities[nationality];
   region_new = find_region(nationality, region);
   if (!region_new) {
      add_failed_mess("Cannot find the region " + region +
                      " in the nationality " + nat + ".  It must be "
                      "one of " +
                      query_multiple_short(nationality->query_regions()) + ".\n");
      return 0;
   }

   // Try and find the region.
#ifdef DEBUG
   debug_printf("Nationality is %O, region is %O, player is %O.\n", 
      nationality, region_new, this_player()->query_short());
#endif
   change_nationality(this_player(), nat, region_new);
   loc = nationality->query_region_start_location(region_new);
   this_player()->move_with_look(loc, "$N arrive$s into the world.",
                                 "$N disappears to a better life.");
   this_player()->remove_property("not real newbie");
   if (clonep(this_object())) {
      call_out((: dest_me :), 10);
      if (_sign) {
         _sign->dest_me();
      }
   }
   return 1;
}

int do_info(string nat, string region) {
   string region_new;
   string nationality;

   nationality = lower_case(nat);
   if (!_nationalities[nationality]) {
      add_failed_mess("There is no nationality " + nationality + ".\n");
      return 0;
   }
   nationality = _nationalities[nationality];
   region_new = find_region(nationality, region);
   if (!region_new) {
      add_failed_mess("Cannot find the region " + region +
                      " in the nationality " + nat + ".  It must be "
                      "one of " +
                      query_multiple_short(nationality->query_regions()) + ".\n");
      return 0;
   }

   // Try and find the region.
#ifdef DEBUG
   debug_printf("Nationality is %O, region is %O, player is %O.\n", 
      nationality, region_new, this_player()->query_short());
#endif
   write(nationality->query_region_description(region_new) + "\n");
   return 1;
}

int do_nationality_info(string nationality) {
   string ret;
   string region;

   nationality = lower_case(nationality);
   if (!_nationalities[nationality]) {
      add_failed_mess("There is no nationality " + nationality + ".\n");
      return 0;
   }

   nationality = _nationalities[nationality];

   ret = nationality->query_information() +
         "Regions you can choose:\n";
   foreach (region in nationality->query_regions()) {
      ret += "  * " + region + "\n";
   }
   write(ret);
   return 1;
}

void init() {
  add_command("info", "<word'nation'>", (: do_nationality_info($4[0]) :));
  add_command("info", "<word'nation'> <string'region'>", (: do_info($4[0], $4[1]) :));
  add_command("choose", "<word'nationality'> <string'region'>", (: do_choose($4[0], $4[1]) :));
}

void fixup_player(object player) {
   object ob;

   ob = clone_object(__FILE__);
   player->move_with_look(ob);
}

string query_quit_handler() {
   return __FILE__;
}
