/**
 * This is the group handler.  It does all the handling of
 * player-run adventuring groups, but not alone.  It's
 * accompanied by the group commands, effect and shadow.
 * For paths to these files, consult /include/group_handler.h
 *
 * This system owes its design first and foremost to the irreplaceable
 * Ceres who wrote the original system.  Next come all the valuable
 * Creators who shared their knowledge of LPC.  Last but not least
 * are the countless playtesters who dedicated their time and worked
 * out all (or most of) the kinks and provided feedback and suggestions.
 *
 * So give 'em all a big hug.
 *
 * @author Tape
 * @started February 99
 */

#include <group_handler.h>
#include <broadcaster.h>
#include <login_handler.h>

int _loaded;
int _groups_formed;

mapping _groups;

class group
{
   int start_time;               // Time() when group was created
   string short;                 // Group's short description
   string leader_name;           // Group's leader's name
   object leader;                // Group's leader
   object *members;              // Group's members
   object *invited;              // People invited to group
}


int is_group( string group );
int is_member( string name, object person );
int is_invited( string name, object person );
object *invitations_to( string name );
string query_group_short( string name );
object *members_of( string name );
object leader_of( string name );
string short_to_name( string short );
int create_group( string name );
int remove_group( string name );
int add_invite( string name, object person, int flag );
int remove_invite( string name, object person );
int add_member( string name, object person );
int remove_member( string name, object person );
varargs int set_leader( string name, object person, object appointer );
void notify_group( string name, object broadcaster, mixed message );
varargs void disband_group( string name, mixed message );
varargs object shuffle_new_leader( string group, int way, object *exclude );
void leader_goes_linkdead( string player, string event_type );
void handle_group_follow( string group, object who, object *what,
   int unfollow, int silent );
void broadcast_to_groups( string *name, string message );
string *query_groups();
int set_group_short( string, string);

/** @ignore yes */
void create()
{
   _groups = ([ ]);
   _loaded = time();
   _groups_formed = 0;
} /* create() */


/** @ignore yes */
void dest_me() {

   string name;

   broadcast_to_groups( 0, "%^BOLD%^WARNING%^RESET%^: The group handler is being destructed."
      "  All active groups will be disbanded.  It should be possible to "
      "recreate the group almost immediately afterwards.  If not, please "
      "file a bug report for the \"group\" command." );
      
   foreach( name in query_groups() ) {
      disband_group( name, 0 );
   }

} /* dest_me() */


/** @ignore yes */
void stats_please() {
   printf( "The handler was loaded on %s.  Since then, "
      "%i groups have been formed.\n", ctime( _loaded ),
      _groups_formed );
} /* stats_please() */


/**
 * This returns the names of all groups currently in the database.
 * @return the names of all groups in database
 */
string *query_groups() {
   return keys( _groups );
} /* query_groups() */


/**
 * This function checks if such a group exists at the moment.
 *
 * @param group the name of the group
 * @return 1 if the group exists, 0 otherwise
 */
int is_group( string group ) {
   return !undefinedp( _groups[ group ] );
} /* is_group() */


/**
 * This function allows you to check whether a person is a member
 * of a group.
 *
 * @param name the name of the group
 * @param person the person you want to check for membership
 *
 * @return 1 if he is a member, 0 if he isn't (or the group doesn't exist)
 *
 */
int is_member( string name, object person ) {

   if( !is_group( name ) ) {
      return 0;
   }
   
   if( member_array( person, _groups[ name ]->members ) == -1 ) {
      return 0;
   }

   return 1;

} /* is_member() */


/**
 * This function allows you to check whether an invitation for a
 * person is pending in a specific group.
 *
 * @param name the name of the group
 * @param person the person to be checked
 *
 * @return 1 if such an invitation exists, 0 if not
 */
int is_invited( string name, object person )
{
   if( member_array( person, _groups[ name ]->invited ) != -1 )
   {
      return 1;
   }
   
   return 0;
} /* is_invited() */


