/*  -*- LPC -*-  */
/*
 * $Id: vessel.c,v 1.66 2003/07/23 14:32:19 terano Exp $
 */
/**
 * This is a container specifically for holding liquids.  The hope is
 * to reduce the memory requirements by taking all of the liquid stuff
 * out of /obj/container, since most of the containers are sacks and
 * chests.  Also, the reaction handler will replace the potion space.
 * <p>
 * Some additional notes:
 * <ol>
 *    <li>As is (hopefully) documented elsewhere, the base units of
 *        volume for most continuous media are drops and pinches, both of
 *        which are roughly 1/4 of a cc.  This means that water has
 *        200 drops per weight unit (1g/cc).  Non-continuous objects
 *        are assumed to be the same density as water.
 * </ol>
 * @@author Jeremy
 */


#define REACTION_HANDLER ("/obj/handlers/reaction")
#define TASTE_AMT 200

#include <tasks.h>
#include <move_failures.h>
#include <volumes.h>

inherit "/obj/baggage";

private int leak_rate;
private int hb_count;
private int sub_query_contents;
private int volume;
private int max_volume;
private int cont_volume;
private int is_liquid;
private nosave int* _fraction;
private nosave object* _liquids;

int drink_amount(int drinking, object player);
private int query_fighting(object player);


/**
 * This method sets the leak rate of the container.  The leak rate is how
 * fast stuff leaks out of the container.
 * @param i the new leak rate of the container
 * @see query_leak_rate() */
void set_leak_rate(int i) { leak_rate = i; }


/**
 * This method returns the leak rate of the container
 * @see set_leak_rate()
 * @return the current leak rate */
int query_leak_rate() { return leak_rate; }


/**
 * This method sets the maximum volume of the container.
 * @param v the new maximum volume
 * @see add_volume()
 * @see query_max_volume() */
void set_max_volume(int v) { max_volume = v; }


/**
 * This method returns the current maxium volume associated with this
 * container.
 * @return the current maximum volume */
int query_max_volume() { return max_volume; }


string *leak_verb = ({ " drips slowly",
                       " drips",
                       " dribbles",
                       " trickles slowly",
                       " trickles",
                       " trickles rapidly",
                       " pours sluggishly",
                       " pours",
                       " streams",
                       " gushes",
                       " fountains"
                     });


string *drink_pat = ({ "[from] <direct:object>",
                       "<fraction> {of|from} <direct:object>"
                   });                   
string *splash_pat = ({ "[from] <direct:object> {on|to|onto} <indirect:living>",
                       "<fraction> {of|from} <direct:object> {on|to|onto} "
                       "<indirect:living>"
                   });                   
string *apply_pat = ({ "[from] <direct:object> {on|to} <indirect:living>",
                       "<fraction> {of|from} <direct:object> {on|to} "
                       "<indirect:living>"
                   });                   
string *pour_pat = ({ "<direct:object> {from|into} <indirect:object>",
                      "<fraction> of <direct:object> {from|into} <indirect:object>"
                  });                  
string *fill_pat = ({ "<indirect:object> <fraction> full {from|into} <direct:object>",
                      "<indirect:object> <fraction> up {from|into} <direct:object>",
                      "<indirect:object> {from|into} <direct:object>"
                  });
                  
                  
void create() {
    do_setup++;
    ::create();
    do_setup--;
    if ( !do_setup ) {
      this_object()->setup();
      this_object()->reset();
    }
    add_help_file("vessel");
} /* create() */


/** @ignore yes */
void init() {
    ::init();
    add_command("drink", drink_pat);
    add_command("quaff", drink_pat);
    add_command("splash", splash_pat);
    add_command("rub", apply_pat);
    add_command("apply", apply_pat);
    add_command("pour", pour_pat);
    add_command("taste", "<direct:object>");
    add_command("sip", "<direct:object>");
    //add_command("smell",
    add_command("fill", fill_pat);
    add_command("empty", "<direct:object>");
} /* init() */


/**
 * This returns an adjective for how full the current object is with
 * liquid.  This is used in the parse command handling code.
 * @return the fullness adjective
 * @see query_max_volume() */
