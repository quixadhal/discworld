/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: reaction.c,v 1.13 2002/02/24 06:18:32 taffyd Exp $
 * $Log: reaction.c,v $
 * Revision 1.13  2002/02/24 06:18:32  taffyd
 * Patched a runtime
 *
 * Revision 1.12  2001/07/26 04:38:17  tannah
 * Ceres' new version with fixes for runtimes.
 *
 * Revision 1.9  2001/07/18 23:35:49  tannah
 * replaced set_amount with adjust_amount and reordered a few things
 * in check_reaction()
 *
 * Revision 1.8  2001/06/30 05:42:04  pinkfish
 * Fix it up to stop spamming /log/catch.
 *
 * Revision 1.7  2000/04/10 05:08:25  ceres
 * No idea.
 *
 * Revision 1.6  1999/05/02 04:46:21  jeremy
 * Added merging of effects.
 *
 * Revision 1.5  1998/09/10 17:02:03  jeremy
 * ratios can now be int or array of ints (previously had to be floats).
 *
 * Revision 1.4  1998/08/04 04:14:35  jeremy
 * Fixed typo (I hope) in range-of-ratios section, and added more comments.
 *
 * Revision 1.3  1998/05/08 03:20:26  jeremy
 * Took out unused variables, and added update_from_files() (untested).
 *
 * Revision 1.2  1998/04/21 03:45:39  jeremy
 * Added check for a->query_amount() going to 0 in the main loop.
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 *
*/
/*
 *  reaction handler - Handles reactions between substances that have
 *      been mixed together.
 *
 *  A few notes of what I have planned:
 *
 *  the substance mapping will have as its keys the substance names
 *  (as returned by query_medium_alias()).
 *  The value will be another mapping (one for each substance).  This
 *  second-level mapping will have as its keys the substances with
 *  which it reacts.  To save space, the names of the two substances
 *  will be compared, and the alphanumerically lower one will be the
 *  index into the top-level mapping.
 *
 *  The value of the second-level mapping is the tricky part.
 *  It is currently a class that contains all the relevant information.
 *  See the definition of the class for more info.
 *
 *  The following pseudo-objects may be used in the message strings and
 *  function call strings; process_string is explicitly called in both
 *  cases:
 *    #env# - replaced by the filename of the environment
 *    #env2# - replaced by the filename of the environment of the enviroment
 *    #obj_a# - The object being added to the container
 *    #obj_b# - The object with which 'a' is reacting
 *    #obj_x# - The transient object, or the result object if no transient
 *
 *  Various debugging messages will be printed if this_player() has a
 *  property call "debug_rh" with a non-zero value.
 *
 *  To Do:
 *    Handle concentrations.
 *
 */

#include <move_failures.h>
#include <reaction.h>

#define REACTIONS "/obj/reactions/"
#define SUBSTANCES 10

mapping substance = allocate_mapping(SUBSTANCES);
int debug_rh = 0;

void restore_from_files();

void write_dbg(string str) { if (debug_rh) write(str); }

void create() {
    seteuid("/secure/master"->creator_file(file_name(this_object())));
    restore_from_files();
}

// Merge the effects mappings, weighted by relative amounts, and return
// a mapping that can be used to add the new effects.  Note that, since
// add_xxx_effect() is cumulative, the result reflects the difference.
mapping merge_effects(mapping effects_a, mapping effects_m, int q_a, int q_m) {
    int q_t, strength;
    string key;
    mapping ret = ([ ]);

    if(!mapp(effects_a) || !mapp(effects_m))
      return 0;

    q_t = q_a + q_m;
    if ( !q_t ) {
        return 0;
    }

    foreach (key in keys(effects_a)) {
        if (!intp(effects_a[key]) || !intp(effects_m[key])) {
            continue;
        }
        if (undefinedp(effects_m[key])) {
            strength = (effects_a[key]*q_a)/q_t;
        } else {
            strength = (effects_a[key]*q_a + effects_m[key]*q_m)/q_t
                - effects_m[key];
            map_delete(effects_m, key);
        }
        ret += ([key: strength]);
    }
    foreach (key in keys(effects_m)) {
        if (!intp(effects_m[key])) {
            continue;
        }
        strength = (effects_a[key]*q_a + effects_m[key]*q_m)/q_t
            - effects_m[key];
        ret += ([key: strength]);
    }
    return ret;
}

