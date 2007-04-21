/**
  *  Wallpaper glue.  Used in conjunction with /obj/misc/wallpaper and a paint   
  *  brush to decorate the walls of playerhouses.
  *
  *  One pot used per wall.
  *
  *  When adding this object to shops, please call add_glue() on the pot.  
  *  This will give it one application's worth of glue.
  *
  *  @author Una
  *  @date August 27th, 2002
  *
  **/

inherit "/obj/vessel";
 
#define VOLUME 5000
  
object add_glue();  
  
void setup() {
  
    set_name("glue");
    set_short("pot of wallpapering glue");
    add_adjective( ({ "pot", "of", "wallpapering", "wallpaper" }) );
    add_alias("pot");
    add_plural("pots");
    set_max_volume(VOLUME);
    set_long("This is a small pot of wallpapering glue, essential "
        "for anybody who's going to be doing anything involving "
        "sticking paper to walls or playing mean tricks on their "
        "friends.\n");
    set_value(480);
    set_closed();
       
} /* setup() */

object add_glue() {

    object glue;
    
    glue = clone_object( "/obj/reagents/generic_liquid.ob" );
    glue->set_name( "glue" );
    glue->set_short( "sticky white wallpapering glue" );
    glue->add_adjective( ({ "sticky", "white", "wallpapering", 
        "wallpaper"  }));
    glue->set_amount( VOLUME );
    glue->set_continuous();
    glue->set_long( "This is some thick and gloopy wallpapering glue.  "
        "Along with some wallpaper and a brush, you could probably "
        "do some fine home decorating.\n");
    glue->set_pile_name( "puddle" );
    glue->add_eat_effect( "/std/effects/ingested/poison", 10 );
    glue->move( this_object() );
    
    return glue;

} /* add_glue() */
