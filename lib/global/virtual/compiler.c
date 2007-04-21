/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: compiler.c,v 1.9 2002/11/30 22:14:50 presto Exp $
 * $Log: compiler.c,v $
 * Revision 1.9  2002/11/30 22:14:50  presto
 * clone /obj/shield for stuff in /obj/armours/shield/
 *
 * Revision 1.8  2001/03/22 18:47:22  pinkfish
 * Make the virtual name used in a different way.
 *
 * Revision 1.7  1999/11/17 09:08:25  pinkfish
 * Fix up the path for the virtual objects.
 *
 * Revision 1.6  1999/11/17 09:03:16  pinkfish
 * Add in transport type of virtual object.
 *
 * Revision 1.5  1999/06/22 16:05:14  ceres
 * Removed caching, it wasn't helping much
 *
 * Revision 1.4  1999/06/22 02:45:39  ceres
 * Modified to reduce file reads
 *
 * Revision 1.3  1998/09/06 17:42:53  olorin
 * Fixed the change to work when the virtual file was changed.
 * Note to PF, rm needs unguarded:)
 *
 * Revision 1.2  1998/08/17 11:48:52  pinkfish
 * Make it use a static non-removable file.
 *
 * Revision 1.1  1998/01/06 04:55:54  ceres
 * Initial revision
 * 
 * Originally by person or persons unknown (finkpish@discworld?)
 *               Maybe by Zellski@discworld
 *
 * 12/93-1/94 Bannor@NewMoon
 *            Fixed the Class and Include keywords.
 * 9-Jul-94   Bannor@NewMoon
 *              Added DELAY_WRITE code to write the temp file to
 *              a string then write the file all at once.  This
 *              reduces the calls of write_file() (and thus valid_write()
 *              and all it's overhead, ie, reduces lag).
 * 1-Oct-95   Jeremy@Discworld
 *              Added clothing class
 * 23-May-95  Olorin@discworld
 *              Modified the room class to call reset() on the object 
 *              before returning it, and to always clone the object to
 *              ensure correct registration of the object.
 * 23-Aug-97 Pinkfish@discworld
 *              Changed to add in the food type.
 */
 
#include "virtual.h"
#include "compiler.h"
 
void    initialize_methods();
string  strip_string( string str );
 
 
#define WHITESPACE(c) (c == 10 || c == 32 || c == '\n')
 
 
mapping o_funcs;
mapping r_funcs;
mapping arm_funcs;
mapping clo_funcs;
mapping sca_funcs;
mapping wep_funcs;
mapping cont_funcs;
mapping food_funcs;
mapping mon_funcs;
mapping clotrans_funcs;
object  return_ob;
int     tmp_file_no;
int     debug_file;             /* don't rm .c file if non-zero */

void    create()
{
  seteuid("Root");
  initialize_methods();
  o_funcs = O_FUN;
  r_funcs = O_FUN + R_FUN;
  arm_funcs = ARM_FUN + O_FUN;
  clo_funcs = CLO_FUN + O_FUN;
  sca_funcs = SCA_FUN + O_FUN;
  wep_funcs = WEP_FUN + O_FUN;
  mon_funcs = MON_FUN + O_FUN;
  cont_funcs = CONT_FUN + O_FUN;
  food_funcs = FOOD_FUN + CONT_FUN + O_FUN;
  clotrans_funcs = O_FUN + CLO_FUN + CLOTRANS_FUN;
}                               /* create() */

object  query_return_ob()
{
    return return_ob;
}
 
void    set_return_ob( object ob )
{
    return_ob = ob;
}
 
void    initialize_methods()
{
    SERVER->add_method( "r", file_name( this_object() ), "compile_r", 
                        CLONE|LOAD );
    SERVER->add_method( "ro", file_name( this_object() ), "compile_r", 
                        CLONE|LOAD );
    SERVER->add_method( "ob", file_name( this_object() ), "compile_ob" );
    SERVER->add_method( "arm", file_name( this_object() ), "compile_arm" );
    SERVER->add_method( "clo", file_name( this_object() ), "compile_clo" );
    SERVER->add_method( "trans", file_name( this_object() ), "compile_clotrans" );
    SERVER->add_method( "sca", file_name( this_object() ), "compile_sca" );
    SERVER->add_method( "wep", file_name( this_object() ), "compile_wep" );
    SERVER->add_method( "mon", file_name( this_object() ), "compile_mon" );
    SERVER->add_method( "food", file_name( this_object() ), "compile_food" );
    SERVER->add_method( "cont", file_name( this_object() ), "compile_cont" );
    SERVER->add_method( "imp", file_name( this_object() ), "compile_imp" );
}                               /* initialize_methods() */
 
#define Error(s) write(s); log_file( "VO_COMPILER", s); return 0;
 
mixed  *extract_arguments( string str );
 
void    do_clone( object dest, string str )
{
    object  ob;
 
    ob = clone_object( str );
    if( ob )
        ob->move( dest );
}                               /* do_clone() */
 
object  prop_to_fun( string file, mapping funs, string virt_class, 
                    string virt_include, int clone )
{
    string *segments, *ind, data, s1, dir, tmp_name, tmp_val;
    string file_data;  /* used to build the file for writing */
    mixed  *val;
    int     i;
    int pos;
 
 
/*
 * Ok...  what we do is stick it in the same dir as the file we start
 * with, with the same name, but a nice number on ze end.
 */
    debug_file = 0;             /* debugging off */
    file_data = "";             /* init file_data */
    pos = strsrch(file, "/", -1);
    tmp_name = file[0..pos] + "." + file[pos+1..] + "_virtual_.c";
    if (file_size(tmp_name) <= 0 ||
        stat(file)[1] > stat(tmp_name)[1]) {
       tmp_file_no++;
       if( find_object( tmp_name ) ) {
           tmp_name->dest_me();
       }
       ind = explode( file, "/" );
       dir = implode( ind[ 0..sizeof( ind ) - 2 ], "/" );
       unguarded((: rm, tmp_name :));
 
       file_data = "#include <"+ virt_include +">\n\n"
                "void dest_me() { destruct( this_object() ); }\n\n"
                "void create()\n{\n"
                "  seteuid( (string)\"/secure/master\"->creator_file( file_name( this_object() ) ) );\n"
                "} /* create() */\n"
                "\nobject create_virtual_ob() {\n"
                "   object clone;\n\n";
 
       data = read_file( file );
       if( !data )
       {
           Error( "prop_to_fun() : file " + file + " not found\n" );
       }
 
       /* Lines beginning with a # are a comment... */
       /* Break into segments at comments */
       segments = explode( "$\n" + data, "\n#" );
       if( !segments )
       {
           Error( "prop_to_fun() : Nothing but comments?\n" );
       }
       /* Remove dummy $ (?) */
       segments[ 0 ] = segments[ 0 ][ 1..(sizeof(segments[ 0 ]) - 1) ];
       /* Remove comment lines */
       for( i = 1; i < sizeof( segments ); i++ ) {
           if( sscanf( segments[ i ], "%s\n%s", s1, segments[ i ] ) != 2 ) {
               segments[ i ] = "";
           }
       }
       /* Join segments together again */
       data = implode( segments, "\n" );
    
       /* See example file for explanation of syntax. */
    
       segments = explode( strip_string( data ), "::" );
       // Debug
       //printf("%O\n", segments);
       /* sizeof(segments) can be odd if the last line has no argument */
       if (sizeof( segments ) % 2) {
         segments += ({""});
       }
       ind = allocate( sizeof( segments ) / 2 );
       val = allocate( sizeof( segments ) / 2 );
    
       for( i = 0; i < sizeof( ind ); i++ )
       {
           ind[ i ] = segments[ i * 2 ];
           val[ i ] = replace( segments[ i * 2 + 1 ], "\n", " " );
           /* look for virtual compiler meta char */
           if( ind[ i ][ 0..0 ] == "#" )
           {
               ind[ i ] = lower_case( ind[ i ] );
               if( ind[ i ] == "#debug" )
               {
                   /* debug errent virtual programs, ie, don't rm */
                   /* the .c file if debug_file is non-zero */
                   sscanf( val[ i ], "%d", debug_file );
               }
               else
               {
                   if( ind[ i ] == "#class" )
                   {
                       /* use new base object for cloning */
                       /* remove any leading (or trailing) spaces */
                       virt_class = replace( val[i], " ", "" );
                   }
                   else
                   {
                       if( ind[ i ] == "#include" )
                       {
                           /* include the file in setup(), just before the */
                           /* object is cloned.  */
                           tmp_val = val[i];
                           file_data += "#include " +
                                       replace( tmp_val, " ", "" ) +
                                       "\n";
                       }
                   }
               }
           }                       /* if( ..# ) */
       }
       file_data += "  clone = (object)SERVER->create_virtual_object( " +
                   virt_class + ", 1 );\n"
                   "  clone->add_property( VIRTUAL_NAME_PROP, \""
                   + file + (clone ? "#1" : "") + "\" );\n";
 
       for( i = 0; i < sizeof( ind ); i++ )
       {
           ind[ i ] = lower_case( ind[ i ] );
           if( ind[ i ][ 0..0 ] == "#" )
           {
               /* it's a virtual keyword don't stick it in the .c file */
               continue;
           }
           /* A quick fix to handle function pointers */
           val[ i ] = replace( val[ i ], ({"(:", "bind( (:",
                                           ":)", ":), clone )" }) );
           if( ind[ i ][ 0..0 ] == "$" )
           {
               /* stuff that starts with "$" are 'undefined' function
                * names in the base object */
               {
                   file_data +="  call_other( clone, \"" +
                               ind[ i ][ 1..100 ] + "\", " + val[ i ] +
                               " );\n";
               }
               continue;
           }                       /* if( ...$ ) */
           if( funs[ ind[ i ] ] )
           {
               if( !pointerp( funs[ ind[ i ] ] ) )
               {
                   /* "val[i][1], val[i][2], val[i][3], val[i][4], val[i][5]); */
                   file_data += "  call_other( clone, \"" +
                               funs[ ind[ i ] ] + "\", " + val[ i ] + " );\n";
               }
               else
               {
                   /*
                    * call_other( funs[ind[i]][1], funs[ind[i]][0], clone,
                    * val[i][0], val[i][1], val[i][2],
                    * val[i][3], val[i][4], val[i][5]);
                    */
                   file_data += "  call_other( " + funs[ ind[ i ] ][ 1 ] +
                               ", " + funs[ ind[ i ] ][ 0 ] + ", " + val[ i ] +
                               " );\n";
               }
           }
           else
           {
               write( "Error: Unknown index " + ind[ i ] + "\n" );
           }
       }
       //file_data +="  \"" + file_name( this_object() ) + "\"->"
       //            "set_return_ob( clone );\n} /* create() */\n";
       file_data += "   return clone;\n} /* create_virtual_ob() */\n";
       /* Ok, now write the file */
       unguarded((: write_file, tmp_name, file_data :));
   }
 
 
   if( data = catch( return_ob = tmp_name->create_virtual_ob() ) )
   {
        write( "Error loading " + file + ", " + data + "\n" );
        if( debug_file )
        {
            write( "File not removed.\n" );
        }
        else
        {
            //unguarded((: rm, tmp_name :));
        }
    }
    else
    {
        tmp_name->dest_me();
        //unguarded((: rm, tmp_name :));
        return return_ob;
    }
}                               /* prop_to_fun() */
 
 
string  strip_string( string str )
{
    int     i, j;
 
    j = strlen( str ) - 1;
    for( ; WHITESPACE( str[ i ] ) && i < j; i++ ) ;
 
    for( ; WHITESPACE( str[ j ] ) && j > i; j-- ) ;
 
    return str[ i..j ];
}                               /* strip_space() */
 
 
mixed   str_to_var( string str )
{
    /* We deal with a few different situations here. If our first character is
     * a linefeed, we have a chunk of text that we will take "as is" but strip
     * the linefeeds (for discworld) ignoring the fact that spaces make exist
     * between end-of-line and lf... fuck it, that's a creator problem. :)
     * 
     * After many attempts with explodes and implodes and shit, I am now
     * doing this in a LPC loop. too
     * slow? I dunno. *shrug* */
    int     i;
 
    str = strip_string( str );
    switch( str[ 0 ] )
    {
        case '{':
            {
                str = str[ 1..strlen( str ) - 2 ];
                return map_array( explode( str, "," ), "str_to_var",
                                  this_object() );
            }
        case '"':
            {
                str = str[ 1..strlen( str ) - 2 ];
                return replace( str, "\n", " " ) + "\n";
            }
        case '0'..'9':
        case '-':
            {
                sscanf( str, "%d", i );
                return i;
            }
    }
    return str;
} /* str_to_var() */
 
 
mixed  *extract_arguments( string str )
{
    return map_array( explode( str, "//" ), "str_to_var", this_object() );
} /* extract_arguments() */
 
// modified 26-MAY-97  Olorin 
object  compile_r( string name, int clone ) {
    object r;
    r = prop_to_fun( name, r_funcs, "\"/obj/virtual_room.c\"",
                    "virtual_room.h", 1 );
    r->reset();
    return r;
} /* compile_r() */
 
 
object  compile_ob( string name, int clone ) {
    return prop_to_fun( name, o_funcs, "\"/std/object.c\"",
                    "virtual.h", clone );
} /* compile_ob() */
 
 
object  compile_arm( string name, int clone ) {
    if (name[0 .. 19] == "/obj/armours/shield/")
        return prop_to_fun(name, arm_funcs, "\"/obj/shield.c\"",
                        "virtual.h", clone );
    return prop_to_fun( name, arm_funcs, "\"/obj/armour.c\"",
                    "virtual.h", clone );
} /* compile_arm() */
 
// Added 1-Oct-95 -- Jeremy
object  compile_clo( string name, int clone ) {
    return prop_to_fun( name, clo_funcs, "\"/obj/clothing.c\"",
                    "virtual.h", clone );
} /* compile_clo() */
 
object compile_sca( string name, int clone ) {
   return prop_to_fun( name, sca_funcs, "\"/obj/scabbard.c\"",
                    "virtual.h", clone );
} /* compile_sca() */
 
object  compile_wep( string name, int clone ) {
    return prop_to_fun( name, wep_funcs, "\"/obj/weapon.c\"",
                    "virtual.h", clone );
} /* compile_wep() */
 
 
object  compile_mon( string name, int clone ) {
    return prop_to_fun( name, mon_funcs, "\"/obj/monster.c\"",
                    "virtual.h", clone );
} /*compile_mon() */

/* Added by Pinkfish Sat Aug 23 20:21:35 WST 1997 */
object  compile_food( string name, int clone ) {
    return prop_to_fun( name, food_funcs, "\"/obj/food.c\"",
                    "virtual.h", clone );
} /*compile_food() */

/* Added by Pinkfish Sat Aug 23 20:21:35 WST 1997 */
object  compile_cont( string name, int clone ) {
    return prop_to_fun( name, cont_funcs, "\"/obj/cont_medium.c\"",
                    "virtual.h", clone );
} /*compile_cont() */

/* Added by Pinkfish Wed Nov 17 00:56:00 PST 1999 */
object  compile_clotrans( string name, int clone ) {
    return prop_to_fun( name, clotrans_funcs, "\"/obj/clothing_transport.c\"",
                    "virtual.h", clone );
} /*compile_cont() */

object  compile_imp( string name, int clone ) {
    return prop_to_fun( name, o_funcs, "\"/obj/implement.c\"",
                    "virtual.h", clone );
} /* compile_imp() */

int set_debug_status(int d_stat) {
  debug_file=d_stat;
  return debug_file;
} /* set_debug_status */
