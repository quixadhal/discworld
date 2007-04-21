/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: team.c,v 1.3 1999/10/28 02:22:12 ceres Exp $
 * $Log: team.c,v $
 * Revision 1.3  1999/10/28 02:22:12  ceres
 * tweaks
 *
 * Revision 1.2  1998/07/13 04:29:44  pinkfish
 * Fixed up a couple of things that shouldmake it stop giving empty team lists...
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
#define CREATOR "Ceres"
/**
 * This handler deals with grouping players into teams. Teams are entirely
 * optional.  The handler deals with creating, joining, leaving and ending
 * teams.  It also provides functions to find out who's a member of which
 * team etc.<p>
 *
 * Commands will be available to do group things (group chatting, group
 * health etc.) By default team members will protect each other and follow
 * the leader.
 *
 * @author Ceres
 * @started June 96
 */

#include <login_handler.h>

#define SHADOW "/std/shadows/misc/team"
#define MAX_GROUP_SIZE 5

class group {
  object leader;    // the team leader
  int policy;       // the follow policy
  object *members;  // list of members
  int locked;       // is this group open to new members
}

mapping groups,     // all the teams
        members;    // which team each player is a member of

private void check_group(string g_name);
private void disband_group(string g_name, string p_name);
private void tidy_members(string g_name);

/** @ignore yes */
void create() {
  members = ([ ]);
  groups = ([ ]);
} /* create() */

/** @ignore yes */
void dest_me() {
  destruct(this_object());
} /* dest_me() */

/**
 * When a player leaves the game automatically remove them from their team.
 * this function is called automatically by the login handler for every
 * player who leaves the game
 * @param p_name the player name who is leaving
 * @param type the type of action being preformed
 * @see /obj/handlers/login_handler
 */
void leaving(string p_name, string type) {
  class group record;
  string g_name;

  if((type != "logout") || (!members[p_name]))
    return;

  g_name = members[p_name];
  record = (class group)groups[g_name];

  map_delete(members, p_name);

  if(!record)
    return;
  
  // no leader? Then disband the group.
  if((record->leader == 0) || ((record->leader)->query_name() == p_name)) {
     disband_group(g_name, p_name);

#ifdef LOG_FILE    
    log_file(LOG_FILE, sprintf("%s %s has left the game, the %s has been "
                             "disbanded.\n", ctime(time()), p_name, g_name));
#endif    

  } else {
#ifdef LOG_FILE
    log_file(LOG_FILE, sprintf("%s %s left the game while a member of the %s\n",
                             ctime(time()), p_name, g_name));
#endif    
    record->members = delete(record->members,
                             member_array(0, record->members), 1);
  }
  return;
} /* leaving() */

/* Group modification functions */

/**
 * This method creates a new team.
 * @param g_name the name of the group
 * @param leader the leader of the group (object)
 * @param policy the policy of the group
 * @see end_group()
 */
int new_group(string g_name, object leader, int policy) {
  class group record;
  
  if(!undefinedp(groups[g_name]))
    return 0;

  record = new(class group);

  record->leader = leader;
  record->policy = policy;
  record->members = ({ leader });

  groups[g_name] = record;

  members[leader->query_name()] = g_name;

  leader->set_title( "TEAM", "Team Leader of "+ g_name );
   clone_object( SHADOW )->setup_shadow( leader );

#ifdef LOG_FILE
  log_file(LOG_FILE, sprintf("%s %s created by %s\n", ctime(time()),
                           g_name, leader->query_name()));
#endif  
  return 1;
} /* new_group() */

/**
 * This method ends a group.  Closes it, finishes it, done, finito.
 * @param g_name the name of the group to close
 * @see new_group()
 */
int end_group(string g_name) {
  class group record;
  object player;
  
  if(undefinedp(groups[g_name]))
    return 0;

  record = groups[g_name];
  
  // mark all group members as not being members of a group anymore
   foreach( player in record->members )
      if ( player ) {
         map_delete( members, player->query_name() );
         player->remove_title( "TEAM" );
         player->destruct_team_shadow();
      }
   
  
  map_delete(groups, g_name);

#ifdef LOG_FILE
  log_file(LOG_FILE, sprintf("%s %s disbanded\n", ctime(time()), g_name));
#endif  
  return 1;
} /* end_group() */

// join an already existing team
/**
 * This method joins a player into an existing group.
 * @param g_name the name of the group to join
 * @param player the player to join to the group
 * @see new_group()
 * @see end_group()
 * @see leave_group()
 */
int join_group(string g_name, object player) {
  class group record;

  if(undefinedp(groups[g_name]))
    return 0;

  record = groups[g_name];
  record->members += ({ player });

  members[player->query_name()] = g_name;

   player->set_title( "TEAM", "Team Member of "+ g_name );
   clone_object( SHADOW )->setup_shadow( player );

#ifdef LOG_FILE
  log_file(LOG_FILE, sprintf("%s %s joined by %s\n", ctime(time()),
                           g_name, player->query_name()));
#endif  
  return 1;
} /* join_group() */

// leave a team
/**
 * This method removes a player from the group.
 * @param g_name the name of the group to leave
 * @param player the player to leave the group
 * @see join_group()
 */
