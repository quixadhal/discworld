/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ho_ld.c,v 1.12 2002/08/16 00:11:52 presto Exp $
 * $Log: ho_ld.c,v $
 * Revision 1.12  2002/08/16 00:11:52  presto
 * Changed to allow you to switch from one hand to the other, or between
 * one hand and two hands without unholding first
 *
 * Revision 1.11  2002/08/03 19:25:36  ceres
 * New combat system version
 *
 * Revision 1.9  2001/03/13 05:04:22  pinkfish
 * Fix up holding if you are already holding one of the items.
 *
 * Revision 1.8  2001/03/09 01:50:43  pinkfish
 * Fix up some problems in hold.
 *
 * Revision 1.7  2001/03/07 22:02:49  pinkfish
 * *** empty log message ***
 *
 * Revision 1.6  2001/03/07 21:59:30  terano
 *  Forcibly unlocked by pinkfish
 *
 * Revision 1.5  2000/03/10 23:27:18  gruper
 * I *hope* I fixed an index out of bounds bug,
 * but who knows - maybe I broke it even worse :)
 *
 * Revision 1.4  1999/07/07 23:24:52  pinkfish
 * Fix up some problems with hold that caused it not to work with things
 * that fiddled with the number of limbs available.
 *
 * Revision 1.3  1999/06/04 00:59:35  pinkfish
 * Fix up hold to unhold from multiple hands.
 *
 * Revision 1.2  1998/10/08 09:52:49  pinkfish
 * Fixed up to work correctly with things that do not wish to be unheld.
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
/* Command to hold weapons. This _should_ be fairly simple, however it is */
/* complicated by dealing with 1. a variable number of limbs and 2. printing */
/* out all the appropriate holding/unholding/failing/suceeding messages. */

inherit "/cmds/base";

#define TP this_player()

