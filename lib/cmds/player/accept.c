/*  -*- LPC -*-  */
/**
 * This handles both the surrender and club accepting stuff.
 * The surrender stuff was done by Sin and the club stuff by Pinkfish.
 */

#include <clubs.h>
#include <broadcaster.h>

inherit "/cmds/base";

mixed do_surrender(object player) {
  object *victims;

  victims = this_player()->query_surrenderers();
  if (!victims  ||  member_array(player, victims) == -1) {
    add_failed_mess("Sorry, but $I has not offered to surrender to you.\n",
                    ({ player }) );
    return 0;
  }
  this_player()->remove_surrenderer(player);
  player->accepted_surrender(this_player());
  write("Good show!\n");
  return 1;
} /* do_surrender() */

mixed do_club(string club_name, object *players) {
   object ob;
   string club;
   int ok;

   foreach (ob in players) {
      club = this_player()->query_respond_command(CLUB_RESPOND_TYPE, ob);
      if (club) {
         if (lower_case(club) == lower_case(club_name)) {
            // Whooo!  Found the club!  They accept!
            // Check to see if they are already a member, other wise
            // add them.
            if (CLUB_HANDLER->is_member_of(club_name,
                                           this_player()->query_name())) {
               add_failed_mess("You are already a member of '" +
                               CLUB_HANDLER->query_club_name(club_name) +
                               "'.\n");
            } else {
               CLUB_HANDLER->add_member(club_name,
                                        this_player()->query_name());
               add_succeeded_mess("$N join$s '" +
                               CLUB_HANDLER->query_club_name(club_name) +
                               "' with an invite from $I.\n",
                               ({ ob }) );
	       ob->remove_respond_command(CLUB_RESPOND_TYPE, ob);
               ok++;
               all_inventory(this_player())->event_joined_club(this_player(),
                                                               club_name);
            }
         } else {
            add_failed_mess("$I is inviting you to join '" + 
                            CLUB_HANDLER->query_club_name(club) +
                            "' not '" + club_name + "'.\n", ({ ob }));
         }
      } else {
         add_failed_mess("$I is not inviting you to join any clubs.\n",
                         ({ ob }));
      }
   }
   return ok;
} /* do_club() */

int do_family(string family, 
              string relationship,
              object *players) {
   object ob;
   int ok;
   string curr_family;
   class family_response_data frog;

   curr_family = this_player()->query_family_name();
   if (curr_family) {
      curr_family = CLUB_HANDLER->query_club_name(curr_family);
   }

   relationship = CLUB_HANDLER->query_ungendered_relationship(relationship);

   if (!relationship) {
      add_failed_mess("Could not figure out the relationship.\n");
      return 0;
   }

   foreach (ob in players) {
      frog = this_player()->query_respond_command(CLUB_FAMILY_RESPOND_TYPE, 
                                                  ob);
      if (!family) {
         family = frog->family;
      }
      if (frog && frog->family) {
         if (lower_case(frog->family) == lower_case(family)) {
            // Whooo!  Found the family!  They accept the relationship!
  	    if (lower_case(frog->relationship) != lower_case(relationship)) {
               add_failed_mess("You were asked to have the relationship of '" +
                     CLUB_HANDLER->query_relationship_gender(frog->relationship,
                                     this_player()->query_female()) +
                               "', not '" +
                     CLUB_HANDLER->query_relationship_gender(relationship,
                                     this_player()->query_female()) +
                               "' in the family '" +
                               CLUB_HANDLER->query_club_name(frog->family) +
                               "'.\n");
	    } else if (CLUB_HANDLER->is_relationship(frog->family,
                                                     this_player()->query_name(),
                                                     ob->query_name(),
                                                     relationship) &&
                       frog->family == curr_family) {
               add_failed_mess("You have already setup a relationship of '" +
                     CLUB_HANDLER->query_relationship_gender(relationship,
                                     this_player()->query_female()) +
                               "' with $I.\n", ({ ob }) );
            } else {
               if (!CLUB_HANDLER->is_member_of(frog->family,
                                               this_player()->query_name())) {
                  if (frog->family != curr_family &&
                      curr_family) {
                     if (!CLUB_HANDLER->move_family_member(curr_family,
                                                           this_player()->query_name(),
                                                           frog->family)) {
                       add_failed_mess("Unable to move you into the family " +
                                       frog->family + " for some reason.\n");
                       continue;
                     }
                  } else {
                     if (!CLUB_HANDLER->add_member(frog->family,
                                                  this_player()->query_name())){
                       add_failed_mess("Unable to add you into the family " +
                                       frog->family + " for some reason.\n");
                       continue;
                     }
                  }
                  this_player()->set_family_name(frog->family);
               }
	       if (!CLUB_HANDLER->is_relationship(frog->family,
                                                  this_player()->query_name(),
                                                  ob->query_name(),
                                                  relationship)) {
                  if (!CLUB_HANDLER->add_relationship(ob->query_family_name(),
                                                     ob->query_name(),
                                                     frog->family,
                                                     this_player()->query_name(),
                                                     relationship)) {
                     add_failed_mess("Unable to add the relationship to $I for "
                                     "some reason.\n");
                  } else {
                     add_succeeded_mess("$N set$s up a relationship with $I " +
                                        "in '" +
                                        CLUB_HANDLER->query_club_name(frog->family) +
                                        "'.\n",
                                        ({ ob }) );
                     ok++;
                  }
               } else {
                  add_succeeded_mess("$N set$s up a relationship with $I " +
                                     "in '" +
                                     CLUB_HANDLER->query_club_name(frog->family) +
                                     "'.\n",
                                     ({ ob }) );
                  ok++;
               }
               ob->remove_respond_command(CLUB_FAMILY_RESPOND_TYPE, ob);
            }
         } else {
            add_failed_mess("$I is inviting you to a relationship in '" + 
                            CLUB_HANDLER->query_club_name(frog->family) +
                            "' not '" + family + "'.\n", ({ ob }));
         }
      } else {
         add_failed_mess("$I is not inviting you to setup any relationships.\n",
                         ({ ob }));
      }
   }
   return ok;
} /* do_family() */

mixed *query_patterns() {
  return ({ "<indirect:living:here>", (: do_surrender($1[0]) :),
            "invite from <indirect:living:here> to <string'club name'>",
                (: do_club($4[1], $1) :),
            "relationship from <indirect:living:here> to <string'family'> as <string'relationship'>",
                (: do_family($4[1], $4[2], $1) :),
            "relationship from <indirect:living:here> as <string'relationship'>",
	       (: do_family(this_player()->query_family_name(), $4[1], $1) :) });
} /* query_patterns() */
