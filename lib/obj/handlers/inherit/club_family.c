/**
 * This is the handler for all things clubby, a club being a group of
 * players.  Each club must have a unique name.  It also handles elections
 * for various club positions.
 * @author Pinkfish
 * @started Sun Sep 27 03:35:42 EDT 1998
 */
inherit "/obj/handlers/inherit/club_personal";

#define __CLUBS_H_NO_CLASSES

#include <clubs.h>
#include <player_handler.h>

// Special variables to control the club inner workings.
private nosave mapping _valid_relationships;
private nosave mapping _gender_relationship;

private void add_valid_relationship(string relation,
                                    string relation2,
                                    int flags);
private void add_gender_relationship(string region,
                                     string relationship,
                                     string male,
                                     string female);
string* query_gender_relationships(string relationship);

/**
 * The stuff releated to families.
 * @member relationships the relations in this family
 * @member never_join the list of people who can never join
 */
class family_info {
   mapping relationships;
   string *never_join;
   // Couple of extra variables for when I think of something else to go
   // in here.
   string tmp;
   int tmp2;
}

#define CLUB_INTERFAMILY_FLAG 1
#define CLUB_JOIN_FAMILY_FLAG 2
/**
 * The information associated with relationship names.
 * @member opposite the opposite relationship name
 * @member flags flags associated with the relationship's
 */
class relationship_type_data {
   string opposite;
   int flags;
}

void create() {
   _valid_relationships = ([ ]);
   _gender_relationship = ([ ]);

   ::create();

   add_valid_relationship("spouse",
                          "spouse",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "spouse",
                           "husband",
                           "wife");

   add_valid_relationship("lover",
                          "lover",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "lover",
                           "lover",
                           "lover");

   add_valid_relationship("sibling",
                          "sibling",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "sibling",
                           "brother",
                           "sister");

   add_valid_relationship("sibling-in-law",
                          "sibling-in-law",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "sibling-in-law",
                           "brother-in-law",
                           "sister-in-law");

   add_valid_relationship("parent-in-law",
                          "child-in-law",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "parent-in-law",
                           "father-in-law",
                           "mother-in-law");
   add_gender_relationship("Ankh-Morpork",
                           "child-in-law",
                           "son-in-law",
                           "daughter-in-law");

   add_valid_relationship("child",
                          "parent",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "child",
                          "son",
                          "daughter");
   add_gender_relationship("Ankh-Morpork",
                           "parent",
                           "father",
                           "mother");

   add_valid_relationship("grandparent",
                          "grandchild",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "grandparent",
                           "grandfather",
                           "grandmother");
   add_gender_relationship("Ankh-Morpork",
                           "grandchild",
                           "grandson",
                           "granddaughter");

   add_valid_relationship("godchild",
                          "godparent",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "godchild",
                           "godson",
                           "goddaughter");
   add_gender_relationship("Ankh-Morpork",
                           "godparent",
                           "godfather",
                           "godmother");

   add_valid_relationship("uncle",
                          "niece",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "uncle",
                           "uncle",
                           "aunt");
   add_gender_relationship("Ankh-Morpork",
                           "niece",
                           "nephew",
                           "niece");

   add_valid_relationship("cousin",
                          "cousin",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "cousin",
                           "cousin",
                           "cousin");

   add_valid_relationship("master",
                          "maid",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "maid",
                           "butler",
                           "maid");
   add_gender_relationship("Ankh-Morpork",
                           "master",
                           "master",
                           "mistress");

   add_valid_relationship("apprentice",
                          "tutor",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "apprentice",
                           "apprentice",
                           "apprentice");
   add_gender_relationship("Ankh-Morpork",
                           "tutor",
                           "tutor",
                           "tutor");

   add_valid_relationship("guardian",
                          "ward",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "guardian",
                           "guardian",
                           "guardian");
   add_gender_relationship("Ankh-Morpork",
                           "ward",
                           "ward",
                           "ward");

   add_valid_relationship("ex-spouse",
                          "ex-spouse",
                          CLUB_INTERFAMILY_FLAG | CLUB_JOIN_FAMILY_FLAG);
   add_gender_relationship("Ankh-Morpork",
                           "ex-spouse",
                           "ex-husband",
                           "ex-wife");
} /* create() */

