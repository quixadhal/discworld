/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: leatherwork.c,v 1.16 2002/12/02 23:26:43 ceres Exp $
 * $Log: leatherwork.c,v $
 * Revision 1.16  2002/12/02 23:26:43  ceres
 * Modified for new skills tree
 *
 * Revision 1.15  2002/06/05 14:36:52  ohdamn
 * Fixed typo
 *
 * Revision 1.14  1999/10/24 12:01:58  taffyd
 * Fixes!
 *
 * Revision 1.13  1999/10/03 17:31:25  taffyd
 * Fixed typo.
 *
 * Revision 1.12  1999/04/05 18:57:06  ceres
 * Modified to use perform_task
 *
 * Revision 1.11  1998/09/22 06:05:00  ceres
 * Missed one.
 *
 * Revision 1.10  1998/09/22 05:53:08  ceres
 * Fixed bug with TM reporting
 *
 * Revision 1.9  1998/08/22 01:42:56  pinkfish
 * Change to the second bersion fo the command base.
 *
 * Revision 1.8  1998/08/19 10:51:00  pinkfish
 * Fix up to work withj the new base object.
 *
 * Revision 1.7  1998/03/14 18:26:49  pinkfish
 * Chnaged to a help file.
 *
 * Revision 1.6  1998/02/06 05:27:05  macchirton
 * board commonroom
 *
 * Revision 1.3  1998/02/02 01:08:19  macchirton
 * Fixed the teaching
 *
 * Revision 1.2  1998/01/24 17:53:16  macchirton
 * Fixed minimum cost problem
 *
 * Revision 1.1  1998/01/06 05:26:23  ceres
 * Initial revision
 * 
*/
#include <money.h>
#include <tasks.h>

#define TEACH_LEVEL 10
#define LEARN_LEVEL 10
#define SKILL "crafts.materials.leatherwork"
#define SKILLS ({ "crafts.materials.leatherwork", \
                  "crafts.materials.needlework" })
/*weaving is closest skill to sewing.*/

#define A_MAX 5
#define C_MAX 100
#define A_COND 5
#define C_COND 25
#define SCALE 50

inherit "/cmds/guild_base";
 
void create() {
   ::create();
   set_nroff_file("leatherwork");
   add_teach_skill(SKILL, 20, 10);
   set_command_name("leatherwork");
} /* create() */ 

