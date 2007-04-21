inherit "/std/dom/error_tracker_master";

void create() {
    set_domain( "mudlib" );
    set_name( "The Mudlib's Lord of the Whinge" );
    set_other_dirs( ({
        "/std%",
        "/obj%",
        "/global%",
        "/cmds%",
        "/mudlib%",
        "/include%",
        "/net%",
        "/soul%",
        "/room%",
        "/www%",
      }) );
    set_period( 604800 );
    ::create();
}
