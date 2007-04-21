/**
 * New holding code!
 * Because the old stuff wasn't OO enough for me.
 * @author Pinkfish
 * @changes See the RCS log, but this change is made by Terano
 *
 * This has the controlling code for holding/wielding objects
 */

/**
 * This stuff is setup by other parts of /std/living, and can't be
 * changed.
 */
nosave mixed *_using;
nosave mixed *_weapons;

//This is somewhat strangely named, it is actually an int that is 1 on
//success and 0 on fail.

#define START_VALUE "1"

/**
 * We have to keep the interfaces the same so has to not break things
 */
int *set_hold(object ob, int location, int limbs);
int *set_unhold(object ob);
private int sub_hold(object ob, int pos, int first);

/**
 * @ignore
 */
void create()
{
   _weapons = ({ });
}

/**
 * This method returns the an array of the names of the limbs 
 * on the living object.
 * @return an array of limbs
 */
string *query_limbs()
{
   //Be type safe!
   string race_ob;
   string *limbs;

   race_ob = this_object()->query_race_ob();

   if (!race_ob) {
      _using = ({ });
      return ({ });
   }
   limbs = race_ob->query_limbs();

   if (!_using) {
      _using = allocate(sizeof(limbs));
   }
   return limbs;
}

string* query_using_array() {
   return copy(_using);
} /* query_using_array() */


/**
 * This method returns the current holding array of the living object.
 * This has one element in the array for each object and a 0 is in the
 * array if that limb is not holding anything.  The positions correspond
 * to the positions returned by the query_limbs() function.
 * @return an array of held objects
 * @see query_holding()
 * @see query_free_limbs()
 * @see query_weapons()
 */
object *query_holding()
{
   if (!arrayp(_using)) {
      //Generate a using array
      query_limbs();
   }
   _using =
      map(_using, (: $1 ? (environment($1) == this_object()? $1 : 0) : 0 :));
   return copy(_using);
}                               /* query_holding() */

/**
 * This method returns the list of limbs the object is being held in.
 * @param ob the object to check the limbs for
 * @return the limbs it is held in
 */
string* query_holding_limbs(object ob) {
   string* ret;
   int i;
   object* holding;
   string* limbs;

   ret = ({ });
   holding = query_holding();
   limbs = query_limbs();
   for (i = 0; i < sizeof(holding); i++) {
      if (holding[i] == ob) {
         ret += ({ limbs[i] });
      }
   }
   return ret;
}

/**
 * This method returns the number of free limbs on the living object.
 * A free limb!  Yes, have a couple of free legs, beat the rush!
 * Buy now!
 * @return the number of free limbs
 * @see query_holding()
 * @see query_limbs()
 */
int query_free_limbs()
{
   if (!arrayp(_using)) {
      query_limbs();
   }
   return
      sizeof(filter(_using, (: !$1 || environment($1) != this_object() :)));
}


/**
 * This method returns the currently held weapons on the living object.
 * This is an array of held items which are weapons and can be used
 * in combat.
 * @return the array of held weapons
 * @see query_holding()
 */
object *query_weapons()
{
// I don't quite understand the bit after the ||
// Also, it runtimes if $1 is 0  --Presto
//   return filter(_weapons, (: $1 || environment($1) != this_object() :));
   return filter(_weapons, (: $1 :));
}

/**
 * This method sets the object as unheld.  It will attempt to remove
 * the object from a held limb.
 * <p>
 * The return array contains the index of the limbs from which the
 * item was removed, if the array is 0 size then no items were
 * removed.
 * @param ob the object to unhold
 * @return a array of limb numbers
 * @see set_hold()
 * @see query_limbs()
 */
int *set_unhold(object ob)
{
   int *pos;

   if (!objectp(ob)) {
      return ({ });
   }
   if (member_array(ob, _using) == -1) {
      return ({ });
   }

   if (!ob->set_holder(0)) {
      return ({ });
   }

   pos = find_member(ob, _using);

   if (ob->query_weapon()) {
      _weapons = filter(_weapons, (: $1 != $(ob) :));
   }

   if (ob->query_armour()) {
      this_object()->remove_armour(ob);
   }

   _using = map(_using, (: $1 == $(ob) ? 0 : $1 :));

   this_object()->do_burden_call();
   return pos;
}