string *fullness_adjectives() {
   if (!max_volume) return ({ "totally", "empty" });
   switch (100 * volume / max_volume) {
   case 0..4: return ({ "totally", "empty" });
   case 5..13: return ({ "empty" });
   case 65..94: return ({ "full" });
   case 95..100: return ({ "totally", "full" });
   default: return ({ });
   }
} /* fullness_adjectives() */


/** @ingore yes */
mixed stats() {
  return ::stats() + ({
      ({ "leak_rate", leak_rate }),
      ({ "volume", volume }),
      ({ "cont_volume", cont_volume }),
      ({ "liquid", is_liquid }),
      ({ "max_volume", max_volume })
    });
} /* stats() */


int cmp_amount_r(object a, object b) {
    return ((int)b->query_amount() - (int)a->query_amount());
} /* cmp_amount() */


int cmp_weight_r(object a, object b) {
    if (a->query_weight() || b->query_weight())
      return ((int)b->query_weight() - (int)a->query_weight());
    return cmp_amount_r(a, b);
} /* cmp_weight() */


private void figure_out_liquids() {
   _liquids = filter(all_inventory(this_object()), (: $1->query_liquid() :));
   _liquids = sort_array(_liquids, "cmp_amount_r", this_object());
} /* figure_out_liquids() */


/**
 * This method returns the description of the liquid inside the vessel.
 * @return the current liquids description */
string query_liquid_desc() {
    object *contents, *solids = ({});
    int i;
    string desc, *shorts;
    mixed *others;

    contents = all_inventory(this_object());
    if( !sizeof( contents ) ) {
       return (string) 0;
    }

    figure_out_liquids();
    solids = contents - _liquids;
    if (sizeof(_liquids)) {
       _liquids = sort_array(_liquids, "cmp_amount_r", this_object());
       others = unique_array(_liquids->a_short(),
                          (: "/global/events"->convert_message($1) :));
       shorts = ({ });
       for (i = 0; i < sizeof(others); i++) {
          shorts += ({ others[i][0] });
       }
       desc = shorts[0];
       if (sizeof(shorts) > 1) {
          desc += ", mixed with ";
          if (sizeof(shorts) > 4) {
             desc += "other liquids";
          } else {
             desc += query_multiple_short(shorts[1..]);
          }
       }
    } else {
       desc = "";
    }
    if (!sizeof(solids)) {
        return desc;
    }
    solids = sort_array(solids, "cmp_weight_r", this_object());
    others = unique_array(solids,
                          (: $1->query_continuous()?
                           this_player()->convert_message($1->a_short()) :
                           $1 :) );

    for (i = 0; i < sizeof(others); i++) {
       if( arrayp( others ) ) {
          others[i] = others[i][0];
       } else {
          // Er...  OK, so wtf is it?
          tell_creator( "gruper", "others is %O.\n", others );
       }
    }

    if (sizeof(_liquids)) {
       desc += ", with ";
    }
    if (sizeof(others) > 10) {
        desc += "various undissolved substances";
    } else {
        desc += query_multiple_short(others);
    }
    if (sizeof(_liquids)) {
       desc += " floating in it";
    }
    return desc;
} /* query_liquid_desc() */


/**
 * This method returns the fullness description of the vessel.
 * @return the fullness description of the vessel */
string query_fullness_desc() {
   int full;
   
/* Ok, here comes a terrible kludge, but it's all I can think of 
 * that will fix a buglet without introducing more serious Bugs.
 * It's a hack around continous objects and their volumes.   - Tilly */
   
   full = (100 * volume) / max_volume;
   if( full < 1 && query_liquid_desc() ) {
      full = 1;
   }
   switch (full) {
      case 0:       return "";
      case 1..12:   return "It is almost empty.";
      case 13..37:  return "It is about one-quarter full.";
      case 38..62:  return "It is about half full.";
      case 63..87:  return "It is about three-quarters full.";
      case 88..95:  return "It is almost full.";
      case 96..100: return "It is completely full.";
      default:      return "Its capacity defies the laws of physics.  " 
                           "Please submit a bug report.";
    }
}


/* This is a bit of a kludge, but I need a way to inhibit query_contents()
 * in long() of /obj/baggage.  It may be that I could permanently
 * replace query_contents() with query_liquid_desc(), but I'm not sure... */