// This is used to merge the new substance with any amounts of the same
// type already in the container, so that the total is checked.
object merge_cont_medium(object a, string medium_alias) {
    object medium;
    int no_join, q_a, q_m;
    mapping effects_a, effects_m, eff;
    string key;

    a->remove_alias(medium_alias);
    no_join = a->query_no_join();
    if ((medium = present(medium_alias, environment(a))) && !no_join) {
        a->set_no_join();
        a->move("/room/void");
        q_a = a->query_amount();
        q_m = medium->query_amount();
        medium->adjust_amount(q_a);
        // Adjust strengths of effects
        effects_a = copy(a->query_eat_effects());
        effects_m = copy(medium->query_eat_effects());
        eff = merge_effects(effects_a, effects_m, q_a, q_m);
        if (mapp(eff)) {
           foreach (key in keys(eff)) {
               medium->add_eat_effect(key, eff[key]);
           }
        }
        effects_a = copy(a->query_external_effects());
        effects_m = copy(medium->query_external_effects());
        eff = merge_effects(effects_a, effects_m, q_a, q_m);
        if(eff) {
          foreach (key in keys(eff)) {
            medium->add_external_effect(key, eff[key]);
          }
        }
        call_out("dest_substance", 0, a);
        return medium;
    } else {
        a->add_alias(medium_alias);
        return a;
    }
}

