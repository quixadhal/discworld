/**
 * A handler to track player houses.  This handler should only be used for
 * general houses that are available for sale or rent. Don't add club or
 * special (eg. guildmaster) houses here or they'll get sold or rented :)
 *
 * The handler stores information about registered player houses, makes houses
 * vacant if their owners are idle and charges rent on rented houses.
 *
 * @author Ceres
 */

#include <housing.h>
#include <login_handler.h>
#include <am_time.h>
#include <nomic_system.h>

#define BANK "/obj/handlers/bank_handler"
#define DEFAULT_BANK "Bing's First"
#define DEFAULT_PLACE "Ankh-Morpork"
#define MAIL_HANDLER "/obj/handlers/mailer"

#define SAVE_FILE "/save/player_housing/housing"

/**
 * This class stores all the data about a given house. The class is used
 * within the houses mapping. The key of the mapping is the basename of the
 * house (the filename of the main room) and the value is this class, which
 * explains why this class doesn't have a name or filename field for the house.
 *
 * @field other_rooms a string array of the other rooms in the house.
 * @field owner the name of the current owner. This may also be 'For Sale' or
 * 'Under Offer' while the house is vacant or being auctioned.
 * @field region The region the house is in eg. 'Ankh-Morpork'
 * @field type The type of house. 1 for rented, 0 for owned.
 * @field value The current value of the house. For rented houses this is the
 * monthly rent, for owned houses this is the purchase price.
 *
 */
class housing {
  string *other_rooms;
  string owner;
  string address;
  string region;
  int type;
  int value;
  string bank;
}

void load_file();
string query_housing_area_bank(string area);

private nosave string *_tmp_houses;
private nosave string *_tmp_rentals;

private mapping houses;
private int last_paid;
// This is a mapping to setup a link between housing and a specific citizenship
// area.
private mapping _housing_area;
private mapping _area_money;

/** @ignore yes */
void create() {
  houses = ([ ]);
  _housing_area = ([ ]);
  _area_money = ([ ]);
  seteuid("Root");
  load_file();
  if (!_housing_area) {
    _housing_area = ([ ]);
  }
  if (!_area_money) {
    _area_money = ([ ]);
  }
  call_out("check_owners", 900 + random(1800), 0);
  call_out("check_rent", 900 + random(1800));
}

/*
 * Utility functions
 */
/** @ignore yes */
void save_file() {
  unguarded((: cp, SAVE_FILE + ".o", SAVE_FILE + ".o.bak" :));
  unguarded((: save_object, SAVE_FILE :));
}

/** @ignore yes */
void load_file() {
  if (file_size(SAVE_FILE+".o") > 0)
    unguarded((: restore_object, SAVE_FILE :));
}

#ifdef 0
int change() {
  class housing nhouse;
  string house;

  foreach(house in keys(houses)) {
    write(sizeof(houses[house]) + "\n");
    if(sizeof(houses[house]) == 6) {
      nhouse = new(class housing,
                   other_rooms: houses[house]->other_rooms,
                   owner: houses[house]->owner,
                   address: houses[house]->address,
                   region: houses[house]->region,
                   type: houses[house]->type,
                   bank: houses[house]->region
               );
      houses[house] = nhouse;
    }
  }
  save_file();
  return 1;
}
#endif

/**
 * Add a new house to the handler.
 *
 * @param house the basename of the house
 * @param other_rooms an array of other rooms in the house
 * @param address a string for the address
 * @param region the region (used for house sales)
 * @param type an integer. 1 for owned, 0 for rented.
 * @return 1 for success, 0 for failure
 */
int add_house(string house, string *other_rooms, string address,
              string region, int type) {
  if(!houses)
    houses = ([ ]);

  if(houses[house])
    return 0;

  houses[house] = new(class housing,
                      other_rooms: uniq_array(other_rooms),
                      address: address,
                      region: region,
                      type: type,
                      bank: query_housing_area_bank(region)
                  );
  save_file();
  return 1;
}

/**
 * Modify a houses information.
 *
 * @param house the basename of the house
 * @param other_rooms an array of other rooms in the house
 * @param address a string for the address
 * @param region the region (used for house sales)
 * @param type an integer. 1 for owned, 0 for rented.
 * @return 1 for success, 0 for failure
 */
