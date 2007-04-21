/* this is to be inherited by effect shadows -*- LPC -*- */

object player;
int id;

object attach_to_player(object p, int i) {
  player = p;
  id = i;
  return shadow( player, 1 );
}

void remove_effect_shadow(int i) {
  if (i == id) { if( this_object() ) destruct( this_object() ); return; }
  player->remove_effect_shadow(i);
}

protected mixed arg() {
   int enum;
   enum = (int)player->sid_to_enum( id );
   if ( enum == -1 ) {
#ifdef DEBUG
     log_file( "EFFECTS", time() +": no effect for "+
               file_name( this_object() ) +"\n           on "+
               file_name( player ) +"\n" );
#endif
      destruct( this_object() );
      return 0;
   }
   return (mixed)player->arg_of( enum );
} /* arg() */

protected void set_arg(mixed newarg) {
   player->set_arg_of(player->sid_to_enum(id), newarg);
}

protected void remove_this_effect() {
  player->delete_effect( (int)player->sid_to_enum( id ) );
} /* remove_this_effect() */
