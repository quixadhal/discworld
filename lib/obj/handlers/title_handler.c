/**
 * This handler will keep track of special player titles.  It will allow
 * them to be added or removed based on specific events.  Use the
 * include <player.h> to get the path to this file.
 * @author Pinkfish
 * @started Wed Sep 27 14:00:31 PDT 2000
 */
#include <login.h>

private mapping _pending;
private mapping _titles;
private nosave string* _allowed_multiple;

#define SAVE_FILE "/save/titles"

void load_me();
void save_me();

class blob {
   string ob;
   string func;
}

void create() {
   _pending = ([ ]);
   _titles = ([ ]);
   load_me();

   //
   // List of titles you can use with others...  (Like adjectives).
   //
   _allowed_multiple = ({ "very reverend", "mostly reverend",
                          "reverend mother", "mad granny",
                          "mad nanny", "mad mr", "mad ms", "mad miss",
                          "mad mrs" });
} /* create() */

/**
 * This method checks to see if the title is an allowed multiple title.
 * @param title the title to check
 * @return 1 if it is allowed, 0 if not
 */
int is_allowed_multiple_title(string title) {
   return member_array(lower_case(title), _allowed_multiple) != -1;
} /* is_allowed_multiple_title() */

/**
 * This method returns the list of allowed multiple titles.
 * @return the list of allowed multiple titles
 */
string* query_allowed_multiple_titles() {
   return _allowed_multiple;
} /* query_allowed_multiple_titles() */

/**
 * This method adds a person to the pending list for gaining a specific
 * title.  If the person is currently on then they will get the title
 * immediately.  If they are not on then it will be added to them when they
 * next log on.
 * @param person the person to give the title too
 * @param title the title to give them
 */
void add_title_to_player(string person, string title) {
   object ob;

   ob = find_player(person);
   if (ob) {
      ob->add_player_title(title);
      return ;
   }

   if (!_pending[person]) {
      _pending[person] = ({ });
   }
   _pending[person] += ({ title });
   save_me();
} /* add_title_to_player() */

/**
 * This method returns the list of titles pending for the specific person.
 * @param person the person to get the titles for
 * @return the array of pending titles
 */
string* query_pending_titles(string person) {
   if (_pending[person]) {
      return _pending[person];
   }
   return ({ });
} /* query_pending_titles() */

/**
 * Add a method to be called to check if the person is allowed to have
 * the specified title.  This should be added to control things like
 * special titles.
 * @example
 * add_controlled_title("magistrate", NOMIC_HANDLER, "is_magistrate_somewhere");
 * @param title the title to control
 * @param ob the object to call the method on
 * @param function the function to call
 */
void add_controlled_title(string title, string ob, string func) {
   class blob fluff;

   fluff = new(class blob);
   fluff->ob = ob;
   fluff->func = func;
   _titles[title] = fluff;
   save_me();
} /* add_controlled_title() */

/**
 * This method returns the blob for the controlled title.
 * @param title the title which is controlled
 * @return the blob for the controlled title
 */
protected class blob query_controlled_title(string title) {
   return _titles[title];
} /* query_controlled_title() */

/**
 * This method checks a player when they logon for titles.
 * @param player the player to check
 */
void check_player(object player) {
   string* titles;
   string title;

   if (!player) {
      return ;
   }
   if (_pending[player->query_name()]) {
      foreach (title in _pending[player->query_name()]) {
         player->add_player_title(title);
      }
      map_delete(_pending, player->query_name());
      save_me();
   }
   titles = player->query_all_player_titles();
   foreach (title in titles) {
      if (_titles[title]) {
         if(!find_object(_titles[title]->ob) ||
            !call_other(_titles[title]->ob, _titles[title]->func,
                        player->query_name())) {
           player->remove_player_title(title);
         }
      }
   }
} /* check_player() */

/** @ignore yes */
void login_handler_call(string person, string type) {
   if (type == LOGIN) {
      call_out((: check_player(find_player($1)) :), 2, person);
   }
} /* login_handler_call() */

/** @ignore yes */
void save_me() {
   unguarded((: save_object(SAVE_FILE) :));
} /* save_me() */

/** @ignore yes */
void load_me() {
   unguarded((: restore_object(SAVE_FILE, 1) :));
} /* load_me() */
