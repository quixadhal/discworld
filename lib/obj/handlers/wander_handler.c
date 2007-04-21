/*  -*- LPC -*-  */
/*
 * $Locker:  $
 */

/**
 * Handles npcs wanderting randomly around the place and npcs
 * following a route to a destination.
 * @see /obj/monster->move_me_to()
 * @see /std/effects/npc/goto_destination
 * @author Wodan
 * @author Pinkfish
 */
// rewritten by Wodan 19-6-97.
#include <player.h>

#define MH "/obj/handlers/map"
#define LAST_LOC_PROPERTY "last locations"

void do_move_after();

//recycled as long as the npc travels, only movetime changes
class route_traveller { //we use this to store npcs on a route
  object monster;
  int movetime;
  int delay;
  string dest;
}

//dumped after its movetime
class wander_group { //monsters that will move at movetime
  object *monsters;
  int movetime;
}

private class wander_group *wanderers = ({});
private mapping wanderpos = ([]);
private class route_traveller *travellers = ({});
private mapping travelpos = ([]);
private int move_call_out_id;
private int after_call_out_id;

private void do_move_after();
private void move_monsters();
void delete_move_after( object monster );
private void do_wander(int running_away, object monster);

/** @ignore yes */
mixed query_next_mover() {
  return wanderers[0];
} /* query_next_mover() */

/** @ignore yes */
mixed query_anything(string thing){
  return copy(fetch_variable(thing));
} /* query_anything() */

/**
 * Is this npc a moving npc?
 * @param ob the npc to check to see if it moves
 * @return 1 if it is moving, 0 if it is not.
 */
int query_moving_monster(object ob) {
  return member_array(ob, travellers) != -1;
} /* query_moving_monster() */

/** @ignore yes */
mixed query_route_monster() {
  return travellers[0];
} /* query_route_monster() */

/** @ignore yes */
int find_pos(mixed *arr, function func, int val, int ref found){
  //search for place to insert in the lists
  //searching algorithm ripped from the driver
  int mid, high, low;

  high = sizeof(arr) - 1;
  low = 0;
  while(high >= low){
    mid = (high + low) / 2;
    if(val < (*func)(arr[mid]))
      high = mid - 1;
    else if(val > (*func)(arr[mid]))
      low = mid + 1;
    else break;//found it
  }
  found = !(high < low);
  return mid;
} /* find_pos() */

/** @ignore yes */
private void add_traveller(class route_traveller new_traveller){
  if(sizeof(travellers)){
    int found, mid;
    //get list position
    mid = find_pos(travellers, (: $1->movetime :), new_traveller->movetime, 
                   ref found);

    if(!found){
      //no place yet
      if(new_traveller->movetime > travellers[mid]->movetime)
        mid++;
      if(mid == sizeof(travellers)){
        travellers += ({new_traveller});
        if(find_call_out(move_call_out_id) == -1 && sizeof(travellers) ) {
          move_call_out_id = call_out((: move_monsters :),
                                      travellers[0]->movetime - time() );
        }
        return;
      }
    }

    if(mid == 0){ //start of list
      travellers = ({new_traveller}) + travellers;
      remove_call_out(move_call_out_id);
      move_call_out_id = call_out((: move_monsters :),
                                      new_traveller->movetime - time());

    } else {
        travellers = insert(travellers, new_traveller, mid); 
    }

  } else { //no list yet
    travellers = ({ new_traveller });
    move_call_out_id = call_out((: move_monsters :),
                                      new_traveller->movetime - time());
  }
} /* add_traveller() */

/**
 * Called from the npc to start them mmoving.  This should not need to
 * be called inside your code.  It is called from the function
 * move_me_to in the npc code.
 * @see /obj/monster->move_me_to()
 * @param delay the delay between each move
 * @param dest the destination room for the npc
 * @return 0 if it failed and 1 on success
 */
