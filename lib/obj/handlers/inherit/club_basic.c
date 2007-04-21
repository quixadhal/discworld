/**
 * This is the handler for all things clubby, a club being a group of
 * players.  Each club must have a unique name.  It also handles elections
 * for various club positions.
 * @author Pinkfish
 * @started Sun Sep 27 03:35:42 EDT 1998
 */
#include <clubs.h>
#include <broadcaster.h>
#include <player_handler.h>

/**
 * The basic club information class.
 * @member actual_name the actual name of the club
 * @member founder the founder of the club
 * @member recruiters the people who can recruit for the club
 * @member members the members of the club
 * @member type the type of the club
 * @member accounts the accounts in the club
 * @member last_paid when the balance was last paid
 * @member last_touched when the club was last touched
 * @member extra_data specific data for each type of club
 * @member region the region the club is in
 */
class club_info {
   string actual_name;
   string founder;
   string *recruiters;
   string *members;
   int type;
   mapping accounts;
   int last_paid;
   /**
    * This keeps track of when the club was last fiddled with.
    * It will be used to check the timeout stuff.
    */
   int last_touched;
   mixed extra_data;
   // Description of the club for others to enjoy.
   string description;
   string region;
}

#define CLUB_CACHE_SIZE 20

// The saved stuff...
private mapping _club_names;
private string *_observers;

// The cache stuff.
private nosave int _no_cache_hits;
private nosave int _no_cache_requests;
private nosave int _no_cache_miss;
private nosave int _cache_call_out;
private nosave mapping _cache;
private nosave string *_cache_order;

#define SAVE_FILE_NAME "/save/clubs"
#define SAVE_FILE_DIR "/save/clubs/"

protected void save_club(string name);
protected void load_main();
protected void save_main();
int is_club(string club_name);
int remove_recruiter(string name, string recruiter);
int is_recruiter_of(string name, string recruiter);
int query_club_type(string name);
void check_extra_information(string club_name, string member, int login);
int is_family(string name);
int disband_club(string name);
protected void set_club_changed(string name);
protected void send_broadcast_message(string club,
                                      string message);
protected void send_observer_event(string event_name,
                                   string *args ...);

void create() {
    seteuid(master()->creator_file(file_name()));

   _club_names = ([ ]);
   _cache = ([ ]);
   _cache_order = ({ });
   _observers = ({ });

   load_main();
} /* create() */

/** @ignore yes */
string query_cap_name() {
   return "Club controller";
} /* query_cap_name() */

/**
 * This method loads the data from the disk.
 */
protected void load_main() {
   unguarded( (: restore_object(SAVE_FILE_NAME, 1) :) );
} /* load_me() */

/**
 * This method loads the data from the disk.
 */
protected void save_main() {
   unguarded( (: save_object(SAVE_FILE_NAME, 1) :) );
} /* load_me() */

/**
 * This method normalises the name for lookups so that we don't
 * get names too confused.  Thanks to Terano for this idea.
 * @param name the name to normalise
 * @return the normalised name
 */
string normalise_name(string name) {
   return replace_string(lower_case(name), " ", "_");
} /* normalise_name() */

/**
 * Make the cache to the correct size.
 */
private void fixup_cache() {
   int i;

   if (sizeof(_cache_order) > CLUB_CACHE_SIZE) {
      for (i = sizeof(_cache_order) - CLUB_CACHE_SIZE; i >= 0; i--) {
         if (_club_names[_cache_order[i]]) {
            save_club(_cache_order[i]);
         }
         map_delete(_cache, _cache_order[i]);
      }
      _cache_order = _cache_order[sizeof(_cache_order) - CLUB_CACHE_SIZE + 1..];
   }
} /* fixup_cache() */

/**
 * This method either loads the data into the cache or it
 * reads it from the cache.
 */
protected class club_info query_club_info(string name) {
   class club_info bing;

   name = normalise_name(name);

   _no_cache_requests++;
   if (_cache[name]) {
      _no_cache_hits++;
      return _cache[name];
   }
   