/**
 * This function lists all invitations to a particular group.
 * @param name the name of the group
 * @return objects who have been invited to the group
 */
object *invitations_to( string name )
{
   if( !is_group( name ) )
   {
      return 0;
   }

   return _groups[ name ]->invited;
} /* invitations_to() */


/**
 * This returns the short description of a group.
 * @param name the name of the group
 * @return the short of the group
 */
string query_group_short( string name )
{
   if( !is_group( name ) )
   {
      return 0;
   }
   return _groups[ name ]->short;
} /* query_group_short() */


/**
 * This function returns all the members of a group.
 * @param name the name of the group
 * @return an object * of the members
 */
object *members_of( string name )
{
   
   if( !is_group( name ) )
   {
      return 0;
   }
   
   return _groups[ name ]->members;
} /* members_of() */


/**
 * This function returns the person who is currently
 * leading the group, if any.
 *
 * @param name the name of the group
 * @return the object pointing to the leader, 0 if none
 */
object leader_of( string name )
{
   if( !is_group( name ) )
   {
      return 0;
   }
   return _groups[ name ]->leader;
} /* leader_of() */


/**
 * This function returns the time when the group was created.
 * @param name the name of the group
 * @return the time when the group was created
 */
int query_start_time( string name )
{
   if( !is_group( name ) )
   {
      return 0;
   }
   return _groups[ name ]->start_time;
} /* query_start_time() */


/**
 * This function concatenates the short of a group to a valid name
 * that can then be used with create_group().  The function will
 * also return invalid if a channel with such a name exists in the
 * broadcaster.
 *
 * @param short the short of the group
 * @return the name of the group, or "" if invalid
 */
string short_to_name( string short )
{
   
   string *words;
   
   short = lower_case( short );   
   words = explode( short, " " );
   words -= INVALID_WORDS;
   
   if( !sizeof( words ) ) {
      return "";
   }

   short = implode( words, " " );

   if( BROADCASTER->query_channel_members( "group_" + short ) ) {
      return "";
   }   
   
   return short;

} /* short_to_name() */

   
/**
 * This function allows you to create a new group.  Note that the
 * name of the group shouldn't be just any name.  It should first
 * be filtered through short_to_name().  The short, set with
 * set_group_short() can be set to anything.  The "name" of a
 * group is mostly used internally to query and set stuff in
 * the handler.  The short is visible to players.
 *
 * @param name the name of the new group
 * @param founder points to the thing creating the group
 *
 * @see short_to_name()
 * @see set_group_short()
 *
 * @return 1 on success, 0 if the group already exists
 */
int create_group( string name )
{
   if( is_group( name ) ) {
      return 0;
   }
   
   _groups += ([ name : new( class group ) ]);
   
   _groups[ name ]->members = ({ });
   _groups[ name ]->invited = ({ });
   _groups[ name ]->start_time = time();

   // For stats.
   _groups_formed++;
   
   return 1;
   
} /* create_group() */

/**
 * This method renames the group.
 * @param group the old group name
 * @param new_group the new group name
 */
int rename_group(string group, string new_group) {
   object member;

   if (is_group(new_group) || new_group == group) {
      return 0;
   }

   _groups[new_group] = _groups[group];
   map_delete(_groups, group);
   set_group_short(new_group, new_group);
   foreach (member in _groups[new_group]->members) {
      if (!member) {
         continue;
      }
      // Remove the old group stuff.
      BROADCASTER->remove_object_from_channel( "group_" + group, member );

      member->group_membership_removed();

      member->add_effect( EFFECT, new_group);

      // Add him to his group's channel.

      BROADCASTER->add_object_to_channel( "group_" + new_group, member );

      member->set_title( GROUP_TITLE, "a member of " +
      query_group_short( new_group ) );

   }
   notify_group( new_group, this_object(), ({ "", "The group has been renamed to " +
      new_group + "." }) );
   return 1;
}