int move_me_please(int delay, string dest) {
  class route_traveller new_traveller;
  object monster;

  monster = previous_object();
  if(!intp(delay))
    return 0;

  if(travelpos[monster]) //get the old one out
    //we don't really remove it because member_array doesn't work on arrays of
    //classes.
    travelpos[monster]->monster = 0;

  if(delay < 5)
    delay = 5;

  new_traveller = new (class route_traveller, monster:monster, delay:delay,
             movetime:time() + delay, dest:dest);
  travelpos[monster] = new_traveller;
  
  add_traveller(new_traveller);
} /* move_me_please() */

/**
 * Actually moves everyone around the place.
 */
private void move_monsters(){
  class route_traveller tmp;
  int virtual_move, right, refuse;
  object monster;
  string true_location, destination, direc;

  tmp = travellers[0];
  monster = tmp->monster;

  /* First remove it from the current list */
  if(sizeof(travellers) > 1)
    travellers = travellers[1..];
  else 
    travellers = ({});
  
  if (monster) {
    virtual_move = monster->query_virtual_move();
    true_location = monster->query_true_location();
    direc = monster->get_next_route_direction();
    if (direc) {
      if ( !virtual_move ){
//    We don't need to check it if we're doing it.
// catch only returns something on errors, mostly in loading rooms, so the move 
// did not happen, so we _do_ need to try again, don't remove code you don't
// understand, please!
        if(catch(monster->do_command(direc)))
          catch(monster->do_command(direc)); //we can still move in a room if an item
                                             //in that room doesn't load and
                                             //caused an error
      } else {
        if ( "/room/virtual" != file_name( environment( monster ) ) )
          right = 1;
        if ( right ) {
          environment( monster )->set_destination( direc );
          destination = environment( monster )->query_destination( direc, monster );
        } else
          destination = (string)MH->qdes( direc, true_location );
        if ( right ) {
          if ( monster->check_anyone_here() || find_object( destination ) ){
            if(catch(monster->do_command( direc )))
              catch(monster->do_command( direc ));
          } else {
            "/room/virtual"->force_load();
            monster->move( find_object( "/room/virtual" ) );
            true_location = destination;
            monster->set_true_location( true_location );
          }
        } else {
          if ( !find_object( destination ) ) {
            true_location = destination;
            monster->set_true_location( true_location );
          } else {
            object ob = environment(monster);
            ob->remove_exit(direc);
            ob->add_exit(direc, destination,
               "plain" );
            monster->do_command(direc);
            ob->remove_exit(direc);
          }
        }
      }
    }
    tmp->movetime = time() + tmp->delay; //next movetime

    /* Put it back into the list */
    if ( direc && (!(refuse = monster->query_stop_moving()))) {
      add_traveller( tmp );
    } else {
      if(!refuse)
        refuse = (file_name(environment(tmp->monster)) != tmp->dest);
      tmp->monster->stopped_route(!refuse);
      map_delete( travelpos, tmp->monster );
    }
  } else //remove elements with 0 as a key
    travelpos = filter(travelpos, (: $1 :));
  
  if(find_call_out(move_call_out_id) == -1 && sizeof(travellers) ) {
    move_call_out_id = call_out((: move_monsters :),
                                      travellers[0]->movetime - time() );
  }
} /* move_monsters() */

