/*  -*- LPC -*-  
 * $Locker:  $
 * $Id: death_shadow.c,v 1.2 1998/03/30 01:14:23 olorin Exp $
 * $Log: death_shadow.c,v $
 * Revision 1.2  1998/03/30 01:14:23  olorin
 * added functions so remember if the death cost a life, and if resurrect
 * should restore it.
 *
 * Added autodoc stuff as well.
 *
 * Revision 1.1  1998/01/06 04:39:04  ceres
 * Initial revision
 */
object my_player;
int cost_life, resurrect_adds_life;

/** @ignore yes */
int test_add(object ob) {
   if (ob->query_property("dead usable"))
      return 1;
   return 0;
} /* test_add() */

/** 
 * This function remembers if the death cost a life
 * @param i 1 if it cost a life, 0 if it didn't
 */
void set_death_cost_life( int i ) { cost_life = i; }

/** 
 * This function remembers if the death cost a life
 * @return 1 if it cost a life, 0 if it didn't
 */
int query_death_cost_life() { return cost_life; }

/** 
 * This function remembers if resurrect should restore a life
 * @param i 1 if resurrect should restore a life, 0 if it shouldn't
 */
void set_resurrect_adds_life( int i ) { resurrect_adds_life = i; }

/** 
 * This function remembers if resurrect should restore a life
 * @return 1 if resurrect should restore a life, 0 if it shouldn't
 */
int query_resurrect_adds_life() { return resurrect_adds_life; }

/** @ignore yes */
void attack() {
} /* attack() */

/** @ignore yes */
void adjust_hp() {
} /* adjust_hp() */

/** @ignore yes */
void set_hp() {
} /* set_hp() */
 
/** @ignore yes */
void do_death() {
/* He's already dead...  sheeze.  */
} /* do_death() */
 
/** 
 * This function sets this object up as a clone of ob, and if done
 * while the player is reloading, it'll also restore the values returned 
 * by query_resurrect_adds_life() and query_death_cost_life() 
 * @param ob the object to shadow
 */
void setup_shadow( object ob ) {
   shadow( ob, 1 );
   my_player = ob;
   
   /* restore status */
   cost_life = my_player->query_property( "death cost life" );
   my_player->remove_property( "death cost life" );
   resurrect_adds_life = my_player->query_property( "resurrect should add life" );
   my_player->remove_property( "resurrect should add life" );
} /* setup_shadow() */

/** @ignore yes */
int attack_by( object ob ) {
   tell_object( ob, "This person is a ghost... You cannot attack them.\n" );
   ob->stop_fight( my_player );
   return 1;
} /* attack_by() */

/** @ignore yes */
int attack_ob( object ob ) {
   write( "Your hand passes right through it!  You are just a ghost!\n" );
   return 0;
} /* attack_ob() */

/** @ignore yes */
string short() {
   if ( !my_player )
      return "Death shadow";
   return "the ghost of "+my_player->short();
} /* short() */

/** @ignore yes */
void dest_death_shadow() {
   destruct( this_object() );
} /* dest_death_shadow() */

/** @ignore yes */
int cast() {
   write( "Casting spells when you are dead?\n" );
   return 1;
} /* cast() */

/** @ignore yes */
int do_shout() {
   write( "You are dead, that is not possible.\n" );
   return 1;
} /* do_shout() */

/** @ignore yes */
int do_echo() {
   write( "You are dead...\n" );
   return 1;
} /* do_echo() */

/** @ignore yes */
int do_guild_command() {
   write( "You are dead!\n" );
   return 1;
} /* do_guild_command() */

/** @ignore yes */
int do_race_command() {
   write( "You are dead!\n" );
   return 1;
} /* do_race_command() */

/** @ignore yes */
int score() {
   write( "You are just a disembodied spirit.  What use has a wispy thing "
         "like you have for a collection of meaningless numbers?\n" );
   return 1;
} /* score() */
 
/** @ignore yes */
void second_life() {
} /* second_life() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   if ( my_player ) {
      /* save status before save */
      my_player->add_property( "death cost life", cost_life );
      my_player->add_property( "resurrect should add life", 
                              resurrect_adds_life );
      return my_player->query_dynamic_auto_load();
   } else
      return ([ ]);
} /* query_dynamic_auto_load() */

