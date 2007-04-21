#include <state_change.h>
void dest_me() { destruct( this_object() ); }

class state_change item;
mapping data = ([ ]);

mixed data_return_0() {
  item = new( class state_change );
  item->result = "/obj/reagents/water.ob";
  item->factor = 0.0;
  item->func = ({});
  data["witchesbrew:Water"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/water.ob";
  item->factor = 0.0;
  item->func = ({});
  data["ruinedbrew:Water"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/brewed_healing_tea.ob";
  item->factor = 1.0;
  item->func = ({
    (: $2->add_eat_effect( "/std/effects/ingested/basic_healing",
       to_int( $1->query_eat_effects()[
              "/std/effects/ingested/basic_healing" ] *
           ( 4 + $1->query_property( "brew bonus" ) ) ) ) :),
  });
  data["witchesbrew:UnbrewedComfreyYarrowTea"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/burnt_tea.ob";
  item->factor = 1.0;
  item->func = ({});
  data["ruinedbrew:UnbrewedComfreyYarrowTea"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/brewed_antidote_elixir";
  item->factor = 1.0;
  item->func = ({
    (: $2->add_eat_effect( "/std/effects/ingested/poison_antidote",
       to_int( $1->query_eat_effects()[
           "/std/effects/ingested/poison_antidote"] *
       ( 4 + $1->query_property( "brew bonus" ) ) ) ) :)
  });
  data["witchesbrew:UnbrewedBasilFennelSpiderTea"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/burnt_tea.ob";
  item->factor = 1.0;
  item->func = ({});
  data["ruinedbrew:BasilFennelSpiderTea"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/brewed_intelligence_tea.ob";
  item->factor = 1.0;
  item->func = ({
    (: $2->add_eat_effect( "/std/effects/ingested/int_bonus",
       to_int( this_player()->query_skill_bonus(
             "magic.methods.physical.brewing" ) /
       ( 10 - $1->query_property( "brew bonus" ) ) ) ) :),
    });
  data["witchesbrew:UnbrewedSageIntPotion"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/burnt_tea.ob";
  item->factor = 1.0;
  item->func = ({});
  data["ruinedbrew:UnbrewedSageIntPotion"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name("liquid") :),
        (: $2->set_short("milky white liquid") :),
        (: $2->add_adjective( "milky", "white"  ) :),
        (: $2->set_long("This is a milky white liquid, $pile_size$.") :),
        (: $2->set_medium_alias("BrewedEyebrightCarrotBunnyEyeballTea") :),
        (: $2->add_eat_effect("/std/effects/ingested/blind_protect",
               to_int( this_player()->query_skill_bonus(
               "magic.methods.physical.brewing" ) /
               ( 10 - $1->query_property( "brew bonus" ) ) ) ) :),
    });
  data["witchesbrew:EyebrightCarrotBunnyEyeballTea"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/burnt_tea.ob";
  item->factor = 1.0;
  item->func = ({});
  data["ruinedbrew:EyebrightCarrotBunnyEyeballTea"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/brewed_cold_cure.ob";
  item->factor = 1.0;
  item->func = ({
     (: $2->add_eat_effect("/std/effects/ingested/cold_cure",
        to_int( $1->query_eat_effects()["/std/effects/ingested/cold_cure"] *
               ( 4 + $1->query_property( "brew bonus" ) ) ) ) :),
     (: $2->add_eat_effect("/std/effects/ingested/garlic_breath",
        $1->query_eat_effects()["/std/effects/ingested/garlic_breath"] ) :),
   });
  data["witchesbrew:ColdCureSyrup"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/burnt_cold_cure.ob";
  item->factor = 0.25;
  item->func = ({});
  data["ruinedbrew:ColdCureSyrup"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/black_tea.ob";
  item->factor = 1.0;
  item->func = ({ });
  data["witchesbrew:UnbrewedBlackTea"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/burnt_tea.ob";
  item->factor = 1.0;
  item->func = ({});
  data["ruinedbrew:UnbrewedBlackTea"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/burnt_tea.ob";
  item->factor = 1.0;
  item->func = ({ });
  data["witchesbrew:BlackTea"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/burnt_tea.ob";
  item->factor = 1.0;
  item->func = ({});
  data["ruinedbrew:BlackTea"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/water.ob";
  item->factor = 1.0;
  item->func = ({});
  data["burn:Water"] = item;
  item = new( class state_change );
  item->factor = 1.0;
  item->result = "/obj/reagents/blackened_mess.ob";
  item->func = ({ (: $2->add_eat_effect( "/std/effects/ingested/poison", 400 ) :) });
  data["burn:UnheatedAchorionPurple"] = item;
  item = new( class state_change );
  item->factor = 1.0;
  item->result = "/obj/reagents/blackened_mess.ob";
  item->func = ({ (: $2->add_eat_effect( "/std/effects/ingested/poison", 400 ) :) });
  data["burn:UndistilledAchorionPurple"] = item;
  item = new( class state_change );
  item->factor = 1.0;
  item->result = "/obj/reagents/blackened_mess.ob";
  item->func = ({ (: $2->add_eat_effect( "/std/effects/ingested/poison", 400 ) :) });
  data["burn:UndistilledFightClubPoison"] = item;
  item = new( class state_change );
  item->factor = 1.0;
  item->result = "/obj/reagents/blackened_mess.ob";
  item->func = ({ (: $2->add_eat_effect( "/std/effects/ingested/poison", 400 ) :) });
  data["burn:UndistilledHallucinogenPoison"] = item;
  item = new( class state_change );
  item->factor = 1.0;
  item->result = "/obj/reagents/blackened_mess.ob";
  item->func = ({ (: $2->add_eat_effect( "/std/effects/ingested/poison", 400 ) :) });
  data["burn:UndistilledSpiderPoison"] = item;
  item = new( class state_change );
  item->factor = 1.0;
  item->result = "/obj/reagents/blackened_mess.ob";
  item->func = ({ (: $2->add_eat_effect( "/std/effects/ingested/poison", 400 ) :) });
  data["burn:UndistilledBlowfishPoison"] = item;
  item = new( class state_change );
  item->factor = 1.0;
  item->result = "/obj/reagents/blackened_mess.ob";
  item->func = ({ (: $2->add_eat_effect( "/std/effects/ingested/poison", 400 ) :) });
  data["burn:UnheatedWeakConPoison"] = item;
  item = new( class state_change );
  item->factor = 1.0;
  item->result = "/obj/reagents/blackened_mess.ob";
  item->func = ({ (: $2->add_eat_effect( "/std/effects/ingested/poison", 400 ) :) });
  data["burn:UnheatedConPoison"] = item;
  item = new( class state_change );
  item->factor = 1.0;
  item->result = "/obj/reagents/blackened_mess.ob";
  item->func = ({ (: $2->add_eat_effect( "/std/effects/ingested/poison", 400 ) :) });
  data["burn:UnheatedConfusePoison"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/water.ob";
  item->factor = 1.0;
  item->func = ({});
  data["distill:Water"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/achorion_purple.ob";
  item->factor = 0.01;
  item->func = ({ (: $2->add_eat_effect( "/std/effects/poisons/hp_new",
    to_int( $3->query_skill_bonus( "covert.items.poisons" ) ) ) :) });
  data["distill:UndistilledAchorionPurple"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/achorion_purple.ob";
  item->factor = 0.01;
  item->func = ({ (: $2->add_eat_effect( "/std/effects/poisons/hp_new",
    to_int( $3->query_skill_bonus( "covert.items.poisons" ) / 5 ) ) :) });
  data["distill:UnheatedAchorionPurple"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/angry_poison.ob";
  item->factor = 0.01;
  item->func = ({ (: $2->add_eat_effect( "/std/effects/poisons/angry",
    to_int( $3->query_skill_bonus( "covert.items.poisons" ) ) ) :) });
  data["distill:UndistilledFightClubPoison"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/hallucinogen_poison.ob";
  item->factor = 0.01;
  item->func = ({ (: $2->add_eat_effect( "/std/effects/ingested/hallucination",
    to_int( $3->query_skill_bonus( "covert.items.poisons" ) * 10 ) ) :) });
  data["distill:UndistilledHallucinogenPoison"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/spider_poison.ob";
  item->factor = 0.5;
  item->func = ({ (: $2->add_eat_effect( "/std/effects/poisons/spider_venom",
    to_int( $3->query_skill_bonus( "covert.items.poisons" ) * 10 ) ) :) });
  data["distill:UndistilledSpiderPoison"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/bloat.ob";
  item->factor = 0.5;
  item->func = ({ (: $2->add_eat_effect( "/std/effects/poisons/bloat",
    to_int( $3->query_skill_bonus( "covert.items.poisons" ) * 1.5 ) ) :) });
  data["distill:UndistilledBlowfishPoison"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "snake_poison" ) :),
      (: $2->set_short( "thick white liquid" ) :),
      (: $2->add_adjective( ({ "thick", "white", "liquid" }) ) :),
      (: $2->set_long( "This is $amount_size$ of some thick white liquid.\n" ) :),
      (: $2->set_medium_alias( "DistilledSnakePoison" ) :),
    });
  data["distill:UndistilledSnakePoison"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 15.0;
  item->func = ({
      (: $2->set_name( "mess" ) :),
      (: $2->set_short( "nasty fibrous mess" ) :),
      (: $2->add_alias( "wheat" ) :),
      (: $2->add_adjective( ({ "nasty", "fibrous" }) ) :),
      (: $2->set_long("This is $pile_size$ of a nasty, "
                      "fibrous mess.  Some of it looks "
                      "a bit like flour.\n") :),
      (: $2->set_medium_alias( "WheatMess" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["grind:Wheat"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 10.0;
  item->func = ({
      (: $2->set_name( "wheat" ) :),
      (: $2->set_short( "wheat flour" ) :),
      (: $2->add_alias( "flour" ) :),
      (: $2->add_adjective( "ground" ) :),
      (: $2->set_long("This is $pile_size$ of soft, brown "
                      "flour, $amount_size$.  Perhaps you "
                      "could make bread with it?\n") :),
      (: $2->set_medium_alias( "WheatFlour" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["grind:WheatBerries"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
      (: $2->set_name( "clover" ) :),
      (: $2->set_short( "ground clover" ) :),
      (: $2->add_alias( "plant" ) :),
      (: $2->add_plural( "plants" ) :),
      (: $2->add_adjective( "ground" ) :),
      (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks "
            "like it might have been clover.\n" ) :),
      (: $2->set_medium_alias( "GroundClover" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["grind:Clover"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/coffee.ob";
  item->factor = 5.0;
  item->func = ({ });
  data["grind:CoffeeBean"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("yarrow") :),
        (: $2->set_short("ground yarrow") :),
        (: $2->add_alias("plant") :),
        (: $2->add_plural( "plants" ) :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks "
                    "like it might've been yarrow.\n") :),
        (: $2->set_medium_alias("GroundYarrow") :),
        (: $2->add_eat_effect("/std/effects/ingested/cum_poison", 12) :),
        (: $2->add_eat_effect("/std/effects/ingested/basic_healing", 2) :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Yarrow"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("comfrey") :),
        (: $2->set_short("ground comfrey") :),
        (: $2->add_alias("plant") :),
        (: $2->add_plural( "plants" ) :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It looks "
                    "like it might've been comfrey.\n") :),
        (: $2->set_medium_alias("GroundComfrey") :),
        (: $2->add_eat_effect("/std/effects/ingested/cum_poison", 16) :),
        (: $2->add_eat_effect("/std/effects/ingested/basic_healing", 4) :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Comfrey"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
   (: $2->set_name( "mushroom" ) :),
   (: $2->set_short( "ground mushroom" ) :),
   (: $2->add_alias( "madman" ) :),
   (: $2->add_adjective( "ground" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  Judging from the "
          "green tint, it looks like it might've been Floppy Madman.\n" ) :),
   (: $2->set_medium_alias( "GroundFloppyMadman" ) :)
   });
  data["grind:FloppyMadman"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("fleabane") :),
        (: $2->set_short("ground fleabane") :),
        (: $2->add_alias("plant") :),
        (: $2->add_plural( "plants" ) :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It looks "
                    "like it might've been Klatchian fleabane.\n") :),
        (: $2->set_medium_alias("GroundKlatchianFleabane") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:KlatchianFleabane"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.2;
  item->func = ({
        (: $2->set_name("fleabane") :),
        (: $2->set_short("powdered fleabane") :),
        (: $2->add_alias("plant") :),
        (: $2->add_plural( "plants" ) :),
        (: $2->add_adjective(({"powdered"})) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It looks "
                    "a slight lavender hue with specs of orange.\n") :),
        (: $2->set_medium_alias("PowderedKlatchianFleabane") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:GroundKlatchianFleabane"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("mayweed") :),
        (: $2->set_short("ground mayweed") :),
        (: $2->add_alias("plant") :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It looks "
                    "like it might've been stinking mayweed.\n") :),
        (: $2->set_medium_alias("GroundStinkingMayweed") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:StinkingMayweed"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.2;
  item->func = ({
        (: $2->set_name("mayweed") :),
        (: $2->set_short("powdered mayweed") :),
        (: $2->add_alias("plant") :),
        (: $2->add_plural( "plants" ) :),
        (: $2->add_adjective(({"powdered"})) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It looks "
                    "a brownish green color, as well as a very strong "
                    "odor.\n") :),
        (: $2->set_medium_alias("PowderedStinkingMayweed") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:GroundStinkingMayweed"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
      (: $2->set_name( "walnut" ) :),
      (: $2->set_short( "ground walnut" ) :),
      (: $2->add_adjective( "ground" ) :),
      (: $2->add_plural( "walnuts" ) :),
      (: $2->set_long( "This is a pile of ground walnut.\n" ) :),
      (: $2->set_medium_alias( "GroundWalnut" ) :),
   });
  data["grind:Walnut"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 0.04;
  item->func = ({
      (: $2->set_name( "garlic" ) :),
      (: $2->set_short( "ground garlic" ) :),
      (: $2->add_alias( "garlic" ) :),
      (: $2->add_adjective( ({ "wild", "ground" }) ) :),
      (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  Judging from the "
         "smell, it must have been garlic.\n" ) :),
       (: $2->set_medium_alias( "GroundGarlic" ) :),
      (: $2->add_eat_effect("/std/effects/ingested/garlic_breath", 200) :),
      (: $2->set_decay_speed(0) :)
   });
  data["grind:WildGarlic"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 0.04;
  item->func = ({
      (: $2->set_name( "garlic" ) :),
      (: $2->set_short( "ground garlic" ) :),
      (: $2->add_alias( "garlic" ) :),
      (: $2->add_adjective( "ground" ) :),
      (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  Judging from the "
         "smell, it must have been garlic.\n" ) :),
       (: $2->set_medium_alias( "GroundGarlic" ) :),
      (: $2->add_eat_effect("/std/effects/ingested/garlic_breath", 200) :),
      (: $2->set_decay_speed(0) :)
    });
  data["grind:GarLic"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("parsley") :),
        (: $2->set_short("ground parsley") :),
        (: $2->add_alias("plant") :),
        (: $2->add_plural( "plants" ) :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It smells "
                    "like it might've been parsley.\n") :),
        (: $2->set_medium_alias("GroundParsley") :),
        (: $2->add_eat_effect("/std/effects/ingested/breath_fresh", 2) :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Parsley"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("sage") :),
        (: $2->set_short("ground sage") :),
        (: $2->add_alias("plant") :),
        (: $2->add_plural( "plants" ) :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It smells "
                    "like it might've been sage.\n") :),
        (: $2->set_medium_alias("GroundSage") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Sage"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("rosemary") :),
        (: $2->set_short("ground rosemary") :),
        (: $2->add_alias("plant") :),
        (: $2->add_plural( "plants" ) :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It smells "
                    "like it might've been rosemary.\n") :),
        (: $2->set_medium_alias("GroundRosemary") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Rosemary"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("thyme") :),
        (: $2->set_short("ground thyme") :),
        (: $2->add_alias("plant") :),
        (: $2->add_plural( "plants" ) :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It smells "
                    "like it might've been thyme.\n") :),
        (: $2->set_medium_alias("GroundThyme") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Thyme"] = item;
}

mixed data_return_1() {
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("basil") :),
        (: $2->set_short("ground basil") :),
        (: $2->add_alias("plant") :),
        (: $2->add_plural( "plants" ) :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It smells "
                    "like it might've been basil.\n") :),
        (: $2->set_medium_alias("GroundBasil") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Basil"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("eyebright") :),
        (: $2->set_short("ground eyebright") :),
        (: $2->add_alias("plant") :),
        (: $2->add_plural( "plants" ) :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It smells "
                    "like it might've been eyebright.\n") :),
        (: $2->set_medium_alias("GroundEyebright") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Eyebright"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 0.05;
  item->func = ({
        (: $2->set_name("spider") :),
        (: $2->set_short("ground arachnid") :),
        (: $2->add_alias("arachnid") :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground arachnid, "
                    "$amount_size$.  It looks "
                    "very chitinous.\n") :),
        (: $2->set_medium_alias("GroundSpider") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Spider"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("fennel") :),
        (: $2->set_short("ground fennel") :),
        (: $2->add_alias(({"seed", "seeds"})) :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It looks "
                    "like it might've been fennel.\n") :),
        (: $2->set_medium_alias("GroundFennel") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Fennel"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("carrot") :),
        (: $2->set_short("ground carrot") :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It looks "
                    "very orange.\n") :),
        (: $2->set_medium_alias("GroundCarrot") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:CarRot"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 10.0;
  item->func = ({
        (: $2->set_name("eye") :),
          (: $2->add_alias("eyes") :),
        (: $2->set_short("ground eyes") :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It looks "
                    "very bloody.\n") :),
        (: $2->set_medium_alias("GroundBunnyEyeball") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:BunnyEyeball"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 20.0;
  item->func = ({
        (: $2->set_name("crumbs") :),
        (: $2->set_short("molasses cookie crumbs") :),
        (: $2->add_adjective(({"molasses", "cookie"})) :),
        (: $2->set_long("This is some cookie crumbs.  Yum.\n") :),
        (: $2->set_medium_alias("GroundMolassesCookie") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:MolassesCookie"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 10.0;
  item->func = ({
        (: $2->set_name("radish") :),
        (: $2->set_short("ground radish") :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It looks "
                    "very orange.\n") :),
        (: $2->set_medium_alias("GroundRadish") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Radish"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 10.0;
  item->func = ({
        (: $2->set_name("parsnip") :),
        (: $2->set_short("ground parsnip") :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It looks "
                    "a bit like a pile of pine sawdust.\n") :),
        (: $2->set_medium_alias("GroundParsnip") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Parsnip"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("oregano") :),
        (: $2->set_short("ground oregano") :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It looks "
                    "very Genuan.\n") :),
        (: $2->set_medium_alias("GroundOregano") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Oregano"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 10.0;
  item->func = ({
        (: $2->set_name("foxglove") :),
        (: $2->set_short("ground foxglove") :),
        (: $2->add_adjective("ground") :),
        (: $2->add_alias( "plant" ) :),
        (: $2->add_plural( "plants" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
          "$amount_size$.  You can tell it used to be foxglove by the "
          "little purple flowers scattered in with the green stuff.\n") :),
        (: $2->set_medium_alias("GroundFoxglove") :),
        (: $2->add_eat_effect( "/std/effects/ingested/cum_poison", 12 ) :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Foxglove"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("violet") :),
        (: $2->set_short("ground violet") :),
        (: $2->add_adjective("ground") :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
                    "$amount_size$.  It looks "
             "very, well, violet.\n") :),
        (: $2->set_medium_alias("GroundViolet") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Violet"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 10.0;
  item->func = ({
        (: $2->set_name("cabbage") :),
        (: $2->set_short("shredded cabbage") :),
        (: $2->add_adjective("shredded") :),
        (: $2->set_long( "This is $pile_size$ of shredded cabbage, "
                    "$amount_size$.  "
             "Hopefully, not a sentinent one.\n") :),
        (: $2->set_medium_alias("GroundCabbage") :),
        (: $2->set_decay_speed(0) :)
    });
  data["grind:Cabbage"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name( "peel" ) :),
        (: $2->set_short( "ground orange peel" ) :),
        (: $2->add_adjective( ({ "ground", "orange" }) ) :),
        (: $2->set_long( "This is $pile_size$ of ground orange peel, "
            "$amount_siuze$.  It is still "
            "a vibrant orange, the citric acid preserving both the colour "
            "and smell of the orange peel.\n" ) :),
        (: $2->set_medium_alias( "GroundOrangePeel" ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:OrangePeel"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "belladonna" ) :),
        (: $2->set_short( "ground belladonna" ) :),
        (: $2->add_adjective( "ground" ) :),
        (: $2->add_alias( "plant" ) :),
        (: $2->add_plural( "plants" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks like it might've been belladonna.\n" ) :),
        (: $2->set_medium_alias( "GroundBelladonna" ) :),
        (: $2->add_eat_effect( "/std/effects/ingested/cum_poison", 12 ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:Belladonna"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "hemlock" ) :),
        (: $2->set_short( "ground hemlock" ) :),
        (: $2->add_adjective( "ground" ) :),
        (: $2->add_alias( "plant" ) :),
        (: $2->add_plural( "plants" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks like it might've been hemlock.\n" ) :),
        (: $2->set_medium_alias( "GroundHemlock" ) :),
        (: $2->add_eat_effect( "/std/effects/ingested/cum_poison", 12 ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:Hemlock"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "monkshood" ) :),
        (: $2->set_short( "ground monkshood" ) :),
        (: $2->add_adjective( "ground" ) :),
        (: $2->add_alias( "plant" ) :),
        (: $2->add_plural( "plants" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks like it might've been monkshood.\n" ) :),
        (: $2->set_medium_alias( "GroundMonkshood" ) :),
        (: $2->add_eat_effect( "/std/effects/ingested/cum_poison", 12 ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:Monkshood"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "wormwood" ) :),
        (: $2->set_short( "ground wormwood" ) :),
        (: $2->add_adjective( "ground" ) :),
        (: $2->add_alias( "plant" ) :),
        (: $2->add_plural( "plants" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks like it might've been wormwood.\n") :),
        (: $2->set_medium_alias( "GroundWormwood" ) :),
        (: $2->add_eat_effect( "/std/effects/ingested/cum_poison", 12 ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:Wormwood"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "bryony" ) :),
        (: $2->set_short( "ground bryony" ) :),
        (: $2->add_adjective( "ground" ) :),
        (: $2->add_alias( "plant" ) :),
        (: $2->add_plural( "plants" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks like it might've been bryony.\n" ) :),
        (: $2->set_medium_alias( "GroundBryony" ) :),
        (: $2->add_eat_effect( "/std/effects/ingested/cum_poison", 12 ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:Bryony"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "achorion" ) :),
        (: $2->set_short( "ground achorion mould" ) :),
        (: $2->add_adjective( ({ "ground", "mould" }) ) :),
        (: $2->add_alias( "mould" ) :),
        (: $2->add_alias( "plant" ) :),
        (: $2->add_plural( "plants" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks like it might've been achorion mould.\n") :),
        (: $2->set_medium_alias( "GroundAchorionMould" ) :),
        (: $2->add_eat_effect( "/std/effects/ingested/cum_poison", 12 ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:AchorionMould"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "trumpet" ) :),
        (: $2->set_short( "ground angel's trumpet" ) :),
        (: $2->add_adjective( ({ "ground", "angel's" }) ) :),
        (: $2->add_alias( "angel's" ) :),
        (: $2->add_alias( "plant" ) :),
        (: $2->add_plural( "plants" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks like it might've been angel's trumpet.\n") :),
        (: $2->set_medium_alias( "GroundAngelsTrumpet" ) :),
        (: $2->add_eat_effect( "/std/effects/ingested/cum_poison", 12 ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:AngelsTrumpet"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "cereus" ) :),
        (: $2->set_short( "ground cereus" ) :),
        (: $2->add_adjective( "ground" ) :),
        (: $2->add_alias( "plant" ) :),
        (: $2->add_plural( "plants" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks like it might've been cereus.\n") :),
        (: $2->set_medium_alias( "GroundCereus" ) :),
        (: $2->add_eat_effect( "/std/effects/ingested/cum_poison", 12 ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:Cereus"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "skullcap" ) :),
        (: $2->set_short( "ground skullcap" ) :),
        (: $2->add_adjective( "ground" ) :),
        (: $2->add_alias( "plant" ) :),
        (: $2->add_plural( "plants" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks like it might've been skullcap.\n") :),
        (: $2->set_medium_alias( "GroundSkullcap" ) :),
        (: $2->add_eat_effect( "/std/effects/ingested/cum_poison", 12 ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:Skullcap"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "weed" ) :),
        (: $2->set_short( "ground tyler weed" ) :),
        (: $2->add_adjective( ({ "ground", "tyler" }) ) :),
        (: $2->add_alias( "tyler" ) :),
        (: $2->add_alias( "plant" ) :),
        (: $2->add_plural( "plants" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks like it might've been tyler weed.\n") :),
        (: $2->set_medium_alias( "GroundTylerWeed" ) :),
        (: $2->add_eat_effect( "/std/effects/ingested/cum_poison", 12 ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:TylerWeed"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "skullcap" ) :),
        (: $2->set_short( "ground skullcap" ) :),
        (: $2->add_adjective( "ground" ) :),
        (: $2->add_alias( "plant" ) :),
        (: $2->add_plural( "plants" ) :),
        (: $2->set_long( "This is $pile_size$ of ground plant, "
            "$amount_size$.  It looks like it might've been skullcap.\n") :),
        (: $2->set_medium_alias( "GroundSkullcap" ) :),
        (: $2->add_eat_effect( "/std/effects/ingested/cum_poison", 12 ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:Skullcap"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "skin" ) :),
        (: $2->set_short( "ground toad skin" ) :),
        (: $2->add_adjective( ({ "ground", "toad" }) ) :),
        (: $2->add_plural( "skins" ) :),
        (: $2->set_long( "This is $pile_size$ of ground skin, "
            "$amount_size$.  It looks like it might've originally belonged "
            "to a toad.\n") :),
        (: $2->set_medium_alias( "GroundToadSkin" ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:SkinOfToad"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 0.5;
  item->func = ({
        (: $2->set_name( "duct" ) :),
        (: $2->set_short( "ground lion bile duct" ) :),
        (: $2->add_adjective( ({ "ground", "lion", "bile" }) ) :),
        (: $2->add_palias( "bile" ) :),
        (: $2->add_plural( "ducts" ) :),
        (: $2->set_long( "This is $pile_size$ of ground bile duct, "
            "$amount_size$.  It looks like it might've originally belonged "
            "to a lion.\n") :),
        (: $2->set_medium_alias( "GroundLionBileDuct" ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:BileDuctOfLion"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "heart" ) :),
        (: $2->set_short( "ground wolf heart" ) :),
        (: $2->add_adjective( ({ "ground", "wolf" }) ) :),
        (: $2->add_plural( "hearts" ) :),
        (: $2->set_long( "This is $pile_size$ of ground heart, "
            "$amount_size$.  It looks like it might've originally belonged "
            "to a wolf.\n") :),
        (: $2->set_medium_alias( "GroundWolfHeart" ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:HeartOfWolf"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "brain" ) :),
        (: $2->set_short( "ground bird brain" ) :),
        (: $2->add_adjective( ({ "ground", "bird" }) ) :),
        (: $2->add_plural( "brains" ) :),
        (: $2->set_long( "This is $pile_size$ of ground brain, "
            "$amount_size$.  It looks like it might've originally belonged "
            "to a bird.\n") :),
        (: $2->set_medium_alias( "GroundBirdBrain" ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:BrainOfBird"] = item;
}

mixed data_return_2() {
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "brain" ) :),
        (: $2->set_short( "ground bird brain" ) :),
        (: $2->add_adjective( ({ "ground", "bird" }) ) :),
        (: $2->add_plural( "brains" ) :),
        (: $2->set_long( "This is $pile_size$ of ground brain, "
            "$amount_size$.  It looks like it might've originally belonged "
            "to a bird.\n") :),
        (: $2->set_medium_alias( "GroundBirdBrain" ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:BrainOfChicken"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "brain" ) :),
        (: $2->set_short( "ground bird brain" ) :),
        (: $2->add_adjective( ({ "ground", "bird" }) ) :),
        (: $2->add_plural( "brains" ) :),
        (: $2->set_long( "This is $pile_size$ of ground brain, "
            "$amount_size$.  It looks like it might've originally belonged "
            "to a bird.\n") :),
        (: $2->set_medium_alias( "GroundBirdBrain" ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:BrainOfDuck"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 0.5;
  item->func = ({
        (: $2->set_name( "brain" ) :),
        (: $2->set_short( "ground bird brain" ) :),
        (: $2->add_adjective( ({ "ground", "bird" }) ) :),
        (: $2->add_plural( "brains" ) :),
        (: $2->set_long( "This is $pile_size$ of ground brain, "
            "$amount_size$.  It looks like it might've originally belonged "
            "to a bird.\n") :),
        (: $2->set_medium_alias( "GroundBirdBrain" ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:BrainOfDuckling"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "brain" ) :),
        (: $2->set_short( "ground bird brain" ) :),
        (: $2->add_adjective( ({ "ground", "bird" }) ) :),
        (: $2->add_plural( "brains" ) :),
        (: $2->set_long( "This is $pile_size$ of ground brain, "
            "$amount_size$.  It looks like it might've originally belonged "
            "to a bird.\n") :),
        (: $2->set_medium_alias( "GroundBirdBrain" ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:BrainOfParrot"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "brain" ) :),
        (: $2->set_short( "ground bird brain" ) :),
        (: $2->add_adjective( ({ "ground", "bird" }) ) :),
        (: $2->add_plural( "brains" ) :),
        (: $2->set_long( "This is $pile_size$ of ground brain, "
            "$amount_size$.  It looks like it might've originally belonged "
            "to a bird.\n") :),
        (: $2->set_medium_alias( "GroundBirdBrain" ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:BrainOfPeacock"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "brain" ) :),
        (: $2->set_short( "ground bird brain" ) :),
        (: $2->add_adjective( ({ "ground", "bird" }) ) :),
        (: $2->add_plural( "brains" ) :),
        (: $2->set_long( "This is $pile_size$ of ground brain, "
            "$amount_size$.  It looks like it might've originally belonged "
            "to a bird.\n") :),
        (: $2->set_medium_alias( "GroundBirdBrain" ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:BrainOfVulture"] = item;
  item = new( class state_change );
  item->result = "/obj/food/sweets/sugar.food";
  item->factor = 5.0;
  item->func = ({ });
  data["grind:SugarCube"] = item;
  item = new( class state_change );
  item->result = "/obj/food/sweets/brown_sugar.food";
  item->factor = 5.0;
  item->func = ({ });
  data["grind:SugarLump"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 0.25;
  item->func = ({
        (: $2->set_name( "powder" ) :),
        (: $2->set_short( "grainy orange powder" ) :),
        (: $2->add_adjective( ({ "grainy", "orange" }) ) :),
        (: $2->add_plural( "powders" ) :),
        (: $2->set_long( "This is $pile_size$ of grainy orange powder, "
            "$amount_size$.  It looks like it might've originally been "
            "solid drain cleaner.\n") :),
        (: $2->set_medium_alias( "GroundDrainCleaner" ) :),
        (: $2->set_decay_speed( 0 ) :)
    });
  data["grind:DrainCleaner"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/water.ob";
  item->factor = 1.0;
  item->func = ({});
  data["heat:Water"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 0.5;
  item->func = ({
      (: $2->set_name( "steeped_achorion" ) :),
      (: $2->set_short( "purple-grey liquid" ) :),
      (: $2->add_adjective( ({ "purple-grey", "liquid" }) ) :),
      (: $2->set_long( "This is $amount_size$ of some purple-grey liquid.\n" ) :),
      (: $2->set_medium_alias( "UndistilledAchorionPurple" ) :),
    });
  data["heat:UnheatedAchorionPurple"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/con_poison.ob";
  item->factor = 0.1;
  item->func = ({ (: $2->add_eat_effect( "/std/effects/poisons/con_new",
    to_int( $3->query_skill_bonus( "covert.items.poisons" ) ) ) :) });
  data["heat:UnheatedWeakConPoison"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/con_poison.ob";
  item->factor = 0.1;
  item->func = ({ (: $2->add_eat_effect( "/std/effects/poisons/con_new",
    to_int( $3->query_skill_bonus( "covert.items.poisons" ) * 10 ) ) :) });
  data["heat:UnheatedConPoison"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/confuse_poison.ob";
  item->factor = 0.1;
  item->func = ({ (: $2->add_eat_effect( "/std/effects/poisons/confuse",
    to_int( $3->query_skill_bonus( "covert.items.poisons" ) * 10 ) ) :) });
  data["heat:UnheatedConfusePoison"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/smokebomb.ob";
  item->factor = 0.5;
  item->func = ({ (: $2->add_eat_effect( "/std/effects/poisons/smokebomb",
    to_int( $3->query_skill_bonus( "covert.items.poisons" ) ) ) :) });
  data["heat:UnheatedSmokeBomb"] = item;
  item = new( class state_change );
  item->result = "/w/taffyd/reagents/caramelised_sugar.food";
  item->factor = 6.4;
  item->func = ({});
  item->playtester = 1;
  data["heat:SuGar"] = item;
  item = new( class state_change );
  item->result = "/w/taffyd/reagents/disgusting_brown_lump.food";
  item->factor = 0.15625;
  item->func = ({ });
  item->playtester = 1;
  data["heat:CaramelisedSugar"] = item;
  item = new( class state_change );
  item->result = "/obj/food/sweets/toffee.food";
  item->factor = 1.0;
  item->func = ({});
  item->playtester = 1;
  data["heat:BrownSugar"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 0.5;
  item->func = ({
  (: $2->set_name("liquid") :),
  (: $2->set_short("cloudy yellowish liquid") :),
  (: $2->set_long("This is a cloudy, yellowish liquid, with faint "
      "traces of red swirls.\n") :),
  (: $2->add_adjective(({ "cloudy", "yellowish", "yellow" })) :)
});
  data["juice:snake left eye"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 0.5;
  item->func = ({
  (: $2->set_name( "juice" ) :),
  (: $2->set_short( "jelly orange juice" ) :),
  (: $2->add_adjective( ({ "jelly", "orange" }) ) :),
  (: $2->set_long( "This is a delicious looking juice that "
      "appears to have been made from juicing up a jelly orange.\n" ) :),
  (: $2->set_medium_alias( "JellyOrangeJuice" ) :),
});
  data["juice:JellyOrange"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
  (: $2->set_name( "juice" ) :),
  (: $2->set_short( "apple juice" ) :),
        (: $2->set_main_plural( "apple juices" ) :),
        (: $2->add_plural( "juices" ) :),
  (: $2->add_adjective( "apple" ) :),
  (: $2->set_long( "This is some transparent golden-brown "
                  "juice made from apples, "
                  "$amount_size$.\n" ) :),
  (: $2->set_medium_alias( "AppleJuice" ) :),
});
  data["juice:Apple"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "blackheart_juice" ) :),
        (: $2->set_short( "apple juice" ) :),
        (: $2->set_main_plural( "apple juices" ) :),
        (: $2->add_plural( "juices" ) :),
        (: $2->add_alias( "juice" ) :),
        (: $2->add_adjective( "apple" ) :),
        (: $2->set_long( "This is some dark, treacly, "
                        "sweet-smelling apple juice, "
                        "$amount_size$.\n" ) :),
        (: $2->set_medium_alias( "BlackheartJuice" ) :),
});
  data["juice:LancreBlackheart"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "disagreeable_juice" ) :),
        (: $2->set_short( "apple juice" ) :),
        (: $2->set_main_plural( "apple juices" ) :),
        (: $2->add_plural( "juices" ) :),
        (: $2->add_alias( "juice" ) :),
        (: $2->add_adjective( "apple" ) :),
        (: $2->set_long( "This is some thin and watery "
                        "sour-smelling apple juice, "
                        "$amount_size$.\n" ) :),
        (: $2->set_medium_alias( "DisagreeableJuice" ) :),
});
  data["juice:GoldenDisagreeable"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name( "billet_juice" ) :),
        (: $2->set_short( "apple juice" ) :),
        (: $2->set_main_plural( "apple juices" ) :),
        (: $2->add_plural( "juices" ) :),
        (: $2->add_alias( "juice" ) :),
        (: $2->add_adjective( "apple" ) :),
        (: $2->set_long( "This is some evil coloured, "
                        "bitter-smelling apple juice, "
                        "$amount_size$.\n" ) :),
        (: $2->set_medium_alias( "BilletJuice" ) :),
        (: $2->add_eat_effect( "/std/effects/ingested/nausea", 50 ) :),
});
  data["juice:GreenBillet"] = item;
  item = new( class state_change );
  item->result = "/std/object";
  item->factor = 1.1;
  item->func = ({
        (: $2->set_name("ice") :),
        (: $2->set_short("ice") :),
        (: $2->set_long("This is a chunk of ice.\n") :)
    });
  data["freeze:Water"] = item;
  item = new( class state_change );
  item->result = "/obj/food";
  item->factor = 0.9;
  item->func = ({
      (: $2->set_name( "potato" ) :),
      (: $2->set_short( "peeled potato" ) :),
      (: $2->add_adjective("peeled") :),
      (: $2->set_long( "This is a raw, peeled potato, ready for what next you do.\n" ) :),
      (: $2->set_medium_alias( "PeeledPotato" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["peel:Potato"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 0.1;
  item->func = ({
      (: $2->set_name( "spider_poison" ) :),
      (: $2->set_short( "pale red liquid" ) :),
      (: $2->add_adjective( ({ "pale", "red", "liquid" }) ) :),
      (: $2->set_long( "This is $amount_size$ of some pale red liquid.\n" ) :),
      (: $2->set_medium_alias( "UndistilledSpiderPoison" ) :),
      (: $2->add_eat_effect( "/std/effects/poisons/spider_venom",
        to_int( $3->query_skill_bonus( "covert.items.poisons" ) / 10 ) ) :),
    });
  data["press:PoisonGlandOfSpider"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 0.1;
  item->func = ({
      (: $2->set_name( "blowfish_poison" ) :),
      (: $2->set_short( "pale brown liquid" ) :),
      (: $2->add_adjective( ({ "pale", "brown", "liquid" }) ) :),
      (: $2->set_long( "This is $amount_size$ of some pale brown liquid.\n" ) :),
      (: $2->set_medium_alias( "UndistilledBlowfishPoison" ) :),
      (: $2->add_eat_effect( "/std/effects/poisons/bloat",
        to_int( $3->query_skill_bonus( "covert.items.poisons" ) / 10 ) ) :),
    });
  data["press:PoisonSacOfBlowfish"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 0.1;
  item->func = ({
      (: $2->set_name( "snake_poison" ) :),
      (: $2->set_short( "thin white liquid" ) :),
      (: $2->add_adjective( ({ "thin", "white", "liquid" }) ) :),
      (: $2->set_long( "This is $amount_size$ of some thin white liquid.\n" ) :),
      (: $2->set_medium_alias( "UndistilledSnakePoison" ) :),
      (: $2->add_eat_effect( "/std/effects/ingested/poison",
        to_int( $3->query_skill_bonus( "covert.items.poisons" ) ) ) :),
    });
  data["press:PoisonSacOfSnake"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
      (: $2->set_name( "venom" ) :),
      (: $2->set_short( "yellow powder" ) :),
      (: $2->add_alias( "powder" ) :),
      (: $2->add_adjective( "yellow" ) :),
      (: $2->set_long( "This is some yellow powder.\n" ) :),
      (: $2->set_medium_alias( "PoisonVenom" ) :),
      (: $2->add_eat_effect("/std/effects/ingested/cum_poison", 16) :),
      (: $2->add_eat_effect("/std/effects/ingested/basic_healing", 4) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["pulverise:Venom"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("Catalyst") :),
        (: $2->set_short("white powder") :),
        (: $2->add_alias("powder") :),
        (: $2->add_adjective("white") :),
        (: $2->set_long("This is some white powder.\n") :),
        (: $2->set_medium_alias("PoisonCatalyst") :),
        (: $2->add_eat_effect("/std/effects/ingested/cum_poison", 12) :),
        (: $2->add_eat_effect("/std/effects/ingested/basic_healing", 2) :),
        (: $2->set_decay_speed(0) :)
    });
  data["pulverise:Catalyst"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("fang") :),
        (: $2->set_short("black powder") :),
        (: $2->add_alias("powder") :),
        (: $2->add_adjective("black") :),
        (: $2->set_long("This is some black powder.\n") :),
        (: $2->set_medium_alias("PoisonFang") :),
        (: $2->set_decay_speed(0) :)
    });
  data["pulverise:Fang"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 50.0;
  item->func = ({
        (: $2->set_name("leaf") :),
        (: $2->set_short("green powder") :),
        (: $2->add_alias("powder") :),
        (: $2->add_adjective("green") :),
        (: $2->set_long("This is some green powder.\n") :),
        (: $2->set_medium_alias("PoisonLeaf") :),
        (: $2->set_decay_speed(0) :)
    });
  data["pulverise:Leaf"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/sulphur_powder.ob";
  item->factor = 10.0;
  item->func = ({});
  data["pulverise:SulphurBlock"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
   (: $2->set_name("alexander") :),
   (: $2->set_short("brandy Alexander") :),
   (: $2->add_adjective( "brandy" ) :),
   (: $2->add_alias( ({ "brandy", "cocktail" }) ) :),
   (: $2->set_long("A creamy brandy Alexander cocktail, full "
             "of brandy, cream and chocolate liqueur goodness.  "
             "It looks perfect.\n") :),
   (: $2->add_eat_effect( "/std/effects/ingested/drunk", 800 ) :),
   (: $2->set_medium_alias( "BrandyAlexander" ) :)
});
  data["shake:UnshakenBrandyAlexander"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
   (: $2->set_name( "cosmopilitian" ) :),
   (: $2->set_short( "Cosmopilitian" ) :),
   (: $2->add_alias( "cocktail" ) :),
   (: $2->set_long( "The perfect Cosmopilitian cocktail.  For "
             "is it not written, \"Things always look better "
             "through the bottom of an empty glass.\"?\n" ) :),
   (: $2->add_eat_effect( "/std/effects/ingested/drunk", 800 ) :),
   (: $2->set_medium_alias( "Cosmopilitian" ) :),
});
  data["shake:UnshakenCosmopilitian"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
   (: $2->set_name( "morporkian" ) :),
   (: $2->set_short( "Morporkian" ) :),
   (: $2->add_alias( "cocktail" ) :),
   (: $2->set_long( "The perfect Morporkian, in drink form.  "
             "This drink does not contain blood, vomit or "
             "sharp knives.\n" ) :),
   (: $2->add_eat_effect( "/std/effects/ingested/drunk", 800 ) :),
   (: $2->set_medium_alias( "Morporkian" ) :),
});
  data["shake:UnshakenMorporkian"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
   (: $2->set_name( "carriage" ) :),
   (: $2->set_short( "Sidecarriage" ) :),
   (: $2->add_alias( "cocktail" ) :),
   (: $2->add_adjective( "side" ) :),
   (: $2->set_long( "A Sidecarriage cocktail, just "
             "the drink for a smooth ride.\n" ) :),
   (: $2->add_eat_effect( "/std/effects/ingested/drunk", 800 ) :),
   (: $2->set_medium_alias( "SideCarriage" ) :),
});
  data["shake:UnshakenSideCarriage"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
   (: $2->set_name( "daiquiri" ) :),
   (: $2->set_short( "Bananana Daiquiri" ) :),
   (: $2->add_alias( "cocktail" ) :),
   (: $2->add_adjective( "bananana" ) :),
   (: $2->set_long( "Mmm, full of mon... ape-y goodness.\n" ) :),
   (: $2->add_eat_effect( "/std/effects/ingested/drunk", 800 ) :),
   (: $2->set_medium_alias( "BananaDaiquiri" ) :),
});
  data["shake:UnshakenBananaDaiquiri"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
   (: $2->set_name( "lady" ) :),
   (: $2->set_short( "Shady Lady" ) :),
   (: $2->add_alias( "cocktail" ) :),
   (: $2->add_adjective( "shady" ) :),
   (: $2->set_long( "An Shady Lady.  For some reason, this "
                  "cocktail seems a little... distasteful, even though "
                  "it smells delicious.\n" ) :),
   (: $2->add_eat_effect( "/std/effects/ingested/drunk", 800 ) :),
   (: $2->set_medium_alias( "ShadyLady" ) :),
});
  data["shake:UnshakenShadyLady"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
   (: $2->set_name( "dwarf" ) :),
   (: $2->set_short( "Well Greased Dwarf" ) :),
   (: $2->add_alias( "cocktail" ) :),
   (: $2->add_adjective( ({ "well", "greased" }) ) :),
   (: $2->set_long( "An Well Greased Dwarf.  The cocktail "
                  "seems short, dark and likely to put you in a vile "
                  "temper.\n" ) :),
   (: $2->add_eat_effect( "/std/effects/ingested/drunk", 800 ) :),
   (: $2->set_medium_alias( "WellGreasedDwarf" ) :),
});
  data["shake:UnshakenWellGreasedDwarf"] = item;
}

mixed data_return_3() {
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "carrot" ) :),
      (: $2->set_short( "sliced carrot" ) :),
      (: $2->add_adjective( "sliced" ) :),
      (: $2->set_long( "This is some sliced carrots, little "
            "orange rings around lighter orange circles.\n" ) :),
      (: $2->set_medium_alias( "SlicedCarrot" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["slice:Carrot"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
        (: $2->set_name("cabbage") :),
        (: $2->set_short("shredded cabbage") :),
        (: $2->add_adjective("shredded") :),
        (: $2->set_long("This is some shredded cabbage.  "
             "Hopefully, not a sentinent one.\n") :),
        (: $2->set_medium_alias("GroundCabbage") :),
        (: $2->set_decay_speed(0) :)
    });
  data["slice:Cabbage"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "rhubarb" ) :),
      (: $2->set_short( "sliced rhubarb" ) :),
      (: $2->add_adjective( "sliced" ) :),
      (: $2->set_long( "This is some sliced rhubarb, sweet little chunks "
          "perfect for a nice pie.\n" ) :),
      (: $2->set_medium_alias( "SlicedRhubarb" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["slice:Rhubarb"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "onion" ) :),
      (: $2->set_short( "sliced onion" ) :),
      (: $2->add_adjective( "sliced" ) :),
      (: $2->set_long( "This is some finely sliced onions.  How very sad...  "
             "could make a bloke break down and sob...\n" ) :),
      (: $2->set_medium_alias( "SlicedOnion" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["slice:Onion"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "parsnip" ) :),
      (: $2->set_short( "sliced parsnip" ) :),
      (: $2->add_adjective( "sliced" ) :),
      (: $2->set_long( "This is some parsnip slices, white and hot.\n" ) :),
      (: $2->set_medium_alias( "SlicedParsnip" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["slice:Parsnip"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "tomato" ) :),
      (: $2->set_short( "sliced tomato" ) :),
      (: $2->add_adjective( "sliced" ) :),
      (: $2->set_long( "This is some sliced tomato, perfect for a sandwich, "
          "but needing more slicing for other purposes.\n" ) :),
      (: $2->set_medium_alias( "SlicedTomato" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["slice:Tomato"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "tomato" ) :),
      (: $2->set_short( "chopped tomato" ) :),
      (: $2->add_adjective( "chopped" ) :),
      (: $2->set_long( "This is some chopped tomato, suitable for cooking.\n" ) :),
      (: $2->set_medium_alias( "ChoppedTomato" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["slice:SlicedTomato"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "potato" ) :),
      (: $2->set_short( "potato chunks" ) :),
      (: $2->add_alias(({"potato chunks", "potato chunk", "chunk", "chunks"})) :),
      (: $2->set_long( "These are some potato chunks.  They could do with "
          "some cooking ere they are eaten.\n" ) :),
      (: $2->set_medium_alias( "PotatoChunks" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["slice:PeeledPotato"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "celery" ) :),
      (: $2->set_short( "sliced celery" ) :),
      (: $2->add_adjective( "sliced" ) :),
      (: $2->set_long( "This is some sliced celery, perfect for a nice "
          "soup or stew.\n" ) :),
      (: $2->set_medium_alias( "SlicedCelery" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["slice:Celery"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "radish" ) :),
      (: $2->set_short( "sliced radish" ) :),
      (: $2->add_adjective( "sliced" ) :),
      (: $2->set_long( "This is some sliced radish, the decorative vegetable.\n" ) :),
      (: $2->set_medium_alias( "SlicedRadish" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["slice:Radish"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "cucumber" ) :),
      (: $2->set_short( "cucumber slices" ) :),
      (: $2->add_alias(({"cucumber slices", "cucumber slice", "slice", "slices"})) :),
      (: $2->add_adjective( "sliced" ) :),
      (: $2->set_long( "These are the remains of a once mighty cucumber, now sliced.  "
          "Tis a great pity.\n" ) :),
      (: $2->set_medium_alias( "SlicedCucumber" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["slice:Cucumber"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "apple" ) :),
      (: $2->set_short( "sliced apple" ) :),
      (: $2->add_adjective( "sliced" ) :),
      (: $2->set_long( "This is some sliced apple, sweet and delicious.\n" ) :),
      (: $2->set_medium_alias( "SlicedApple" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["slice:Apple"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "banana" ) :),
      (: $2->set_short( "sliced banana" ) :),
      (: $2->add_adjective( "sliced" ) :),
      (: $2->set_long( "This is some sliced banana.  It would go "
           "down a treat on buttered toast.\n" ) :),
      (: $2->set_medium_alias( "SlicedBanana" ) :),
      (: $2->set_decay_speed( 0 ) :)
   });
  data["slice:Banana"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_ore.ob";
  item->factor = 0.75;
  item->func = ({
        (: $2->set_name("ore") :),
        (: $2->set_short("refined iron ore") :),
        (: $2->add_adjective(({"refined", "iron"})) :),
        (: $2->set_long("This is $pile_size$ of refined iron ore, "
                        "$amount_size$.\n" ) :),
        (: $2->set_material("refined iron ore") :),
        (: $2->set_material("refined iron ore") :),
        (: $2->set_medium_alias("RefinedIronOre") :)
    });
  data["smelt:RedHotRawIronOre"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_ore.ob";
  item->factor = 0.75;
  item->func = ({
        (: $2->set_name("bloom") :),
        (: $2->set_short("pig of iron bloom") :),
        (: $2->add_adjective(({"pig", "of", "iron"})) :),
        (: $2->add_property("determinate", "a ") :),
        (: $2->add_alias("pig") :),
        (: $2->add_alias("iron") :),
        (: $2->set_material("iron bloom") :),
        (: $2->set_long("This is a lump of crude iron.\n") :),
        (: $2->set_medium_alias("IronBloom") :),
        (: $2->reset_continuous() :)
    });
  data["smelt:RedHotRefinedIronOre"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_ore.ob";
  item->factor = 0.5;
  item->func = ({
        (: $2->set_name("ingot") :),
        (: $2->set_short("wrought iron ingot") :),
        (: $2->add_adjective(({"wrought", "iron"})) :),
        (: $2->add_property("determinate", "a ") :),
        (: $2->add_alias("iron") :),
        (: $2->set_material("wrought iron") :),
        (: $2->set_long("This is an ingot of wrought iron.\n") :),
        (: $2->set_medium_alias("WroughtIron") :),
        (: $2->reset_continuous() :)
    });
  data["hammer:RedHotIronBloom"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({ (:
   $2->set_decay_speed(0),
   $2->set_medium_alias( "SmokeCrystals" ),
   $2->set_name( "smoke_crystal" ),
   $2->set_short( "tiny gray crystals" ),
   $2->set_main_plural( "tiny gray crystals" ),
   $2->set_weight_unit( ({ 1, 50 }) ),
   $2->add_alias( ({ "crystal", "crystals" }) ),
   $2->add_adjective( ({ "tiny", "gray" }) ),
   $2->set_long( "This is $pile_size$ of tiny, dark gray crystals, "
     "$amount_size$.  The individual crystals catch the light on "
     "their facets, and the pile seems to swirl, like smoke.\n" )
:) });
  data["grind:SmokeRock"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({ (:
   $2->set_decay_speed(0),
   $2->set_medium_alias( "CoatedSmokeCrystals" ),
   $2->set_name( "coated_smoke_crystals" ),
   $2->add_alias( "crystals" ),
   $2->set_short( "powdery black crystals" ),
   $2->add_adjective( ({ "powdery", "black" }) ),
   $2->set_long( "This is $pile_size$ of black crystals, about "
     "$amount_size$.  The crystals are covered in fine black powder.\n" )
:) });
  data["air-dry:DampCoatedSmokeCrystals"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_solid.ob";
  item->factor = 1.0;
  item->func = ({ (:
   $2->set_decay_speed(0),
   $2->set_medium_alias( "InertSmokeCrystals" ),
   $2->set_name( "inert_smoke_crystals" ),
   $2->add_alias( "crystals" ),
   $2->set_short( "powdery black crystals" ),
   $2->add_adjective( ({ "powdery", "black" }) ),
   $2->set_long( "This is $pile_size$ of black crystals, about "
     "$amount_size$.  The crystals are covered in fine black powder.\n" )
:) });
  data["air-dry:DampInertSmokeCrystals"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/water.ob";
  item->factor = 1.0;
  item->func = ({});
  data["steep:Water"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "liquid" ) :),
      (: $2->set_short( "darker milky acrid liquid" ) :),
      (: $2->add_adjective( ({ "darker", "milky", "acrid", "liquid" }) ) :),
      (: $2->set_long( "This is $amount_size$ of some darker milky acrid liquid.\n" ) :),
      (: $2->set_medium_alias( "SteepedCereus" ) :),
    });
  data["steep:CereusTea"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/generic_liquid.ob";
  item->factor = 1.0;
  item->func = ({
      (: $2->set_name( "liquid" ) :),
      (: $2->set_short( "dark azure liquid" ) :),
      (: $2->add_adjective( ({ "dark", "azure", "liquid" }) ) :),
      (: $2->set_long( "This is $amount_size$ of some dark azure liquid.\n" ) :),
      (: $2->set_medium_alias( "SteepedSkullcap" ) :),
    });
  data["steep:SkullcapTea"] = item;
  item = new( class state_change );
  item->result = "/obj/reagents/intelligence_tea.ob";
  item->factor = 0.8;
  item->func = ({
    (: $2->add_eat_effect( "/std/effects/ingested/int_bonus" ) :)
  });
  data["featherstir:SageTea"] = item;
  item = new( class state_change );
  item->result = "/obj/food/grains/wheat_berry.food";
  item->factor = 8;
  item->func = ({ });
  data["thresh:Wheat"] = item;
}

mixed data_return() {
  data_return_0();
  data_return_1();
  data_return_2();
  data_return_3();
  return data;
}