/**
 * This sets the short description of a group.
 *
 * @param name the name of the group
 * @param short_desc the short of the group
 *
 * @return 1 on success, 0 if group doesn't exist
 */
int set_group_short( string name, string short_desc )
{
   if( !is_group( name ) ) {
      return 0;
   }
   _groups[ name ]->short = short_desc;
   return 1;
} /* set_group_short() */


/**
 * This function removes a group from the handler and is only used
 * internally.  No notification of this is given to players, and
 * no cleanup is done on their part.
 * If you want to force the deletion of a group, use disband_group()
 * instead.
 *
 * @param name name of the group to be disbanded
 * @return 1 on success, 0 if group doesn't exist
 * @see disband_group()
 */
int remove_group( string name )
{
   if( !is_group( name ) ) {
      return 0;
   }
   
   if( _groups[ name ]->leader_name ) {
      LOGIN_HANDLER->remove_login_call(
         _groups[ name ]->leader_name, "leader_goes_linkdead",
         this_object() );
   }
   
   map_delete( _groups, name );
   
   return 1;
} /* remove_group() */


/**
 * This function allows you to add a person to the invite array
 * of a group.  Only invited people are allowed to join a group.
 * If the call succeeds, an internal call_out is started which
 * runs out after INVITE_TIMEOUT seconds and removes the person
 * from the array.
 *
 * @param name the name of the group
 * @param person the person we're inviting
 * @param flag set to 1 if you don't want the auto-removal of the invite
 *
 * @return 1 on success, 0 if the group doesn't exist or the person is
 *    already invited
 *
 * @see /include/group_handler.h
 */
int add_invite( string name, object person, int flag )
{
   
   if( !is_group( name ) )
   {
      return 0;
   }
   
   if( is_member( name, person ) ) 
   {
      return 0;
   }
   
   if( member_array( person, _groups[ name ]->invited ) != -1 ) 
   {
      return 0;
   }
   
   _groups[ name ]->invited += ({ person });
      
   if( !flag ) 
   {
      call_out( (: remove_invite, name, person :), INVITE_TIMEOUT );
   }
   
   return 1;
} /* add_invite() */


/**
 * This function allows you to remove an invite of a person from
 * a group.
 *
 * @param name the name of the group
 * @param person the person you want to remove
 *
 * @return 1 on success, 0 if group doesn't exist or person hasn't been
 *    invited
 */
int remove_invite( string name, object person ) 
{

   if( !is_group( name ) ) 
   {
      return 0;
   }
   
   if( !is_invited( name, person ) ) 
   {
      return 0;
   }
   
   _groups[ name ]->invited -= ({ person, 0 });
   
   return 1;
   
} /* remove_invite() */


/**
 * This function allows you to add a member to a group.
 *
 * @param name the name of the group
 * @param person the person you're adding
 * @return 1 on success, 0 if the group doesn't exist or person is
 *    already a member
 */
int add_member( string name, object person ) 
{
   if( !is_group( name ) ) 
   {
      return 0;
   }
   
   if( is_member( name, person ) ) 
   {
      return 0;
   }
    
   _groups[ name ]->members += ({ person });
   _groups[ name ]->invited -= ({ person });
   
   // Add the group effect.
   
   person->add_effect( EFFECT, name );
   
   // Add him to his group's channel.
   
   BROADCASTER->add_object_to_channel( "group_" + name, person );
   
   notify_group( name, person, ({ "You have joined the group.",
      person->query_cap_name() + " has joined the group." }) );

   if( sizeof( _groups[ name ]->members ) > 1 )
   {
      handle_group_follow( name, person, ({ _groups[ name ]->leader }), 0, 0 );
   }

   person->set_title( GROUP_TITLE, "a member of " +
      query_group_short( name ) );
      
   return 1;
   
} /* add_member() */