void check_reaction(object a) {
    int amt_a, need_amt_a, amt_b, need_amt_b;
    string name_a, name_b, i_a, i_b, msg, fcn;
    mixed *fcns;
    int i, j, amt_result;
    object x, *bs, ob_a, ob_b;
    float ratio;
    class reaction rcn;

    if (objectp(this_player()))
      debug_rh = this_player()->query_property("debug_rh");
    else
      debug_rh = 0;
    write_dbg("Entering reaction_handler.\n");
    name_a = a->query_medium_alias();
    if (!name_a || !a->query_continuous()) {
        write_dbg("Leaving reaction_handler because there is no A or it is not "
        "continous.\n");
        return; // Only bother if added item is a cont_medium
    }

    if ( a->query_medium_alias() == a->query_name() ) {
        debug_printf( "WARNING: Your cont_medium alias must "
          "not be the same as your name.\n" );
        return;
    }

    a = merge_cont_medium(a, name_a);
    bs = all_inventory(environment(a));
    debug_printf( "A is %O at %O. Bs are: %O.\n", a, environment( a ), bs );
    if ((sizeof(bs) < 2) || (!a->query_amount())) {
        write_dbg("Leaving reaction_handler because there is no B, or there is "
        "0 amounts of A.\n");
        return;
    }
    write_dbg("Checking the following: ");
    for (i = 0; i < sizeof(bs); i++) {
        write_dbg(bs[i]->query_medium_alias() + ", ");
    }
    write_dbg("\n");
    for (i = 0; i < sizeof(bs); i++) {
        // Check if a has been all used up yet.
        if (!a->query_amount()) {
            write_dbg("Leaving reaction_handler.\n");
            return;
        }
        // Find if a reacts with b
        name_b = bs[i]->query_medium_alias();
        if (!name_b || !bs[i]->query_continuous() || !bs[i]->query_amount()) {
            continue;
        }
        // The substance table is indexed in alphabetical order;
        // ob_a and ob_b are used to keep the two objects straight
        // whether or not they had to be swapped (mainly when dealing
        // with amounts).
        if (name_a < name_b) {
            i_a = name_a;
            i_b = name_b;
            ob_a = a;
            ob_b = bs[i];
        } else {
            i_a = name_b;
            i_b = name_a;
            ob_a = bs[i];
            ob_b = a;
        }
        write_dbg("Checking " + i_a + " against " + i_b + ".\n");
        if (!substance[i_a] || !substance[i_a][i_b]) {
            continue;
        }
        write_dbg("...they react.\n");
        rcn = substance[i_a][i_b];
        amt_a = ob_a->query_amount();
        amt_b = ob_b->query_amount();
        ratio = to_float(amt_a) / amt_b;
        if (intp(rcn->ratio)) {
            // These need to be floats
            rcn->ratio = to_float(rcn->ratio);
        }
        if (floatp(rcn->ratio)) {
            write_dbg("Single ratio...\n");
            if (ratio > rcn->ratio) {
                need_amt_b = amt_b;
                // Round up fractions (maybe this could be random?)
                need_amt_a = to_int(ceil(amt_b*rcn->ratio));
                write_dbg("Setting amt_a to "+ to_int(ceil(amt_b*rcn->ratio)) +".\n" );
            } else {
                need_amt_a = amt_a;
                need_amt_b = to_int(ceil(amt_a/rcn->ratio));
                write_dbg("Setting amt_b to "+ to_int(ceil(amt_a/rcn->ratio)) +".\n" );
            }
        } else if (arrayp(rcn->ratio) && (sizeof(rcn->ratio) == 2)) {
            // Ratio range
            // I've gotten confused by this, so here's my rationale:
            //   a/b should be in the range (r[0], r[1]).
            //   If above range, too much of a: take all of b; take as much of
            //     a as necessary to match upper ratio (ie, b*r[1]).
            //   If below range, too much of b: take all of a; take as much of
            //     b as necessary to match lower ratio (ie, a/r[0]).
            //   If in range, take all of both.
            write_dbg("Range of ratios...");
            if (intp(rcn->ratio[0])) {
                rcn->ratio[0] = to_float(rcn->ratio[0]);
            }
            if (intp(rcn->ratio[1])) {
                rcn->ratio[1] = to_float(rcn->ratio[1]);
            }
            if (ratio > rcn->ratio[1]) {
                write_dbg("above range...(" + ratio + ")\n");
                need_amt_b = amt_b;
                need_amt_a = to_int(ceil(amt_b*rcn->ratio[1]));
                write_dbg("Setting amt_a to "+ to_int(ceil(amt_b*rcn->ratio[1])) +".\n" );
                write_dbg("This is because amta is: "+ amt_b +" and ratio is "+ rcn->ratio[1] + ".\n" );
            } else if (ratio < rcn->ratio[0]) {
                write_dbg("below range...(" + ratio + ")\n");
                need_amt_a = amt_a;
                need_amt_b = to_int(ceil(amt_a/rcn->ratio[0]));
                write_dbg("Setting amt_b to "+ to_int(ceil(amt_a/rcn->ratio[0])) +".\n" );
                write_dbg("This is because amta is: "+ amt_a +" and ratio is "+ rcn->ratio[0] + ".\n" );
            } else {
                // Ratio within range, take all of both
                write_dbg("within range...(" + ratio + ")\n");
                need_amt_a = amt_a;
                need_amt_b = amt_b;
            }
        } else {
            error("Illegal reaction ratio; expecting float, int, or "
                  "array of two floats or ints.");
        }
        amt_result = to_int((need_amt_a + need_amt_b)*rcn->result_amt);
        write_dbg("...got ("+amt_a+","+amt_b+"), taking ("+need_amt_a+","+
                  need_amt_b+"), creating " + amt_result + ".\n");
        write_dbg("...creating "+rcn->result+".\n");
        // Create result object
        x = clone_object(rcn->result);
        if (rcn->result_amt) {
            if (function_exists("set_amount", x)) {
                x->set_amount(amt_result);
            } else {
                x->set_weight(amt_result);
            }
        }
        // Print out message when they mix.  This is pretty much
        // obsolete, since 'func' can do it also; but I kept it in
        // anyway.
        msg = rcn->message;
        if (msg) {
            msg = replace(rcn->message, ({
                "#env#", file_name(environment(a)),
                "#env2#", file_name(environment(environment(a))),
                "#obj_a#", file_name(ob_a),
                "#obj_b#", file_name(ob_b),
                "#obj_x#", file_name(x)
              }));
            tell_room(environment(environment(a)), msg);
        }
        // This is a back-door for any special functions that need
        // to be called.
        fcns = rcn->func;
        if (fcns) {
            for (j = 0; j < sizeof(fcns); j++) {
                if (stringp(fcns[j])) {
                    fcn = replace(fcns[j], ({
                        "#env#", file_name(environment(a)),
                        "#env2#", file_name(environment(environment(a))),
                        "#obj_a#", file_name(ob_a),
                        "#obj_b#", file_name(ob_b),
                        "#obj_x#", file_name(x)
                      }));
                } else if (functionp(fcns[j])) {
                    evaluate(fcns[j], ob_a, ob_b, x, environment(a),
                             environment(environment(a)), need_amt_a,
                             need_amt_b);
                }
            }
        }

        // Question: should I move the new object to the container
        // right now, or save it and add them all at once?  It might
        // make the order of reaction different; then again, it may
        // be arbitrary anyway.
        //x->move(environment(a));
        // I decided to postpone the move until after I leave the
        // handler.  Probably not necessary, but it makes debugging
        // easier.
        call_out("move_substance", 0, ({x, environment(a)}));

        // Decrease amounts and dest if gone
        ob_a->adjust_amount( -need_amt_a );
        ob_b->adjust_amount( -need_amt_b );

    }
    write_dbg("Leaving reaction_handler.\n");
}