/** @ignore yes */
private void add_move_after(object monster, int movetime) {
  if(sizeof(wanderers)){
    int found, mid;
    //insert into the list
    
    reset_eval_cost();

    if(sizeof(wanderers) > 5000){
      //those faster clean ups must be killing us, didn't use to get this big
      wanderers=filter(wanderers, function(class wander_group thing){
        return sizeof(filter(thing->monsters, (:$1:)));
      });
      reset_eval_cost();
    }
   
    if (find_call_out(after_call_out_id) == -1)
      after_call_out_id = call_out((: do_move_after :), 0);

    mid = find_pos(wanderers, (: $1->movetime :), movetime, ref found);

    if(!found){
      //no place yet
      if(movetime > wanderers[mid]->movetime)
        mid++;

      if(mid == sizeof(wanderers)){ //end of list
        wanderers += ({ new(class wander_group, monsters:({monster}),
                            movetime:movetime) });
        wanderpos[monster] = wanderers[<1];
        return;
      }

      if(mid == 0){ //start of list
        wanderers = ({ new(class wander_group, monsters:({monster}),
                           movetime:movetime) }) + wanderers;
        wanderpos[monster] = wanderers[0];
        remove_call_out(after_call_out_id);
        after_call_out_id = call_out((: do_move_after :), movetime - time());
        return;
        }

      wanderers = insert(wanderers, new(class wander_group,
                                        monsters:({monster}),
                                        movetime:movetime), mid);
      wanderpos[monster] = wanderers[mid];
      return;
    } else { //add to list for this timeslot
      wanderers[mid]->monsters += ({ monster });
      wanderpos[monster] = wanderers[mid];
      return;
    }
  } else { //no list yet
    wanderers = ({ new(class wander_group, monsters:({monster}),
                       movetime:movetime) });
    wanderpos[monster] = wanderers[0];
    after_call_out_id = call_out((: do_move_after :), movetime - time());
  }
} /* add_move_after() */

/**
 * Put an npc into the random movement group.  This is called from
 * the set_move_after code in the npc object.  You should not need
 * to call this function directrly.
 * @see /obj/monster->set_move_after()
 * @param runaway is this a wimpy attempt?
 */
void move_after( int runaway ) {
  mixed move_after;
  
  if(runaway)
    return do_wander(runaway, previous_object());
  
  move_after = previous_object()->query_move_after();

  delete_move_after(previous_object());

  if (!arrayp(move_after) || (!move_after[0] && !move_after[1])) {
    return;
  }
  
  add_move_after(previous_object(), time() + move_after[ 0 ] + 
                 random( move_after[ 1 ] ));
} /* move_after() */

/**
 * Stops an npc wanderering aimlessly around the place.
 * @param monster the npc to stop wandering
 */
void delete_move_after(object monster) {
  if(wanderpos[monster]) {
    ((class wander_group)wanderpos[monster])->monsters -= ({monster});
  }
  map_delete(wanderpos, monster);
} /* delete_move_after() */

/**
 * The NPCs just wander around.  Not doing anything nessessarily.
 * They never stop, so we never remove them from the list.  Just
 * shuffle their position.
 */
