/**
 * A nice clump of friends to take you out and buy you drinks.
 * @author Pinkfish
 * @started Mon Mar 12 22:21:23 PST 2001
 */
#include <player.h>
#include <player_handler.h>
#include <cmds/friends.h>

private mapping _friends;

void create() {
   _friends = ([ ]);
} /* create() */

/**
 * This method checks to see if the specified person is a friend or not.
 * @param person the person to check
 * @return 1 if they are a friend, 0 if not
 */
int is_friend(string person) {
   if (!_friends) {
     _friends = ([ ]);
   }
   return stringp(_friends[person]);
} /* is_friend() */

/**
 * This method returns the tag associated with the friend.
 * @param person the person to get the information on
 * @return the string of their friendship tag
 */
string query_friend_tag(string person) {
  if(!interactive(previous_object()))
     return _friends[person];

  if(previous_object()->query_name())
    log_file("CHEAT", "%s query_friend_tag called on %s by %s\n",
             ctime(time()), this_object()->query_name(),
             previous_object()->query_name());
  else
    log_file("CHEAT", "%s query_friend_tag called on %s by %s\n",
             ctime(time()), this_object()->query_name(),
             file_name(previous_object()));
  return "";
} /* query_friend_tag() */

/**
 * This method adds a friend to the list.
 * @param person the friend to add
 * @param tag the tag to add
 */
void add_friend(string person, string tag) {
   if (strlen(tag) > PLAYER_MAX_FRIEND_TAG_LEN) {
      return ;
   }
   if (!PLAYER_HANDLER->test_user(person)) {
      return ;
   }

   _friends[person] = tag;
} /* add_friend() */

/**
 * This method removes a friend from the list.
 * @param person the person to remove
 */
void remove_friend(string person) {
   map_delete(_friends, person);
} /* remove_friend() */

/**
 * This method returns the current bunch of friends for this player.
 */
string* query_friends() {
   if (file_name(previous_object()) == FRIENDS_CMD) {
      return keys(_friends);
   }
   if(previous_object()->query_name())
     log_file("CHEAT", "%s query_friends called on %s by %s\n",
              ctime(time()), this_object()->query_name(),
              previous_object()->query_name());
   else
     log_file("CHEAT", "%s query_friends called on %s by %s\n",
              ctime(time()), this_object()->query_name(),
              file_name(previous_object()));
     
   return ({ });
} /* query_friends() */
