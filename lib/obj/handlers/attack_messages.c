/**
 * This handles the standard messages for attacks.  I cannot be bothered
 * rewriting the docs for this.
 * This handles the standard messages for the given damage types:
 *    blunt, sharp, pierce, fire.
 *
 *  There are also subtypes:
 *    blunt - hands, feet, tail, hoof, wrestle, pommel.
 *    sharp - claws, chew, slice, chop.
 *    pierce - bite, beak, stab, horns, tusks.
 * @author pinkfish
 */

mapping weapon_mess;

/** @ignore */
void create() {
  weapon_mess = ([
    "blunt" : ({
      0, "$N swing$s at $I with $D",
        20, "$N tap$s $I in the $z with $D",
        60, "$N hit$s $I in the $z with $D",
        100, "$N bruise$s $P $z with $D",
        140, "$N hit$s $I hard in the $z with $D",
        180, "$N smash$es $I in the $z with $D",
        220, "$N crush$es $P $z with $D",
        5000, "$N mash$es $P $z with $D",
        }),
    "blunt-tail" : ({
      0, "$N swing$s $p tail at $I",
        20, "$N tap$s $I in the $z with $p tail",
        60, "$N slap$s $I in the $z with $p tail",
        100, "$N bruise$s $P $z with $p tail",
        140, "$N hit$s $I hard in the $z with $p tail",
        180, "$N bash$es $I in the $z with $p tail",
        220, "$N smash$es $p tail into $P $z",
        5000, "$N send$s $I reeling with a powerful blow to the $z with $p tail"
        }),
    "blunt-hoof" : ({
      0, "$N kick$s out at $I",
        20, "$N tap$s $I in the $z with $p hoof",
        60, "$N kick$s $I in the $z with $p hoof",
        100, "$N bruise$s $P $z with $p hoof",
        140, "$N hit$s $I hard in the $z with $p hoof",
        180, "$N smash$es $I in the $z with $p hoof",
        220, "$N crush$es $p hoof into $P $z",
        5000, "$N mash$es $P $z with $p hoof"
        }),
    "blunt-wrestle" : ({
      0, "$N grab$s for $I",
        25, "$N grapple$s with $I",
        50, "$N catch$es $I in an arm lock",
        75, "$N grab$s $I around the neck",
        100, "$N poke$s $I in the face",
        125, "$N throw$s $I to the ground",
        150, "$N fling$s $I over $p shoulder",
        175, "$N punch$es $I in the stomach",
        200, "$N twist$s $P arm painfully",
        225, "$N thump$s $P head hard",
        250, "$N kick$s $I in the back",
        275, "$N jump$s up and down on $I",
        5000, "$N slam$s $P head against the ground"
        }),
    "blunt-hands" : ({
      0, "$N punch$es at $I",
        10, "$N poke$s $I in the $z",
        40,"$N tickle$s $I in the $z",
        70, "$N jab$s $I in the $z",
        100, "$N hit$s $I in the $z with an uppercut",
        130, "$N hit$s $I in the $z with a cross",
        160, "$N hit$s $I in the $z with a vicious hook",
        5000, "$N beat$s $P $z to a pulp",
        }),
    "blunt-feet" : ({
      0, "$N kick$s out at $I",
        10, "$N poke$s $I",
        40, "$N tickle$s $I in the $z with $p toes",
        70, "$N kick$s $I in the $z",
        100, "$N viciously boot$s $I in the $z",
        130, "$N kick$s $I in the $z",
        160, "$N viciously kick$s $I in the $z",
        5000, "$N kick$s $P $z into a bloody mess"
        }),
    "blunt-pommel": ({
      0, "$N strike$s at $I with the pommel of $D",
        20, "$N tap$s $I in the $z with the pommel of $D",
        60, "$N knock$s $P $z with the hilt of $D",
        100, "$N bruise$s $P $z with the pommel of $D",
        140, "$N bash$es $I in the $z with the hilt of $D",
        180, "$N beat$s $I in the $z with the pommel of $D",
        220, "$N smash$es the hilt of $D into $P $z",
        5000, "$N smash$es $I in the $z with the pommel of $D",
        }),
    "blunt-cane" : ({
      0, "$N strike$s at $I with $D",
        20, "$N tap$s $P $z with $D",
        60, "$N knock$s $P $z with $D",
        100, "$N strike$s $I lightly in the $z with $D",
        140, "$N strike$s $I in the $z with $D",
        180, "$N strike$s $I hard in the $z with $D",
        220, "$N batter$s $P $z with $D",
        5000, "$N bludgeon$s $P $z with $D",
        }),
    "blunt-self" : ({
      0, "$N swing$s at $I",
        20, "$N tap$s $I in the $z",
        60, "$N hit$s $I in the $z",
        100, "$N bruise$s $P $z",
        140, "$N hit$s $I hard in the $z",
        180, "$N smash$es $I in the $z",
        220, "$N crush$es $P $z",
        5000, "$N mash$es $P $z",
        }),
    "sharp" : ({
      0, "$N slash$es at $I with $D",
        20, "$N snick$s $P $z with $D",
        60, "$N scratch$es $P $z with $D",
        100, "$N nick$s $P $z with $D",
        140, "$N cut$s $P $z with $D",
        180, "$N slice$s $P $z with $D",
        220, "$N hack$s $P $z with $D",
        5000, "$N chop$s up $P $z with $D"
        }),
    "sharp-claws" : ({
      0, "$N rake$s at $I with $p claws",
        10, "$N snick$s $P $z with $p claws",
        40, "$N scratch$es $P $z with $p claws",
        70, "$N nick$s $P $z with $p claws",
        100, "$N claw$s $P $z",
        130, "$N slice$s $P $z up with $p claws",
        160, "$N hack$s $P $z up with $p claws",
        5000, "$N cut$s $P $z into small pieces with $p claws",
        }),
    "sharp-chew" : ({
      0, "$N bite$s at $I",
        10, "$N chew$s on $P $z",
        40, "$N munch$es on $P $z",
        70, "$N gash$es $P $z with $p teeth",
        100, "$N rip$s flesh off $P $z",
        130, "$N tear$s deeply into $P $z",
        160, "$N rip$s $P $z to pieces with $p teeth",
        5000,  "$N rend$s huge chunks of flesh from $P $z",
        }),
    "sharp-slice": ({
      0, "$N slice$s at $I with $D",
        20, "$N just manage$s to slice $P $z with $D",
        60, "$N slice$s $D across $P $z",
        100, "$N shred$s $P $z with $D",
        140, "$N slice$s $D into $P $z",
        180, "$N take$s a sliver off $P $z with $D",
        220, "$N slice$s $D deeply into $P $z",
        5000, "$N neatly fillet$s $P $z with $D",
        }),
    "sharp-chop": ({
      0, "$N swing$s $D at $I",
        20, "$N attempt$s to chop $P $z with $D",
        60, "$N chop$s at $P $z with $D",
        100, "$N chop$s $P $z with $D",
        140, "$N chop$s $D into $P $z",
        180, "$N hack$s into $P $z with $D",
        220, "$N chop$s $D deeply into $P $z",
        5000, "$N chop$s $P $z into pieces with $D",
        }),
    "pierce": ({
      0, "$N thrust$s at $I with $D",
        20, "$N barely hit$s $I in the $z with $D",
        60, "$N jab$s $D into $P $z",
        100, "$N pierce$s $P $z with $D",
        140, "$N impale$s $P $z on $D",
        180, "$N skewer$s $P $z with $D",
        220, "$N run$s $I through the $z with $D",
        5000, "$N make$s a kebab of $P $z with $D",
        }),
    "pierce-spike": ({
      0, "$N swing$s at $I with $D",
        20, "$N barely hit$s $I in the $z with $D",
        60, "$N jab$s $D into $P $z",
        100, "$N spike$s $P $z with $D",
        140, "$N impale$s $P $z on $D",
        180, "$N ventilate$s $P $z with $D",
        220, "$N perforate$s $P $z with $D",
        5000, "$N make$s a mess of $P $z with $D",
        }),
    "pierce-beak" : ({
      0, "$N pecks at $I with $p beak",
        10, "$N snick$s $P $z",
        40, "$N scratch$es $P $z with $p beak",
        70, "$N peck$s $P $z",
        100, "$N viciously peck$s $P $z",
        130, "$N gouge$s a hole in $P $z with $p beak",
        160, "$N slice$s $P $z with $p beak",
        5000, "$N slice$s $P $z into small bits with $p beak",
        }),
    "pierce-stab": ({
      0, "$N thrust$s at $I with $D",
        20, "$N barely stab$s $I in the $z with $D",
        60, "$N stab$s $I in the $z with $D",
        100, "$N messily stab$s $I in the $z with $D",
        140, "$N stab$s $I deeply in the $z with $D",
        180, "$N perforate$s $P $z with $D",
        220, "$N pierce$s $P $z with $D",
        5000, "$N stab$s $D right through $P $z",
        }),
    "pierce-horns": ({
      0, "$N attempt$s to gore $I with $p horns",
        20, "$N prod$s $I in the $z with $p horns",
        60, "$N poke$s $I in the $z with $p horns",
        100, "$N butt$s $I in the $z with $p horns",
        140, "$N pierce$s $I deeply in the $z with $p horns",
        180, "$N ram$s $I in the $z with $p horns",
        220, "$N gore$s $P $z with $p horns",
        5000, "$N impale$s $P $z on $p horns",
        }),
    "pierce-tusks": ({
      0, "$N attempt$s to gore $I with $p tusks",
        20, "$N prod$s $P $z with $p tusks",
        60, "$N poke$s $I in the $z with $p tusks",
        100, "$N jab$s $p tusks into $P $z",
        140, "$N pierce$s $P $z with $p tusks",
        180, "$N stab$s $p tusks deeply into $P $z",
        220, "$N gore$s $P $z with $p tusks",
        5000, "$N impale$s $P $z on $p tusks",
        }),
    "pierce-bite" : ({
      0, "$N attempt$s to bite $I",
        10, "$N nip$s $I in the $z",
        40, "$N nibble$s at $P $z",
        70, "$N bite$s $I in the $z",
        100, "$N sink$s $p teeth into $P $z",
        130, "$N bite$s into $P $z",
        160, "$N sink$s $p teeth deep into $P $z",
        5000, "$N bite$s deeply into $P $z, splintering $P bones",
        }),
    "fire" : ({
      20, "$N smoke$s $I",
        60, "$N toast$s $I",
        100, "$N scorch$es $I",
        140, "$N crisp$s $I",
        180, "$N singe$s $I",
        220, "$N fry$s $I",
        260, "$N roast$s $I",
        300, "$N melt$s $I",
        340, "$N char$s $I",
        380, "$N burn$s $I",
        420, "$N incinerate$s $I",
        460, "$N cremate$s $I",
        5000, "$N vapourise$s $I",
        }),
    ]);
}