/** @ignore yes */
varargs string query_contents(string str, object *obs ) {
    string s, c;

/* This next bit used to check for volume, but to deal with the buglet 
 * mentioned in the previous function, I changed it to check for the 
 * presence of some kind of contents instead.  - Tilly */
    if ( query_liquid_desc() ) {
        c = "\n"+ query_fullness_desc() +"\n";
    } else {
        c = "";
    }

    s = query_liquid_desc();
    if (sub_query_contents) {
        if (s) {
            return str + capitalize(s) + "." + c;
        }
        return ::query_contents(str, obs) + c;
    }
    return ::query_contents(str, obs);
} /* query_contents */

/** @ignore yes */
string short(int dark) {
  object *inv;

  if(query_opaque())
    return ::short(dark);

  inv = all_inventory(this_object());
  if(!sizeof(inv))
    return ::short(dark);

  return ::short(dark) + " of " + 
    "/global/events"->convert_message(query_multiple_short(map(inv, 
    (: $1->query_short() :))));
}

/** @ignore yes */
string long(string str, int dark) {
    string ret;

    sub_query_contents = 1;
    ret = ::long(str, dark);
    sub_query_contents = 0;
    return ret;
} /* long() */

int query_cont_volume() { return cont_volume; }


/**
 * This method returns if this vessel is currenly a liquid.  This means
 * it has a liquid inside it.
 * @see calc_liquid()
 * @return 1 if it is a liquid, 0 if not */
int query_liquid() { return is_liquid; }


/* This method determines if we have any liquids inside us at all. */
void calc_liquid() {
    if (sizeof(filter(all_inventory(), (: $1->query_liquid() :)))) {
      is_liquid = 1;
    } else {
      is_liquid = 0;
    }
}


/**
 * This method returns the current amount of liquid in the container.
 * @return the current amount of liquid in the container */
int query_volume() { return volume; }


/**
 * This method returns the amount of volume left for liquids to be
 * added into.
 * @return the amount of volume left
 * @see add_volume()
 * @see transfer_liquid_to() */
int query_volume_left() {
  if (!query_max_weight()) {
    return max_volume - volume;
  }
  return max_volume - volume - (max_volume*query_loc_weight())/query_max_weight();
} /* query_volume_left() */


/**
 * This method returns the amount of volume left for liquids to be
 * added into.
 * @param vol the amount of volume added
 * @return 1 if the addition was successful, 0 if not
 * @see add_volume() */
int add_volume(int vol) {
   if ((vol <= 0) || !max_volume || (vol + volume <= max_volume)) {
      volume += vol;
      if (previous_object()->query_continuous()) {
          cont_volume += vol;
      }
      return 1;
    }
    // Should spillage be handled here, or in the caller?
    return 0;
} /* add_volume() */


/** @ignore yes */
int add_weight(int n) {
    int v;

    // Debugging.  Can be removed if I forget -- Jeremy
    if (this_player() && (this_player()->query_name() == "pinkfish")) {
      tell_creator("pinkfish", "vessel::add_weight(%O)\n", n);
    }
    if (previous_object()->query_continuous()) {
       return (::add_weight(n));
    }
    //if (!(v = previous_object()->query_amount()))
      v = n*200;
    if (max_volume && (v + volume > max_volume)) {
       return 0;
    }
    if (::add_weight(n)) {
        //printf("Increasing volume by %d (add_weight)\n", v);
        volume += v;
        return 1;
    }
    return 0;
} /* add_weight() */


/**
 * This method removes some volume of liquid from the container.
 * @param vol_lost the amount of volume removed
 * @see add_volume()
 * @see query_volume() */
int remove_volume(int vol_lost)
{
    // Removes equal proportions of all continuous matter.
    int amt_lost, i, orig_cv;
    object *contents;

    if (!cont_volume) {
        return 0;
    }
    orig_cv = cont_volume;
    contents = all_inventory(this_object());
    for (i = 0; i < sizeof(contents); i++) {
        if (contents[i]->query_continuous()) {
            amt_lost = -to_int((int)contents[i]->query_amount()
                               * (to_float(vol_lost) / orig_cv));
            if (!amt_lost) {
                // Always take at least one unit
                amt_lost++;
            }
            contents[i]->adjust_amount(amt_lost);
        }
    }
    return vol_lost;
} /* remove_volume() */


/**
 * This method transfers a given amount of a liquid to a new container.
 * @param dest the destination of the liquid
 * @param vol_xferred the amount of volume transfered */
