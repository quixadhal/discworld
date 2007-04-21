/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: potion.c,v 1.1 1998/01/06 05:03:33 ceres Exp $
 * $Log: potion.c,v $
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 *  This class is the potion information handler.
 * <p>
 *  This will handle all the information related to potions: the skill
 *  required, the failure result, etc. (more to be added as necessary).
 *  Each potion will be an object in itself (generally a virtual object).
 *  There will be an index file which gives the pathname of the object
 *  for each potion "title".  This will be loaded into a mapping on
 *  creation of the handler.  The actual potion objects will be created
 *  as they are referenced (assuming they're in the index) and stored
 *  as the value of another mapping.
 * @author Jeremy
 * @started ??/??/95
 */

inherit "/std/object";

mapping paths = allocate_mapping(10);
mapping potions = allocate_mapping(10);

#define POTION_DIR "/obj/potions/"

// This is a list of directories containing potions.  This allows the
// different classes of potions to be broken up.  Each directory must
// have an index file, as described above.

string *directories = ({
    POTION_DIR,
});


void create() {
    string dir, s, dummy, title, path, index;
    int d, i;

    seteuid("Room");
    for (d = 0; d < sizeof(directories); d++) {
        dir = directories[d];
        index = dir + "index";
        i = 1;
        if (file_size(index) > 0) {
            while (s = read_file(index, i++, 1)) {
                if ((s == "") || (sscanf(s, "#%s", dummy))) {
                    continue;
                }
                if (sscanf(s, "\"%s\"%*s\"%s\"", title, path) != 3) {
                    continue;
                }
                paths[title] = path;
                potions[title] = 0;
            }
        }
    }
    ::create();
} /* create() */

/**
 * This method returns the minimum level of the potion name.  The
 * expand_nickname function will be done on the input name before
 * looking it up.
 * @param s the name to lookup
 * @return the minimum level
 * @see query_chant()
 */
int query_min_level(string s) {
    s = this_player()->expand_nickname(s);
    if (undefinedp(paths[s])) {
        return -1;
    }
    if (!potions[s]) {
        potions[s] = clone_object(paths[s]);
    }
    return potions[s]->query_min_level();
} /* query_min_level() */

/**
 * This method returns the chant of the potion name.  The
 * expand_nickname function will be done on the input name before
 * looking it up.
 * @param s the name to lookup
 * @return the chant
 * @see query_min_level()
 */
string query_chant(string s) {
    s = this_player()->expand_nickname(s);
    if (undefinedp(paths[s])) {
        return 0;
    }
    if (!potions[s]) {
        potions[s] = clone_object(paths[s]);
    }
    return potions[s]->query_chant();
} /* query_chant() */

// Sort of a hack...
/**
 * This method clones up a potion object.  It does not setup the reagent
 * in the potion.
 * @see clone_reagent()
 * @return the potion object
 * @param s the potion to create
 */
object query_potion(string s) {
    s = this_player()->expand_nickname(s);
    if (undefinedp(paths[s])) {
       return 0;
    }
    if (!potions[s]) {
       potions[s] = clone_object(paths[s]);
    }
    return potions[s];
} /* query_potion() */

/**
 * This method creates a reagent object.  It creates the potion object and
 * then creates the reagent on it.
 * @param potion the potion to create
 * @return the regent object
 */
mixed clone_reagent(string s) {
    object pot;
    string potion;

    potion = this_player()->expand_nickname(s);
    if (undefinedp(paths[potion])) {
      return -1;
    }
    pot = query_potion(s);
    printf("Verb = %O\n", query_verb());
    return pot->clone_reagent(query_verb());
} /* clone_reagent() */

/**
 * This method returns the paths that are searched for potion
 * objects.
 * @return the mapping of paths
 */
mapping query_paths() {
    return paths;
} /* query_paths() */

/**
 * This method returns the potion mapping.  This the list of known
 * potions.
 * @return the potion mapping
 */
mapping query_potions() {
    return potions;
} /* query_potions() */