   if (unguarded( (: file_size(SAVE_FILE_DIR + $(name)) :)) > 0) {
      _cache[name] = unguarded( (: restore_variable(read_file(SAVE_FILE_DIR +
                                                              $(name))) :) );
      _cache_order += ({ name });
      if (intp(_cache[name]->accounts)) {
         _cache[name]->accounts = ([ CLUB_DEFAULT_ACCOUNT_NAME : _cache[name]->accounts ]);
      }
      if (sizeof(_cache[name]) == 10) {
         bing = new(class club_info);
         bing->actual_name = _cache[name]->actual_name;
         bing->founder = _cache[name]->founder;
         bing->recruiters = _cache[name]->recruiters;
         bing->members = _cache[name]->members;
         bing->type = _cache[name]->type;
         bing->accounts = _cache[name]->accounts;
         bing->last_paid = _cache[name]->last_paid;
         bing->last_touched = _cache[name]->last_touched;
         bing->extra_data = _cache[name]->extra_data;
         bing->description = _cache[name]->description;
         bing->region = "Ankh-Morpork";
         _cache[name] = bing;
         set_club_changed(name);
      }
      fixup_cache();
      return _cache[name];
   }

   _no_cache_miss++;
   return 0;
} /* query_club_info() */

/**
 * This method saves the data to the disk.
 */
protected void save_club(string name) {
   name = normalise_name(name);
   if (_cache[name]) {
      _club_names[name] = 0;
      unguarded( (: write_file(SAVE_FILE_DIR + $(name),
                               save_variable(_cache[$(name)]),
                               1) :) );
   }
} /* save_club() */

/**
 * This method checks the cache and then saves anything changed to the
 * disk...
 */
protected void save_cache() {
   string name;
   class club_info data;

   foreach (name, data in _cache) {
      if (_club_names[name]) {
         save_club(name);
      }
   }
} /* save_cache() */

/**
 * This method marks the club as being changed.
 */
protected void set_club_changed(string name) {
   name = normalise_name(name);
   if (!undefinedp(_club_names[name])) {
      if (find_call_out(_cache_call_out) == -1) {
         _cache_call_out = call_out((: save_cache :), 0);
      }
      _club_names[name] = 1;
   }
} /* set_club_changed() */

/**
 * This method adds a club to the system.
 */
private void add_club(string name,
                      class club_info data) {
   name = normalise_name(name);
   _cache[name] = data;
   _club_names[name] = 0;
   set_club_changed(name);
} /* add_club() */

/** @ignore yes */
string the_short() {
   return "Club Control";
} /* the_short() */

protected void create_extra_data(string name) {
   class club_info data;

   data = query_club_info(name);
   data->extra_data = 0;
   set_club_changed(name);
} /* create_extra_data() */

/**
 * This method creates a club.  The founder and the recruiter set is
 * initialy set to the founder.
 * @param name the name of the club
 * @param founder the founder of the club
 * @param region the region of the club
 * @return 1 was able to create the club, 0 if unable to create the club
 * @see disband_club()
 * @see change_club_type()
 */
int create_club(string name, string founder, int type, string region) {
   class club_info info;

   if (!stringp(name) || !stringp(founder)) {
      return 0;
   }

   info = new(class club_info);
   info->actual_name = name;
   if (type != CLUB_FAMILY) {
      info->recruiters = ({ founder });
      info->members = ({ founder });
   } else {
      info->recruiters = ({ });
      info->members = ({ });
   }
   info->founder = founder;
   info->last_touched = time();
   info->type = type;
   info->last_paid = time();
   info->description = 0;
   info->accounts = ([ CLUB_DEFAULT_ACCOUNT_NAME : 0 ]);
   info->region = region;
   add_club(name, info);
   create_extra_data(name);
   set_club_changed(name);
   add_club(name, info);
   save_main();
   return 1;
} /* create_club() */

/**
 * This method changes the type of the club.
 * @param name the name of the club to change
 * @param type the new type of the club
 * @return 1 on success, 0 on failure
 * @see create_club()
 * @see disband_club()
 * @see query_club_type()
 */
int change_club_type(string name, 
                     int type) {
   int club_type;
   class club_info info;

   if (is_club(name)) {
      club_type = query_club_type(name);
      info = query_club_info(name);
      if (club_type != type) {
         info->type = (club_type & CLUB_FLAGS_MASK) | type;
         set_club_changed(name);
         create_extra_data(name);
         return 1;
      }
   }
   return 0;
} /* change_club_type() */

/**
 * This method returns the club type of the club.
 * @param name the name of the club to get the type of
 * @return the type of the club
 * @see change_club_type()
 * @see create_club()
 */
int query_club_type(string name) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      if (!data) {
         disband_club(name);
      } else {
         return data->type & CLUB_TYPE_MASK;
      }
   }
} /* query_club_type() */

/**
 * This method makes a clubs membership secret.
 * @param name the name of the club to make secret
 * @return 1 on success, 0 on failure
 * @see query_club_secret()
 * @see reset_club_secret()
 */