int xfer_volume(int vol_xferred, object dest) {
    // Transfers equal portions of all continuous matter to dest.
    // If successful, returns 0; if it failed for some reason, it returns
    // the volume not transferred (note that full checks should be done
    // by the caller).
    int vol_to_go;
    int i;
    int amt_xferred;
    int tmp;
    int orig_cv;
    object *contents;
    object copy;
    string file_path;
    mapping map;

    vol_to_go = vol_xferred;
    if (!cont_volume) {
        return 0;
    }
    orig_cv = cont_volume;
    contents = filter(all_inventory(this_object()),
                      (: $1->query_continuous() :));
    for (i = 0; i < sizeof(contents) && vol_to_go > 0; i++) {
        // This should prevent roundoff errors.
        if (i == sizeof(contents) - 1) {
            amt_xferred = vol_to_go;
        } else {
            amt_xferred = to_int((int)contents[i]->query_amount()
                                 * (to_float(vol_xferred) / orig_cv));
        }
        if (!amt_xferred) {
            // Always take at least one unit
            amt_xferred++;
        }
        file_path = explode( file_name(contents[i]), "#" )[ 0 ];
        copy = clone_object(file_path);
        map = (mapping)contents[i]->query_dynamic_auto_load();
        copy->init_dynamic_arg( map );
        map = (mapping)contents[i]->query_static_auto_load();
        if (map) {
            copy->init_static_arg( map );
        }
        copy->set_amount(amt_xferred);
        tmp = copy->move(dest);
        if (tmp == MOVE_OK) {
            // There should probably be enough checks before here to
            // make sure the move succeeds, or an explanation why it
            // didn't.
            vol_to_go -= amt_xferred;
            contents[i]->adjust_amount(-amt_xferred);
        } else {
            copy->dest_me();
        }
    }
    return vol_to_go;
} /* xfer_volume() */


/** @ignore yes */
void heart_beat() {
    // Note that having a leak rate can be expensive, so it should only
    // be done if it's important to the application (such as using it
    // to impose a time restriction).
    int lost, off;

    if (leak_rate == 0 || !is_liquid) {
        set_heart_beat(0);
        return;
    }
    if (hb_count--) return ;
    hb_count = 10;
    lost = leak_rate;
    if (lost > cont_volume)
      lost = cont_volume;
    off = lost/100;
    if (off > 10)
        off = 10;
 // tell_room(environment(),
 //     capitalize(query_liquid_desc())+leak_verb[off]+" out of the "+
 //     short(1)+".\n");
 /* This is hacked because as far as I can tell there is no way to get a 'generic short'
  * for a continuous liquid, ie: some water, instead of two pints of water. */
    if ( interactive( environment() ) )
        tell_object( environment(), "$C$Some " + query_multiple_short( map( all_inventory(),
        (: $1->query_short() :) ) ) + leak_verb[off] + " out of the "+ short(1) + ".\n" );
    else 
      tell_room( environment(), "$C$Some " + query_multiple_short( map( all_inventory(),
        (: $1->query_short() :) ) ) + leak_verb[off] + " out of the "+ short(1) + ".\n" );
  
  (void)remove_volume(lost);
  if (!cont_volume) {
    set_heart_beat(0);
  }
} /*heart_beat() */