void move_substance(object *ob) {
    // Used to move substance from a call_out
    int vol, vol_left, cont, closed, i, mv_stat;
    object *contents;

    if (!ob[0] || !ob[1]) return;
    vol_left = (int)ob[1]->query_max_volume() - (int)ob[1]->query_volume();
    if (cont = ob[0]->query_continuous()) {
        vol = ob[0]->query_amount();
    } else {
        vol = (int)ob[0]->query_weight()*200;
    }
    closed = ob[1]->query_closed();
    ob[1]->set_open();
    if ((mv_stat = ob[0]->move(ob[1])) != MOVE_OK) {
        write_dbg(sprintf("Move error (%d)...", vol));
        if ((vol > vol_left) && cont && !closed) {
            write_dbg("too much...\n");
            ob[0]->set_amount(vol_left);
            tell_room(environment(ob[1]), ob[0]->short(0) +
                      " leaks out of the " + ob[1]->short(0) + ".\n");
            mv_stat = ob[0]->move(ob[1]);
        }
        if (mv_stat == MOVE_OK) {
            if (closed) ob[1]->set_closed();
        } else {
            // Too big to fit! (or other problem)
            // Destroy container (unless living or a room) and contents.
            contents = all_inventory(ob[1]);
            write_dbg(sprintf("contents = %O\n", contents));
            for (i = 0; i < sizeof(contents); i++) {
                contents[i]->dest_me();
            }
            if (!living(ob[1]) && !function_exists("query_co_ord", ob[1])) {
                tell_room(environment(ob[1]), ob[1]->the_short(0) +
                          " explodes, splattering the contents all over.\n");
                //ob[1]->dest_me();
                ob[1]->move("/room/rubbish");
            }
            //ob[0]->dest_me();
            ob[0]->move("/room/rubbish");
        }
    } else {
        if (closed) ob[1]->set_closed();
    }
}

void dest_substance(object ob) {
    // Used to dest substance from a call_out.
    if (ob) {
        ob->dest_me();
    }
}

mixed *query_reaction(string name_a, string name_b) {
    // Return reaction parameters
    if (!substance[name_a] || !substance[name_a][name_b]) {
        return 0;
    }
    return substance[name_a][name_b];
}

mapping query_reactions() { return substance; }

// This should eliminate the need for the (now deleted) database manipulation
// functions.  Basically, all reactions in all files in the directory
// REACTIONS will be loaded.  This uses /obj/handler/data.  See the
// documentation on it and /include/reaction.h for the file format.

void restore_from_files() {
    string *files;
    int i;

    // This is the file with the #include and stuff that should only be
    // there once.
    files = ({ "base.hdr" });
    // These are the real data files
    files += get_dir(REACTIONS + "*.rcn");
    for (i = 0; i < sizeof(files); i++) {
        files[i] = REACTIONS + files[i];
    }
    //printf("files = %O\n", files);
    substance = "/obj/handlers/data"->compile_data(files);
}

void update_from_files(string fn) {
    mapping new_substance;
    string a, b;

    new_substance = "/obj/handlers/data"->compile_data( ({ fn }) );
    foreach (a in keys(new_substance)) {
      if (undefinedp(substance[a])) {
        substance[a] = ([ ]);
      }
      foreach (b in keys(new_substance[a])) {
        substance[a][b] = new_substance[a][b];
      }
    }
}