/**
 * This function allows you to remove a person from a group.
 *
 * @param name the name of the group
 * @param person the person you want removed
 *
 * @return 1 on success, 0 if group doesn't exist or person isn't a
 *    member
 */
int remove_member( string name, object person )
{

   object member;

   if( !is_group( name ) )
   {
      return 0;
   }
   
   if( !is_member( name, person ) )
   {
      // Not a member.
      return 0;
   }
   
   if( person )
   {

      notify_group( name, person, ({
         "You have left the group.",
      person->query_cap_name() + " has left the group." }) );   
      
      foreach( member in person->query_assisting() )
      {

         // Stop assisting members.
         
         if( !member )
         {
            continue;
         }
         
         member->remove_assister( person );
      }

      foreach( member in person->query_assisters() )
      {
      
         // Stop being assisted.
         
         if( !member )
         {
            continue;
         }
         
         member->remove_assisting( person );
      }
      
   }

   _groups[ name ]->members -= ({ person });

   // Stop listening to the group's channel.
   
   BROADCASTER->remove_object_from_channel( "group_" + name, person );
   
   if( person )
   {
      // Delete the effect.
      person->group_membership_removed();
   }

   if( person == leader_of( name ) && sizeof( members_of( name ) ) )
   {

      // If the person was the leader, assign a new one.

      if (sizeof(members_of(name)) == 1) {
         notify_group( name, this_object(), "The current leader has left "
            "the group, you are now all alone.  Better start recruiting.");
      } else {
         notify_group( name, this_object(), "The current leader has left "
            "the group.  A new leader will be chosen randomly." );
      }

      if( !shuffle_new_leader( name, 0 ) )
      {
         // Couldn't find a suitable new leader (which really shouldn't
         // happen unless everyone is net dead, afaik).
         
         notify_group( name, this_object(), "The choosing of a new "
            "leader has failed (oh dear).  The group is hereby "
            "disbanded.\n" );

         call_out( "disband_group", 0, name );

         return 1;
      }
   }

   // Stop following everyone.
   
   handle_group_follow( name, person, _groups[ name ]->members, 1, 1 );

   foreach( member in _groups[ name ]->members ) {

      // Make sure people stop following you.

      handle_group_follow( name, member, ({ person }), 1, 1 );
   }

   // Remove the "whois" title.
   if (person && objectp(person)) { 
      person->remove_title( GROUP_TITLE );
   }
      
   // If the group's empty, remove it altogether.
   
   if( !sizeof( members_of( name ) ) ) {
      remove_group( name );
   }

   return 1;

} /* remove_member() */


/**
 * This function allows you to set a new leader for the group.
 *
 * @param name the name of the group
 * @param person the new leader
 * @param appointer if this != 0, group is told he set the new leader
 *
 * @return 1 on success, 0 if the group doesn't exist
 */