int modify_house(string house, string *other_rooms, string address,
              string region, int type) {
  if(!houses)
    houses = ([ ]);

  if(!houses[house])
    return 0;

  houses[house] = new(class housing,
                      other_rooms: uniq_array(other_rooms),
                      address: address,
                      region: region,
                      type: type,
                      bank: query_housing_area_bank(region));
  save_file();
  return 1;
}

/**
 * Rename a house. This will remove the "main" room and replace it with
 * another.
 * @param house the current basename of the house
 * @param newhouse the new basename.
 */
int rename_house(string house, string newhouse) {
  if(!houses[house])
    return 0;
  if(houses[newhouse])
    return 0;
  houses[newhouse] = houses[house];
  if(member_array(newhouse, houses[newhouse]->other_rooms) != -1)
    houses[newhouse]->other_rooms -= ({ newhouse });
  map_delete(houses, house);
  save_file();
  return 1;
}

/**
 * Remove a house from the list of houses
 *
 * @param house the basename of the house
 * @return 1 for success 0 for failure
 */
int remove_house( string house ) {
  if(!houses[house])
    return 0;
  
  map_delete( houses, house );
  return 1;
}

/**
 * Add secondary rooms to a house.
 *
 * @param house the basename of the house
 * @param an array of basenames for the rooms
 * @return 1 for success 0 for failure
 */
int add_rooms(string house, string *rooms) {
  if(!houses[house])
    return 0;

  houses[house]->other_rooms = uniq_array(houses[house]->other_rooms + rooms);
  save_file();
  return 1;
}

/**
 * Remove secondary rooms from a house.
 *
 * @param house the basename of the house
 * @param an array of basenames for the rooms
 * @return 1 for success 0 for failure
 */
int remove_rooms(string house, string *rooms) {
  if(!houses[house])
    return 0;

  houses[house]->other_rooms -= rooms;
  save_file();
  return 1;
}


/**
 * Set the owner of this house.
 *
 * @param house The basename of the house
 * @param owner The new owner of the house
 * @return 1 for success 0 for failure
 */
int set_owner(string house, string owner) {
  string room;
  
  if(!houses)
    return 0;
  if(!houses[house])
    return 0;
  if(!owner)
    return 0;
  
  // Inform the house and all its rooms that the owner has changed.
  owner = lower_case(owner);
  house->ownership_change(houses[house]->owner, owner);
  foreach(room in houses[house]->other_rooms)
    room->ownership_change(houses[house]->owner, owner);
  
  log_file("HOUSING", "%s Owner for %s set to %s from %s.\n",
           ctime(time())[4..15], house, owner, houses[house]->owner);
  houses[house]->owner = owner;
  save_file();
  return 1;
}

/**
 * Set the region of this house.
 *
 * @param house The basename of the house
 * @param region The new region of the house
 * @return 1 for success 0 for failure
 */
int set_region(string house, string region) {
  if(!houses) {
    return 0;
  }
  if(!houses[house]) {
    return 0;
  }
  if(!stringp(region)) {
    return 0;
  }
  
  // Inform the house and all its rooms that the owner has changed.
  houses[house]->region = region;
  save_file();
  log_file("HOUSING", "%s Region for %s set to %s\n",
           ctime(time())[4..15], house, region);
  return 1;
}

/**
 * Set the address of this house.
 *
 * @param house The basename of the house
 * @param address The new address of the house
 * @return 1 for success 0 for failure
 */
int set_address(string house, string address) {
  if(!houses) {
    return 0;
  }
  if(!houses[house]) {
    return 0;
  }
  if(!stringp(address)) {
    return 0;
  }
  
  // Inform the house and all its rooms that the owner has changed.
  houses[house]->address = address;
  save_file();
  log_file("HOUSING", "%s Address for %s set to %s\n",
           ctime(time())[4..15], house, address);
  return 1;
}

/**
 * Find out the current owner of this house.
 *
 * @param house The basename of the house
 * @return the owners name.
 */
string query_owner(string house) {
  string tmp;
  
  if(!houses)
    return 0;
  if(houses[house])
    return replace(houses[house]->owner, " (In Arrears)", "");

  foreach(tmp in keys(houses))
    if(member_array(house, houses[tmp]->other_rooms) != -1)
      return replace(houses[tmp]->owner, " (In Arrears)", "");
}

