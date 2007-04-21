/* -*- LPC -*- */
/**
 * This command is used by the player to create a relationship with another
 * person into the current family they are in.
 * @author Pinkfish
 * @started November 1st 1998
 */
#include <clubs.h>
inherit "/cmds/base";


string show_relationship(object from, object to, string rel, object telling);

/**
 * This method creates a realtionship with someone else.
 */
int do_relationship(string relationship, object *players, int force) {
   object player;
   string family;
   string my_family;
   string their_family;
   string rel;
   class family_response_data frog;
   int ok;
   string *relations;

   my_family = this_player()->query_family_name();
   if (!my_family) {
      add_failed_mess("You must be in a family to form a relationship with "
                      "someone.\n");
      return 0;
   }

   my_family = CLUB_HANDLER->query_club_name(my_family);

   rel = CLUB_HANDLER->query_ungendered_relationship(relationship);
   if (!rel) {
      add_failed_mess("The relationship '" + relationship + 
                      "' is not valid.  The valid relationships are " +
                      query_multiple_short(
                              CLUB_HANDLER->query_all_relationships()) + 
                      ".\n");
      return 0;
   }
   relationship = rel;

   foreach (player in players) {
      // Make sure the family name is correct.
      player->check_family_name();
      their_family = player->query_family_name();
      relations = CLUB_HANDLER->query_relationships_to(my_family,
                                                      this_player()->query_name(),
                                                      player->query_name());

      if (!their_family && sizeof(relations)) {
         //
         // They lost their family somehow, so zap all these
         // relationships.
         //
         foreach (rel in relations) {
            CLUB_HANDLER->remove_relationship(my_family,
                                              this_player()->query_name(),
                                              their_family,
                                              player->query_name(),
                                              rel);
         }
      }

      rel = CLUB_HANDLER->query_relationship_gender(relationship,
                                                    player->query_female());
      if (sizeof(relations)) {
         if (member_array(relationship, relations) != -1 &&
             force) {
            family = my_family;
	    tell_object(player, this_player()->the_short() + 
                             " offers to use "
                             "the relationship of " + rel + " with "
                             "you in the family " + my_family + " to allow "
                             "you to join the family.  WARNING! "
                             "This "
                             "will cause you to join the family.\n" +
                             "This will setup the relationships of: " +
                             show_relationship(this_player(), player,
                                               relationship, player) + "\n" +
                             "To accept the offer type:\n"
                             "accept relationship from " +
                             this_player()->query_name() + " to " +
                             my_family + " as " + rel + "\n");
            frog = new(class family_response_data);
            frog->family = family;
            frog->relationship = relationship;
            player->add_respond_command(CLUB_FAMILY_RESPOND_TYPE, 
                                        this_player(),
                                        frog);
            ok++; 
            add_succeeded_mess(({
                               "$N offer$s to setup a relationship of " +
                               rel + " with $I.\nThis will setup the "
                               "relationships of: " +
                               show_relationship(this_player(), player,
                                                 relationship, this_player()) + "\n",
                               "$N offer$s to setup a relationship of " +
                               rel + " with $I.\n",
                                }),
                               ({ player }));
         } else {
            add_failed_mess("You are already related to $I, you can only force "
                            "them to join your family.\n", ({ player }) ); 
         }
      } else if (player->query_family_name()) {
         if (!force) {
	    // Ok, they are already in a family.
	    if (!CLUB_HANDLER->is_valid_interfamily_relationship(relationship)) {
	       add_failed_mess("$I is in a family already and '" + 
                               rel +
			       "' is not a valid interfamily relationship.\n");
	       continue;
	    }
	    family = player->query_family_name();
	    tell_object(player, this_player()->the_short() + 
                             " offers to setup "
                             "a relationship of " + rel + " with "
                             "you.  This will not cause you to change the "
                             "family.\n" +
                             "This will setup the relationships of: " +
                             show_relationship(this_player(), player,
                                               relationship, player) + "\n" +
                             "To accept the offer type:\n"
                             "accept relationship from " +
                             this_player()->query_name() + 
                             " as " + rel + "\n");
	 } else {
            if (!CLUB_HANDLER->is_valid_join_family_relationship(relationship)) {
               add_failed_mess("$I is in a family already and the "
                               "relationship '" + rel +
                               "' is not valid to cause them to join "
                               "your family.\n");
               continue;
            }
            family = my_family;
	    tell_object(player, this_player()->the_short() + 
                             " offers to setup "
                             "a relationship of " + rel + " with "
                             "you in the family " + my_family + ".  WARNING! "
                             "This "
                             "will cause you to join the family.\n" +
                             "This will setup the relationships of: " +
                             show_relationship(this_player(), player,
                                               relationship, player) + "\n" +
                             "To accept the offer type:\n"
                             "accept relationship from " +
                             this_player()->query_name() + " to " +
                             my_family + " as " + rel + "\n");
	 }
         frog = new(class family_response_data);
         frog->family = family;
         frog->relationship = relationship;
         player->add_respond_command(CLUB_FAMILY_RESPOND_TYPE, 
                                     this_player(),
                                     frog);
         ok++; 
         add_succeeded_mess(({
                            "$N offer$s to setup a relationship of " +
                            rel + " with $I.\nThis will setup the "
                            "relationships of: " +
                            show_relationship(this_player(), player,
                                              relationship, this_player()) + "\n",
                            "$N offer$s to setup a relationship of " +
                            rel + " with $I.\n",
                             }),
                            ({ player }));
      } else {
         tell_object(player, this_player()->the_short() + " offers to setup "
                             "a relationship of " + rel + " with "
                             "you in the family " + my_family + ".  WARNING! "
                             "This "
                             "will cause you to join the family.\n" +
                             "This will setup the relationships of: " +
                             show_relationship(this_player(), player,
                                               relationship, player) + "\n" +
                             "To accept the offer type:\n"
                             "accept relationship from " +
                             this_player()->query_name() + " to " +
                             my_family + " as " + rel + "\n");
         frog = new(class family_response_data);
         frog->family = my_family;
         frog->relationship = relationship;
         player->add_respond_command(CLUB_FAMILY_RESPOND_TYPE, 
                                     this_player(), 
                                     frog);
         ok++;
         add_succeeded_mess(({
                            "$N offer$s to setup a relationship of " +
                            rel + " with $I.\nThis will setup the "
                            "relationships of: " +
                            show_relationship(this_player(), player,
                                              relationship, this_player()) + "\n",
                            "$N offer$s to setup a relationship of " +
                            rel + " with $I.\n",
                             }),
                            ({ player }));
      }
   }
   return ok;
} /* do_relationship() */