varargs int set_leader( string name, object person, object appointer )
{

   object *followers, old_leader, member;
   
   if( !is_group( name ) )
   {
      return 0;
   }
   
   if( !is_member( name, person ) )
   {
      return 0;
   }
   
   if( !person )
   {
      return 0;
   }
   
   if( _groups[ name ]->leader_name )
   {
      LOGIN_HANDLER->remove_dynamic_login_call(
         _groups[ name ]->leader_name, "leader_goes_linkdead",
         base_name( this_object() ) );
   }

   old_leader = _groups[ name ]->leader;
   
   if( old_leader )
   {
      
      // Those who were following the old leader will now follow the new
      // leader.
      
      followers = ( old_leader->query_followers() & _groups[ name ]->members );
      followers -= ({ 0 });
      followers += ({ old_leader });
      
      foreach( member in followers )
      {
         handle_group_follow( name, member, ({ old_leader }), 1, 1 );
         handle_group_follow( name, member, ({ person }), 0, 1 );
      }
      
      // Set the title back to "only" a member.
      
      old_leader->set_title( GROUP_TITLE, "a member of " +
         query_group_short( name ) );
   }
   
   _groups[ name ]->leader = person;

   if( userp( person ) ) {
      _groups[ name ]->leader_name = person->query_name();
      LOGIN_HANDLER->add_dynamic_login_call( person->query_name(),
         "leader_goes_linkdead", base_name( this_object() ) );
   }
   else
   {
      // How can a non-user become the leader?  Beats me, but I'm paranoid.
      _groups[ name ]->leader_name = 0;
   }
   
      
   if( !appointer )
   {
      notify_group( name, person, ({ "You are now the leader of "
         "the group.", person->query_cap_name() + " is now the leader "
         "of the group." }) );
   }
   else
   {
      notify_group( name, appointer, "By the power vested in " +
         appointer->query_cap_name() + ", " + person->query_cap_name() +
         " has been appointed as the new leader of the group." );
   }

   person->set_title( GROUP_TITLE, "the leader of " +
      GROUP->query_group_short( name ) );

   return 1;
} /* set_leader() */


/**
 * This function broadcasts a message to the group's channel using
 * the broadcaster handler.  The first argument specifies the
 * group's name (not short), which also acts as the channel
 * name.  The second argument is the object doing the broadcasting.
 * The third argument varies.  It can either be a simple string,
 * in which case that string is printed as the message.  It can
 * also be a two-element string array.  The first element is
 * printed only to the object specified in the second argument.
 * The second element is printed to everyone else.
 *
 * @param name the name of the group
 * @param object person or object doing the broadcasting
 * @param message the message to be broadcasted
 *
 */
void notify_group( string name, object broadcaster, mixed message ) {
   BROADCASTER->broadcast_to_channel( broadcaster, "group_" + name,
      ({ message, time() }) );
} /* notify_group() */


/**
 * This function does a clean removal of a group from the handler.
 * If a message is specified, it is broadcasted to the group
 * before all members are removed.
 *
 * @param name the name of the group to be disbanded
 * @param message message to be broadcasted, if any
 * @see remove_group()
 */
varargs void disband_group( string name, mixed message ) {

   object bugger, leader, *members;

   if( !is_group( name ) ) {
      return;
   }
   
   if( message ) {   
      notify_group( name, this_object(), message );
   }
   
   members = members_of( name );
   leader = leader_of( name );
   
   if( leader ) {
      members -= ({ leader });
   }
   
   foreach( bugger in members ) {
      remove_member( name, bugger );
   }
   
   remove_member( name, leader );
   
   remove_group( name );
   
} /* disband_group() */


/**
 * This function allows you to choose a new leader for the group
 * in a variety of ways.  It only includes players who are
 * interactive().
 *
 * @param group the name of the group
 * @param way 0 for random (no other ways present atm)
 * @param exclude an object array of members to exclude from the start
 * @return an object pointing to the new leader, or 0
 */
varargs object shuffle_new_leader( string group, int way, object *exclude ) {
      
   object leader;
   object *members;
   
   if( !is_group( group ) ) {
      return 0;
   }
   
   members = members_of( group );
   
   if( !sizeof( members ) ) {
      return 0;
   }
   
   if( exclude ) {
      members -= exclude;
   }
   
   members = filter( members, (: $1 && interactive( $1 ) :) );

   if( !sizeof( members ) ) {
      return 0;
   }

   switch( way ) {
      case 0:
         leader = members[ random( sizeof( members ) ) ];
         if( set_leader( group, leader ) ) {
            return leader;
         }
         return 0;
      default:
         return 0;
   }
   
} /* shuffle_new_leader() */


// Caution: The following function contains undocumented, convoluted code.
//          Browse at your own risk.

