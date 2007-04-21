/**
  This effect has a classification
  of "coffee.nose".
   It was written by Gin for Starbanks.
 */
#include <effect.h>

/** @ignore yes */
void beginning( object player, int time ) {
    if (time > 1800)
        time = 1800;

    tell_object( player, "\n" );
    tell_room( environment( player ), (string)player->the_short() + " drinks some of his coffee and ends up with a small amount of froth on "+player->query_possessive()+" nose.\n", player);
    player->submit_ee( 0, time, EE_REMOVE );
    player->add_extra_look( this_object() );

} /* beginning() */

/** @ignore yes */
int merge_effect( object player, int time1, int time2 ) {
    int newtime;
     newtime = (time1 > time2 ) ? time1 : time2; 
     newtime = ( newtime > 1800 ) ? 1800 : newtime; 
    player->submit_ee( 0, newtime, EE_REMOVE );
    return newtime;
} /* merge_effect() */

/** @ignore yes */
void end( object player, int time ) {
    if ( !player->query_property( "dead" ) )
        tell_object( player, "A small amount of frothy Coffee drops onto the floor.\n" );
    player->remove_extra_look( this_object() );
} /* end() */

/** @ignore yes */
string query_classification() { return "coffee.nose"; }
/** @ignore yes */
string extra_look( object player ) {
    return capitalize( player->query_pronoun()+" looks "
      "like "+player->query_pronoun()+" has a small amount of creamy, frothy coffee on "+player->query_possessive()+" nose.\n" );
}/* end query_extra_look() */
