/**
 * This room will handle craft shop like things, where you sell stuff to
 * the store and it will sell it onto other players after taking
 * a margin.  This can be used for potions, pottery, swords, anything!
 * @author Pinkfish
 * @started Thu Feb  5 15:39:57 CST 1998
 */
#include <money.h>
#include <shops/craft_shop.h>

inherit "/std/shops/inherit/craft_shop_category";

private nosave int _minimum_age_to_vote;
private nosave int _minimum_age_to_nominate;
private nosave int _minimum_number_nominated;
private nosave int _minimum_number_voted;
private nosave int _percentage_needed_creation;
private nosave int _percentage_needed_deletion;
private nosave int _nomination_time;
private nosave int _election_time;
private nosave int _deletion_time;

#define DAY (24 * 60 * 60)

//
// Predefinitions.
//
protected void setup_category(function func,
                              int value,
                              string name,
                              object* sellables);
protected void choose_category(string category,
                              function func,
                              int value,
                              string name,
                              object* sellables,
                              string* categories);

void create() {
   if (!_minimum_age_to_vote) {
      _minimum_age_to_vote = 2 * DAY;
   }
   if (!_minimum_age_to_nominate) {
      _minimum_age_to_nominate = 10 * DAY;
   }
   if (!_minimum_number_nominated) {
      _minimum_number_nominated = 4;
   }
   if (!_nomination_time) {
      _nomination_time = 7 * DAY;
   }
   if (!_election_time) {
      _election_time = 7 * DAY;
   }
   if (!_deletion_time) {
      _deletion_time = 7 * DAY;
   }
   if (!_minimum_number_voted) {
      _minimum_number_voted = 20;
   }
   if (!_percentage_needed_creation) {
      _percentage_needed_creation = 75;
   }
   if (!_percentage_needed_deletion) {
      _percentage_needed_deletion = 75;
   }
   ::create();
} /* create() */

/**
 * This is the function used as a call back from the controller to deal with
 * category based stuff.
 * @param name the name of the category being processed
 * @param state the state the category is currently in
 * @param voted the number of people who voted
 * @param yes the number who voted yes
 * @param no the number who voted no
 * @param abstain the number who abstained
 * @return 0 on error, timeout of the next stage
 */
protected int category_function(string name,
                      int state,
                      string* voted,
                      int yes,
                      int no,
                      int abstain) {
   switch (state) {
   case CRAFT_CATEGORY_STATE_NOMINATING :
      if (sizeof(voted) > _minimum_number_nominated) {
         return _election_time;
      }
      break;
   case CRAFT_CATEGORY_STATE_VOTING :
      if (sizeof(voted) > _minimum_number_voted) {
         if (yes * 100 / (no + yes) >= _percentage_needed_creation) {
            return 1;
         }
      }
      break;
   case CRAFT_CATEGORY_STATE_DELETEING :
      if (sizeof(voted) > _minimum_number_voted) {
         if (yes * 100 / (no + yes) >= _percentage_needed_deletion) {
            return 1;
         }
      }
   }
   return 0;
} /* category_function() */

/**
 * This method sets the minimum age you need to be to vote in category
 * elections.
 * @param age the minimum age to vote
 */
void set_minimum_age_to_vote(int age) {
   _minimum_age_to_vote = age;
} /* set_minimum_age_to_vote() */

/**
 * This method sets the minimum age you need to be to nominate a new
 * category.
 * @param age the minimum age to nominate
 */
void set_minimum_age_to_nominate(int age) {
   _minimum_age_to_nominate = age;
} /* set_minimum_age_to_nominate() */

/*
 * This method sets the minimum number of people you need before a
 * nomination is successful.
 * @param num the minimum number of people to nominate
 */
void set_minimum_nomination_number(int num) {
   _minimum_number_nominated = num;
} /* set_minimum_nomination_number() */

/**
 * This method sets the amount of time it takes for a category nomination
 * to go through the process.
 * @param time the nomination time
 */
void set_nomination_time(int tim) {
   _nomination_time = tim;
} /* set_nomination_time() */

/**
 * This method sets the amount of time it takes for a category election
 * to go through the process.
 * @param time the election time
 */
void set_election_time(int tim) {
   _election_time = tim;
} /* set_election_time() */

/**
 * This method sets the amount of time it takes for a category deletion
 * to go through the process.
 * @param time the deletion time
 */
void set_deletion_time(int tim) {
   _deletion_time = tim;
} /* set_deletion_time() */