/** @ignore */
string parse_string(string mess, object him, object me, object wep,
                    string zone) {

   return replace( replace( mess, ({
      "$mposs$ $D", (string)wep->poss_short() }) ),
         ({ "$hname$", (string)him->query_name(),
            "$mname$", (string)me->query_name(),
            "$hcname$", (string)him->the_short(),
            "$mcname$", (string)me->the_short(),
            "$hposs$", (string)him->query_possessive(),
            "$mposs$", (string)me->query_possessive(),
            "$hpname$", (string)him->poss_short(),
            "$mpname$", (string)me->poss_short(),
            "$hpron$", (string)him->query_pronoun(),
            "$mpron$", (string)me->query_pronoun(),
            "$hobj$", (string)him->query_objective(),
            "$mobj$", (string)me->query_objective(),
            "$wname$", (string)wep->query_name(),
            "$wcname$", (string)wep->short(0),
            "$zname$", zone})
   );
} /* parse_string() */

mixed query_message( int dam, string type, object him, object me,
                     string name, object wep, string zone) {
  int j;
  mixed ind;
  string tmp;

  if(!zone) {
    zone = "body";
  } else {
    if(zone[sizeof(zone)]-1 == 's')
      zone = zone[0..<2];
  }

  ind = wep->query_attack_message(name, type);

  if (!ind) {
    // When we changed weapons to use AT_SKILL instead of AT_NAME
    // it broke unarmed combat.
    if(type == "unarmed") {
      switch(name) {
      case "bite":
      case "beak":
      case "stab":
      case "horns":
      case "tusks":
        type = "pierce";
        break;
      case "claws":
      case "chew":
      case "slice":
      case "chop":
        type = "sharp";
        break;
      default:
        type = "blunt";
      }
    }
    ind = weapon_mess[type+"-"+name];
    if(!ind) {
      ind = weapon_mess[type];
      if (!ind) {
        return 0;
      }
    }
  }

  /* Find the correct message */
  while (j < sizeof(ind) && dam > ind[j]) {
    j += 2;
  }
  if (j >= sizeof(ind)) {
    j = sizeof(ind)-2;
  }

  ind = ind[j+1];
  /* If it is a string, new format */
  if (stringp(ind)) {
    tmp = replace(ind,
                  ({ "$N", (string)me->the_short(),
                       "$p ", (string)me->query_possessive() +" ",
                       "$r", (string)me->query_pronoun(),
                       "$o", (string)me->query_objective(),
                       "$z", zone,
                       "$D", wep->poss_short(),
                       "$I", him->the_short(),
                       "$P", (string)him->the_poss_short(),
                       "$O", (string)him->query_objective(),
                       "$es", "es", "$s", "s" }) );

    ind = ({ replace( ind, ({ "$N", "you", "$p ", "your ",
                                "$r", "you", "$o", "you",
                                "$es", "", "$s", "",
                                "$D", wep->poss_short(),
                                "$z", zone,
                                "$I",  him->the_short(),
                                "$P", him->the_poss_short(),
                                "$O", him->query_objective(),
                                
                                }) ),
               tmp,
               tmp });
    return ind;
  }
  return ({
    parse_string(ind[0], him, me, wep, zone),
      parse_string(ind[1], him, me, wep, zone),
      parse_string(ind[2], him, me, wep, zone)});
} /* query_message() */