/**
 * Check if a given house is registered.
 *
 * @param house The basename of the house
 * @return 1 if its registered, 0 otherwise.
 */
int query_house(string house) {
  if(!houses)
    return 0;
  if(!houses[house])
    return 0;
  return 1;
}

/**
 * Set the bank account to be used for rent on this house.
 *
 * @param house The basename of the house
 * @param owner The new bank to use
 * @return 1 for success 0 for failure
 */
int set_bank(string house, string bank) {
  if(!houses)
    return 0;
  if(!houses[house])
    return 0;
  if(!bank)
    return 0;
  if(((class housing)houses[house])->type != RENT)
    return 0;
  houses[house]->bank = bank;
  save_file();
  return 1;
}

/**
 * Return which bank account rent will be removed from for a house.
 *
 * @param house The basename of the house
 * @return the bank name.
 */
string query_bank(string house) {
  if(!houses)
    return 0;
  if(!houses[house])
    return 0;
  return houses[house]->bank;
}

/**
 * Get a list of the houses rooms.
 *
 * @param house The basename of the house
 * @return a list of rooms.
 */
string *query_rooms(string house) {
  string *tmp;
  
  if(!houses)
    return 0;
  if(!houses[house])
    return 0;

  if(sizeof(houses[house]->other_rooms)) {
    tmp = houses[house]->other_rooms;
    tmp += ({ house });
  } else
    tmp = ({ house });
  
  return tmp;
}

/**
 * Find out the address of a house.
 *
 * @param house The basename of the house
 * @return the address.
 */
string query_address(string house) {
  if(!houses)
    return 0;
  if(!houses[house])
    return 0;

  return houses[house]->address;
}

/**
 * Find out the region a house is in for sale/rental purposes.
 *
 * @param house The basename of the house
 * @return the region.
 */
string query_region(string house) {
  if(!houses)
    return 0;
  if(!houses[house])
    return 0;

  return houses[house]->region;
}
/**
 * Set the type of a house. ie. Whether it's rented or owned.
 * (1 for rented, 0 for owned).
 *
 * @param house The basename of the house
 * @return the type.
 */
int set_type(string house, int type) {
  if(!houses)
    return 0;
  if(!houses[house])
    return 0;
  if(type < 0 || type > 1)
    return 0;

  ((class housing)houses[house])->type = type;
  save_file();

  return 1;
}

/**
 * Find out the type of a house. ie. Whether it's rented or owned.
 *
 * @param house The basename of the house
 * @return the type.
 */
int query_type(string house) {
  if(!houses)
    return -1;
  if(!houses[house])
    return -1;

  return ((class housing)houses[house])->type;
}

/**
 * Find out the current value of a house.
 *
 * @param house The basename of the house
 * @return the value.
 */
int query_value(string house) {
  if(!houses)
    return 0;
  if(!houses[house])
    return 0;

  return houses[house]->value;
}

/**
 * Function to get a list of houses that are vacant in a given region.
 * This function is used by the auction system or other sales mechanism.
 *
 * @param region Region to list houses in
 * @return a list of houses.
 */
string *query_vacant(string region) {
  string house, *sale_list;

  sale_list = ({ });
  foreach(house in keys(houses)) {
    if(stringp(houses[house]->owner)) {
      if (lower_case(houses[house]->owner) == "for sale" &&
          houses[house]->region == region) {
        sale_list += ({ house });
      }
    } else {
      debug_printf("Bad owner for house %O\n", house);
    }
  }

  return sale_list;
}

/**
 * Mark a house as being For Sale this puts it in the list of possible
 * houses to go into the vacant list.
 *
 * @param house the basename of the house
 * @return 1 for success 0 for failure
 */
int set_for_sale(string house) {
  if(!houses || !houses[house])
    return 0;

  log_file("HOUSING", "%s %s set to For Sale from %s\n",
           ctime(time())[4..15], house, houses[house]->owner);
  
  houses[house]->owner = "For Sale";
  houses[house]->bank = DEFAULT_BANK;
  save_file();
  return 1;
}

/**
 * Mark a house as being Under Offer (to prevent anyone else trying to sell
 * it too.
 *
 * @param house the basename of the house
 * @return 1 for success 0 for failure
 */