/** @ignore yes */
int do_pour(object *to, mixed *args_b, mixed *args_a, mixed *args) {
    int m, n, volume_needed, their_volume, their_max, ovf, xfer_result;

    if (query_fighting(this_player())) {
       add_failed_mess("You cannot attempt to do this while in combat.\n");
       return 0;
    }

    if (environment(this_object()) != this_player()) {
        add_failed_mess("You aren't carrying $D.\n");
        return 0;
    }
    if (sizeof(args) == 5) {
        //m = args[1];
        //n = args[2];
        sscanf(args[0] + " " + args[1], "%d %d", m, n);
        if ((m > n) || (m < 0) || (n <= 0)) {
            add_failed_mess("Interesting fraction you have there!\n");
            return 0;
        }
    } else {
        m = 1;
        n = 1;
    }

    if (query_locked()) {
        add_failed_mess("$C$$D $V$0=is,are$V$ locked!\n");
        return 0;
    }
    if (query_closed()) {
        if (do_open()) {
            write("You open the " + short(0) + ".\n");
        } else {
            add_failed_mess("You cannot open $D.\n");
            return 0;
        }
    }

    if (cont_volume <= 0) {
        add_failed_mess("$C$$D has nothing to pour!\n");
        return 0;
    }

    if (sizeof(to) > 1) {
        add_failed_mess("You can only pour into one object at a time.\n");
        return 0;
    }

    their_volume = (int)to[0]->query_volume();
    their_max = (int)to[0]->query_max_volume();
    if (their_max <= 0) {
        add_failed_mess("$C$" + to[0]->the_short(0) +
            " doesn't look like it can be filled!\n");
        return 0;
    }

    if (their_volume >= their_max) {
        add_failed_mess("The " + to[0]->short(0)
              + " $V$0=is,are$V$ full to the brim already.\n");
        their_volume = their_max;
    }
    if ((m == 1) && (n == 1)) {
        volume_needed = volume;
    } else {
        volume_needed = max_volume * m / n;
    }
    if (volume < volume_needed) {
        add_failed_mess("$C$$D $V$0=is,are$V$ less than " + m + "/" + n +
              " full.\n");
        return 0;
    }
    if (volume_needed > 120) {
        // +/- 1 ounce (could make this skill-dependent...)
        volume_needed += random(240) - 120;
    }
    if (volume_needed > (their_max - their_volume)) {
        volume_needed = their_max - their_volume;
        ovf = 1;
    }

    if (volume_needed > cont_volume) {
        add_failed_mess("You drain the " + short(0) + " into the "
              + to[0]->short(0) + " but it $V$0=is,are$V$ not enough.\n");
        volume_needed = cont_volume;
        this_player()->add_succeeded(to[0]);
    } else {
        this_player()->add_succeeded(to[0]);
    }

    xfer_result = xfer_volume( volume_needed, to[0] );

    //If the result is less then needed, then it worked.
    //If it is exactly the same, then 0 was transferred.
    if (xfer_result < volume_needed) {
        this_player()->add_succeeded_mess(this_object(), "$N $V $D into $I.\n", ({to[0]}));
    }
    else {
        add_failed_mess( "You were unable to $V $D into $I.\n", ({to[0]}));
        return 0;
    }
    if (ovf) {
        this_player()->add_succeeded_mess(this_object(), "$N $V $D into $I, " +
            "spilling some in the process.\n", ({to[0]}));
    }
    return 1;
} /* do_pour() */

/** @ignore yes */
int do_fill(object *to, mixed *args_b, mixed *args_a, mixed *args) {
    int m;
    int n;
    int i;
    int run_out;
    int volume_needed;
    int their_volume;
    int their_max;
    int amount_not_poured;
    int ok;

    if (query_fighting(this_player())) {
       add_failed_mess("You cannot attempt to do this while in combat.\n");
       return 0;
    }

    if (sizeof(args) == 4) {
        //m = args[1];
        //n = args[2];
        sscanf(args[1] + " " + args[2], "%d %d", m, n);
        if ((m > n) || (m < 0) || (n <= 0)) {
            add_failed_mess("Interesting fraction you have there!\n");
            return 0;
        }
    } else {
        m = 1;
        n = 1;
    }

    if (query_closed() &&
        query_locked()) {
        add_failed_mess("$C$$D $V$0=is,are$V$ locked!\n");
        return 0;
    }

    if (query_closed()) {
        if (do_open()) {
            write("You open the " + short(0) + ".\n");
        } else {
            add_failed_mess("You cannot open $D.\n");
            return 0;
        }
    }

    if (cont_volume <= 0) {
        add_failed_mess("$C$$D has nothing to pour!\n");
        return 0;
    }

    run_out = 0;
    for (i = 0; i < sizeof(to) && !run_out; i++) {
        if ((environment(this_object()) != this_player()) &&
            (environment(to[i]) != this_player())) {
            write("You're not carrying " + the_short() + " or " +
                  to[i]->the_short() + ".\n");
            continue;
        }

        if (to[i]->query_closed()) {
            add_failed_mess("$I $V$0=is,are$V$ closed.\n", to[i..i]);
            continue;
        }

        their_volume = (int)to[i]->query_volume();
        their_max = (int)to[i]->query_max_volume();
        if (their_max <= 0) {
            add_failed_mess("$I doesn't look like it can be filled!\n",
                            to[i..i]);
            continue;
        }

        if (their_volume >= their_max) {
            add_failed_mess("$I $V$0=is,are$V$ full to the brim already.\n", to[i..i]);
            continue;
        }

        volume_needed = their_max * m / n;
        if (their_volume >= volume_needed) {
            add_failed_mess("$I $V$0=is,are$V$ more than " + m + "/" + n +
                  " full already.\n", to[i..i]);
            continue;
        }
        if (volume_needed > 120) {
            // +/- 1 ounce (could make this skill-dependent...)
            volume_needed += random(240) - 120;
        }
        if (volume_needed > their_max) {
            volume_needed = their_max;
        }
        ok++;
        volume_needed -= their_volume;

        if (volume_needed > cont_volume) {
            add_failed_mess("You drain " + the_short() + " into "
                  + to[i]->the_short() + " but it $V$0=is,are$V$ not enough.\n");
            volume_needed = cont_volume;
            run_out = 1;
            this_player()->add_succeeded(to[i]);
        } else {
            this_player()->add_succeeded(to[i]);
        }
        amount_not_poured = xfer_volume(volume_needed, to[i]);
        if (amount_not_poured) {
           ok--;
        }
    }

    if (!ok) {
       add_failed_mess("You cannot seem to do anything useful with this "
                       "container, it seems unwilling to accept what you "
                       "offer.\n");
    }

    return ok;
} /* do_fill() */


