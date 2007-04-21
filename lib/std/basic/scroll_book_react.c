/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: scroll_book_react.c,v 1.3 2002/08/08 02:08:52 ceres Exp $
 */
/**
 * This is the reaction handling code for both spell books and scrolls.
 */
#include <tasks.h>

#define SKILL "magic.items.scroll"
#define CONTROL_SKILL "magic.spells.special"

/**
 * This method is called to destroy the scroll.  This must be overridden
 * at a higher level.
 */
void destroy_this_scroll() {
} /* destry_this_scroll() */

/**
 * This method checks to see if the spell reacts with anything.
 * @param paper who to check (us hopefull)
 */
void check_react(object player) {
   object ob;
   int level;
   int ac;
   string check_spell;
   object paper;
   object *items;
   object env;

   if (!environment(player)) {
      return;
   }

   // First make sure we are in a player or a pet/fruitbat etc.
   env = environment(player);
   while (env && !living(env)) {
      env = environment(env);
   }

   // Check to see if they are owned by someone.
   if (env && !userp(env) && !env->query_owner()) {
      return ;
   }

   items = filter(deep_inventory(environment(player)),
                  (: $1->query_magic_scroll() ||
                     $1->query_spell_book() :));

   foreach (paper in items) {
      level += paper->query_spell_power_level();
   }

   // 
   // No need to do this since we should be included in the above list.
   //
   //level += _spell->query_power_level();

   if (random(sqrt(level)) <= 5 + random(5)) {
      return;
   }
  
   // find out who is carrying this object
   ob = environment(player);
   while(ob) {
      if(living(ob)) {
         break;
      } else {
         ob = environment(ob);
      }
   }

   if(!ob || !living(ob)) {
      return;
   }

   if(ob) {
      switch(TASKER->perform_task(ob, CONTROL_SKILL, level*2 + random(4),
                                  TM_FREE)) {
      case AWARD:
         tell_object(ob, "Suddenly the spell "+
                      this_object()->query_spell_name()+ " on the "+
                     player->the_short()+" gets out of control and tries to go "
                     "wild.\n");
         tell_room(environment(ob), "Suddenly " + ob->one_short() +
                   " looks very startled as the spell " +
                   this_object()->query_spell_name() +
                   " on "+player->the_short()+" gets out of control and "
                   "tries to go wild.\n", ob);
         
         write("%^YELLOW%^You feel you have a better grasp of controlling "
               "spells on scrolls.\n.%^RESET%^\n" );
      case SUCCEED:
      
         return;
      default:
         tell_object(ob, "Suddenly the spell " +
                     this_object()->query_spell_name() + " on the "+
                     player->the_short() +
                     " gets out of control and tries to go wild.\n");
         tell_room(environment(ob), "Suddenly " + ob->one_short() +
                   " looks very startled as the spell " + 
                   this_object()->query_spell_name() +
                   " on " + player->the_short() + " gets out of control and "
                   "tries to go wild.\n", ob);
       
         tell_object(ob, "Unfortunately you cannot get control of it and "
                     "before flying away into nowhere it tears wildly at "
                     "your mind.\n");
         tell_room(environment(ob), "Unfortunately "+ob->one_short()+" cannot "
                   "get control of it and it flies away to freedom.\n", ob);
   
   
         ac = (int)ob->query_ac("magic", level * 10);
         level -= ac;
   
         ob->adjust_hp(-level, this_object());
         ob->adjust_tmp_int(-2 + random(2));
   
         destroy_this_scroll();
         break;
      }
   }
} /* check_react() */

/** @ignore yes */
string query_death_reason() {
   return " a spell going wild and getting out of control.";
} /* query_death_reason() */