int set_under_offer(string house) {
  if(!houses || !houses[house])
    return 0;

  log_file("HOUSING", "%s %s set to Under Offer from %s\n",
           ctime(time())[4..15], house, houses[house]->owner);
  
  houses[house]->owner = "Under Offer";
  houses[house]->value = 0; // So the old rent isn't visible anymore.
  save_file();
  return 1;
}

/**
 * Mark a house as being in arrears.  This is the step just prior to being
 * repossessed.
 *
 * @param house the basename of the house
 * @return 1 for success 0 for failure
 */
int set_in_arrears(string house) {
  if(!houses || !houses[house])
    return 0;

  log_file("HOUSING", "%s %s set to In Arrears from %s\n",
           ctime(time())[4..15], house, houses[house]->owner);
  
  houses[house]->owner = houses[house]->owner + " (In Arrears)";
  save_file();
  return 1;
}

/**
 * Set the value of a particular house.
 *
 * @param house the basename of the house
 * @param value the value in monetary units.
 * @return 1 for success 0 for failure
 */
int set_value(string house, int value) {
  if(!houses || !houses[house])
    return 0;

  houses[house]->value = value;
  save_file();
  return 1;
}
/**
 * @ignore yes
 * This function is called by the refresh handler when a player refreshes
 * or deletes.
 */
int player_refreshed(mixed name, int refresh_type) {
  class housing house;
  string hname;
  
  if(objectp(name))
    name = name->query_name();

  foreach(hname, house in houses)
    if(house->owner == name) {
      log_file("HOUSING", "%s %s refreshed or deleted.\n", ctime(time())[4..15],
               house->owner);
      set_for_sale(hname);
    }
  
  return 1;
}

/**
 * This method sets a mapping between a housing area and the citizenship
 * required to hold a house there.
 * @param area the housing area to link
 * @param citizenship the citizenship the housing area needs
 */
void set_housing_area_citizenship(string area, string citizenship) {
   _housing_area[area] = citizenship;
   save_file();
}

/**
 * This method removes the mapping between a housing area and it's citizenship
 * requirements.
 * @param area the housing area to remove the link from
 */
void remove_housing_area_citizenship(string area) {
   map_delete(_housing_area, area);
   save_file();
}

/**
 * This method returns the current mappings between the housing area and the
 * required citizenship.
 * @param area the area to check
 * @return the citizenship required, 0 if none
 */
string query_housing_area_citizenship(string area) {
   return _housing_area[area];
}

/**
 * This method returns all of the current mappings between the housing
 * area and the citizenship.  This shouuld not be used for anything
 * except debugging.
 * @return all the housing area stuff
 */
mapping query_all_housing_area_citizenships() {
   return copy(_housing_area);
}

void set_housing_area_money( string area, string bank, string place ) {

    if( !_area_money[area] ) {
        _area_money[area] = ([ ]);
    }
    if( bank ) {
        _area_money[area]["bank"] = bank;
    }
    if( place ) {
        _area_money[area]["place"] = place;
    }
    save_file();
    
} /* set_housing_area_money() */

/**
 * This method returns the current mappings between the housing area and the
 * initial bank to use.
 * @param area the area to check
 * @return the initial bank used, DEFAULT_BANK if none set
 */
string query_housing_area_bank(string area) {
    if( _area_money[area] && _area_money[area]["bank"] ) {
        return _area_money[area]["bank"];
    }
    else {
        return DEFAULT_BANK;
    }
} /* query_housing_area_bank() */

/**
 * This method returns the current mappings between the housing area and the
 * money area ("Ankh-Morpork", "Counterweight Continent", etc) to use.
 * @param area the area to check
 * @return the money area used, DEFAULT_PLACE if none set
 */
string query_housing_area_place(string area) {
    if( _area_money[area] && _area_money[area]["place"] ) {
        return _area_money[area]["place"];
    }
    else {
        return DEFAULT_PLACE;
    }
} /* query_housing_area_place() */



/** @ignore yes
 * This function goes through the houses and checks that the owners
 * are still active.
 */
