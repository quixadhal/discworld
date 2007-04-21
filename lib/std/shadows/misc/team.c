/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: team.c,v 1.4 1999/10/28 02:09:56 ceres Exp $
 * 
*/
#define TEAM "/obj/handlers/team"

object player;

void setup_shadow( object thing ) {
   shadow( thing, 1 );
   player = thing;
} /* setup_shadow() */

void destruct_team_shadow() { destruct( this_object() ); }

string extra_score() {
   string team;
   team = (string)TEAM->query_group( player );
   if ( !team ) {
      call_out( "destruct_team_shadow", 1 );
      return (string)player->extra_score();
   }
   return (string)player->extra_score() +
         "You are a team member of "+ team +".\n";
} /* extra_score() */

varargs int adjust_xp( int number, int shared ) {
   int plvl, olvl, divisor;
   string team;
   object other, *others;

   if ( ( number < 0 ) || !shared )
      return (int)player->adjust_xp( number );
   team = (string)TEAM->query_group( player );
   if ( !team ) {
      call_out( "destruct_team_shadow", 1 );
      return (int)player->adjust_xp( number );
   }
   others = (object *)TEAM->query_members( team );
   if ( !others ) {
      call_out( "destruct_team_shadow", 1 );
      return (int)player->adjust_xp( number );
   }
   if ( sizeof( others ) == 1 )
      return (int)player->adjust_xp( number );

   // the more people in the team the smaller the shared Xp.
   divisor = (sizeof(others)-1/ 2);
   if(divisor)
     number /= divisor;
   
   plvl = (int)player->query_level() / 2;
   foreach ( other in others - ({ player }) )
      if ( objectp( other ) ) {
         if ( environment( other ) != environment( player ) )
            continue;
         olvl = (int)other->query_level();
         if ( ( olvl > plvl ) || !plvl )
            other->adjust_xp( number, 0 );
         else
            other->adjust_xp( ( number * ( 1 +
                  ( 99 * olvl ) / plvl ) ) / 100, 0 );
      }
   return (int)player->adjust_xp(number * divisor);
} /* adjust_xp() */

varargs int add_skill_level( string skill, int lvl, mixed exp ) {
  int diff_0, diff_1, diff, width_0, width;
  string team;
  object other, *others;

  // no team advancement unless it's a TM
  if ( ( lvl != 1 ) || intp(exp) ||
       (objectp(exp) && base_name(exp) == base_name(this_object())))
    return (int)player->add_skill_level( skill, lvl, exp );

  if(!exp)
    exp = previous_object();
  
  // Do some checks to make sure they're still in the team.
  team = (string)TEAM->query_group( player );
  if ( !team ) {
    call_out( "destruct_team_shadow", 1 );
    return (int)player->add_skill_level( skill, lvl, exp );
  }
  others = (object *)TEAM->query_members( team );
  if ( !others ) {
    call_out( "destruct_team_shadow", 1 );
    return (int)player->add_skill_level( skill, lvl, exp );
  }
  
  if(skill == "other.perception" || skill[<7..] == ".points")
    return (int)player->add_skill_level( skill, lvl, exp );
   
  if ( sizeof( others ) == 1 )
    return (int)player->add_skill_level( skill, lvl, exp );

  // If they didn't actually get an advance due to hitting a limit or
  // whatever then don't give their team an advance either.
  if(!player->add_skill_level(skill, lvl, exp))
     return 0;
   
  diff_0 = (int)player->query_skill_bonus(skill);
  diff_1 = (int)player->query_skill(skill);
  width_0 = (int)player->stat_modify( 25 * 25, skill );
  foreach ( other in others - ({ player }) ) {
    if (!objectp( other ) )
      continue;
    
    if ( environment( other ) != environment( player ) )
      continue;

    if(diff_0 - (int)other->query_skill_bonus(skill) >
       diff_1 - (int)other->query_skill(skill))
      diff = diff_0 - (int)other->query_skill_bonus( skill );
    else
      diff = diff_1 - (int)other->query_skill(skill);
    width = (int)other->stat_modify( width_0, skill );

    if (random( width + diff * diff + sqrt(other->query_skill(skill)))
        < (width/2) &&
        other->add_skill_level( skill, lvl, this_object())) {
      tell_object( other, "%^YELLOW%^By watching "+
                   (string)player->the_short() +
                   ", you feel you've learnt something.%^RESET%^\n" );
    }
  }
  return 1;
} /* add_skill_level() */
