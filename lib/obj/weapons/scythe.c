inherit "obj/weapon";
 
void setup() {
  new_weapon(10000);
  add_immune("blunt");
  set_damage_chance(10);
  add_immune("sharp");
  add_attack("scythe",    0, 100, 180, 0, 0, "sharp");
  add_attack("drain",     0, 5,   150, 0, 0, "magic");
  add_attack("cut",       0, 5,   50,  0, 0, "sharp");
  add_attack("devestate", 0, 10,  0,   0, 0, "sharp");
  set_weap_skill("fighting.combat.melee.sharp");
  set_weight(10);
  adjust_money(500, "silver");
  add_property("determinate", "");
  set_name("scythe");
  add_adjective("deaths");
  add_adjective("death's");
  set_short("Death's scythe");
  set_main_plural("Death's scythes");
  add_plural("scythes");
  set_long("This is the scythe that Death uses.  It's blade is so fine and sharp that you can actually see through it.  There is a faint blue glow to it.\n");
  set_read_mess("Forged by the Creator.\n");
}
 
int write_message(int dam, object him, object me, string type, string attack) {
  switch (attack) {
    case "drain" :
      write("You drain "+him->query_name()+"'s soul with the scythe.\n");
      say(me->query_cap_name()+" drains "+him->query_name()+"'s soul with the scythe.\n", him);
      tell_object(him,me->query_cap_name()+" drains your soul with the scythe.\n");
      break;
    case "cut" :
      say(me->query_cap_name()+" cuts cleanly into "+him->query_name()+".\n", him);
      write("You cut cleanly into "+him->query_name()+".\n");
      tell_object(him, me->query_cap_name()+" cuts cleanly into you.\n");
      break;
    case "devestate" :
      say(me->query_cap_name()+" devastates "+him->query_name()+".\n",him);
      write("You devastate "+him->query_name()+".\n");
      tell_object(him, me->query_cap_name()+" devestates you.\n");
      break;
    default :
      ::write_message(dam, him, me, type, attack);
      break;
  }
}

mixed init_arg(mixed args) {
  ::init_arg(args);
  if (!"/obj/weapons/scythe"->query_property("been loaded"))
    "/obj/weapons/scythe"->add_property("been loaded", 1);
  else
    call_out("dest_me", 0);
}
