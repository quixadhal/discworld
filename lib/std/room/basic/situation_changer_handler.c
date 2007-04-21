#include <situations.h>

/* This is a changer and handler rolled into one.  The base
   object stores all the situations and the clones of it
   point its sitdata for theirs. 

   The situations for the base object may be setup
   using the setup_situations function which is only
   called on the base, not the clones.  Otherwise
   you can call add_situations as usual but you its good
   to check that the room's changer is not a clone first.
    */ 

inherit "/std/room/basic/situation_changer";

void add_situation( mixed label, class situation sit ) {
  if (!clonep()) ::add_situation( label, sit );
  else write("You can't add situations to a clone of this object.");
}

/* The base object will be used to manage a room */
object set_room(object room_o) { 
  //  tell_creator("shaggy","set room on %O\n",this_object());

  if (clonep() ) {
    object base;

    base = find_object(base_name(this_object()));
    if (!base) {
      (base_name(this_object()))->parp();
      base = find_object(base_name(this_object()));
    }
    //    if (!base) tell_creator("shaggy","%O just wouldn't load!\n",base_name(this_object()));
    
    if (!(base->query_room())) {
      //      tell_creator("shaggy","Using %O instead of %O\n",base,this_object());
      call_out( "dest_me", 2 );
      return room_o->set_situation_changer( base );
    }
    sitdata = base->query_situations();
  }      

  return ::set_room( room_o );
}

void dest_me() {
  //  tell_creator("shaggy","Test Desting %O %O\n,",this_object(),
  //	       sizeof(children(base_name(this_object()))));

  if (clonep()) ::dest_me();
  else if (sizeof(children(base_name(this_object())))==1) ::dest_me();
  return;
}

void create() {
//  tell_creator("shaggy","create on %O\n",this_object());
  
  if (!clonep()) this_object()->setup_situations();
}
