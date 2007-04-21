/**
  *  A wallpaper object for playerhousing
  *
  *  To create a roll with a pattern, just call make_roll( <design> )
  *  on one.
  *
  *  To apply this wallpaper to a playerhousing wall, you also need 
  *  a pot of glue and a paint brush.
  *
  *  @author Una
  *  @27th August, 2002
  *
  **/

inherit "/std/object";

private nosave string _design;

varargs int do_paper(string indirect, object pot);

void setup() {

    set_name( "wallpaper" );
    set_short( "roll of wallpaper" );
    set_main_plural( "rolls of wallpaper");
    add_adjective( ({ "roll", "of"}));
    add_plural_adjective("rolls");
    add_alias(({"paper"}));
    set_long( "This is a large roll of wallpaper, probably enough to "
        "cover a single wall completely.\n");
    set_value( 480 );
    set_weight( 10 );
    
} /* setup() */


void make_roll( string design ) {

    _design = design;
    set_short( "roll of " + _design + " wallpaper" );
    set_main_plural( "rolls of " + _design + " wallpaper");
    add_adjective( ({ _design })) ;
    set_long( "This is a large roll of " + _design + " wallpaper.  It "
        "looks like there's just enough of it to cover a single wall "
        "completely.  If you had some wallpaper glue you could probably "
        "paste it to the wall.\n");
  
} /* make_roll() */


/** @ignore yes */
void init() {
    
    this_player()->add_command("paste", this_object(),
       "<direct:object> to <indirect:object:here'cardinal direction'> using <indirect:object'pot'>", (: do_paper( $4[1], $1[1][0] ) :));
  
} /* init() */


string query_paper_mess() {

    if( _design ) {
        return _design + " wallpaper";
    }
    else {
        return "strangely featureless wallpaper";
    }
    
} /* query_paper_mess() */


varargs int do_paper(string indirect, object pot) {
    object *obs;
  
    if(!indirect && pot) {
        if(function_exists("what_to_paint", pot))
            printf(pot->what_to_paint());
        else
            printf("Help: paste " + pot->short() + " with wallpaper\n");
            return 1;
    }

    obs = match_objects_for_existence("glue", pot);

    if(pot->query_closed()) {
        this_player()->add_failed_mess(this_object(), "$I is "
            "closed.\n", ({ pot }));
        return 0;
    }

    if(!pot->query_volume(obs) || !sizeof(obs)) {
        this_player()->add_failed_mess(this_object(), "$I appears to "
            "be empty.\n", ({ pot }));
        return 0;
    }

    if(!sizeof(filter(this_player()->query_holding() - ({ 0 }),
        (: $1->id("brush") :)))) {
        this_player()->add_failed_mess(this_object(), "You must be holding a "
            "paint brush to paint the walls with glue.\n", ({ }));
        return 0;

    }

    if(!environment( this_player() )->test_occupier( this_player()->
        query_name() ) ) {
    
        this_player()->add_failed_mess(this_object(), "You don't own this "
            "house.\n", ({ }));
        return 0;
    }
  

    if(!function_exists("decorate_surface", environment(this_player()))) {

        this_player()->add_failed_mess(this_object(), "You can't $V the "
            "walls here.\n", ({ }));
        return 0;
    }
  
    if(!environment(this_player())->decorate_surface(indirect, 
        "papered with " + query_paper_mess())) {
        
        this_player()->add_failed_mess(this_object(), "You can't $V the $D" 
            " to the " + indirect + " using $I for some reason.\n", 
            ({ pot }));
        return 0;
    }
  
    pot->remove_volume(pot->query_volume());
    this_object()->move("/room/rubbish");
    
    this_player()->add_succeeded_mess(this_object(), "$N $V $D to the " + 
        indirect + ", dawbing it with paste from $I and pressing it firmly "
        "against the surface.\n", ({ pot }) );
    return 1;
    
    
    
} /* do_paper() */


/** @ignore yes */ 
mapping int_query_static_auto_load() {
    mapping tmp;

    tmp = ::int_query_static_auto_load();
    return ([ "::" : tmp,
              "design" : _design,
            ]);
} /* int_query_static_auto_load() */


/** @ignore yes */ 
mapping query_static_auto_load() {

    if ( base_name(this_object()) + ".c" == __FILE__ )
        return int_query_static_auto_load();
    return ([ ]);
    
} /* query_static_auto_load() */


/** @ignore yes */ 
void init_static_arg(mapping args) {
    if (args["::"])
    
    ::init_static_arg(args["::"]);
  
    if(!undefinedp(args["design"]))
        _design = args["design"];
    
    if(_design)
        make_roll(_design);
} /* init_static_arg() */
