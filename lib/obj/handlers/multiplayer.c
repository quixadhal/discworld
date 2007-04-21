/* $Id: multiplayer.c,v 1.17 2003/05/30 06:23:05 ceres Exp $ */
/**
 * Keeps track of the marked multi players.
 * Called by the login handler, checking people against a list of
 * previous multiplayers, and if they are on the list, check if
 * they are already on.
 * <pre><code>
 * vars are:
 *  string *gits: an array of names of suspected multiplayers
 *  string *denied_parcels: an array of names of multiplayers prevented from
 *                          using the parcel system for sending parcels
 *  mapping allowed: the keys are the names of verified non-multiplayers,
 *                   the values are the names of people they are allowed to
 *                   play with.
 * </pre></code>
 */

#include <login_handler.h>
#include <playerinfo.h>

#define SAVE_FILE   "/save/multiplayer"
#define ALIST "/secure/alist"
#define ALLOWED_ITEMS ({ "/obj/misc/club_badge", "/obj/misc/al_receipt" })
string *denied_parcels;
mapping allowed;

protected void create() {
  string denied_parcel;
  int changed = 0;

  denied_parcels = ({ });
  allowed = ([ ]);
  seteuid(master()->creator_file(file_name()));
  if (file_size(SAVE_FILE + ".o") > 0) {
    unguarded((: restore_object, SAVE_FILE, 1 :));
  }

  foreach (denied_parcel in denied_parcels) {
    if (!PLAYER_HANDLER->test_user(denied_parcel)) {
      denied_parcels -= ({ denied_parcel });
      changed = 1;
    }
  }
  if (changed) {
    unguarded((: save_object, SAVE_FILE :));
  }
}

private void save_me() {
  unguarded((: save_object, SAVE_FILE :));
}

/**
 * Called by the player object to see if a logon inform should be given
 * about duplicate players from the same IP address.
 * @param user the player being checked
 * @param others the others logged on from the same IP
 * @return array of not allowed dups
 */
string *check_allowed(mixed user, mixed *others) {
  string *not_allowed;
  mixed tmp, alt;

  if(objectp(user))
    user = user->query_name();

  // See if they're an alt of someone. If so check if their alt
  // has an allowed list.
  if(!allowed[user] && PLAYERINFO_HANDLER->query_alt_of(user) != "")
    user = PLAYERINFO_HANDLER->query_alt_of(user);

  not_allowed = ({ });
  foreach(tmp in others) {
    if(objectp(tmp))
      tmp = tmp->query_name();
    alt = PLAYERINFO_HANDLER->query_alt_of(tmp);
    if(!alt)
      alt = tmp;
    if(!allowed[user] || member_array(alt, allowed[user]) == -1)
      not_allowed += ({ tmp }); 
  }

  return not_allowed;
}

/**
 * Adds a player to the multiplayer list.
 * @param git the name of the suspected multiplayer
 * @return 0 if they are not a user, 1 if they are
 */
int add_git(string git) {
  PLAYERINFO_HANDLER->add_entry(this_player(), git, "multiplay",
                                "Caught multiplaying.\n");
  return 1;
}

/**
 * Adds a player to the allowed multi IP list.
 * @param user the name of the player
 * @param other the name of another user which is not an alt
 * @return 0 if they are not a user, 1 if they are
 */
int add_allowed(string user, string other) {
  int oldsize, changed;

  if (!(PLAYER_HANDLER->test_user(user) && PLAYER_HANDLER->test_user(other))) {
    return 0;
  }
  if(PLAYERINFO_HANDLER->query_alt_of(user))
    user = PLAYERINFO_HANDLER->query_alt_of(user);
  if(PLAYERINFO_HANDLER->query_alt_of(other))
    other = PLAYERINFO_HANDLER->query_alt_of(other);

  if (undefinedp(allowed[user])) {
    allowed[user] = ({ other });
  } else {
    oldsize = sizeof(allowed[user]);
    allowed[user] |= ({ other });
  }
  if (!oldsize || (oldsize != sizeof(allowed[user]))) {
    changed = 1;
    PLAYERINFO_HANDLER->add_entry(this_player(), user, "multiplay",
                                  "Added to multiplayer handler as allowed "
                                  "together with " + other + ".\n");
  }

  if (undefinedp(allowed[other])) {
    allowed[other] = ({ user });
  } else {
    oldsize = sizeof(allowed[other]);
    allowed[other] |= ({ user });
  }
  if (!oldsize || (oldsize != sizeof(allowed[other]))) {
    changed = 1;
    PLAYERINFO_HANDLER->add_entry(this_player(), other, "multiplay",
                                  "Added to multiplayer handler as allowed "
                                  "together with " + user + ".\n");
  }
  if (changed) {
    call_out((: save_me :), 1);
  }
  return 1;
} /* add_allowed() */

