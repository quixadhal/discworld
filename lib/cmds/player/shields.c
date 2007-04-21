#include <armour.h>
#include <combat.h>

inherit "/cmds/base";

string construct_shield_message( object player, int not_self );

int cmd() {
  string mess;
  
  mess = construct_shield_message(this_player(), 0);
  
  /* If they have shields, give message,
   * otherwise, tell them the bad news. */
  if( sizeof( mess ) ) {
    tell_object( this_player(), "Arcane protection status:\n" + mess );
  } else {
    tell_object( this_player(), "You do not have any arcane protection.\n" );
  }
  
  return 1;
}


string construct_shield_message( object player, int not_self ) {
  int i, j;
  string mess;
  mixed *callbacks;
  string *res;
  class armour_callback cb;
  class combat_special cs;
  
  mess = "";
  callbacks = player->query_armour_callbacks();
  
  for( i = 0; i < sizeof(callbacks); i++) {
    for(j=0; j < sizeof(callbacks[i]); j++) {
      cb = callbacks[i][j];
      if(cb->callback[0]) {
        res = (cb->callback[0])->query_arcane_shields(player);
        if(sizeof(res))
          mess += " * $I$+3,+0=$C$" + res[not_self] + "$I$-3,-0=\n";
      }
    }
  }

  callbacks = player->query_specials();
  for(i=0; i<sizeof(callbacks); i++) {
    cs = callbacks[i];
    if(cs->callback[0]) {
      res = (cs->callback[0])->query_arcane_shields(player);
        if(sizeof(res))
          mess += " * $I$+3,+0=$C$" + res[not_self] + "$I$-3,-0=\n";
    }
  }
  
  return mess;
  
}


mixed *query_patterns() {
  return ({ "", (: cmd :) });
}