/**
 * This method checks to see if they are fighting anyone and if anyone
 * (that can see them) is fighting them.
 * @param player the player to check */
private int query_fighting(object player) {
   object ob;

   if (!player || !environment(player)) {
      return 0;
   }

   if (player->query_fighting()) {
      return 1;
   }

   foreach (ob in all_inventory(environment(player))) {
      if (living(ob)) {
         if (ob->query_attacker_list() &&
             member_array(this_player(), ob->query_attacker_list()) != -1) {
            return 1;
         }
      }
   }

   if (environment(ob)->query_mirror_room()) {
      foreach (ob in all_inventory(environment(player)->query_mirror_room())) {
         if (living(ob)) {
            if (member_array(this_player(), ob->query_attacker_list()) != -1) {
               return 1;
            }
         }
      }
   }
} /* query_fighting() */


/**
 * This method checks to see if the person doing the drinking can hold onto
 * their bottle without loosing it while in combat.  Warning!  This code
 * may be used in other objects to deal with handling drinking while in
 * combat.
 * @return 1 if the bottle is stopped, 0 if it is not
 * @param player the player doing the drinking
 * @param me the object being drunk */
int is_fighting_bottle_smashed(object player,
                               object me) {
   object* fighting;
   object ob;
   object weapon;
   string skill;
   string my_skill;
   int bonus;
   int stopped;

   stopped = 0;

// See if we are in combat and give a chance to drop the item if we
// are.  Chance is higher if we have no free hands. */
   if (query_fighting(player)) {
   // Ok, we are in combat.  Check out free hands.  The more people
   // we are fighting the harder it is.
      fighting = filter(player->query_attacker_list(),
                        (: environment($1) == $2 :), environment(player));
      if (query_holder()) {
         bonus = 0;
         bonus += -10 * player->query_free_limbs();
      } else {
         bonus = 20;
      }
      if (sizeof(fighting)) {
         bonus += (sizeof(fighting) - 1) * 20;
      }
      if (player->query_free_limbs() < 2 &&
          !query_holder()) {
         bonus += 50 - player->query_free_limbs() * 25;
      }

   // Check against the other persons fighting skill.
      foreach (ob in fighting) {
      // If they are using a weapon use the weapon skill, otherwise unarmed.
         if (sizeof(ob->query_holding() - ({ 0 }))) {
            weapon = (ob->query_holding() - ({ 0 }))[0];
            skill = weapon->query_weapon_type();
            if (skill == "mixed" ||
                (skill != "sharp" && skill != "blunt" && skill != "pierce")) {
               skill = ({ "sharp", "blunt", "pierce" })[random(3)];
            }
            skill = "fighting.combat.melee." + skill;
         } else {
            skill = "fighting.combat.melee.u