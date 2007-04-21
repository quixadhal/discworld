#include <player.h>

int protecting;
string protector;
object player;
object *allowed = ({ });

void setup_shadow(object thing, object *victors)
{
  string *bits;

  protector = 0;
  if (environment(thing)) {
    bits = explode(file_name(environment(thing)), "/") - ({ "", "." });
    if (sizeof(bits) > 1 && bits[0] == "d") {
      object mast;
      mast = load_object(implode(bits[0..1] + ({ "master" }), "/"));
      if (mast)
        protector = mast->query_protector_string();
    }
  }
  if (!protector) protector = "The Rules of Fair Play";
  shadow( thing, 1 );
  player = thing;
  protecting = 1;
  call_out( "dest_surrender_shadow", 60 * 5 );
  allowed = victors;
}

int can_be_nasty()
{
  object who;
  object *inv;

  if (!protecting)
    return 1;
  if (!environment(player))
    return 1;
  inv = all_inventory(environment(player));
  foreach (who in allowed)
    if (member_array(who, inv) != -1)
      return 0;
  return 1;
}

string query_surrender_protector() { return protector; }

void zap_surrender_shadow() {
   protecting = 0;
   tell_object( player, "You lose "+ protector +"'s protection.\n" );
   call_out( "dest_surrender_shadow", 1 );
} /* zap_surrender_shadow() */

int adjust_hp( int number, object thing, object weapon, string attack ) {
   if ( ( number > 0 ) || !protecting || 
       !thing || !pk_check(thing, player ) || player == this_player() )
      return (int)player->adjust_hp( number, thing, weapon, attack );
   return (int)player->query_hp();
} /* adjust_hp() */

int set_hp( int number, object thing ) {                     
   int current;
   current = (int)player->query_hp();
   if (number > current || !protecting || !thing)
      return (int)player->set_hp( number, thing );
   return current;
} /* set_hp() */
 
/*
 * Hopefully there's no way a PKer can call these directly...
 *
 * void do_death() { return; }
 * int second_life() { return 1; }
*/

int cannot_walk()
{
  if (!can_be_nasty()) {
    notify_fail("You can't leave while your subduer is here.\n");
    return 1;
  }
  zap_surrender_shadow();
  return player->cannot_walk();
}

int attack_by( object thing ) {
   if (!protecting)
      return (int)player->attack_by( thing );
   tell_object( thing, (string)player->the_short() +
         " is protected from combat by "+ protector +".\n" );
   tell_object( player, "You are protected from combat by "+
         protector +".\n" );
   thing->stop_fight( player );
   return 0;
} /* attack_by() */

int attack_ob( object thing ) {
  if (!can_be_nasty())
    return 0;
  zap_surrender_shadow();
  return (int)player->attack_ob( thing );
} /* attack_ob() */

int query_unambushable() { return 1; }

void do_ambushed() {
   write( protector +" stops you from harming "+
         (string)player->the_short() +".\n" );
   tell_object( player, protector +" stops "+
         (string)this_player()->the_short() +" from harming you.\n" );
} /* do_ambushed() */

int no_offense() {
  if (!can_be_nasty())
    return 1;
  zap_surrender_shadow();
  return (int)player->no_offense();
} /* no_offense() */

mixed query_property( string word ) {
  if (protecting && word == "no attack")
    return 1;
  return (mixed)player->query_property( word );
} /* query_property() */

int query_player_killer() {
   return 0;
}

int command_shadowed( string verb, string args ) {
  if ( member_array( verb, ({ "cast", "scathe" }) ) == -1 )
    return (int)player->command_shadowed( verb, args );
  if (!can_be_nasty())
    return notify_fail("You can't do that while you're subdued\n");
  zap_surrender_shadow();
  return (int)player->command_shadowed( verb, args );
} /* command_shadowed() */

varargs int adjust_xp( int number, int shared ) {
#ifdef DEBUG
  log_file( "PROTECTED", ctime( time() ) +": "+ (string)player->query_name() +
         " gained "+ number +"xp ["+ implode( map_array( previous_object( -1 ),
         (: file_name( $1 ) :) ), ", " ) +"]\n" );
#endif  
   return (int)player->adjust_xp( number, shared );
} /* adjust_xp() */

varargs int add_skill_level( string skill, int lvl, mixed exp ) {
   string what;
   if ( !exp )
      exp = previous_object();
   if ( objectp( exp ) )
      what = " from "+ file_name( exp );
   else
      what = " for "+ exp +"xp";
#ifdef DEBUG
   log_file( "PROTECTED", ctime( time() ) +": "+ (string)player->query_name() +
         " gained "+ lvl +" in "+ skill + what +" ["+
         implode( map_array( previous_object( -1 ), (: file_name( $1 ) :) ),
         ", " ) +"]\n" );
#endif   
   return (int)player->add_skill_level( skill, lvl, exp );
} /* add_skill_level() */

int test_remove(object thing, int flag, mixed dest)
{
  allowed -= ({ 0 });
  if (member_array(this_player(), allowed) != -1)
    return 1;
  else
    return player->test_remove(thing, flag, dest);
}

int allowed_to_loot(object who, object obj)
{
  if (member_array(who, allowed) != -1) {
    object wb;

    if (!obj)
      return 1;
    wb = obj->query_worn_by();
    if (wb == player && player->remove_armour(obj))
      return 0;
    return 1;
  } else
    return player->allowed_to_loot(who);
}

void dest_surrender_shadow()
{
  /* When the shadow wears off, NPCs should run off to /room/rubbish */
  if (!player->query_property("player")) {
    player->move("/room/rubbish", "",
      player->one_short() + " runs off to recuperate.");
  } else {
    tell_object(player, "You feel that you can leave now.\n");
  }
  destruct(this_object());
}

int query_sanctuary()
{
  if (protecting) return 1;
  else return player->query_sanctuary();
}

int do_quit()
{
  if (!can_be_nasty()) {
    write("You cannot quit while your subduer is here.\n");
    return 1;
  }
  return player->do_quit();
}

int quit_alt()
{
  if (!can_be_nasty()) {
    write("You cannot quit while your subduer is here.\n");
    return 1;
  }
  return player->do_quit();
}