int set_club_secret(string name) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      data->type |= CLUB_SECRET_FLAG;
      set_club_changed(name);
      return 1;
   }
   return 0;
} /* set_club_secret() */

/**
 * This method makes a clubs membership open.
 * @param name the name of the club to make open
 * @return 1 on success, 0 on failure
 * @see query_club_secret()
 * @see set_club_secret()
 */
int reset_club_secret(string name) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      data->type &= ~CLUB_SECRET_FLAG;
      set_club_changed(name);
      return 1;
   }
   return 0;
} /* reset_club_secret() */

/**
 * This method checks to see if the club is secret or not.
 * @param name the name of the club to check for secrecy
 * @return 1 if the club is secret, 0 if not
 * @see set_club_secret()
 */
int query_club_secret(string name) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      return (data->type & CLUB_SECRET_FLAG) != 0;
   }
} /* query_club_secret() */

/**
 * This method returns the region of the club.
 * @param name the name of the club to check
 * @return the club name
 */
string query_club_region(string name) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      return data->region;
   }
   return 0;
} /* query_club_region() */

/**
 * This method disbands the club.  The club will be totaly zapped and
 * everything about it efficently munched.
 * @param name the name of the club to disband
 * @return 1 on success, 0 on failure
 * @see create_club()
 * @see check_extra_infromation()
 */
int disband_club(string name) {
   class club_info data;

   name = normalise_name(name);
   if (is_club(name)) {
      data = query_club_info(name);
      if (data) {
         log_file("CLUB", ctime(time()) + ": disbanded '" + 
                       this_object()->query_club_name(name) + "'; balance = " +
                       this_object()->query_balance(name, CLUB_DEFAULT_ACCOUNT_NAME) + "; fees due = " +
                       ctime(this_object()->query_time_fees_due(name)) +
                       "\n");
      } else {
         log_file("CLUB", ctime(time()) + " disbanded '" + name + "' "
                          "which has a bad data file.\n");
      }
      map_delete(_club_names, name);
      map_delete(_cache, name);
      unguarded( (: rm(SAVE_FILE_DIR + $(name)) :) );
      save_main();
      send_observer_event("club_event_disband_club", name);
      return 1;
   }
   return 0;
} /* disband_club() */

/**
 * This method returns the names of all the clubs currently in the list
 * of clubs.
 * @return the list of current clubs
 * @see create_club()
 * @see disband_club()
 */
string *query_clubs() {
   return keys(_club_names);
} /* query_clubs() */

/**
 * This method returns the insignia object associated with the club.
 * @param name the name of the club for the insignia object
 * @return the path of the club insignia object
 * @see create_club()
 */
string query_insignia_path(string name) {
   return "/obj/misc/club_badge";
} /* query_insignia_path() */

/**
 * This method touches the club and resets the timeout date.  This should
 * be done now and then by the club to make sure it does not timeout.
 * @param name the name of the club to reset the timeout for
 * @see check_clubs()
 */
void touch_club(string name) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      data->last_touched = time();
      set_club_changed(name);
   }
} /* touch_club() */

/**
 * This method returns the recruiters of the club.
 * @param name the club name to get the recruiters of
 * @return the recruiters of the club
 * @see add_recruiter()
 * @see remove_recruiter()
 */
string *query_recruiters(string name) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      return data->recruiters;
   }
   return ({ });
} /* query_recruiters() */

/**
 * This method returns the founder of the club.
 * @param name the club name to get the founder of
 * @return the founder of the club
 * @see create_club()
 */
string query_founder(string name) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      return data->founder;
   }
   return 0;
} /* query_founder() */

/**
 * This method returns the members of the club.
 * @param name the members of the club
 * @return the members of the club
 * @see add_member()
 * @see remove_member()
 */
string *query_members(string name) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      return data->members;
   }
   return ({ });
} /* query_members() */

/**
 * This method adds a recruiter to the club.  A recruiter can only be added if
 * they are already a member.
 * @param name the club name to add the recruiter to
 * @param recruiter the recruiter of the club to add
 * @return 1 on success, 0 on failure
 * @see remove_recruiter()
 * @see query_recruiters()
 * @see add_member()
 */
int add_recruiter(string name, string recruiter) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      if (member_array(recruiter, data->members) != -1 &&
          member_array(recruiter, data->recruiters) == -1) {
         data->recruiters += ({ recruiter });
         set_club_changed(name);
         touch_club(name);
         if (!is_family(name)) {
            send_broadcast_message(name,
                                   capitalize(recruiter) +
                                   " becomes a recruiter for the club.");
         }
         return 1;
      }
   }
   return 0;
} /* add_recruiter() */

