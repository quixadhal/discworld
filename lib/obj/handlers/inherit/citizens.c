/**
 * This handler keeps track of citizenships and magistrates.
 * @author Pinkfish 
 * @started Fri Jun 23 20:38:10 PDT 2000
 */
#include <player_handler.h>
#include <playtesters.h>
#include <player.h>

private mapping _magistrates;
private mapping _citizens;

int is_citizen_of(string area, string citizen);
void save_me();

void create() {
   _citizens = ([ ]);
   _magistrates = ([ ]);
} /* create() */

/**
 * This method creates an area for citizens to womble in.
 * @param area the name of the area to create
 * @param num_magistrates the number of magistrates
 * @return 1 on succes, 0 on failure
 */
int create_area(string area, int num_magistrates) {
   _citizens[area] = ({ });
   _magistrates[area] = allocate(num_magistrates);
   save_me();
} /* create_area() */

/**
 * This method returns all the areas for citizenship.
 * @return the possible areas for citizenship
 */
string* query_citizenship_areas() {
   return keys(_citizens);
} /* query_citizenship_areas() */

/**
 * This method adds a citizen to the specified area.
 * @param area the area to add the citizen too
 * @param citizen the citizen to add to the area
 */
int remove_citizen(string area, string citizen) {
   int pos;

   if (!_citizens[area]) {
      return 0;
   }

   if (member_array(citizen, _citizens[area]) != -1) {
      _citizens[area] -= ({ citizen });
      if (citizen) {
         pos = member_array(citizen, _magistrates[area]);
         if (pos != -1) {
            _magistrates[area][pos] = 0;
         }
      }
      save_me();
      return 1;
   }

   return 0;
} /* remove_citizen() */

/**
 * This method adds a magistrate to the area.  If there are no more
 * magistrate slots then this fails.
 * @param area the area to add the magistrate too
 * @param magistrate the magistrate to add
 */
int add_magistrate(string area, string magistrate) {
   int pos;

   if (!is_citizen_of(area, magistrate)) {
      return 0;
   }

   pos = member_array(0, _magistrates[area]);
   if (pos == -1) {
      return 0;
   }
   _magistrates[area][pos] = magistrate;
   save_me();
   PLAYER_TITLE_HANDLER->add_title_to_player(magistrate, "magistrate");
   return 1;
} /* add_magistrate() */

/**
 * This method sets up a new bunch of elected magistrates.
 * @param area the area to setup them up in
 * @param magistrates the new bunch to setup
 */
protected void setup_magistrates(string area, string* magistrates) {
   int i;
   int num;
   string magistrate;

   if (!_magistrates[area]) {
      return ;
   }

   for (i = 0; i < sizeof(_magistrates[area]); i++) {
      _magistrates[area][i] = 0;
   }

   num = sizeof(_magistrates[area]);
   if (num <= sizeof(magistrates)) {
      magistrates = magistrates[0..num];
   } else {
      magistrates += allocate(num - sizeof(magistrates));
   }
   _magistrates[area] = magistrates;
   save_me();
   foreach (magistrate in magistrates) {
      PLAYER_TITLE_HANDLER->add_title_to_player(magistrate, "magistrate");
   }
} /* setup_magistrates() */

/**
 * This method returns the number of magistrates needed for the area.
 * @param area the area to check the magistrates for
 * @return the number of allow\ed magistrates
 */
int query_number_of_magistrates(string area) {
   if (!_magistrates[area]) {
      return 0;
   }

   return sizeof(_magistrates[area]);
} /* query_number_of_magistrates() */

/**
 * This method returns the current magistrates for the area.
 * @param area the area to get the magistrates for
 * @return the array of the current magistrates
 */
string* query_magistrates(string area) {
   if (!_magistrates[area]) {
      return ({ });
   }

   return _magistrates[area] - ({ 0 });
} /* query_magistrates() */

/**
 * This method adds a citizen to the specified area.
 * @param area the area to add the citizen too
 * @param citizen the citizen to add to the area
 */
int add_citizen(string area, string citizen) {
   //string bing;

   if (!PLAYER_HANDLER->test_user(citizen)) {
      return 0;
   }

   if (!_citizens[area]) {
      return 0;
   }

/*
   foreach (bing in keys(_citizens)) {
      if (member_array(citizen, _citizens[bing]) != -1) {
         return 0;
      }
   }
 */

   _citizens[area] += ({ citizen });
   save_me();
   return 1;
} /* add_citizen() */

/**
 * This method checks to see if the specified person is a citizen or
 * not.
 * @param area the area to check them in
 * @param citizen the person to check for citizenship
 * @return 1 if they are a citizen, 0 if not
 */
int is_citizen_of(string area, string citizen) {
   if (!_citizens[area]) {
      return 0;
   }

/*
   if (PLAYTESTER_HAND->query_playtester(citizen)) {
      return 1;
   }
 */

   if (master()->query_lord(citizen)) {
      return 1;
   }

   return member_array(citizen, _citizens[area]) != -1;
} /* is_citizen_of() */

/**
 * This method checks to see if the specified person is a magistrate or
 * not.
 * @param area the area to check them in
 * @param magistrate the person to check for magistrateship
 * @return 1 if they are a magistrate, 0 if not
 */
int is_magistrate_of(string area, string magistrate) {
   if (!_magistrates[area]) {
      return 0;
   }

   if (master()->query_lord(magistrate)) {
      return 1;
   }

/*
   if (PLAYTESTER_HAND->query_playtester(magistrate)) {
      return 1;
   }
 */

   return member_array(magistrate, _magistrates[area]) != -1;
} /* is_magistrate_of() */

/**
 * This method checks to see if the specified person is a citizen and
 * where they are a citizen.
 * @param citizen the person to find where they are the citizen of
 * @return where they are a citizen, or 0 on failure
 */
string query_citizen(string citizen) {
   string bing;

   foreach (bing in query_citizenship_areas()) {
      if (is_citizen_of(bing, citizen)) {
         return bing;
      }
   }

   return 0;
} /* query_citizen() */

/**
 * This method checks to see if the person is a magistrate anywhere.
 * It will return 1 if they are a magistrate, 0 if they are not.
 * @param person the person to check
 * @return 1 if they are, 0 if not
 */
int is_magistrate_anywhere(string person) {
   string bing;

   foreach (bing in query_citizenship_areas()) {
      if (is_magistrate_of(bing, person)) {
         return 1;
      }
   }

   return 0;
} /* is_magistrate_anywhere() */

/**
 * This method returns the number of citizens in the given area.
 * @param area the area to lookup
 * @return the number of citizens
 */
int query_number_of_citizens(string area) {
   return sizeof(_citizens[area]);
} /* query_number_of_citizens() */