int leave_group(string g_name, object player) {
  class group record;
  
  if(player) {
    map_delete(members, player->query_name());

      player->remove_title( "TEAM" );
      player->destruct_team_shadow();

#ifdef LOG_FILE
    log_file(LOG_FILE, sprintf("%s %s left by %s\n", ctime(time()), g_name,
                             player->query_name()));
#endif    
  }

  if(undefinedp(groups[g_name]))
    return 0;

  record = groups[g_name];

  if(member_array(player, record->members) == -1)
    return -1;

  record->members = delete(record->members,
                           member_array(player, record->members), 1);

  return 1;
} /* leave_group() */


/* Group interrogation functions. */

/**
 * This method determine the owner (leader)  of a team.
 * @param g_name the name of the group to get the owner for
 * @return the object refering to the owner
 * @see query_members()
 * @see query_policy()
 */
object query_owner(string g_name) {
  class group record;

  if(undefinedp(groups[g_name]))
    return 0;

  record = (class group)groups[g_name];

  return record->leader;
} /* query_owner() */

/**
 * This method returns the members of the team.
 * @param g_name the name of the group to get the member of
 * @return the members names as strings
 * @see query_owner()
 * @see query_policy()
 * @see query_leader()
 */
object *query_members(string g_name) {
  class group record;
  
  if(undefinedp(groups[g_name]))
    return 0;

  tidy_members(g_name);

  record = groups[g_name];

  return (object *)record->members;

} /* query_members() */

/**
 * This method returns the policy of the team
 * @param g_name the name of the team
 * @return the policy of the team
 * @see query_members()
 * @see query_leader()
 */
int query_policy(string g_name) {
  class group record;
  
  if(undefinedp(groups[g_name]))
    return 0;

  record = groups[g_name];
  return (int)record->policy;

} /* query_policy() */

/**
 * This method finds out which team player is a member of.
 * @param player the player to find the team of
 * @return the team they are a member of
 */
string query_group(object player) {
  if(undefinedp(members[player->query_name()]))
    return 0;

  return members[player->query_name()];
} /* query_group() */

/**
 * This method determine if a team exists.
 * @param g_name the name of the team
 * @return 1 if the team exists, 0 if not
 * @see new_group()
 * @see query_group()
 */
int test_group(string g_name) {
  if(undefinedp(groups[g_name]))
    return 0;

  return 1;
} /* test_group() */

/**
 * This method checks if its ok to join a group.
 * @param g_name the name of the group
 * @return 1 if the team is full, 0 if not
 * @see join_group()
 */
int query_full(string g_name) {
  class group record;
  object member;
  
  if(undefinedp(groups[g_name]))
    return 0;

  tidy_members(g_name);
  
  record = (class group)groups[g_name];
  
  if(sizeof(record->members) >= MAX_GROUP_SIZE)
    return 1;

  return 0;
} /* query_full() */

/**
 * This method determines if the team is locked.
 * @param g_name the name of the team to test
 * @return 1 if locked, 0 if unlocked
 * @see set_locked()
 */
int query_locked(string g_name) {
  class group record;

  if(undefinedp(groups[g_name]))
    return 0;

  record = (class group)groups[g_name];
  return (record->locked);
} /* query_locked() */

/**
 * This method sets the lock flag on the team.
 * @param g_name the name of the team
 * @param lock the new lock flag
 * @see query_locked()
 */
int set_locked(string g_name, int lock) {
  class group record;

  if(undefinedp(groups[g_name]))
    return 0;

  record = (class group)groups[g_name];
  record->locked = lock;

  return 1;
} /* set_locked() */

// Admin functions for creators

/**
 * This methor returns the list all the current teams.
 * @return an array of all the current team
 */
string *list_groups() {
  string frog;

  foreach (frog in keys(groups)) {
    check_group(frog);
  }
  return keys(groups);
} /* list_groups() */

/* These methods are specificaly for sanity purposes. */
private void disband_group(string g_name, string p_name) {
  class group record;
  object person;
  object member;

  record = (class group)groups[g_name];
  // go through the team members removing them from their team and
  // removing their followers and protectors
  foreach(member in record->members) {

    if(!member) // just in case
      break;
    
    map_delete(members, member->query_name());
    
    foreach(person in member->query_protectors())
      member->remove_protector(person);

    foreach(person in member->query_followers())
      member->remove_follower(person);

    tell_object(member, p_name+" has left the game and so the "+g_name+
                " has been disbanded.\n");
  }
  map_delete(groups, g_name);
} /* disband_group() */

// This method checks the group to see if it has any bad members, ie: 0's.
private void check_group(string g_name) {
  class group record;

  record = (class group)groups[g_name];
  if (record->leader == 0) {
    /* Kill the group. */
    disband_group(g_name, "Your leader");
  } else {
    record->members -= ({ 0 });
  }
} /* check_group() */

private void tidy_members(string g_name) {
  class group record;
  object member;

  record = (class group)groups[g_name];

  foreach(member in record->members)
    if(!member)
      record->members -= ({ 0 });
}