void check_owners(int i) {
  class housing tmp;
  
  if(!_tmp_houses)
    _tmp_houses = keys(houses);

  if(i == sizeof(_tmp_houses))
    return;

  tmp = houses[_tmp_houses[i]];

  if(!tmp->owner ||
     lower_case(tmp->owner) == "for sale" ||
     lower_case(tmp->owner) == "under offer" ||
     strsrch(lower_case(tmp->owner), "(in arrears)") != -1) {
    call_out("check_owners", 1, ++i);
    return;
  }
 
  if(!PLAYER_HANDLER->test_user(lower_case(tmp->owner))) {
    log_file("HOUSING", "%s %s doesn't exist.\n", ctime(time())[4..15],
             tmp->owner);
    set_for_sale(_tmp_houses[i]);
  } else if(!PLAYER_HANDLER->test_creator(tmp->owner)) {
    switch(tmp->type) {
    case SALE:
      if(PLAYER_HANDLER->test_last(tmp->owner) < time() - OWNER_TIMEOUT) {
        log_file("HOUSING", "%s %s is idle last login %s ago [%s].\n",
                 ctime(time())[4..15], tmp->owner,
                 ctime_elapsed(time() - PLAYER_HANDLER->test_last(tmp->owner)),
                 _tmp_houses[i]);
        set_for_sale(_tmp_houses[i]);
      }
      break;
    case RENT:
      if(PLAYER_HANDLER->test_last(tmp->owner) < time() - RENTER_TIMEOUT) {
        log_file("HOUSING", "%s %s is idle last login %s [%s].\n",
                 ctime(time())[4..15], tmp->owner,
                 ctime_elapsed(time() - PLAYER_HANDLER->test_last(tmp->owner)),
                 _tmp_houses[i]);
        set_for_sale(_tmp_houses[i]);
      }
      if (_housing_area[tmp->region] &&
          !NOMIC_HANDLER->is_citizen_of(_housing_area[tmp->region],
                                        tmp->owner)) {
        log_file("HOUSING", "Property %s has an owner %s, who is not "
                 "a citizen of %s [rent %d].\n", _tmp_rentals[i],
                 tmp->owner, _housing_area[tmp->region], tmp->value);
        MAIL_HANDLER->do_mail_message(tmp->owner,
                                      "Housing Rental Office",
                                      "Reposession", "",
                                      "Due to not being a citizen of " +
                                      _housing_area[tmp->region] +
                                      " when your rent was due, your property "
                                      "at " +tmp->address + " has been "
                                      "reposessed and put up for sale.\n\n"
                                      "The rental office.\n");
        set_for_sale(_tmp_rentals[i]);
      }
      break;
    }
  }
  call_out("check_owners", 1, ++i);
}

/** @ignore yes
 *
 * Charge people rent if appropriate.
 */
void check_rent() {
  int *now, daysleft;

  now = AM_TIME_HANDLER->query_am_time_array(time());

  // Check we haven't charged rent this month and charge it.
  if(now[AM_TIME_MONTH] != last_paid) {
    log_file("HOUSING", "Processing rent for " +
             AM_TIME_HANDLER->query_month(now[AM_TIME_MONTH]) + "\n");
    call_out("charge_rent", 0);
    last_paid = now[AM_TIME_MONTH];
    save_file();
  }

  daysleft = AM_TIME_HANDLER->query_days_per_month()[now[AM_TIME_MONTH]-1] -
    now[AM_TIME_DAY_OF_MONTH];
  call_out("check_rent", (daysleft * AM_SECONDS_PER_DAY) + 1);
}


/** @ignore yes
 *
 * This function goes through the houses and checks that the owners
 * are still active.
 */