protected void create_extra_data(string name) {
   class family_info f_info;
   class club_info data;

   data = (class club_info)query_club_info(name);
   if (query_club_type(name) == CLUB_FAMILY) {
      f_info = new(class family_info);
      f_info->relationships = ([ ]);
      f_info->never_join = ({ });
      data->extra_data = f_info;
   } else {
      ::create_extra_data(name);
   }
   set_club_changed(name);
} /* create_extra_data() */

/**
 * This method determines how much the club will cost to run each
 * pay period.
 * @param club_name the name of the club to get the fees for
 * @return the amount the club will cost in the next pay period
 * @see query_time_fees_due()
 */
int query_club_cost_per_period(string club_name) {
   if (is_club(club_name)) {
      if (is_family(club_name)) {
         return FAMILY_COST_PER_YEAR +
              sizeof(query_members(club_name)) * FAMILY_COST_PER_MEMBER_PER_YEAR;
      } else {
         return ::query_club_cost_per_period(club_name);
      }
   }
   return 0;
} /* query_club_cost_per_period() */

/**
 * This method returns the family information.
 * @param the name of the club
 * @return the family information
 */
private class family_info query_family_info(string name) {
   class club_info data;

   if (is_family(name)) {
      data = (class club_info)query_club_info(name);
      return data->extra_data;
   }
} /* query_family_info() */

/**
 * This method adds a valid relationship to the current list.
 * @param relation1 the first relationship
 * @param relation2 the second relationship
 * @see query_opposite_relationship()
 * @see is_valid_interfamily_relationship()
 * @see is_valid_join_family_relationship()
 * @see is_valid_relationship()
 */
void add_valid_relationship(string relation1,
                            string relation2,
                            int flags) {
   class relationship_type_data frog;

   frog = new(class relationship_type_data);
   frog->opposite = relation2;
   frog->flags = flags;
   _valid_relationships[relation1] = frog;
   frog = new(class relationship_type_data);
   frog->opposite = relation1;
   frog->flags = flags;
   _valid_relationships[relation2] = frog;
} /* add_valid_relationship() */

/**
 * This method returns the opposite relationship to the specified one.
 * @param relation the relation to return the opposite of
 * @return the opposite of the relationship
 * @see add_valid_relationship()
 * @see is_valid_interfamily_relationship()
 * @see is_valid_join_family_relationship()
 * @see is_valid_relationship()
 */
string query_opposite_relationship(string relation) {
   if (_valid_relationships[relation]) {
      return _valid_relationships[relation]->opposite;
   }
   return 0;
} /* query_opposite_relationship() */

/**
 * This method checks to make sure the specified relationship is valid.
 * @param relation the relationship to check for validity
 * @return 1 if the relationship is valid
 * @see query_opposite_relationship()
 * @see is_valid_interfamily_relationship()
 * @see is_valid_join_family_relationship()
 * @see add_valid_relationship()
 */
int is_valid_relationship(string str) {
   return _valid_relationships[str] != 0;
} /* is_valid_relationship() */

/**
 * This method checks to see if the relation is a valid interfamily one.
 * @param str the relationship to check
 * @return 1 if the relation is a valid interfamily one
 * @see is_valid_relationship()
 * @see is_valid_join_family_relationship()
 */
int is_valid_interfamily_relationship(string str) {
   class relationship_type_data frog;

   frog = _valid_relationships[str];
   if (frog) {
      if (frog->flags & CLUB_INTERFAMILY_FLAG) {
         return 1;
      }
   }
   return 0;
} /* is_valid_interfamily_relationship() */

/**
 * This method checks to see if the relation is a valid join family one.
 * @param str the relationship to check
 * @return 1 if the relation is a valid join family one
 * @see is_valid_relationship()
 * @see is_valid_interfamily_relationship()
 * @see add_valid_relationship()
 * @see query_opposite_relationship()
 */
int is_valid_join_family_relationship(string str) {
   class relationship_type_data frog;

   frog = _valid_relationships[str];
   if (frog) {
      if (frog->flags & CLUB_JOIN_FAMILY_FLAG) {
         return 1;
      }
   }
   return 0;
} /* is_valid_interfamily_relationship() */

/**
 * This method returns all the valid relationships.
 * @return all the current valid relationship types
 * @see add_valid_relationship()
 * @see is_valid_interfamily_relationship()
 * @see is_valid_join_family_relationship()
 * @see is_valid_relationship()
 * @see query_opposite_relationship()
 */
string *query_all_valid_relationships() {
   return keys(_valid_relationships);
} /* query_all_valid_relationships() */