/**
 * Remove a person from allowed multi IP list.
 * @param user the name of the player to remove
 * @return 0 if they are not in the list, 1 if they are
 */
int delete_allowed(string user) {
  if(PLAYERINFO_HANDLER->query_alt_of(user))
    user = PLAYERINFO_HANDLER->query_alt_of(user);
  if (undefinedp(allowed[user]))
    return 0;
  map_delete(allowed, user);
  call_out((: save_me :), 1);
  PLAYERINFO_HANDLER->add_entry(this_player(), user, "multiplay",
                                "Deleted from multiplayer handler "
                                "(allowed).\n");
  return 1;
} /* delete_allowed() */

/**
 * Returns the current allowed multi player IP list
 * @return the list of allowed multi players
 */
mapping query_allowed() {
  return copy(keys(allowed));
} /* query_allowed() */

/**
 * Adds a user to the list of users not allowed to use the parcel system
 * Only Lords and Liaison deputies are allowed to add a user to this array
 * @param user the name of the player
 */
int add_denied_parcel(string user) {
  if (!"/secure/master"->query_lord(geteuid(previous_object())) &&
      !"/d/liaison/master"->query_deputy(geteuid(previous_object()))) {
    write("You do not have permission to add a user to this list.\n");
    return 0;
  }
  if (member_array( user, denied_parcels) != -1) {
    denied_parcels -= ({ user });
  }
  if (!PLAYER_HANDLER->test_user(user)) {
    return 0;
  }
  denied_parcels += ({ lower_case(user) });
  call_out((: save_me :), 1);
  PLAYERINFO_HANDLER->add_entry(this_player(), user, "multiplay",
                            "Denied the use of the parcel system.\n");
  return 1;
} /* add_denied_parcel() */

/**
 * Removes a user from the list of users not allowed to use the parcel system
 * Only Lords and Liaison deputies are allowed to remove a user from this array
 * @param user the name of the player
 * @return 0 if they are not in the list, 1 if they are
 */
int remove_denied_parcel(string user) {
  if (!"/secure/master"->query_lord(geteuid(previous_object())) &&
      !"/d/liaison/master"->query_deputy(geteuid(previous_object()))) {
    write("You do not have permission to remove a user to this list.\n");
    return 0;
  }
  if (member_array(user, denied_parcels) == -1)
    return 0;
  denied_parcels -= ({ user });
  call_out((: save_me :), 1);
  PLAYERINFO_HANDLER->add_entry(this_player(), user, "multiplay",
                           "Allowed the use of the parcel system.\n");
  return 1;
} /* remove_denied_parcel() */

/**
 * Checks user against the parcels array
 * @param user the name of the player
 * @returns 0 if they are not in the list, 1 if they are
 */
int member_denied_parcel( string user ) {
  if (member_array(user, denied_parcels) == -1)
    return 0;
  return 1;
} /* member_denied_parcel() */

/**
 * Returns the current list of players denied use of the parcel system.
 * @return the list of denied parcel users
 */
string *query_denied_parcels() {
  return copy(denied_parcels);
} /* query_denied_parcels() */

/** @ignore yes */
varargs void mark_log(string user, string comment) {
  if (!comment) {
    log_file("MULTIPLAYERS", ctime(time())
             + " " + user
             + " Previous log entries handled.\n");
    PLAYERINFO_HANDLER->add_entry(this_player(), user, "multiplay",
                                  "Previous log entries handled.\n");
  } else {
    log_file("MULTIPLAYERS", ctime(time())
             + " " + user
             + ": " + comment + "\n" );
    PLAYERINFO_HANDLER->add_entry(this_player(), user, "multiplay", 
                                  comment + "\n");
  }
} /* mark_log() */

/** @ignore yes */
string query_name() {
  return "Multiplayer handler";
}

