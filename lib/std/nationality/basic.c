/**
 * The basic nationality inherit.  The nationality keeps track of any
 * specific information about the nationality.
 * @author Pinkfish
 * @started Tue Jun  4 15:08:33 PDT 2002
 */

inherit "/std/basic/setup";

class region {
   string description;
   string start_location;
   string accent_ob;
}

private string _name;
private string _information;
private string _same_nat_description;
private string _different_nat_description;
private string _start_loc;
private string _language;
private string _accent;
private string _currency;

private mapping _regions;

void create() {
   _regions = ([ ]);
   // This is mostly just in case we need it in the future.
   do_setup();
}

/**
 * This method adds a region to the nationality.
 * @param name the name of the region
 * @param description the description of the region
 * @param start the start location of the region
 * @param accent the accent object for this region
 */
void add_region(string name, string description, string start, string accent) {
   class region bing;

   bing = new(class region,
              description : description,
              start_location : start,
              accent_ob : accent);
   _regions[name] = bing;
}

/**
 * This method sets the name of the nationality.
 * @param name the name of the nationality
 */
void set_name(string name) {
   _name = name;
}

/**
 * This method returns the name of the nationality.
 * @return the name of the nationality
 */
string query_name() {
   return _name;
}

/**
 * This method sets the description of the nationality when viewed
 * by someone who is also the same nationality.
 * @param description the description of the nationality
 */
void set_same_nationality_description(string description) {
   _same_nat_description = description;
}

/**
 * This method returns the description of the nationality when viewed
 * by someone who is also the same nationality.
 * @return the description of the nationality
 */
string query_same_nationality_description() {
   return _same_nat_description;

}
/**
 * This method sets the description of the nationality when viewed
 * by someone who is of a different nationality.
 * @param description the description of the nationality
 */
void set_different_nationality_description(string description) {
   _different_nat_description = description;
}

/**
 * This method returns the description of the nationality when viewed
 * by someone who is of a different nationality.
 * @return the description of the nationality
 */
string query_different_nationality_description() {
   return _different_nat_description;
}

/**
 * This method returns the description shown to the person looking
 * at the character.  This is currently disabled.
 * @param looker the person doing the looking
 * @param person the person being looked at
 * @return a nice description
 */
string query_look_description(object person, object looker) {
   return "";
   if (person->query_nationality() == looker->query_nationality()) {
      return capitalize(person->query_pronoun()) + " " +
             _same_nat_description;
   } else {
      return capitalize(person->query_pronoun()) + " " +
             _different_nat_description;
   }
}

/**
 * This method sets the start location of the nationality.
 * @param start_loc the start location of the nationality
 */
void set_default_start_location(string start_loc) {
   _start_loc = start_loc;
}

/**
 * This method returns the start location of the nationality.
 * @return the start location of the nationality
 */
string query_default_start_location() {
   return _start_loc;
}

/**
 * This method sets the language of the nationality.
 * @param language the language of the nationality
 */
void set_language(string language) {
   _language = language;
}

/**
 * This method returns the language of the nationality.
 * @return the language of the nationality
 */
string query_language() {
   return _language;
}

/**
 * This is the informaton the player will see in the start room.  Should
 * be full of happy stuff to make people want to be from the this place.
 * @param info the information to use
 */
void set_information(string info) {
   _information = info;
}

/**
 * This method returns the information the player will see in the start
 * room.
 * @return the information about the nationality
 */
string query_information() {
	if (!_information) {
      _information = "The $C$" + query_name() + " nationality is yet to "
                     "be described.\n";
   }
   return _information;
}

/**
 * This method returns the names of all the regions in nationality.
 * @return the names of the regions
 */
string* query_regions() {
   return keys(_regions);
}

/**
 * This method returns the description for the specific regions.
 * @param region the region
 * @return the description
 */
string query_region_description(string region) {
   return _regions[region]->description;
}

/**
 * This method returns the start location for the specific regions.
 * @param region the region
 * @return the start location
 */
string query_region_start_location(string region) {
   return _regions[region]->start_location;
}

/**
 * This method returns the start location, if the region does not have one
 * set it uses the default one.
 * @param region the region to look at first
 */
string query_start_location(string region) {
   if (_regions[region] && _regions[region]->start_location) {
      return _regions[region]->start_location;
   }
   return _start_loc;
}

/**
 * This method returns the accent object for the specific region.
 * @param region the region to find the accent for
 * @return the accent object
 */
string query_region_accent_ob(string region) {
   if (_regions[region]) {
      return _regions[region]->accent_ob;
   }
   return 0;
}

/**
 * This method sets the currency area to use for the nationality.
 * @param currency the currency to use
 */
void set_currency_area(string area) {
   _currency = area;
}

/**
 * This method returns the current area to use for the nationality.
 * @return the currency area
 */
string query_currency_area() {
   return _currency;
}