/** @ignore yes */
void leader_goes_linkdead( string player, string event_type ) {

   string group;
   object player_ob, *members;

   // This function only takes care of leader reassignment when
   // the current leader goes netdead.  Quitting or otherwise
   // destructing is taken care of in the shadow.  Don't ask me
   // why, I'm just lazy.

   if( event_type != NETDEATH && event_type != RECONNECT ) {
      LOGIN_HANDLER->remove_dynamic_login_call( player,
         "leader_goes_linkdead", base_name( this_object() ) );
      return;
   }
   
   if( !player_ob = find_player( player ) ) {
      LOGIN_HANDLER->remove_dynamic_login_call( player,
         "leader_goes_linkdead", base_name( this_object() ) );
      return;
   }
   
   group = player_ob->query_group();
   
   if( !group ) {
      LOGIN_HANDLER->remove_dynamic_login_call( player,
         "leader_goes_linkdead", base_name( this_object() ) );
      return;
   }
      
   if( _groups[ group ]->leader_name != player ) {
      // Well that was weird.  A login call should only be added for
      // the current leader.
      LOGIN_HANDLER->remove_dynamic_login_call( player,
         "leader_goes_linkdead", base_name( this_object() ) );
      return;
   }
   
   members = members_of( group );
   members -= ({ player_ob });
   
   if( !sizeof( members ) ) {
      // Looks like a one-man group.  Let's wait until he wakes up.
      return;
   }

   LOGIN_HANDLER->remove_dynamic_login_call( player,
      "leader_goes_linkdead", base_name( this_object() ) );

   notify_group( group, this_object(), "The current leader "
      "has gone netdead.  A new leader will be selected at random." );
   
   if( !shuffle_new_leader( group, 0, 0 ) ) {
      notify_group( group, this_object(), "No eligible leaders "
         "found.  The group is disbanded." );
      disband_group( group );
   }
   
} /* leader_goes_linkdead() */


/** @ignore */
void handle_group_follow( string group, object who, object *what,
   int unfollow, int silent ) {

   string short, mess_to_me, mess_to_others;
      
   switch( unfollow ) {
      case 0:
         what = filter( what, (: $1->add_follower( $( who ) ) :) );
         if( !sizeof( what ) ) {
            mess_to_me = "You begin following noone.";
            break;
         }
         short = query_multiple_short( what );
         mess_to_me = "You begin following " + short + ".";
         mess_to_others = who->query_cap_name() + " begins following " +
            short + ".";
         break;

      case 1:
         what = filter( what, (: $1->remove_follower( $( who ) ) :) );
         if( !sizeof( what ) ) {
            mess_to_me = "You stop following noone.";
            mess_to_others = 0;
            break;
         }
         short = query_multiple_short( what );
         mess_to_me = "You stop following " + short + ".";
         mess_to_others = who->query_cap_name() + " stops following " +
            short + ".";
         break;
      default:
         printf( "Barf.\n" );
         return;
   }

   if( !silent ) {
      notify_group( group, who, ({ mess_to_me, mess_to_others }) );
   }
   
} /* handle_group_follow() */


/**
 * With this function you can broadcaster a message to all or
 * some of the groups currently active.  It will not include
 * your name, so make sure you identify yourself if necessary.
 *
 * If "name" is empty or 0, the message will be broadcasted
 * to all groups.
 *
 * @param name a string array with names of groups
 * @message the message to be broadcasted
 */
void broadcast_to_groups( string *name, string message ) {
   
   string group;
   string *groups;
   
   if( name && sizeof( name ) ) {
      groups = name;
   }
   else {
      groups = keys( _groups );
   }
   
   foreach( group in groups ) {
      notify_group( group, this_player(), message );
   }

} /* broadcast_to_groups() */


mapping query_dynamic_auto_load() {
   return ([ "groups" : _groups,
             "groups formed" : _groups_formed ]);
}

void init_dynamic_arg(mapping map) {
   _groups = map["groups"];
   _groups_formed = map["groups formed"];
}