int cmd( object *things, int costing ) {
   int bonus, cond, low, max, diff, cost, per, val, award;
   string place;

   if ( !environment( this_player() )->query_property( "leatherwork" )&&
      member_array(1,  (all_inventory(environment(this_player()))->
      query_property("leatherwork"))) == -1 )
      return notify_fail( "You are not in a leatherworking shop, "
            "so you cannot repair anything.\n" );
   place = (string)environment( this_player() )->query_property( "place" );
   if ( !place || ( place == "" ) ){
 /* THIS IS WHERE ANY PLACE CHECKING SHOULD GO */
      place = "default";
   }
   if ( sizeof( things ) > 1 )
      return notify_fail( "You can only repair one thing at a time.\n" );
/*
   if (member_array("leather", things[0]->query_material()) != -1) 
*/
   if (things[0]->query_material() != "leather") 
      return notify_fail( (string)things[ 0 ]->the_short() +
            " is not made of leather!  Wrong shop!\n" );
   if ( things[ 0 ]->query_worn_by() )
      return notify_fail( "You should probably remove "+
            (string)things[ 0 ]->the_short() +
            " before you jab yourself with a needle "
            "while trying to patch it up.\n" );
   per = 100 -
    (int)environment( this_player() )->query_discount( this_player() );

   if ( per < 0 )
      per = 0;
   val = (int)this_player()->query_value_in( place );
   if ( place != "default" )
      val += (int)this_player()->query_value_in( "default" );
   low = (int)things[ 0 ]->query_lowest_cond();
   if ( !low )
      low = 1;
   cond = (int)things[ 0 ]->query_cond();
   max = (int)things[ 0 ]->query_max_cond();
   if ( !max )
      max = 1;
   if ( 100 * cond > 98 * max )
      return notify_fail( (string)things[ 0 ]->the_short() +
            " is already in top condition.\n" );
   diff = max - cond;

/* ********** Type 1. not very damaged. simple sewing wil do. */
   if ((100 * diff)/max > 70){
     bonus = (int)this_player()->query_skill_bonus( SKILLS[ 1 ] );
     cost = (( diff * sqrt( (int)things[ 0 ]->query_full_value() ) ) / 
       (max * 50) + 13);
    if ( ( diff > bonus ) && !costing ) {
       diff = ( 150 * diff ) / max;
       switch ( (int)TASKER->perform_task(this_player(), SKILLS[1],
                                          diff, TM_COMMAND ) ) {
         case AWARD :
            award = 1;
         case SUCCEED :
            diff = max - cond;
            break;
         default :
            diff = 0;
      }
    }
   if ( ( val < cost ) && !costing )
      return notify_fail( "You cannot afford the thread to repair "+
            (string)things[ 0 ]->the_short() +".\n" );
   if ( costing ) {
      write( "To attempt to repair "+ (string)things[ 0 ]->the_short() +
            ", it would probably cost you about "+
            (string)MONEY_HAND->money_value_string( cost, place ) +".\n" );
      return 1;
   }
   things[ 0 ]->adjust_cond( diff );
   this_player()->pay_money( (mixed *)MONEY_HAND->create_money_array( cost,
         place ), place );
   if ( award) {
      write( "%^YELLOW%^"+ replace( ({ "As you begin to fix $I, you realise "
            "how to make better use of the materials.", "As you work on $I, "
            "you find that you're able to fix it completely.", "You discover "
            "that you can fix $I more effectively." })[ random( 3 ) ], "$I",
            (string)things[ 0 ]->the_short() ) +"%^RESET%^\n" );
      
   }
   if ( 100 * (int)things[ 0 ]->query_cond() > 98 * max )
      write( "You sew up all the holes in "+ (string)things[ 0 ]->the_short() +
            ", bringing it to top condition.\n" );
   else
      write( "You manage to sew up some of the holes in "+ (string)things[ 0 ]->the_short() +
            " but a few pop back open due to poor stitching.\n" );
   write( "The thread and sinew costs you "+
         (string)MONEY_HAND->money_value_string( cost, place ) +".\n" );
   say( (string)this_player()->one_short() +" fixes up "+
         (string)things[ 0 ]->a_short() +".\n" );
   return 1;

  }else{
/* ************************************************ */
  bonus = (int)this_player()->query_skill_bonus( SKILLS[ 0 ] );
  cost = ( diff * sqrt( (int)things[ 0 ]->query_full_value() ) ) / 
      (max * 3) +123;

   /* if it's too damaged, a patch is needed*/
   if ( !cond ) {
      write( "You begin to work on "+ (string)things[ 0 ]->the_short() +
            " when it falls apart!  It must have been too damaged to fix.\n" );
      say( (string)this_player()->one_short() +" begins to work on "+
            (string)things[ 0 ]->the_short() +" when it falls to scraps!\n" );
      things[ 0 ]->break_me();
      return 1;
   }
    if ( ( diff > bonus ) && !costing ) {
       diff = ( 150 * diff ) / max;
       switch ( (int)TASKER->perform_task(this_player(), SKILLS[0],
                                          diff, TM_COMMAND) ) {
         case AWARD :
            award = 1;
         case SUCCEED :
            diff = max - cond;
            break;
         default :
            diff = 0;
      }
    }
   if ( ( val < cost ) && !costing )
      return notify_fail( "You cannot afford the leather to repair "+
            (string)things[ 0 ]->the_short() +".\n" );
   if ( costing ) {
      write( "To attempt to repair "+ (string)things[ 0 ]->the_short() +
            ", it would probably cost you about "+
            (string)MONEY_HAND->money_value_string( cost, place ) +".\n" );
      return 1;
   }
   things[ 0 ]->adjust_cond( diff );
   this_player()->pay_money( (mixed *)MONEY_HAND->create_money_array( cost,
         place ), place );
   if ( award) {
     write("%^YELLOW%^"+ replace( ({ "As you begin to cut and sew the "
           "leather patches for $I, you realise how to make better use of "
           "the leather.", "As you work on $I, you find that you're able "
           "to repair it completely.", "You discover that you can repair "
           "$I more effectively." })[ random( 3 ) ], "$I",
            (string)things[ 0 ]->the_short() ) +"%^RESET%^\n" );
   }
   if ( 100 * (int)things[ 0 ]->query_cond() > 98 * max )
      write( "Your patching of "+ (string)things[ 0 ]->the_short() +
            " goes quite well, and you bring it to top condition.\n" );
   else
      write( "You manage to create reasonably good patches for "+
             (string)things[ 0 ]->the_short() +
             ", but the size wasn't quite right, and the stitching doesn't "
             "look like it will hold very well.\n" );
   write( "The repair materials cost you "+
          (string)MONEY_HAND->money_value_string( cost, place ) +".\n" );
   say( (string)this_player()->one_short() +" fixes up "+
        (string)things[ 0 ]->a_short() +".\n" );
   return 1;
 }/*endtype of repairs if */
} /* cmd() */

int teach( object thing ) {
   if ( (int)this_player()->query_skill( SKILL ) < TEACH_LEVEL )
      return -1;
   if ( (int)thing->query_skill( SKILL ) < LEARN_LEVEL )
      return -2;
   thing->add_known_command( "leatherwork" ); 
   return 1;
} /* teach() */

mixed *query_patterns() {
  return ({ "<indirect:object:me>", (: cmd($1, 0) :),
            "cost <indirect:object:me>", (: cmd($1, 1) :) });
}