/**
 * This method adds a member to the club.
 * @param name the name of the club to add the recruiter to
 * @param member the member of the club to add
 * @return 1 on success, 0 on failure
 * @see add_recruiter()
 * @see query_recruiters()
 * @see query_members()
 * @see remove_member()
 */
int add_member(string name, string member) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      if (member_array(member, data->members) == -1) {
         data->members += ({ member });
         set_club_changed(name);
         touch_club(name);
         if (!is_family(name)) {
            send_broadcast_message(name,
                                   capitalize(member) + " joins the club.");
         }
         return 1;
      }
   }
   return 0;
} /* add_member() */

/**
 * This method removes a member from the club.
 * @param name the name of the club to remove a member from
 * @param member the members name to remove
 * @return 1 on success, 0 on failure
 * @see query_members()
 * @see add_member()
 * @see remove_member()
 */
int remove_member(string name, string member) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      if (member_array(member, data->members) != -1) {
         if (is_recruiter_of(name, member)) {
            remove_recruiter(name, member);
         }
         data->members -= ({ member });
         set_club_changed(name);
         check_extra_information(name, member, 0);
         if (!is_family(name)) {
            send_broadcast_message(name,
                                   capitalize(member) + " leaves the club.");
         }
         send_observer_event("club_event_remove_member", name, member);
         return 1;
      }
   }
   return 0;
} /* remove_member() */

/**
 * This method removes a recruiter from the club.
 * @param name the name of the club to remove the member from
 * @param recruiter the recruiter to remove
 * @return 1 on success, 0 on failure
 * @see add_recruiter()
 * @see query_recruiters()
 * @see add_member()
 */
int remove_recruiter(string name, string recruiter) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      if (member_array(recruiter, data->recruiters) != -1) {
         data->recruiters -= ({ recruiter });
         set_club_changed(name);
         if (!is_family(name)) {
            send_broadcast_message(name,
                                   capitalize(recruiter) +
                                   " stops being a recruiter for the club.");
         }
         return 1;
      }
   }
   return 0;
} /* remove_recruiter() */

/**
 * This method returns the capitalised and un messed name of the club.
 * @param club_name the name of the club
 * @return the un messed name of the club
 * @see is_club()
 */
string query_club_name(string club_name) {
   class club_info data;

   if (is_club(club_name)) {
      data = query_club_info(club_name);
      if (!data) {
         return club_name;
      }
      return data->actual_name;
   }
   return 0;
} /* query_club_name() */

/**
 * This method returns the description of the club.
 * @param club_name the name of the club to get the description of
 * @return the club description, 0 if the club is not found
 * @see query_club_name()
 * @see create_club()
 * @see set_club_description()
 */
string query_club_description(string club_name) {
   class club_info data;

   if (is_club(club_name)) {
      data = query_club_info(club_name);
      return data->description;
   }
   return 0;
} /* query_club_description() */

/**
 * This method sets the description of the club.
 * @param club_name the name of the club to set the description of
 * @param description the new description of the club
 * @return 1 on success, 0 on failure
 * @see query_club_description()
 * @see create_club()
 */
int set_club_description(string club_name, string description) {
   class club_info data;

   if (is_club(club_name)) {
      data = query_club_info(club_name);
      data->description = description;
      set_club_changed(club_name);
      return 1;
   }
   return 0;
} /* set_club_description() */

/**
 * This method returns the time at which the club dues are again due.
 * @param club_name the name of the club to get the date for
 * @see check_clubs()
 * @see query_club_cost_per_period()
 */
int query_time_fees_due(string club_name) {
   class club_info data;

   if (is_club(club_name)) {
      data = query_club_info(club_name);
      return data->last_paid + CLUB_PAY_PERIOD;
   }
   return 0;
} /* query_time_fees_due() */

/**
 * This method determines how much the club will cost to run each
 * pay period.
 * @param club_name the name of the club to get the fees for
 * @return the amount the club will cost in the next pay period
 * @see query_time_fees_due()
 */
int query_club_cost_per_period(string club_name) {
   if (is_club(club_name)) {
      return CLUB_COST_PER_YEAR +
           sizeof(query_members(club_name)) * CLUB_COST_PER_MEMBER_PER_YEAR;
   }
   return 0;
} /* query_club_cost_per_period() */