string show_relationship(object from, object to, string rel, object telling) {
   string opp;
   string from_arg;
   string to_arg;

   opp = CLUB_HANDLER->query_opposite_relationship(rel);
   if (from == telling) {
      from_arg = "are";
   } else {
      from_arg = "is";
   }
   if (to == telling) {
      to_arg = "are";
   } else {
      to_arg = "is";
   }
   return from->the_short() + " " + from_arg + " the " + 
          CLUB_HANDLER->query_relationship_gender(rel, from->query_female()) +
          " of " + to->the_short() + " and " +
          to->the_short() + " " + to_arg + " the " +
          CLUB_HANDLER->query_relationship_gender(opp, to->query_female()) +
          " of " + from->the_short() + ".";
} /* show_relationship() */

int list_relationships() {
   string *relationships;

   relationships = CLUB_HANDLER->query_all_relationships();
   relationships = map(sort_array(relationships, 1), (: capitalize($1) :));
   printf("The currently allowed relationships are:\n   %-=*s\n",
          this_player()->query_cols() - 4, 
          query_multiple_short(relationships) + ".");
   return 1;
} /* list_relationships() */

mixed *query_patterns() {
   return ({ "<string'relationship'> with <indirect:player:here>",
                (: do_relationship($4[0], $1, 0) :),
             "<string'relationship'> with <indirect:player:here> and join family",
                (: do_relationship($4[0], $1, 1) :),
             "list",
               (: list_relationships() :) });
} /* query_patterns() */