/**
 * This method adds a the different gender types for the
 * relationship.
 * @param region the region the relationship is in
 * @param relationship the relationship to set the genders for
 * @param male the male gender type
 * @param female the female gender type
 * @see query_all_relationships()
 * @see query_ungendered_relationships()
 */
private void add_gender_relationship(string region,
                                     string relationship,
                                     string male,
                                     string female) {
   _gender_relationship[male] = relationship;
   _gender_relationship[female] = relationship;
   if (!mapp(_gender_relationship[relationship])) {
      _gender_relationship[relationship] = ([ ]);
   }
   _gender_relationship[relationship][region] = ({ male,
                                                   female });
} /* add_gender_relationship() */

/**
 * This returns the real relationship type based on the possible
 * genderised relationship.
 * @param relationship the relationship to get the real relationship for
 * @return the degendered relationship name
 * @see add_gender_relationship()
 * @see query_all_relationship_types()
 */
string query_ungendered_relationship(string relationship) {
   if (stringp(_gender_relationship[relationship])) {
      return _gender_relationship[relationship];
   }
   if (mapp(_gender_relationship[relationship])) {
      return relationship;
   }
   return 0;
} /* query_ungendered_relationship() */

/**
 * This method returns all the possible relationships, including the
 * gendered ones.
 * @return the array of possible relationships
 * @see query_ungendered_relationship()
 * @see add_gender_relationship()
 */
string *query_all_relationships() {
   return keys(_gender_relationship);
} /* query_all_relationships() */

/**
 * This method returns the genderised version of the relationship.
 * @param name relationship the relation to get the gender of
 * @param female 0 for male, 1 for female
 * @param region the region the relation is in
 * @see query_all_relationships()
 * @see query_ungendered_relationship()
 * @see add_gender_relationships()
 * @see query_all_relationships()
 */
string query_relationship_gender(string relationship,
                                 int female,
                                 string region) {
   string rel;

   rel = query_ungendered_relationship(relationship);
   if (!rel ||
       female < 0 ||
       female > 1) {
      return 0;
   }
   if (!region || !_gender_relationship[rel][region]) {
      region = "Ankh-Morpork";
   }
   return _gender_relationship[rel][region][female];
} /* query_relationship_gender() */

private void real_add_relationship(string name,
                                   string person,
                                   string dest,
                                   string relationship) {
   class family_info womble;
   class relationship_data plum;

   womble = query_family_info(name);
   if (!womble->relationships[person]) {
      womble->relationships[person] = ({ });
   }
   plum = new(class relationship_data);
   plum->related_to = dest;
   plum->relationship = relationship;
   womble->relationships[person] += ({ plum });
   set_club_changed(name);
} /* real_add_relationship() */

private void real_remove_relationship(string name,
                                      string person,
                                      string dest,
                                      string relationship) {
   class family_info womble;
   int i;

   womble = query_family_info(name);
   for (i = 0; i < sizeof(womble->relationships[person]); i++) {
      if (womble->relationships[person][i]->related_to == dest &&
          womble->relationships[person][i]->relationship == relationship) {
         womble->relationships[person] = womble->relationships[person][0..i-1] +
                                         womble->relationships[person][i+1..];
         set_club_changed(name);
      }
   }
} /* real_add_relationship() */

/**
 * This method adds a relationship between two members of a family.
 * @param name the name of the family
 * @param person the person the relationship is from
 * @param dest the person the relationship is to
 * @param dest_name the name of the family the other person is in
 * @param relationship the relationship they have to you
 * @return 1 if succcessful, 0 if not
 */
int add_relationship(string name,
                     string person,
                     string dest_name,
                     string dest,
                     string relationship) {

   if (is_family(name) &&
       is_family(dest_name) &&
       is_member_of(name, person) &&
       is_member_of(dest_name, dest) &&
       is_valid_relationship(relationship)) {
      real_add_relationship(name,
                            person,
                            dest,
                            relationship);
      // Add the reverse to the other person.
      real_add_relationship(dest_name,
                            dest,
                            person,
                            query_opposite_relationship(relationship));
      save_club(name);
      return 1;
   }
   return 0;
} /* add_relationship() */

/**
 * This method returns the relationships the selected person has to
 * the other people in the family.
 * @param name the name of the family
 * @param person the person who to query the relationships for
 * @return the list of all the relationships the person has
 */
class relationship_data *query_relationships(string name,
                                             string person) {
   class family_info womble;

   if (is_family(name)) {
      womble = query_family_info(name);
      if (womble->relationships[person]) {
         return womble->relationships[person];
      }
   }
   return ({ });
} /* query_relationships() */