/**
 * This method checks to see if the specified club exists.
 * @param name the name of the club to check for existance
 * @return 1 if it is a club, 0 if not
 * @see query_club_name()
 */
int is_club(string name) {
   name = normalise_name(name);
   if (!undefinedp(_club_names[name])) {
      return 1;
   }
   return 0;
} /* is_club() */

/**
 * This method checks to see if the specified club exists and is an
 * elected club.
 * @param name the name of the club to check to see for an elected type
 * @return 1 if the club is an elected type
 */
int is_elected_club(string name) {
   name = normalise_name(name);
   if (is_club(name)) {
      return query_club_type(name) == CLUB_ELECTED;
   }
   return 0;
} /* is_elected_club() */

/**
 * This method checks to see if the specified club exists and is an
 * personal club.
 * @param name the name of the club to check to see for an personal type
 * @return 1 if the club is an personal type
 */
int is_personal_club(string name) {
   if (is_club(name)) {
      return query_club_type(name) == CLUB_PERSONAL;
   }
   return 0;
} /* is_personal_club() */

/**
 * This method checks to see if the club type is actually a family.
 * @param name the name of the family to check
 * @return 1 if the club is a family
 */
int is_family(string name) {
   if (is_club(name)) {
      return query_club_type(name) == CLUB_FAMILY;
   }
   return 0;
} /* is_family() */

/**
 * This method will determine if the specified person is a member of the
 * club.
 * @param name the name of the club to find the member of
 * @param member the member to check for the existance of
 * @return 1 if they are a member, 0 if they are not
 */
int is_member_of(string name, string member) {
   if (is_club(name)) { 
      return member_array(member, query_members(name)) != -1;
   }
   return 0;
} /* is_member_of() */

/**
 * This method will determine if the specified person is a recruiter of the
 * club.
 * @param name the name of the club to find the recruiter of
 * @param recruiter the person is check for the recruiter
 * @return 1 if they are a recruiter, 0 if they are not
 * @see add_recruiter()
 * @see remove_recruiter()
 */
int is_recruiter_of(string name, string recruiter) {
   if (is_club(name)) {
      return member_array(recruiter, query_recruiters(name)) != -1;
   }
   return 0;
} /* is_recruiter_of() */

/**
 * This method will determine if the specified person is the founder of
 * the club.
 * @param name the name of the club to check the founder of
 * @param founder the person to check for being the founder
 * @return 1 if they are in the position, 0 if not
 * @see create_club()
 */
int is_founder_of(string name, string founder) {
   name = normalise_name(name);
   if (is_club(name)) {
      return query_founder(name) == founder;
   }
   return 0;
} /* is_founder_of() */

/**
 * This method creates an account in the club.
 * @param name the name of the club
 * @param account the name of the account
 */
int create_account(string name,
                   string account) {
   class club_info data;

   if (!account) {
      account = CLUB_DEFAULT_ACCOUNT_NAME;
   }
   if (is_club(name)) {
      data = query_club_info(name);
      if (undefinedp(data->accounts[account])) {
         data->accounts[account] = 0;
         touch_club(name);
         set_club_changed(name);
         return 1;
      }
   }
   return 0;
} /* create_account() */

/**
 * This method will pay a certain amount of money to club.  This will be
 * how long the club is payed until.  The club will cost a certain
 * amount for each member as well as a base cost.
 * @param name the name of the club
 * @param amount the amount to change the balance by
 * @param type the tyope of the transaction
 * @param person the person removeing the money
 * @param account the account the money is coming from
 * @return the amount of money not able to be placed in the account
 * @see remove_money()
 * @see query_balance()
 * @see query_transactions()
 */
int add_money(string name,
              int amount,
              int type,
              string person,
              string account) {
   class club_info data;

   if (!account) {
      account = CLUB_DEFAULT_ACCOUNT_NAME;
   }
   if (is_club(name) && amount > 0) {
      data = query_club_info(name);
      if (!undefinedp(data->accounts[account])) {
         data->accounts[account] += amount;
         touch_club(name);
         set_club_changed(name);
         return 1;
      }
   }
   return 0;
} /* add_money() */

/**
 * This method removes money from the account.
 * @param name the name of the club
 * @param amount the amount to change the balance by
 * @param type the tyope of the transaction
 * @param person the person removeing the money
 * @param account the account the money is coming from
 * @return 1 if the removal is a success
 * @see pay_money()
 * @see query_balance()
 * @see query_transactions()
 */
