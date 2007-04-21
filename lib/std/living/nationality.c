/**
 * The details for handling nationality.
 * @author Pinkfish
 * @started Wed Jun 26 13:22:26 PDT 2002
 */
#include <living.h>
#include <config.h>

private class living_nationality _nationality_data;

/**
 * This method returns the nationality of the living.
 * @return the nationality of the living object
 */
string query_nationality() {
   if (_nationality_data && _nationality_data->nationality) {
      return _nationality_data->nationality;
   }
   return 0;
}

/**
 * This method returns the nationality region of the living.
 * @return the nationality region of the living object
 */
string query_nationality_region() {
   if (_nationality_data && _nationality_data->region) {
      return _nationality_data->region;
   }
   return 0;
}

/**
 * This method returns the nationality data of the living.  The
 * data is any free form data needed by the nationality.
 * @return the nationality data of the living object
 */
mixed query_nationality_data() {
   if (_nationality_data && _nationality_data->data) {
      return _nationality_data->data;
   }
   return 0;
}

/**
 * This method sets the nationality of the living.
 * @param nationaltiy the new nationality
 *
 * @see setup_nationality
 */
void set_nationality(string nationality) {
   if (!_nationality_data) {
      _nationality_data = new(class living_nationality);
   }
   _nationality_data->nationality = nationality;
}

/**
 * This method sets the nationality region of the person.
 * @param region the new region of the person
 *
 * @see setup_nationality
 */
void set_nationality_region(string region) {
   if (!_nationality_data) {
      _nationality_data = new(class living_nationality);
   }
   _nationality_data->region = region;
}

/**
 * This method sets the nationality data of the person.  The
 * data is any extre free form data needed by the nationality.
 *
 * @param data the new data for the living
 */
void set_nationality_data(mixed data) {
   if (!_nationality_data) {
      _nationality_data = new(class living_nationality);
   }
   _nationality_data->data = data;
}

/**
 * This method returns the name of the nationality.
 *
 * @return the name of the nationality
 */
string query_nationality_name() {
   if (_nationality_data && _nationality_data->nationality) {
      return _nationality_data->nationality->query_name();
   }
   return 0;
}

/**
 * This method returns the description of the nationality.
 * @param looker the person looking at the them
 * @return the name of the nationality
 */
string query_nationality_description(object looker) {
   if (_nationality_data && _nationality_data->nationality) {
      return _nationality_data->nationality->query_look_description(this_object(), looker);
   }
   return 0;
}

/**
 * This method returns the accent to use for this nationality/region
 * setup on the player.
 * @return the accent object to use
 */
string query_nationality_accent_ob() {
   if (_nationality_data && _nationality_data->nationality) {
      return _nationality_data->nationality->query_region_accent_ob(_nationality_data->region);
   }
}

/**
 * This method returns the start location for this player/npc.
 * @return the start location
 */
string query_nationality_start_location() {
   if (_nationality_data && _nationality_data->nationality) {
      return _nationality_data->nationality->query_start_location(_nationality_data->region);
   }
   return CONFIG_START_LOCATION;
}
