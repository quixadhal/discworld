/*  -*- LPC -*-  */
/*
 * $Locker: taffyd $
 * $Id: cp.c,v 1.3 2000/11/16 03:09:08 ceres Exp taffyd $
 * $Log: cp.c,v $
 * Revision 1.3  2000/11/16 03:09:08  ceres
 * Modified to check valid_copy
 *
 * Revision 1.2  1999/07/18 20:35:15  wodan
 * don't include . and .. in * expansions.
 *
 * Revision 1.1  1998/01/06 05:10:49  ceres
 * Initial revision
 * 
*/
/* cp command, trial out by Turrican for a commands daemon. */

inherit "/cmds/base";

mixed cmd(string str) {
    string *filenames, dest, *fnames;
    int loop, fs;
    int show_ok;
    string src;

    if(!str)
        return notify_fail("Usage : cp file [file|dir...]\n");

    show_ok = 0;

    fnames = explode(str, " ");
    filenames = this_player()->get_files(implode(fnames[0..sizeof(fnames) -2],
        "/"));
    filenames = filter(filenames, (:$1[<1] != '.':));

    if(!sizeof(filenames)) {
        notify_fail("Usage : cp file [file|dir...]\n");
        return 0;
    }
    dest = fnames[sizeof(fnames) - 1];
    dest = this_player()->get_path(dest);

    if(!dest) {
        write("No destination\n");
        return 1;
    }

    for(loop = 0; loop < sizeof(filenames); loop++) {
        src = filenames[loop];

        if(!master()->valid_copy(src, geteuid(this_player()), ""))
            return notify_fail("Permission denied.\n");

        if ( file_size( src ) == -1 ) {
            write("No such file : " + str + "\n");
            continue;
        }

        fs = file_size(dest);

        if (fs > -1) {
            write("File exists : " + dest + "\n");
            continue;
        }

        switch ( cp( src, dest ) ) {
        case 1:
            show_ok = 1; 
            break;

        case -1: 
            write( src + " is unreadable.\n" );
            break;

        case -2:
            write( dest + " is unreadable.\n" );
            break;

        case -3:
            write( "An I/O error has occurred copying " + 
              src + " to " + dest + "\n" );
            break;

        default:
            write( "Unable to copy " + src + " to " + dest + "\n" );
        }			
    }

if ( show_ok ) {
    write( "Ok.\n" );
}

return 1;
}