private void do_wander(int running_away, object monster) {
  int right, virtual_move;
  string destination, *direcs, *room_zones, true_location, *move_zones,
         *last_locs;

  if ( !monster )
    return ;
  if ( !environment( monster ) || monster->query_property( PASSED_OUT_PROP ) )
    return ;
  if ( !running_away &&
       sizeof( (object *)monster->query_attacker_list() ) )
    return ;
  if ( (int)monster->query_hp() <= 0 )
    return ;

  /* Find the current location and the exits from it */
  virtual_move = monster->query_virtual_move();
  true_location = monster->query_true_location();
  if (1 || !virtual_move || 
       (virtual_move && "/room/virtual" != file_name(environment(monster))))
    right = 1;
  if ( right )
   direcs = (string *)environment( monster )->query_direc( monster );
  else
    direcs = (string *)MH->qdir( true_location );

  last_locs = monster->query_propery( LAST_LOC_PROPERTY );
  if (!pointerp(last_locs))
    last_locs = ({ });
  /* Determine which one to go down. */
   if (!direcs) direcs = ({ });
  direcs = shuffle( direcs );
  while ( sizeof( direcs ) ) {
    if ( right ) {
      direcs[ 0 ] = (string)environment( monster )->expand_alias( direcs[ 0 ] );
//      environment( monster )->set_destination( direcs[ 0 ] );
      destination = environment( monster )->query_destination( direcs[0], monster );
    } else
      destination = (string)MH->qdes( direcs[ 0 ], true_location );
    if ( (!destination && !virtual_move) || (sizeof(direcs) > 1 &&
                          member_array(destination, last_locs) != -1 ) ||
         (0 && stringp(destination) && !find_object(destination) && !virtual_move) ) {
      direcs = direcs[1..];
      continue;
    }
    /* Correct zones... */
    move_zones = (string *)monster->query_move_zones();
    if ( sizeof( move_zones ) ) {
      if ( find_object( destination ) )
        room_zones = (string *)destination->query_zones();
      else
        room_zones = (string *)MH->query_zones( destination );
      if (!room_zones ||
          (sizeof(room_zones) == sizeof(room_zones - move_zones))) {
        direcs = direcs[1..];
        continue;
      }
    }
    break;
  }

  last_locs += ({ destination });
  if (sizeof(last_locs) > 4)
    last_locs = last_locs[sizeof(last_locs)-4..];
  monster->add_property( LAST_LOC_PROPERTY, last_locs);

  /*
   * Ok, if we are not going to move virtualy, do something
   * exciting.
   */
  if (sizeof(direcs)) {
    if (1 || !virtual_move ||
        (right && monster->check_anyone_here()) ||
        (find_object( destination ) ) ) {
      object ob;
      if (!right) {
        /* This means we are in the void, fiddle the exits... 
         */
        ob = environment(monster);
        ob->remove_exit(direcs[0]);
        ob->add_exit(direcs[0], destination, "plain");
        reset_eval_cost();
      }
      monster->do_move(direcs[0]);
      if(!right)
        ob->remove_exit(direcs[0]);
    } else {
      /* virtual movement... */
      if (virtual_move) {
        "/room/virtual"->force_load();
        monster->move( find_object("/room/virtual"));
        monster->set_true_location(destination);
      } 
    }
  }
} /* do_wander() */

private void do_move_after() {
  object monster;
  int movetime, *move_after, zero;
  class wander_group tmp;

  reset_eval_cost();

  if(sizeof(wanderers) > 5000){
    //those faster clean ups must be killing us, didn't use to get this big
    wanderers=filter(wanderers, function(class wander_group thing){
      return sizeof(filter(thing->monsters, (:$1:)));
    });
    reset_eval_cost();
  }

  while (sizeof(wanderers) && wanderers[0]->movetime <= time()) {
    reset_eval_cost();
    tmp = wanderers[0];
    wanderers = wanderers[1..];
    foreach(monster in tmp->monsters){
      map_delete(wanderpos, monster);
      if (objectp(monster) && environment(monster)) {
        move_after = monster->query_move_after();
        if (move_after[0] || move_after[1]) {
          movetime = time() + move_after[ 0 ] + random( move_after[ 1 ] );
          if( movetime < (time() + 2) ) {
            movetime = time() + 2;
          }
          add_move_after( monster, movetime );
          reset_eval_cost();
          if (!travelpos[monster]) { //we should not be on a route
            do_wander(0, monster);
          }
        }
      } else {
        zero = 1;
      }
    }
  }

  if (find_call_out(after_call_out_id) == -1 && sizeof(wanderers)) {
    after_call_out_id = call_out((: do_move_after :),
                                        wanderers[0]->movetime - time());
  }

  if(zero) {
    wanderpos = filter(wanderpos, (: $1 :));
  }
} /* do_move_after() */

mapping query_dynamic_auto_load() {
   return ([
    "wanderers" : wanderers,
    "wanderpos" : wanderpos,
    "travellers" : travellers,
    "travelpos" : travelpos
     ]);
}

void init_dynamic_arg(mapping map) {
  if (map) {
     wanderers = map["wanderers"];
     wanderpos = map["wanderpos"];
     travellers = map["travellers"];
     travelpos = map["travelpos"];
     do_move_after();
  }
}
