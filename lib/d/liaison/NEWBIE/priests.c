/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: priests.c,v 1.8 2003/02/19 06:54:24 ceres Exp $
 */

#include "path.h"

inherit PATH + "inside";

string list_deities();

void setup() {
  set_short( "Priests' Gazebo" );
  set_light( 75 );
  add_property("no teleport", 1);
  set_long( "As you enter this blessed and holy gazebo, a warm feeling of "
            "peace and calmness overwhelms you.  You get the feeling that "
            "this must be what it's like to be a priest.  Around the room "
            "you can see items of priesthood: jars of fluff, holy symbols "
            "and coloured rods.  A small sign with a green aura sits in the "
            "corner. If you 'read sign' you can get some information "
            "about the priests.\n"
            "A small notice, blessed into oblivion, states that this is not "
            "the place where you actually join the guild.\n" );

   set_zone( "Newbie" ); 

   add_item( ({ "jar", "jar of fluff", "jars of fluff", "fluff", 
             "holy symbol", "coloured rod", "relic", "symbol", "rod" }), 
             "There are many items around the room, but their function is "
             "beyond you for the most part." );
   add_item( "notice",
             "This blessed notice rests peacefully against a holy relic." );

   add_sign( "The sign is covered in a soothing green aura, and sits "
             "in the corner of the room.\n", 
             FOYER->format_sign("This holy guild, and its many "
             "gods, is based at the Temple of Small Gods in Ankh-Morpork.  "
             "You may learn how to heal and resurrect, shield with the " 
             "power of your faith, and many other holy powers.  Some gods "
             "shun healing almost completely, preferring to concentrate on "
             "causing strife and mayhem among the citizens of Discworld.  Ask"
             " some of the other priests in the city about finding a god "
             "that will suit your needs, and help back you up.\n"
             "The following gods are desirous of priests: " + list_deities() +
             ".  Many gods require particular alignments.  Use 'finger "
             "<deity>' to find out a particular deities alignment "
             "requirements and visit the alignment room to get yours "
             "adjusted if you wish.\n"), 0, 0, "general");


   /* Make it so you cannot godmother out of here */
   add_property("no godmother", 1);
   
   add_exit( "garden", PATH +"guilds_foyer", "gate" );
}

string list_deities() {
  string *deities;
  int i;
  
  deities = unguarded((: get_dir("/save/deities/") :));
  deities -= ({ ".", ".." });

  for(i=0; i<sizeof(deities); i++) {
    deities[i] = replace(deities[i], ".o", "");
    deities[i] = replace(deities[i], ".gz", "");
  }
  deities = map (filter(deities,
                   (: "/obj/handlers/deity_handler"->query_want_priests($1)
                    :)), (: capitalize ($1) :));
  
  return query_multiple_short(deities);
}
