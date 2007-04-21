/*  -*- LPC -*-  */
/*
 * $Locker:  $
 *
 * $Id: theft_base.c,v 1.12 2003/04/21 23:54:24 ceres Exp $
 *
 */


/*  Base for stealing. Does standard checks and has useful functions. */
#include <thief.h>
#include <obj_parser.h>
#include <player.h>

#define SKILL "covert.manipulation.stealing"
#define PERCEPTION "other.perception"

inherit "/cmds/guild_base";

int query_theft_command() { return 1; }

int victim_checks(object thief, object victim) {
  if(thief == victim)
    return notify_fail("Be serious!\n");
  
  if(thief->query_property("dead"))
    return notify_fail("Be serious, you're dead!\n");
  
  if(thief->query_auto_loading())
    return notify_fail("You don't have all your equipment yet..\n");

  if(victim->query_auto_loading())
    return notify_fail( "Be sporting; "+ (string)victim->the_short() +
                        " doesn't have "+ (string)victim->query_possessive() +
                        " equipment yet.\n" );

  if(userp(victim) && !interactive(victim))
    return notify_fail( "You can't " + query_verb() +
                        " from a net dead statue.\n");

  if(victim->query_property("nosteal"))
    return notify_fail("You cannot " + query_verb() + " from " +
                       victim->the_short() + ".\n");
  
  if(victim->query_sanctuary())
    return notify_fail("You can't snatch from someone who is protected.\n");

  if(pk_check(thief, victim))
    return notify_fail("You feel it would be wrong to " + query_verb() +
                       " from "+ victim->short()+".\n");

  if(victim->query_creator() && !thief->query_creator() ) {
    notify_fail("Stop trying to steal from creators.\n");
    thief->adjust_tmp_dex(-10);
    return 0;
  }

  return 1;
}

mixed get_item(object thief, mixed location, string str) {
  class obj_match result;
  
  result = (class obj_match)match_objects_in_environments(str, location, 0, thief);

  if(result->result != OBJ_PARSER_SUCCESS)
    return notify_fail("Pssst, they don't have one of those.\n");

  result->objects = filter(result->objects,
                           (: environment($1) == $(location) :));
  if(!sizeof(result->objects))
    return notify_fail("Pssst, they don't have one of those.\n");

  if(sizeof(result->objects) != 1)
    return notify_fail("You can only " + query_verb() +
                       " one thing at a time.\n");

  return result->objects[0];
}

int combat_checks(object thief, object victim) {
  if(thief->query_fighting()) {
    notify_fail("You can't attempt to " + query_verb() +
                " while in battle.\n");
    return 2;
  }
  
  if(member_array(thief, victim->query_attacker_list()) != -1) {
    notify_fail("You cannot attempt to " + query_verb() + " from someone "
                "that is fighting you.\n");
    return 1;
  }
  
  return 0;
}

int item_checks(object victim, object item, int wielded_ok, int worn_ok) {
  
  if(item->query_liquid() && item->query_food_object())
    return notify_fail("You cannot " + query_verb() + " liquids!\n");

  if(item->query_property("nosteal") ||
     (!wielded_ok && item->query_wielded() == victim) ||
     (!worn_ok && item->query_worn_by() == victim)) 
    return notify_fail("You cannot " + query_verb() + " " +
                       item->the_short() + " from " +
                       victim->the_short() + ".\n");

#ifdef QUOTA_CHECK  
  if(!check_player_quota(victim, item))
    return notify_fail("A quick reveals that " + query_verb() + "ing " +
                       item->the_short() + " would exceed " +
                       victim->query_possessive() + "'s theft quota.\n");
#endif  
  return 1;
}

int calc_value(object ob) {
  int value;
  object tmp, *inv;
  string name;


  //if object is /obj/package only check contents 
  //because package value is value of its contents
  sscanf( file_name( ob ), "%s#%*d", name );
  if( name = "/obj/package" ) {
    value = 0;
  }
  else {
    value = ob->query_value();
  }
  inv = deep_inventory(ob);
  if(sizeof(inv))
    foreach(tmp in inv)
      value += tmp->query_value();

  return value;
}

int check_player_quota(object victim, object ob) {
  object *things, thing;
  int quota, reported, valid, value;
  
  value = 0;
  things = ({ ob });
  things += deep_inventory(ob);
  
  foreach(thing in things) {
    if(thing->query_property("money") ) {
      value += thing->query_value_in("Ankh-Morpork");
    } else {
      value += thing->query_value();  
    }
  }

  quota = QUOTA->query_player_quota(victim->query_name());
  reported = QUOTA->query_player_reported(victim->query_name());
  valid = QUOTA->query_player_valid(victim->query_name());
  
  if((quota - (reported + valid)) < value)
    return 0;
  
  return 1;
}

int steal_item(object thief, object victim, object item) {
  object *stolen;
  string sh;
  
  if(item->move(thief))
    return notify_fail( "You cannot " + query_verb() + " " +
                        item->the_short()+" from "+
                        (string)victim->the_short() +".\n");

  stolen = ({ item });
  stolen += deep_inventory(item);
  tell_object(thief, "You steal "+query_multiple_short(stolen, "the" )+
              " from "+ victim->the_short() +".\n" );

  if( environment(thief)->query_theft_handler() && 
      regexp(environment(thief)->query_theft_handler(), "/d/am/") && 
        thief->query_name() == "wobin" )
      stolen = THEFT_INSURANCE->check_items(stolen, victim, thief);

  event( environment( thief ), "theft", thief, victim, stolen );
  event( stolen, "theft", thief, victim );

  //Simplify wards a bit.

  item->event_ward( thief, victim );
    
  if(thief->query_property("player") &&
     query_ip_number(thief) == query_ip_number(victim) &&
     sizeof(PLAYER_MULTIPLAYER_HANDLER->check_allowed(thief, ({victim})))) {
    sh = capitalize(thief->query_name())+ " stole " +
      query_multiple_short(stolen) +
      " from " + victim->query_name() + " while logged in from the same IP "
      "address.";
    user_event("inform", sh, "multiplayer");
    sh = victim->convert_message(sh);
    log_file("MULTIPLAYERS", ctime(time()) + ": " + sh + "\n");
  }
  
  return 1;
}
