#include "path.h"
/*
 * Example room for using /obj/surf_save and /obj/cont_save.
 */

inherit "/std/room";

object shelf, wardrobe;

void setup() {
   set_short("room of shelf and wardrobe examples");
   add_property( "determinate", "the " );
   set_long( "This room gives some simple examples of how to make shelves, "
            "wardrobes and such that'll save the content for you.\n"
            "To make a shelf, which is characterized by having a surface "
            "you can put things on, you make a clone of /obj/surf_save, "
            "configure it like all cloned objects, and call set_save_file( "
            "file_name ) do tell what file should be used to save the "
            "objects that are put on it.  If the savefile isn't in /save/ "
            "you should call check_euid() on the shelf before calling "
            "set_save_file().\n"
            "Making a wardrobe, you do the same, except you that it is "
            "/obj/cont_save you should clone.\n"
            "Note that since these are fixed parts of the room, and not "
            "something that is put here, I clone them directly in reset() "
            "instead of doing a call_out() to a function that makes them.\n" );
   set_light( 100 );
   add_exit( "back", LEARNING +"search", "path" );
   add_exit( "exit", ENTRANCE, "path" );
   /* The "commented functions" and "keywords" properties are specific for 
    * rooms in the learning domain and is used to make it possible to search
    * for subjects. 
    * See /d/learning/handlers/search.c for the way this is done.
    */
   add_property( "commented functions", 
                ({ "check_euid", "set_save_file" }) );
   add_property( "keywords", 
                ({ "shelf", "wardrobe", "save" }) );
} /* setup() */

void reset() {
   if (!shelf) {
      /* The first parts of making a shelf it covered elsewhere, it's 
       * basically a matter of cloning the object and make it look like 
       * it should
       */
      shelf = clone_object("/obj/surf_save"); 
      shelf->set_name( "shelf" );
      shelf->set_short( "oaken shelf" );
      shelf->add_adjective( "wooden" );
      shelf->add_adjective( "oak" );
      shelf->add_adjective( "oaken" );
      shelf->add_property( "there", "hanging from the wall" );
      shelf->set_long( "This shelf is made from oak and "
                      "varnished to bring out the glow.\n" );
      shelf->set_weight( 2000 );
      shelf->set_max_weight( 2000 );
      shelf->reset_get();
      /* This call will allow the shelf to write the  savefile in the same
       * domain as the room is in, if you don't do this call, the savefile
       * will have to be somewhere in /save/ 
       */
      shelf->check_euid(); 
      /* This call tells the shelf what file should be used to save the stuff
       * placed on it.
       */
      shelf->set_save_file( SAVEPATH+"/oak_shelf" );
      /* end by putting the shelf in the room */
      shelf->move( this_object() );
   }
   if (!wardrobe) {
      wardrobe = clone_object("/obj/cont_save");
      wardrobe->set_name( "wardrobe" );
      wardrobe->set_short( "oaken wardrobe" );
      wardrobe->add_adjective( "wooden" );
      wardrobe->add_adjective( "oaken" );
      wardrobe->add_adjective( "oak" );
      wardrobe->set_long( "This wardrobe is made from oak and "
                         "varnished to bring out the glow.\n" );
      wardrobe->set_weight( 2000 );
      wardrobe->set_max_weight( 2000 );
      wardrobe->reset_get();
      /* This call will allow the wardrobe to write the savefile in the same
       * domain as the room is in, if you don't do this call, the savefile
       * will have to be somewhere in /save/ 
       */
      wardrobe->check_euid();
      /* This call tells the wardrobe what file should be used to save the
       * stuff placed in it.
       */
      wardrobe->set_save_file( SAVEPATH+"/wardrobe" );
      /* end by putting the wardrobe in the room */
      wardrobe->move( this_object() );
   }
} /* reset() */
