#define ROOM "/w/elwood/pat/rooms/";
#define MONS "/w/elwood/pat/mons/";
inherit "/std/room";
object mons, weapon, armour, ob, critter;
int x, y, z, i, j;
void add_monster();
void setup()
 {
  set_light(70);
  set_short("Short");
  set_long("long desc.\n");
/*****************************
*  add_exit("north", ROOM+"destination", "corridor");
* add_exit("south", ROOM+"destination", "corridor");
*  add_exit("east", ROOM+"destination", "corridor");
*  add_exit("west", ROOM+"destination", "corridor");
*******************************************************/ 
  add_item("item", "item desc");
  add_item("item", "item desc"); 
  add_item("item", "item desc"); 
   add_item("item", "item desc"); 
  mons = clone_object("/obj/monster");
  mons->set_name("name");
  mons->set_short("name");
  mons->set_long("long desc.\n");
  mons->set_al(0);
  mons->adjust_money("platinum", 5);
  mons->set_level(500);
  mons->set_race("human");
  mons->add_alias("alias");
  mons->set_class("fighter");
  mons->set_gender(0);
  mons->add_achat(20, ({ "Monster pounds you into a "+
		    "puddle of blood and body parts.\n"}));
  mons->add_attack("pound",0,100,10,20,30, "magic");
  mons->equip();
  weapon = (object)"/obj/handlers/armoury"->request_weapon("mace", 100);
  weapon->set_name("weapon");
  weapon->set_short("Weapon");
  weapon->set_long("A very good weapon.\n");
  weapon->add_alias("weapon");
  weapon->add_main_plural("Weapons");
  weapon->add_plural("weapons");
  weapon->set_enchant(50);
  weapon->add_attack("pound",0,100,10,20,30, "magic");
  weapon->move(mons);
  armour = (object)"/obj/handlers/armoury"->request_armour("plate armour", 100);
  armour->set_name("plate armour");
  armour->set_short("Plate Armour");
  armour->set_long("Very good armour.\n");
  armour->add_alias("armor");
  armour->add_main_plural("armours");
  armour->add_plural("armour");
  armour->set_enchant(50);
  armour->move(mons);
  mons->move(this_object());
}
