/**
 *  Bugcheck command migrated from the badge
 *  @author Arienne
 *  @started 18/07/2002 01:56
 *
 */

#define WEIRD_OBJECT_LOG "/log/secure/playtesters/bugcheck-weird-objects"

inherit "/cmds/base";

varargs int cmd(object *obs) {
    string* s;
    string txt;

    foreach ( object ob in obs ) {
        if ( stringp( ob->query_property( "virtual name" ) ) )
            s = explode( ob->query_property( "virtual name" ), "/" );
        else
            s = explode( base_name( ob ), "/" );

        switch ( s[ 0 ] ) {
        case "d":
            write(capitalize( ob->the_short() ) +
                         " is a domain thing.  It's probably safe to "
                         "bugrep it directly.\n" );
            break;
            
        case "w":
            write(capitalize( ob->the_short() ) +
                         "... why do you have a creator item?\n" );
            log_file ("ILLEGAL_OBJECT", "%s: %s tried to bugcheck %s.\n\n",
              ctime(time()), this_player()->query_name(), base_name (ob));                         
            break;
            
        case "std":
            write(capitalize( ob->the_short() ) +
                         " is a weird MUD library thing.  If you bugrep "
                         "it, the response might be slow.  It may be a "
                         "better idea to bugrep the room that the object "
                         "was found in.\n" );
            break;
            
        case "obj":
          switch (s[1]) {
            case "weapons":
            case "armours":
            case "clothes":
            case "food":
            case "jewellery":
              write(capitalize( ob->the_short() ) +
                         " is a Disc-wide item, but belongs to a directory "
                         "that is checked regularly.  Best to make the "
                         "bugrep directly on the item.\n" );

            break;
            default:              
              write(capitalize( ob->the_short() ) +
                         " is a fairly general Disc-wide object.  If you "
                         "bugrep it, the response might be slow.  It would "
                         "most likely be a better idea to bugrep the room "
                         "in which the object was found.\n" );
            break;
          }
        break;
        case "global":
          switch (s[1]) {
            case "player":
            case "lord":
            case "playtester":
              if ( ob == this_player() ) {
                  txt = "are";
              }
              else {
                  txt = "is";
              }

              write(capitalize( ob->the_short() ) +
                         " " + txt + " the general " + s[1] + " object... if your "
                         "bugrep is on this, then bugrep this object.  If "
                         "your bugrep is along the frivolous lines of '" 
                         + ob->the_short() +" " + txt + " overpowered!', "
                         "then don't "
                         "bugreport it... be sure and raise the issue on the "
                         "nearest bulletin board.\n" );
            break;
          }            
        break;
        default:
            write("I've got no idea about " +
                         ob->the_short() + ".  Sorry it didn't work out.\n" );
            log_file( WEIRD_OBJECT_LOG, implode( s, "/" ) + "\n" );
            break;
        }
    }
    
    return 1;
} /* cmd() */

int cmd_room() {
   write("Bug report the room for anything that seems to be odd in the room, "
         "descriptions being wrong, items you look at being wrong.  Anything "
         "odd you see that does not seem to fit anywhere else should also be "
         "reported to the room.\n");
   return 1;
}

mixed *query_patterns() {
   return ({ "<indirect:object:'thing'>", (: cmd($1) :),
             "here", (: cmd_room :)
    });
} /* query_patterns() */