void charge_rent(int i) {
  class housing tmp;
  string owner;
  
  if(!_tmp_rentals)
    _tmp_rentals = keys(houses);
  
  if(i >= sizeof(_tmp_rentals))
    return;
  
  tmp = houses[_tmp_rentals[i]];
  // Find the next rental house in the list that has an owner
  while(i < sizeof(_tmp_rentals) &&
        (tmp->type != RENT || !tmp->owner ||
         lower_case(tmp->owner) == "for sale" ||
         lower_case(tmp->owner) == "under offer")) {
    
    if(++i < sizeof(_tmp_rentals))
      tmp = houses[_tmp_rentals[i]];
  }
  
  if(i >= sizeof(_tmp_rentals)) {
    return;
  }

  call_out("charge_rent", 1, (i+1));

  // Sanity check in case they have no bank
  if(!tmp->bank) {
    set_bank( _tmp_rentals[i], query_housing_area_bank( tmp->region ) );
    log_file("HOUSING", "Property %s had no bank, setting it to %s.\n", 
             _tmp_rentals[i], tmp->bank);  
  }
  
  if(!tmp->value) {
    log_file("HOUSING", "Property %s has a rent of zero.\n",
             _tmp_rentals[i]);
  } else if(strsrch(tmp->owner, "(In Arrears)") != -1) {
    // This owner is in arrears on their payments.
    owner = replace(tmp->owner, " (In Arrears)", "");
    if(BANK->query_account(owner, tmp->bank) < tmp->value * 3) {
      // They don't have the requisite 3 months rent so the property
      // is repossessed.
      log_file("HOUSING", "%s %s failed to pay %d (%d) for %s [%s] "
               "property repossessed.\n",
               ctime(time())[4..15], owner, tmp->value * 3, 
               BANK->query_account(owner, tmp->bank), _tmp_rentals[i],
               tmp->bank);
      MAIL_HANDLER->do_mail_message(owner,
                                    "Housing Rental Office",
                                    "Reposession", "",
                                    "Due to insufficient funds in your "
                                    "account at " + tmp->bank + " bank your "
                                    "property\n"
                                    "at " +tmp->address + " has been "
                                    "reposessed.\n\n"
                                    "The rental office.\n");
      // Take what we can to cover outstanding rent.
      BANK->adjust_account(owner, tmp->bank,
                           -(BANK->query_account(owner, tmp->bank)));
      set_for_sale(_tmp_rentals[i]);
    } else {
      // Have them pay last months rent, a months late fee and this 
      // month's rent and then return the house to normal ownership.
      BANK->adjust_account(owner, tmp->bank, - (tmp->value * 3));
      log_file("HOUSING", "%s %s paid arrears of %d for %s [%s]\n",
               ctime(time())[4..15], tmp->owner, tmp->value * 3, 
               BANK->query_account(owner, tmp->bank), _tmp_rentals[i],
               tmp->bank);
      set_owner(_tmp_rentals[i], owner);
    }
  } else if(BANK->query_account(tmp->owner, tmp->bank) < tmp->value) {
    log_file("HOUSING", "%s %s failed to pay %d (%d) for %s [%s] "
             "property set in arrears.\n", ctime(time())[4..15], tmp->owner,
             tmp->value, BANK->query_account(tmp->owner, tmp->bank),
             _tmp_rentals[i], tmp->bank);
    MAIL_HANDLER->do_mail_message(tmp->owner,
                                  "Housing Rental Office",
                                  "In Arrears", "",
                                  "Due to insufficient funds in your account "
                                  "at " + tmp->bank + " bank your property\n"
                                  "at " +tmp->address + " is now in arrears."
                                  "\n\n"
                                  "Next month you will be charged three times "
                                  "the normal rent (this months rent, a one "
                                  "month late fee and next months rent).\n\n"
                                  "Failure to pay will result in repossession "
                                  "of your property.\n\n"
                                  "The rental office.\n");
    set_in_arrears(_tmp_rentals[i]);
  } else {
    BANK->adjust_account(tmp->owner, tmp->bank, -tmp->value);
    log_file("HOUSING", "%s %d rental paid by %s for %s\n",
             ctime(time())[4..15], tmp->value, tmp->owner, _tmp_rentals[i]);
  }
  
  if(tmp->owner != "For Sale" &&
     BANK->query_account(tmp->owner, tmp->bank) < (tmp->value * 3)) {
    
    MAIL_HANDLER->do_mail_message(tmp->owner,
                                  "Housing Rental Office",
                                  "Shortage of funds", "",
                                  "Your bank account currently has funds for "
                                  "less than three months rent.\n\n"
                                  "The rental office.\n");
  }
  
  debug_printf("Doing callout %d.", (i+1));
}

/** @ignore yes
 * Get a list of all the houses that exist. This is just a debug function.
 * @return a mapping of all the houses.
 */
mapping query_houses() {
  return copy(houses);
}

/** @ignore yes */
mixed stats() {
  return ({
    ({ "total houses", sizeof(keys(houses)) }),
      ({ "paid for" , AM_TIME_HANDLER->query_month(last_paid) }),
      });
}