/**
 * This method moves a person from one family to another family.
 * @param curr_family the current family
 * @param person the persons name
 * @param dest_family the destination family
 * @return 1 if successful moved
 * @see add_member()
 */
int move_family_member(string curr_family,
                       string person,
                       string dest_family) {
  class relationship_data *stuff;
  class family_info womble;

  if (is_family(curr_family) &&
      is_family(dest_family) &&
      is_member_of(curr_family, person)) {
    stuff = query_relationships(curr_family,
                                person);
    /* Remove the relationships first. */
    womble = query_family_info(curr_family);
    map_delete(womble->relationships, person);
    set_club_changed(curr_family);
    remove_member(curr_family, person);

    /* Add the new relationship. */
    add_member(dest_family, person);
    womble = query_family_info(dest_family);
    womble->relationships[person] = stuff;
    set_club_changed(dest_family);
    return 1;
  }
  return 0;
} /* move_family_member() */

/**
 * This method returns how the first player is related to the second player.
 * @param name the family the relationship is in
 * @param person the person the relationship is from
 * @param related to person they are related to
 * @return the array of relationships they have
 */
string *query_relationships_to(string name,
                               string person,
                               string related) {
   class relationship_data frog;
   string *relationships;

   relationships = ({ });
   foreach (frog in query_relationships(name, person)) {
      if (frog->related_to == related) {
         relationships += ({ frog->relationship });
      }
   }
   return relationships;
} /* query_relationships_to() */

/**
 * This method checks to see if the specified relationship exists.
 * @param name the family the relationship is in
 * @param person the person the relationship is from
 * @param related to person they are related to
 * @param relationship the type of relationship they have
 * @return 1 if the relationship exists, 0 if not
 */
int is_relationship(string name,
                    string person,
                    string related,
                    string relationship) {
   class relationship_data frog;

   foreach (frog in query_relationships(name, person)) {
      if (frog->related_to == related &&
          frog->relationship == relationship) {
         return 1;
      }
   }
   return 0;
} /* is_relationship() */

/**
 * This method removes a relationship from the club.
 * @param name the name of the family
 * @param person the person the relationship is from
 * @param dest the person the relationship is to
 * @param relationship the relationship they have to you
 * @return 1 if succcessful, 0 if not
 */
int remove_relationship(string name,
            string person,
                        string dest_name,
      string dest,
      string relationship) {
   if (is_relationship(name, person, dest, relationship)) {
      real_remove_relationship(name,
                   person,
             dest,
             relationship);
      if (dest_name) {
         real_remove_relationship(dest_name,
                      dest,
                person,
                      query_opposite_relationship(relationship));
      }
      return 1;
   }
   return 0;
} /* remove_relationship() */

/**
 * This method checks the extra information for the club.  Please note
 * this does *not* save the information.  You need to do this yourself.
 * @param name the name of the club to check
 * @see check_clubs()
 * @see remove_member()
 */
protected void check_family_information(string name,
                                        string member,
                                        int startup) {
   class family_info fam_data;
   //class relationship_data rel;
   string family;

   fam_data = query_family_info(name);
   if (fam_data) {
      /*
       * Check the stuff in the club to make sure the positions and stuff
       * are still valid.
       */
      if (member &&
         fam_data->relationships[member]) {

         family = PLAYER_HANDLER->test_family(member);
         if (!family ||
              normalise_name(family) != name) {
            if (find_player(member)) {
               find_player(member)->set_family_name(family);
            } else {
               log_file("CLUB", ctime(time()) +
                                ": removed " +
                                member + " from " + family +
                                " family deficent.\n");
               remove_member(name, member);
            }
         }

/*
         foreach (rel in fam_data->relationships[member]) {
            family = PLAYER_HANDLER->test_family(rel->related_to);
            if (!family) {
               * Remove the relationship... *
               log_file("CLUB", ctime(time()) +
                                ": removed relationship between " +
                                member + " <-> " + rel->related_to +
                                " (" + rel->relationship +
                                "; family deficent.\n");
               real_remove_relationship(name,
                                   member,
                                   rel->related_to,
                                   rel->relationship);
            }
         }
 */

         /* Are they a member of any family now?  Hmm... */
         if (!undefinedp(fam_data->relationships)) {
            map_delete(fam_data->relationships, member);
            set_club_changed(name);
         }
      }
   }
} /* check_family_information() */