void record_object(string action, object player, object item) {
  mixed stuff;

  stuff = item->query_property("dropped");
  if(sizeof(stuff) && player->query_name() != stuff[0])
    stuff = ({ player->query_name(), query_ip_number(player), time(),
                 stuff[0] });
  else
    stuff = ({ player->query_name(), query_ip_number(player), time() });
  item->add_property("dropped", stuff, 1800);
}

int check_multiplayers(string action, object player, object item) {
  mixed stuff;
  string direction, verb, res, is;
  
  if(objectp(player) && !interactive(player))
    return 0;
  
  stuff = item->query_property("dropped");
  if(!stuff)
    return 0;

  is = "";
  switch(action) {
  case "get":
    verb = " got ";
    direction = " from ";
    break;
  case "give":
    verb = " was given ";
    direction = " by ";
    is = "is ";
    break;
  case "slip":
    verb = " was slipped ";
    direction = " by ";
    is = "is ";
    break;
  case "palm":
    verb = " palmed ";
    direction = " from ";
    break;
  case "plant":
    verb = " planted "; 
    direction = " on ";
    is = "is ";
    break;
  case "recover":
    verb = " recovered ";
    direction = " from ";
    break;
  }
    
  if(stuff[0] != player->query_name() &&
     query_ip_number(player) == stuff[1] &&
     sizeof(check_allowed(player, ({ stuff[0]}) ))) {
    // Simple IP check.
    res = capitalize(player->query_name()) + verb + item->a_short() +
      direction + stuff[0] + " who " + is +"logged in from the same "
      "address.";
  } else if(direction != " by " &&
            player->query_name() == stuff[0] &&
            player->query_refresh_time() > stuff[2]) {
    // Refresh check.
    res = capitalize(player->query_name()) + verb + item->a_short() +
      " after a refresh.";
  } else if(ALIST->query_alt(player->query_name(), stuff[0]) == 100) {
    // Registered alt check.
    res = capitalize(player->query_name()) + verb + item->a_short() +
      direction + stuff[0] + " who is " + player->query_possessive() +
      " alt.";
  } else if(ALIST->query_alt(player->query_name(), stuff[0]) >= 60) {
    // Possible alt check.
    res = capitalize(player->query_name()) + verb +  item->a_short() +
      direction + stuff[0] + " who is POSSIBLY " +
      player->query_possessive() + " alt.";
  } else if(sizeof(stuff) == 4) {
    // Do third party checks.

    if(player->query_name() == stuff[3]) {
      if(player->query_refresh_time() > stuff[2]) {
        // Indirect refresh check.
        res = capitalize(player->query_name()) + verb + item->a_short() +
          " after a refresh" + direction + stuff[0] + ".";
      }
    } else if(ALIST->query_alt(player->query_name(), stuff[3]) == 100) {
      // Indirect registered alt check.
      res = capitalize(player->query_name()) + verb + item->a_short();
      if(stuff[0] != player->query_name())
        res += " via " + stuff[0];
      res += direction + stuff[3] + " who is " + player->query_possessive() +
        " alt.";
    } else if(ALIST->query_alt(player->query_name(), stuff[3]) >= 60) {
      // Indirect possible alt check.
      res = capitalize(player->query_name()) + verb + item->a_short();
      if(stuff[0] != player->query_name())
        res += " via " + stuff[0];
      res += direction + stuff[3] + " who is _POSSIBLY_ " +
        player->query_possessive() + " alt.";
    }
  } else if((PLAYER_HANDLER->test_creator(stuff[0]) ||
             PLAYER_HANDLER->test_property(stuff[0], "test character")) &&
            !player->query_creator() &&
            !player->query_property("test character") &&
            member_array(base_name(item), ALLOWED_ITEMS) == -1 &&
            base_name(item)[0..14] != "/d/liaison/TOYS") {

    res = capitalize(player->query_name()) + verb + item->a_short() +
      direction + stuff[0];
    user_event("inform", res, "cheat");
    log_file("/d/admin/log/CHEAT", ctime(time()) + ": " +
             player->convert_message(res) + "\n");
    return 0;
  }
  if(res) {
    user_event("inform", res, "multiplayer");
    log_file("MULTIPLAYERS", ctime(time()) + ": " +
             player->convert_message(res) + "\n");
  }

  return 0;
}