/**
 * This method sets the object as held.  It will attempt to hold it starting
 * at the given position in the limbs array.
 * <p>
 * The return array contains the index of the limbs from which the
 * item was added, if the array is 0 size then no items were
 * added.
 * @param ob the object to hold
 * @param pos the position in the limb array to start holding at
 * @param limbs the number of limbs to be used (1 or 2 usually). leave as
 * zero for default.
 * @return a array of limb numbers
 * @see set_unhold()
 * @see query_limbs()
 *
 * Editors Note: I have changed set_hold so that it no longer 
 * automatically unholds objects. I am moving that functionality to the
 * hold command. If you want your objects to be in a specific limb,
 * you have to free it up yourself using set_unhold.
 * If you don't care which limbs, you just have to free up enough limbs
 * to hold it.
 */
int *set_hold(object ob, int pos, int limbs)
{

   int limb_count;
   int *free = ({ });
   int *used = ({ });
   int total = 0;
   int temp = 0;
   int ok;
   int failed;

   if (environment(ob) != this_object()) {
      debug_printf("Bad environment");
      return ({ });
   }

   limb_count = sizeof(this_player()->query_limbs());
   if (!arrayp(_using)) {
      query_limbs();
   }

   /** 
    * Find out if we have enough free arms.. if not scream and die.
    *  I want to do it this way, because the hold command should
    * be responsable for freeing limbs.. not us.
    */

   //We are trying any and all limbs.
   if (pos == -1) {
      total = ob->query_no_limbs();
      if (total > this_object()->query_free_limbs()) {
         debug_printf("Too many limbs");
         return ({ });
      }
      //Find out which limbs are free
      free = find_member(0, _using);

      if (!sizeof (free)) {
        return ({ });
      }
      // Start with the first one and do the normal hold attempts.      
      pos = free[0];
   }
   //It wants a specific position - is it free?
   if (objectp(_using[pos]) && environment(_using[pos]) == this_object()) {
      //Nope! Remember, only you can make sure your limbs are free before
      //using them!
      return ({ });
   }
   //Set the first one that they asked for, guess the rest ourselves.
   ok = sub_hold(ob, pos, 1);

   if (ok) {
      used += ({ pos });
   } else {
      return ({ });
   }

   //Initialise our 'guessing' variables.
   //Get all free limbs and note the used limb.
   if(limbs)
     total = limbs - 1;
   else
     total = ob->query_no_limbs() - 1;

   free = find_member(0, _using);
   temp = 0;

   while (temp < total && temp < sizeof(free)) {
      //Oops, we've run out of limbs!
      if (temp > limb_count) {
         set_unhold(ob);
         return ({ });
      }

      sub_hold(ob, free[temp], 0);
      if (ok) {
         used += ({ free[temp] });
         temp++;
      } else {
         failed = 1;
      }
   }

   if (failed) {
      set_unhold(ob);
      return ({ });
   } else {
     this_object()->do_burden_call();
      return used;
   }
}

/**
 * Does a lot of the grunt work in holding, does all the validation
 * for each limb for starters. More stuff might come later!
 * Tannah - you can mask this if you want.
 * Terano.
 */
private int sub_hold(object ob,
                     int pos,
                     int first)
{
   int success;

   // Must be in our inventory to hold it.
   if (environment(ob) != this_object()) {
      return 0;
   }
   //If failed_mess = 1 [note that this means that we haven't failed yet]
   //and if we haven't held it yet. [If failed mess is one and there is no 
   //holder, we haven't tried yet.]
   if (first) {
      success = ob->set_holder(this_object(), pos);
      if (!success) {
         return 0;
      }
   }
   _using[pos] = ob;

   if (ob->query_weapon()) {
      if (member_array(ob, _weapons) == -1) {
         _weapons += ({ ob });
      }
   }
   return 1;
}
