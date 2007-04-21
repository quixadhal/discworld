/* Shadow to protect against: magic smoke and other things that use
 * run_away, ranged weapons (long shot), and KLK insects. Thanks to
 * Ceres for coding the octagon, which I borrowed a better part of
 * this code from. - Aragorn
 */

object player;

void setup_shadow( object thing ) {
   shadow( thing, 1 );
   player = thing;
} /* setup_shadow() */

void dest_shadow() { destruct( this_object() ); }

void run_away() {
  tell_object(player, "You feel too peaceful to run away.\n");
}

int pk_check( mixed thing1, mixed thing2, int off_line ) {
  return 1;
}

void start_attack(){}


//Changed so you no longer regen HP or GP while you're in here.
//It makes a nice incentive not to stay in here indefinately, 
//also it stops situations where the player exposes themselves to 
//a slow damage source [such as holy sacrifice, or a slow poison]
//and then reaps the gain while standing in here to avoid to
//penalty.
void adjust_hp( int number, object attacker ) {

  if(player->query_attacker_list()) {
    foreach(attacker in player->query_attacker_list()) {
      attacker->stop_fight(player);
      player->stop_fight(attacker);
    }
  }
  
  //if( number < 0 )
    return;

  //player->adjust_hp( number );
}

void adjust_gp( int number ) {
  if ( number < 0 ) 
    return player->adjust_gp( number );
  return;
}

int attack_by(object ob) {
  ob->stop_fight(player);
  player->stop_fight(ob);
  return 1;
}