int remove_money(string name,
                 int amount,
                 int type,
                 string person,
                 string account) {
   class club_info data;

   if (!account) {
      account = CLUB_DEFAULT_ACCOUNT_NAME;
   }
   if (is_club(name) && amount > 0) {
      data = query_club_info(name);
      if (!undefinedp(data->accounts[account])) {
         data->accounts[account] -= amount;
         touch_club(name);
         set_club_changed(name);
         return 1;
      }
   }
   return 0;
} /* remove_money() */

/**
 * This method returns the balance of the club.
 * @param name the name of the club
 * @param account the name of the account
 * @return the current balance of the club
 * @see pay_money()
 * @see remove_money()
 */
int query_balance(string name,
                  string account) {
   class club_info data;

   if (!account) {
      account = CLUB_DEFAULT_ACCOUNT_NAME;
   }
   if (is_club(name)) {
      data = query_club_info(name);
      return data->accounts[account];
   }
   return 0;
} /* query_balance() */

/**
 * This method returns the names of all the accounts in the club.
 * @param club_name the name of the club
 * @return the names of all the accounts
 */
string* query_account_names(string name) {
   class club_info data;

   if (is_club(name)) {
      data = query_club_info(name);
      if (data) {
         return keys(data->accounts);
      }
   }
   return ({ });
} /* query_account_names() */

/**
 * This method checks to see if the account exists for the club.
 * @param club_name the name of the name
 * @param account the name of the account to checlk
 * @return 1 if it exists, 0 if it does not
 */
int is_account_of(string club_name, string account) {
   return member_array(account, query_account_names(club_name)) != -1;
} /* is_account_of() */

/**
 * This method determines if the club is a creator club or not.  A
 * club is considered a creator club if the founder is a creator.
 * @param club_name
 * @return 1 if is a creator club, 0 if not
 */
int is_creator_club(string club_name) {
   if (is_club(club_name)) {
      if (PLAYER_HANDLER->test_creator(query_founder(club_name))) {
         return 1;
      }
   }
   return 0;
} /* is_creator_club() */

/**
 * This method checks to see if the specified thingy is an observer.
 * @param obs the observer to check
 * @return 1 on success, 0 on failure
 */
int is_observer(string obs) {
   if (member_array(obs, _observers) != -1) {
      return 1;
   }
   return 0;
} /* is_observer() */

/**
 * Adds an objec to the list to be informed of changes about the
 * clubs.
 * @param obs the name of the object to inform of changes
 * @return 1 on success, 0 on failure
 */
int add_observer(string obs) {
   if (!is_observer(obs) &&
       file_size(obs) > 0) {
      _observers += ({ obs });
      save_main();
      return 1;
   }
   return 0;
} /* add_observer() */

/**
 * This method removes an observer.
 * @param obs the obeserver to remove
 * @return 1 on success, 0 on failure
 */
int remove_observer(string obs) {
   if (is_observer(obs)) {
      _observers -= ({ obs });
      save_main();
      return 1;
   }
   return 0;
} /* remove_observer() */

/**
 * This method returns the current list of observers.
 * @return the current list of observers
 */
string *query_observers() {
   return _observers;
} /* query_observers() */

/**
 * This method calls a function on all the observers to tell them
 * when an event has taken place.
 * @param event_name the name of the event
 * @param args the arguments to the event
 */
protected void send_observer_event(string event_name,
                                   string *args ...) {
   string bing;

   foreach (bing in _observers) {
      if (file_size(bing) > 0) {
         call_out((: call_other($1, $2, $3 ...) :), 
                  0, 
                  event_name, 
                  bing, 
                  args ...); 
      } else {
         remove_observer(bing);
      }
   }
} /* send_observer_event() */

/**
 * This method sends a broadcast to the clubs talker channel.
 * @param club the name of the club to send the message to
 * @param mess the message to send
 */
protected void send_broadcast_message(string club,
                                      string message) {
   BROADCASTER->broadcast_to_channel(this_object(),
                                     lower_case(query_club_name(club)),
                                     ({ message, 0 }));
} /* send_club_message() */

/**
 * This method returns all the stats of the object.  Things about cache
 * hits and stuff.
 * @ignore yes
 */
mixed *stats() {
   return ({
              ({ "cache hits", _no_cache_hits }),
              ({ "cache requests", _no_cache_requests }),
              ({ "cache miss", _no_cache_miss }),
              ({ "percentage", _no_cache_hits * 100 / _no_cache_requests }),
           });
} /* stats() */
