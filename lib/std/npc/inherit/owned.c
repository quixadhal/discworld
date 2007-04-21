/**
 * Common functionality for an npc that is owned (ie summoned/controlled
 * by someone else).
 * @author Aquilo
 */

/* 
Store player owners as string name as it means they remain owner over su's.
*/

private mixed _owner;

/**
 * Set who owns us. 
 * @param owner either player name, player ob, or other living ob
 */
object set_owner( mixed owner ){
  if( stringp(owner) )
    _owner = owner;
  if( objectp(owner) && living(owner) ){
    if( userp(owner) )
      _owner = owner->query_name();
    else
      _owner = owner; 
  }

  return _owner;
}

/**
 * @return the ob pointing to the thing that owns/controls us
 */
object query_owner() {
  if( stringp(_owner) )
    return find_player(_owner);
  else
    return _owner;
}


/** 
 * Checked by pk checking code
 * @ignore yes */
object query_owner_pk_check() {
   return query_owner();
}


mixed *stats(){
  return ({ 
    ({ "owner", _owner }),
  });
}
