inherit "obj/monster";
void setup()
{
    set_name("eyewing");
    add_alias("eye");
    set_main_plural("eyewings");
    add_plural("eyes");

    set_short("Eyewing");
    set_long("Here is a creature from the abyss.\n");
    set_race("eyewing");
    set_class("fighter");
    set_level(13);
    set_al(-1000);
    add_attack("lightning", 0, 30, 150, 0, 0, "magic");
}

int write_message(int dam, object target, object me, string type, string attack)
{
  string what;
  
  if(attack == "lightning") {
    what = (dam<20)?"shock bolt":"bolt of lightning";
    say("A " + what + " bursts out from the Eyeball, striking " +
      target->query_cap_name() + " full in the chest!\n", target);
    tell_object(target, "The eyeball fizzes for a moment, then blasts you " +
      "with a " + what + " right in the chest!!!\n");
  } else {
    ::write_message(dam, target, me, type, attack);
  }
}