int cmd(object * obs,
        string str)
{
   int num_limbs;               // this players limb count
   int avail;                   // limbs available
   int old_avail;               // old limbs available, used for sanity checking
   int reqd;                    // limbs required
   int i;                       // general counter
   int pos;                     // which limb position
   int success;                 // did we succeed
   int *used;                   // limbs used by set_hold or set_unhold
   int *already_used;           // limbs used by already held weapons
   int *limbs_used;             // limb positions used by already held objects

   object ob;
   object weapon;
   object *using;               // limbs currently in use
   object *fails;               // weapons we failed to hold
   object *unfails;             // weapons we failed to unhold
   object *holds;               // weapons we sucessfully held
   object *unholds;             // weapons we successfully unheld
   object *already;             // weapons already held
   object *no_change;           // objects that we are already holding the way
                                //   we requested

   string *limbs;               // limb names;
   string *hhands;              // hands we successfully held weapons in
   string *uhands;              // hands we successfully unheld weapons from

   // calc how many hands will be needed
   limbs = TP->query_limbs();
   num_limbs = sizeof(limbs);
   avail = TP->query_free_limbs();
   no_change = ({ });

   already = filter(obs, (: $1->query_holder() :));

   if (sizeof(already) > 0) {
      using = this_player()->query_holding();
      foreach (ob in already)  {
         limbs_used = find_member(ob, using);
         if (str)  {
            if (sizeof(limbs_used) == 1  &&
                member_array(str, limbs) == limbs_used[0])
            {
               no_change += ({ ob });
            }
            else already -= ({ ob });
         }
         else if (sizeof(limbs_used) != ob->query_no_limbs())
            already -= ({ ob });
      }
      if (sizeof(obs) == sizeof(no_change))  {
         add_failed_mess("You are already holding " +
                         query_multiple_short(obs) +
                         " in your " + str + ".\n");
         return -1;
      }
      else if (sizeof(obs) == sizeof(already)) {
         return notify_fail("You are already holding " +
                            query_multiple_short(obs) + ".\n");
      } else {
         obs -= already;
         if (sizeof(already) > 0)
            write(sprintf("You are already holding %s, you instead try to "
                          "hold %s.\n", query_multiple_short(already),
                          query_multiple_short(obs)));
         already_used = map(already, (: $1->query_my_limb() :));
      }
   } else {
      already_used = ({ });
   }

   fails = holds = unholds = hhands = uhands = unfails = ({ });

   // deal with specific hands first
   if (str && obs[0]) {
      pos = member_array(str, limbs);
      if (pos == -1) {
         if (!sizeof(limbs)) {
            return
               notify_fail("You seem to have a singular lack of limbs.\n");
         }
         return notify_fail("Incorrect limb type, must be one of " +
                            query_multiple_short(limbs) + ".\n");
      }

      using = TP->query_holding();

      // clear the appropriate hand
      if (sizeof(unfails) == 0  &&  using[pos]) {
         used = TP->set_unhold(using[pos]);
         if (sizeof(used) == 0) {
            /* This means we cannot hold the object for sure. */
            unfails += ({ using[pos] });
            uhands += ({ limbs[pos] });
         } else {
            for (i = 0; i < sizeof(used) && i < sizeof(limbs); i++) {
               uhands += ({ limbs[used[i]] });
            }
            unholds += ({ using[pos] });
         }
      }

      // Put it down if we are already holding it in another hand
      if ((i = member_array(obs[0], using - unholds)) != -1)  {
         used = TP->set_unhold(obs[0]);
         if (sizeof(used) == 0) {
            /* This means we cannot hold the object for sure. */
            unfails += ({ obs[0] });
            uhands += ({ limbs[i] });
         } else {
            for (i = 0; i < sizeof(used) && i < sizeof(limbs); i++) {
               uhands += ({ limbs[used[i]] });
            }
            unholds += ({ obs[0] });
         }
      }

      if (sizeof(unfails) == 0) {
         // hold in the appropriate hand
         used = TP->set_hold(obs[0], pos, 1);
         if (sizeof(used) == 0) {
            fails += ({ obs[0] });
         } else {
            for (i = 0; i < sizeof(used) && i < sizeof(limbs); i++) {
               hhands += ({ limbs[used[i]] });
            }
            holds += ({ obs[0] });
            success = 1;
         }
      }

   } else {                     // now deal with normal holding

      foreach(ob in obs) {
         reqd += ob->query_no_limbs();
      }

      if (reqd > num_limbs) {
         add_failed_mess("You do not have enough limbs to hold $I.\n", obs);
         return 0;
      }

      old_avail = -1;
      // if necessary put down items until the required hands are available
      // No need for a while, if it canont be done.  It cannot be done.
      // If nothign was able to be put down after one sweep through, we are
      // out of luck.
      while ((avail < reqd) && (avail < num_limbs)) {
         if ((reqd > num_limbs - sizeof(unfails)) || (old_avail == avail)) {
            if (sizeof(unfails)) {
               add_failed_mess
                  ("You do not have enough limbs to hold $I, since " +
                   query_multiple_short(unfails) + " failed " "to unhold.\n",
                   obs);
            } else {
               add_failed_mess
                  ("Not able to free up enough limbs to hold $I.\n", obs);
            }
            return 0;
         }
         old_avail = avail;
         using = TP->query_holding();
         for (i = 0; i < sizeof(using); i++) {
            weapon = using[i];
            if (weapon && member_array(i, already_used) == -1) {
               used = TP->set_unhold(weapon);
               if (sizeof(used)) {
                  for (i = 0; i < sizeof(used) && used[i] < sizeof(limbs);
                       i++) {
                     uhands += ({ limbs[used[i]] });
                  }
                  unholds += ({ weapon });
                  avail = TP->query_free_limbs();
                  break;
               } else {
                  if (member_array(weapon, unfails) == -1) {
                     unfails += ({ weapon });
                  }
                  uhands += ({ limbs[i] });
               }
            }
         }
      }

      if (!sizeof(unfails)) {
         // now try holding the new items
         foreach(ob in obs) {
            using = TP->query_holding();
            pos = 0;

            if (!ob->query_no_limbs() || avail < ob->query_no_limbs()) {
               fails += ({ ob });
               break;
            }
            // hold the new items
            while ((using[pos]) && (pos < num_limbs)) {
               pos++;
            }

            used = TP->set_hold(ob, pos, ob->query_no_limbs());
            if (used == ({ })) {
               fails += ({ ob });
            } else {
               for (i = 0; i < sizeof(used) && i < sizeof(limbs); i++) {
                  hhands += ({ limbs[used[i]] });
               }
               holds += ({ ob });
               success = 1;
               avail = TP->query_free_limbs();
            }
         }
      }
   }

   // printout all the different sodding message types
   if (sizeof(unfails)) {
      tell_object(TP,
                  "You fail to put down " + query_multiple_short(unfails) +
                  " from your " + query_multiple_short(uhands) + ".\n");
      say(TP->one_short() + " fails to put down " +
          query_multiple_short(unholds) + " from " + TP->query_possessive() +
          " " + query_multiple_short(uhands) + ".\n");
   }

   if (sizeof(unholds)) {
      tell_object(TP, "You put down " + query_multiple_short(unholds) +
                  " from your " + query_multiple_short(uhands) + ".\n");
      say(TP->one_short() + " puts down " + query_multiple_short(unholds) +
          " from " + TP->query_possessive() + " " +
          query_multiple_short(uhands) + ".\n");
   }

   if (sizeof(holds)) {
      tell_object(TP,
                  "You hold " + query_multiple_short(holds) + " in your " +
                  query_multiple_short(hhands) + ".\n");
      say(TP->one_short() + " holds " + query_multiple_short(holds) + " in " +
          TP->query_possessive() + " " + query_multiple_short(hhands) +
          ".\n");
   }
   // don't print the failed to hold messages if we managed to hold stuff.
   if (!success && sizeof(fails)) {
      tell_object(TP,
                  "You fail to hold " + query_multiple_short(fails) + ".\n");
      say(TP->one_short() + " fails to hold " + query_multiple_short(fails) +
          ".\n");
   }

   return 1;
}                               /* cmd() */

mixed *query_patterns()
{
   return ({ "<indirect:object:me>", (: cmd($1, 0) :),
             "<indirect:object:me> in [my] {" +
             implode(this_player()->query_limbs(), "|") + "}",
             (: cmd($1, implode($4[1..], " ")) :) });
}                               /* query_patterns() */
