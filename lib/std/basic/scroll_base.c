/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: scroll_base.c,v 1.4 2000/06/28 05:09:39 pinkfish Exp $
 * $Log: scroll_base.c,v $
 * Revision 1.4  2000/06/28 05:09:39  pinkfish
 * Fix up the scroll base to work with the new spells.
 *
 * Revision 1.3  1999/12/01 06:19:53  pinkfish
 * Fix up some bugs.
 *
 * Revision 1.2  1999/11/08 22:39:49  pinkfish
 * Move the reaction code into a seperate file.
 *
 * Revision 1.1  1999/11/04 01:51:34  pinkfish
 * Initial revision
 *
 * 
 */
/**
 * This is the basic control file for scrolls.  It contains all the common
 * scroll handling stuff that goes into both the object and the shadow.
 */
#include <tasks.h>

inherit "/std/basic/scroll_book_react";

#define SKILL "magic.items.scroll"
#define CONTROL_SKILL "magic.spells.special"

private string _spell;

/** @ignore yes */
string long( string word, int dark ) {
   int i;
   int j;
   int chance;
   int level;
   int width;
   string spell_name;

   level = 2 * (int)_spell->query_power_level();
   width = sqrt( level );
   chance = (int)this_player()->query_skill_bonus( SKILL ) - ( level - width );
   chance = ( 100 * chance ) / ( 2 * width + 1 );
   spell_name = "$"+ (string)_spell->query_name() +"$";
   j = strlen( spell_name ) - 1;
   for ( i = 1; i < j; i++ ) {
      if ( random( 100 ) > chance ) {
         switch ( spell_name[ i ] ) {
            case 32 :
               break;
            case 65 .. 90 :
               spell_name = spell_name[ 0 .. i - 1 ] +
                     sprintf( "%c", 65 + random( 26 ) ) +
                     spell_name[ i + 1 .. j ];
               break;
            case 97 .. 122 :
               spell_name = spell_name[ 0 .. i - 1 ] +
                     sprintf( "%c", 97 + random( 26 ) ) +
                     spell_name[ i + 1 .. j ];
               break;
            default :
               spell_name = spell_name[ 0 .. i - 1 ] +
                     spell_name[ i + 1 .. j ];
               j--;
         }
      }
   }
   spell_name = spell_name[ 1 .. j - 1 ];
   return spell_name;
} /* long() */

/**
 * This method returns always returns 1 for a magic scroll.
 * @return always returns 1
 */
int query_magic_scroll() { return 1; }

/**
 * This method returns the current binding force of the scroll.  This is
 * the amount of force the paper exerts to try and keep the spell on the
 * paper.
 * @return the scrolls binding force
 */
int query_binding_force() { return 5; }

/**
 * This method returns the spell associated with the scroll.
 * @return the spell associated with the scroll
 */
string query_spell() { return _spell; }

/**
 * This method returns the power level of the spell current associated
 * with the object.
 */
int query_spell_power_level() {
   if (_spell) {
      return _spell->query_power_level();
   }
} /* query_spell_power_level() */

/**
 * This method sets the spell associated with the scroll.
 * @param word the spells path
 */
void set_spell( string word ) { _spell = word; }

/**
 * This method returns the name o fthe spell on the scroll.
 * @return the name of the spell
 */
string query_spell_name() { return (string)_spell->query_name(); }

/**
 * This method tests the skill needed to cast the spell on the scroll.
 * @return the skill needed to cast
 */
int test_skill() {
   int level;

   level = 2 * (int)( _spell->query_power_level() ) + 1;
   return (int)TASKER->perform_task(this_player(), SKILL, level, TM_FREE);
} /* test_spell() */

/**
 * This method clears the spell off the scroll and turns it back into
 * a normal piece of paper.
 * @param award a tm was awarded on the way
 */
void clear_spell( int award ) {
   if ( award ) {
     write( "%^YELLOW%^"+ ({ "A piece of lore concerning scrolls becomes "
             "clearer for you", "You manage to grasp one of the principles "
             "of magic scrolls more firmly", "A sudden flash of insight "
             "allows you to understand process of casting a spell from a "
             "scroll a little better" })[ random( 3 ) ] +".%^RESET%^\n" );
   }
} /* clear_spell() */

/**
 * This method clears the spell off the scroll and turns it back into
 * a normal piece of paper.
 * @param award a tm was awarded on the way
 */
int cast_spell( string words ) {
   int outcome;

   if ( !words ) {
      words = "";
   }
   outcome = test_skill();
   if ( outcome == FAIL ) {
      return notify_fail( "You can't figure out how to use the scroll.\n" );
   }
   if ( !( _spell->cast_spell( words, 1 ) ) ) {
      return 0;
   }
   clear_spell( outcome == AWARD );
   return 1;
} /* cast_spell() */

/**
 * This method clears the spell off the scroll and turns it back into
 * a normal piece of paper.
 * @param award a tm was awarded on the way
 */
int cast_spell_new( string words , object* targets, object* using ) {
   int outcome;

   if ( !words ) {
      words = "";
   }
   outcome = test_skill();
   if ( outcome == FAIL ) {
      return notify_fail( "You can't figure out how to use the scroll.\n" );
   }
   if ( !( _spell->cast_spell_new( words, 1, targets, using ) ) ) {
      return 0;
   }
   clear_spell( outcome == AWARD );
   return 1;
} /* cast_spell() */

/** @ignore yes */
string query_death_reason() {
   return " a spell going wild and getting out of control.";
} /* query_death_reason() */
