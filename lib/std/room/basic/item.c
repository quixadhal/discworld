/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: item.c,v 1.18 2001/05/30 01:27:24 presto Exp $
 *
 *
 */
#define DEBUG 1;
#include <parse_command.h>
#include <obj_parser.h>

#define POSITION_SIZE 2
#define POSITION_DESC 0
#define POSITION_MULT 1

private mixed *_lng;
private string *_shrt;
private string *_adjs;
private string *_plu;
private string *_name;
#ifdef DEBUG
private string _creator_room;
#endif
private mapping _verb;
private mapping _plural;
private mapping _other_things;
private mapping _pattern;
private int *_cur_desc;
private mapping _gather;
private mapping _position_stuff;
private mapping _count;

/** @ignore yes */
void init() {
    string name;
    mixed rest;

    foreach (name, rest in _other_things) {
//        if (name != "get") {
            this_player()->add_command(name, this_object(), _pattern[name]);
//        }
    }
} /* init() */

#ifdef DEBUG
string query_creator_room() { return _creator_room; }

void print_stuff() {
    // Debugging function
    printf("lng = %O\nshrt = %O\nadjs = %O\nplu = %O\nname = %O\n" +
      "verb = %O\nplural = %O\nother_things = %O\npattern = %O\n" +
      "cur_desc = %O\ngather = %O\n", _lng, _shrt, _adjs, _plu, _name, _verb,
      _plural, _other_things, _pattern, _cur_desc, _gather);
}
#endif

void create() {
#ifdef DEBUG
    _creator_room = file_name(previous_object());
#endif
    _adjs = ({ });
    _lng = ({ "" });
    _shrt = ({ "" });
    _gather = ([ ]);
    _plu = ({ });
    _name = ({ });
    _verb = ([ ]);
    _plural = ([ ]);
    _pattern = ([ ]);
    _cur_desc = ({ });
    _count = ([ ]);
    _other_things = ([ "smell" : 0, "taste" : 0, "read" : 0 ]);
} /* create() */

/** @ignore yes */
int query_visible( object thing ) { return 1; }

/** @ignore yes */
string hide_invis_string() { return ""; }

/**
 * This method returns the base array of shorts to be processed in the
 * other short methods.  This is the short without the leading
 * 'the', 'a' whatever...
 * @return the array of shorts
 */
string *query_short_array() {
    string *ret;
    int i;

    ret = ({ });
    foreach (i in _cur_desc) {
        ret += ({ _shrt[i] });
    }
    return ret;
} /* query_short_array() */

/** @ignore yes */
string short() {
    return query_multiple_short(query_short_array());
} /* short() */

/** @ignore yes */
string pretty_short() {
    string *ret;

    ret = query_short_array();
    if (sizeof(ret)) {
        //return ret[ 0 ];
        return query_multiple_short(ret);
    } else {
        return "something";
    }
} /* pretty_short() */

/** @ignore yes */
string a_short() {
    return implode(map(query_short_array(),
        (: "$mirror_short:" + add_a( $1 ) + "$" :)), "");
} /* a_short() */

/** @ignore yes */
string the_short() {
    return implode(map(query_short_array(),
        (: "$mirror_short:the " +  $1 + "$" :)), "");
} /* the_short() */

/** @ignore yes */
string one_short() {
    return implode(map(query_short_array(),
        (: "$mirror_short:the " + $1 + "$" :)), "");
} /* one_short() */

/** @ignore yes */
string poss_short() {
    return implode(map(query_short_array(),
        (: "$mirror_short:the " + $1 + "$" :)), "");
} /* poss_short() */

/**
 * @ignore 
 * This is used to make the reads of the items not fail.
 */
string query_read_short() {
    return "$name$";
} /* query_read_short() */

/** @ignore yes */
string query_plural() {
    string *ret;
    int i;

    ret = ({ });
    foreach (i in _cur_desc) {
        ret += ({ pluralize(_shrt[i]) });
    }
    return query_multiple_short(ret);
} /* query_plural() */

/** @ignore yes */
string pretty_plural() {
    int i;

    foreach (i in _cur_desc) {
        return pluralize(_shrt[i]);
    }
    return 0;
} /* query_plural() */

/** @ignore yes */
string long( string name, int dark ) {
    int i;
    string *ret;

    ret = ({ });
    foreach (i in _cur_desc) {
        if ( !_lng[ i ] ) {
            continue;
        }
        if (functionp(_lng[ i ])) {
            ret += ({ evaluate(_lng[ i ]) });
        } else {
            ret += ({ _lng[ i ] });
        }
    }
    if ( !sizeof( ret ) ) {
        return "You don't think that the "+name+" is here.\n";
    }
    return implode( evaluate(ret), "\n")+"\n";
} /* long() */

/*
void set_long(string s) { _lng[_cur_desc] = s; }
string query_long() { return _lng[_cur_desc]; }
 */
/**
 * This returns the currently matching indexes for the item object.
 * @return the array of currently matchig indexes
 */
int *query_cur_desc() { return _cur_desc; }
/**
 * This method returns the verbs mapping used in the item object.
 * @return the verbs in the item object
 */
mapping query_verbs() { return _verb; }
/**
 * This method returns the plurals mapping used in the item object.
 * @return the plurals in the item object
 */
mapping query_plurals() { return _plural; }

/**
 * This method returns all the long descriptions for the item object.  The
 * positions in the array correspond to the index used to reference the
 * items.
 * @return the array of long descriptions
 */
string *query_lng() { return _lng; }
/**
 * This method returns all the short descriptions for the item object.  The
 * positions in the array correspond to the index used to reference the
 * items.
 * @return the array of long descriptions
 */
string *query_shrt() { return _shrt; }

/**
 * This method figures out what gatherables are available for the
 * current description set.  The array contains the values passed
 * into the add_item stuff with the "gather" index.
 * @return the current gatherables
 */
mixed *query_gather() {
    mixed *g = ({ });
    int i;

    foreach (i in _cur_desc) {
        if (_gather[i]) {
            g += ({ _gather[i] });
        }
    }
    return g;
} /* query_gather() */

/** @ignore yes */
int drop() { return 1; }
/** @ignore yes */
int get() { return 1; }

/** @ignore yes */
int query_item(string str) { return _verb[str]; }

/**
 * This method setups all the internal stuff, including the long
 * description.
 * @param bits the bits to look through
 * @param index the index which refers to the item in question
 */
private void setup_bits_for_item(int index, mixed bits) {
    int i;
    string str;
    mixed pat;

    if (!pointerp(bits)) {
        _lng[index] = bits;
        return ;
    }

    _lng[index] = "You see nothing special.\n";
    for (i = 0; i < sizeof(bits); i += 2) {
        /*
         * Figure out what the pattern should be.  We should have apattern
         * specified if the second element of the array is a function
         * pointer, or the array is three long.
         */
        if ((arrayp( bits[i + 1] ) && sizeof( bits[i + 1] ) == 3) ||
          (sizeof( bits[i + 1] ) == 2 && functionp( bits[i + 1][0] ))) {

            pat = bits[i + 1][<1];

            if (!arrayp(pat)) {
                pat = ({ pat });
            }        
        }    
        else {
            pat = ({ "<direct:object>" });
        }

        /* Ok.   Now check the other bits. */
        if (!pointerp(bits[i])) {
            bits[i] = ({ bits[i] });
        }

        /* Go through and add all the relevant data for the bit in. */
        foreach (str in bits[i]) {
            /* The real long description. */
            if (str == "long") {
                _lng[index] = bits[i+1];
            } else if (str == "gather") {
                // I'm not completely certain this is right; I don't
                // quite understand where we are here...
                _gather[index] = bits[i+1];
            } else if (str == "item_count")  {
               if (!_count[index])  _count[index] = bits[i + 1];
            } else if (str == "position" ||
              str == "position multiple") {
                if (!_position_stuff)
                    _position_stuff = ([ ]);
                if (!_position_stuff[index]) {
                    _position_stuff[index] = allocate(POSITION_SIZE);
                }
                if (str == "position") {
                    _position_stuff[index][POSITION_DESC] = bits[i + 1];
                } else {
                    _position_stuff[index][POSITION_MULT] = bits[i + 1];
                }
            } else if (!_other_things[str]) {
                /* A command, hide it away... */
                _pattern[str] = pat;
                _other_things[str] = ([ index : bits[i+1] ]);
            } else {
                if (member_array(pat, _pattern[str]) == -1) {
                    _pattern[str] += pat - (string *)_pattern[str];
                }
                _other_things[str][index]   = bits[i+1];
            }
        }
    }
} /* setup_bits_for_item() */

/**
* This is a helper function for the delete function, it removes everything
* not in the standard static arrays.
* @param index the index to remove
*/
private void remove_bits_for_item(int index) {
    string str;
    string adj;
    mixed value;
    int frog;
    int bing;
    int i;
    int j;

    /* Fix up the gather mapping */
    map_delete(_gather, index);
    foreach (frog, bing in _gather) {
        if (frog > index) {
            map_delete(_gather, frog);
            _gather[frog - 1] = bing;
        }
    }

    /* Fix up the position mapping */
    if (_position_stuff) {
        map_delete(_position_stuff, index);
        foreach (frog, bing in _position_stuff) {
            if (frog > index) {
                map_delete(_position_stuff, frog);
                _position_stuff[frog - 1] = bing;
            }
        }
    }

    /* Fix up the parse_command mapping. */
    foreach (str, value in _other_things) {
        if (value) {
            map_delete(value, index);
            if (!sizeof(value)) {
                map_delete(_other_things, str);
                map_delete(_pattern, str);
            } else {
                foreach (frog, bing in value) {
                    if (frog > index) {
                        map_delete(value, frog);
                        value[frog - 1] = bing;
                    }
                }
            }
        }
    }

    /* Fix up the verbs array */
    foreach (str, value in _verb) {
        for (i = 0; i < sizeof(value); i += 2) {
            if (value[i + 1] > index) {
                value[i + 1] -= 1;
            } else if (value[i + 1] == index) {
                foreach (adj in value[i]) {
                    j = member_array(adj, _adjs);
                    if (j != -1) {
                        _adjs = _adjs[0..i - 1] + _adjs[i + 1..];
                    }
                }
                value = value[0.. i - 1] + value[i + 2..];
                i -= 2;
                j = member_array(str, _name);
                if (j != -1) {
                    _name = _name[0..j - 1] + _name[j + 1..];
                }
            }
        }
        _verb[str] = value;
    }

    /* Fix up the plural array */
    foreach (str, value in _plural) {
        for (i = 0; i < sizeof(value); i += 2) {
            if (value[i + 1] > index) {
                value[i + 1] -= 1;
            } else if (value[i + 1] == index) {
                value = value[0.. i - 1] + value[i + 2..];
                i -= 2;
            }
        }
        _plural[str] = value;
    }
} /* remove_bits_for_item() */

/**
 * This method sets up all the parse command handling stuff that is needed
 * for the given name.
 * @param index the index to associate it with
 * @param name the name to add in 
 * @param no_plural do not add in any plurals
 */
private void add_name_reference(int index, string name, int no_plural) {
    string *bits;
    string plural;
    string s;

    bits = explode(name, " ");
    s = bits[<1];
    if (member_array(s, _name) == -1) {
        _name += ({ s });
    }

    /* First setup the name. */
    if (!_verb[s]) {
        _verb[s] = ({ bits[0..<2], index });
    } else {
        _verb[s] += ({ bits[0..<2], index });
    }

    /* Now setup the plural */
    if (!no_plural) {
        plural = pluralize(s);
        if (member_array(plural, _plu) == -1) {
            _plu += ({ plural });
        }
        if (!_plural[plural]) {
            _plural[plural] = ({ bits[0..<2], index });
        } else {
            _plural[plural] += ({ bits[0..<2], index });
        }
    }

    /* The adjective. */
    _adjs += bits[0..<2];
} /* add_name_reference() */

/**
 * This method setups the item initially.   This is called by the
 * add_item method in the room itself.
 * @param nam the name(s) to add the item for
 * @param long the long description of the item
 * @param no_plural do not add a plural flag
 * @return the index associated with the item
 */
int setup_item(mixed nam, mixed long, int no_plural) {
    int index;
    string str;

    index = sizeof(_lng);
    if (pointerp(nam)) {
        if (sizeof(nam) > 0) {
            _shrt += ({ nam[0] });
            if (no_plural) {
                _plu += ({ "no plural" });
            } else {
                _plu += ({ pluralize(nam[0]) });
            }
        }
        foreach (str in nam) {
            add_name_reference(index, str, no_plural);
        }
    } else {
        _shrt += ({ nam });
        if (no_plural) {
            _plu += ({ "no plural" });
        } else {
            _plu += ({ pluralize(nam) });
        }
        add_name_reference(index, nam, no_plural);
    }

    _lng += ({ 0 });
    setup_bits_for_item(index, long);
    return index;
} /* setup_item() */

/**
 * This method non-destructively modifies the items values.  It will not
 * remove the value for this item, remember this!  If you add again
 * and again you will end up with multiple of the same object.
 * @see /std/room->modify_item_by_name()
 * @param str the name to reference the object by
 * @param long the long bits to change on the item
 * @return 1 on success, 0 on failure
 */
int modify_item_by_index(int index, mixed long) {
    if (index >= sizeof(_lng)) {
        return 0;
    }

    /* Got a match... */
    setup_bits_for_item(index, long);
    return 1;
} /* modify_item_by_index() */

/**
 * This method non-destructively modifies the items values.  It will not
 * remove the value for this item, remember this!  If you add again
 * and again you will end up with multiple of the same object.
 * @see /std/room->modify_item_by_name()
 * @param str the name to reference the object by
 * @param long the long bits to change on the item
 * @return 1 on success, 0 on failure
 */
int modify_item_by_name(string str, mixed long) {
    int index;

    index = member_array(str, _shrt);
    if (index == -1) {
        return 0;
    }

    return modify_item_by_index(index, long);
} /* modify_item_by_name() */

/** @ignore yes */
int modify_item(mixed str, mixed long) {
    if (stringp(str)) {
        return modify_item_by_name(str, long);
    }
    if (intp(str)) {
        return modify_item_by_index(str, long);
    }
    return 0;
} /* modify_item() */

/**
 * This method removes an item by the returned index number from the
 * add_item method.
 * @param index the index number to remove
 * @return 1 on success, 0 on failure
 * @example
 * int _item_number;
 *
 * void bing {
 *    _item_numer = add_item("frog", "It wombles!");
 * } /\* bing() *\/
 * 
 * void remove_bing() {
 *    remove_item(_item_number);
 * } /\* remove_bing() *\/
 */
int remove_item_by_index(int index) {
    if (index >= sizeof(_lng)) {
        return 0;
    }

    _shrt = _shrt[0..index - 1] + _shrt[index + 1..];
    _adjs = _adjs[0..index - 1] + _adjs[index + 1..];
    _lng = _lng[0..index - 1] + _lng[index + 1..];
    _plu = _plu[0..index - 1] + _plu[index + 1..];

    remove_bits_for_item(index);
} /* remove_item_by_index() */

/**
 * This method removes an item by the short description.
 * @param index the index number to remove
 * @return 1 on success, 0 on failure
 */
int remove_item_by_name(string str) {
    int index;

    index = member_array(str, _shrt);
    if (index == -1) {
        return 0;
    }

    return remove_item_by_index(index);
} /* remove_item_by_name() */

/** @ignore yes */
int remove_item(mixed str) {
    if (stringp(str)) {
        return remove_item_by_name(str);
    }
    if (intp(str)) {
        return remove_item_by_index(str);
    }
    return 0;
} /* remove_item() */

/** @ignore yes */
string *parse_command_id_list() { return _name; }
/** @ignore yes */
string *parse_command_plural_id_list() { return _plu; }
/** @ignore yes */
string *parse_command_adjectiv_id_list() { return _adjs; }

// Okay... Sibbis fixed all sorts of small bugs in this... 02/18/1995
//      (BTW: THIS IS UGLY!!!!)
/** @ignore yes */
object query_parse_id(mixed *arr)
{
    string *bits;
    mixed stuff;
    int i, j, match, all_match;

    bits = explode(arr[P_STR], " ");
    bits -= ({ "a", "an", "the" });
    /* all case */
    if (arr[P_THING] == 0)
    {
        stuff = _plural[bits[<1]];
        if (!stuff) {
            stuff = _verb[bits[<1]];
            if (!stuff) {
                return 0;
            }
        }
        _cur_desc = ({ });
        all_match = 0;
        for (j = 0; j < sizeof(stuff); j += 2) {
            match = 1;
            for (i = 0; i < sizeof(bits) - 1; i++) {
                if (member_array(bits[i], stuff[j]) == -1) {
                    match = 0;
                    break;
                }
            }
            if (match) {
                all_match = 1;
                if (member_array(stuff[j+1], _cur_desc) == -1) {
                    _cur_desc += ({ stuff[j+1] });
                }
            }
        }
        if (all_match) {
            return this_object();
        } else {
            return 0;
        }
    }

    /* specific object case */
    if (arr[P_THING] < 0) {
        stuff = _verb[bits[<1]];
        if (!stuff) {
            return 0;
        }
        for (j = 0; j < sizeof(stuff); j += 2) {
            match = 1;
            for (i = 0; i < sizeof(bits) - 1; i++) {
                if (member_array(bits[i], stuff[j]) == -1) {
                    match = 0;
                    break;
                }
            }

            if (++arr[P_THING] != 0) {
                continue;
            }

            /* Get the current thingy out of the list */
            if (match) {
                _cur_desc = ({ stuff[j+1] });
                arr[P_THING] = -10321;
                return this_object();
            }
        }
        return 0;
    }

    /* Lots of objects case.   The objects are specified though. */
    stuff = _plural[bits[<1]];
    if (!stuff) {
        stuff = _verb[bits[<1]];
        if (!stuff) {
            return 0;
        }
    }

    _cur_desc = ({ });
    for (j = 0; j < sizeof(stuff); j += 2) {
        match = 1;
        for (i=0;i<sizeof(bits)-1;i++) {
            /*
             * Modified by Deutha to allow reference to "a thing", "an object", etc.
             */
            if ( member_array( bits[ i ], stuff[ j ] ) == -1 ) {
                match = 0;
                break;
            }
        }
        if (match) {
            if (member_array(stuff[j+1], _cur_desc) == -1) {
                _cur_desc += ({ stuff[j+1] });
            }
            arr[P_THING]--;
            if (arr[P_THING] <= 0) {
                arr[P_THING] = -10786;
                return this_object();
            }
        }
    }

    // return this_object();
    return 0;
} /* query_parse_id() */

/** @ignore yes */
mixed* parse_match_object(string* input,
                          object player,
                          class obj_match_context context) {
   int *stuff;
   mixed *plural;
   mixed *sing;
   string *its, *thems;
   string item;
   int i;
   int j;
   int match;
   int ret;

   // debug_printf( "Input: %O\nPlayer: %O\nContext: %O\n", input, player, context );

   if (input[<1] == "here" && sizeof(input) > 1) {
      input = input[0..<2];
   }

   /* context */
   if ( this_object() == context->it &&
        input[<1] == "it" ) {

      sing = ({ });

      its = map(_cur_desc, (: explode(_shrt[$1], " ")[<1] :));

      foreach( item in its ) {
          if ( arrayp( _verb[ item ] ) ) {
             sing += ({ ({ item }), _verb[ item ][ 1 ] });   
          }
      }

      plural = ({ });
   } else if( input[<1] == "them" &&
              context->plural &&
              member_array( this_object(), context->plural ) != -1) {

      plural = ({ });

      thems = map(_cur_desc, (: explode(_shrt[$1], " ")[<1] :));

      foreach( item in thems ) {
          if ( arrayp( _verb[ item ] ) ) {
             plural += ({ ({ item }), _verb[ item ][ 1 ] });   
          }
      }

      sing = ({ });
   } else {
      plural = _plural[input[<1]];
      sing = _verb[input[<1]];
   }

   if (!sizeof(plural) && !sizeof(sing)) {
      return 0;
   }

   stuff = ({ });
   //
   // Check sinular matches.
   //
   for (i = 0; i < sizeof(sing); i += 2) {
      match = 1;
      for (j = 0; j < sizeof(input) - 1; j++) {
         if (member_array(input[j], sing[i]) == -1) {
            match = 0;
            break;
         }
      }
      if (match) {
         // debug_printf( "Singular: %O\nStuff: %O\nContext: %O\n", sing, stuff, context );

         if (member_array(sing[i+1], stuff) == -1) {
            stuff += ({ sing[i+1] });
            ret |= OBJ_PARSER_MATCH_SINGULAR;
         }
      }
   }
   //
   // Check plural matches.
   //
   for (i = 0; i < sizeof(plural); i += 2) {
      match = 1;
      for (j = 0; j < sizeof(input) - 1; j++) {
         if (member_array(input[j], plural[i]) == -1) {
            match = 0;
            break;
         }
      }
      if (match) {
         if (member_array(plural[i+1], stuff) == -1) {
            stuff += ({ plural[i+1] });
            ret |= OBJ_PARSER_MATCH_PLURAL;
         }
      }
   }


   if (sizeof(stuff)) {
       //
       // We matched, so see if we should have.
       //
       if (context->ordinal) {
          if (context->ordinal > sizeof(stuff)) {
             context->ordinal -= sizeof(stuff);
             return 0;
          }
          context->ignore_rest = 1;
          context->ordinal--;

          _cur_desc = stuff[context->ordinal..context->ordinal];
       } else if (context->number_included) {
          _cur_desc = stuff[0..context->number_included];
          context->number_included -= sizeof(stuff);
          if (context->number_included <= 0) {
             context->ignore_rest = 1;
          }
       } else if (ret & OBJ_PARSER_MATCH_PLURAL) {
          _cur_desc = stuff;
       } else {
          _cur_desc = stuff[0..0];
       }

       return ({ ret, ({ this_object() }) });
   } else {
      return 0;
   }
} /* parse_match_object() */

/** @ignore yes */
void dest_me() {
    destruct(this_object());
    return ;
} /* dest_me() */

/** @ignore yes */
void dwep() {
    destruct(this_object());
    return ;
} /* dwep() */

/** @ignore yes */
int move() { return 1; }

/** @ignore yes */
int command_control(string command, object *indir, string id_arg,
  string ii_arg, string *args, string pattern) {

    int desc, size, num;
    mixed temp;

    if (!_other_things[command]) {
        return 0;
    }

    size = sizeof(_cur_desc);

    foreach (desc in _cur_desc) {
        temp = _other_things[command][desc];

        if (arrayp(temp)) {
            if (sizeof(temp) == 3 || (sizeof(temp) == 2 && !functionp(temp[0]))) {
                num += call_other(temp[0], temp[1], command, indir, id_arg, 
                  ii_arg, args, pattern);                
            }
            else { 
                if (sizeof(temp) == 2 && functionp(temp[0])) {
                        num += evaluate(temp[0], command, indir, id_arg, ii_arg, 
                            args, pattern);                                
                }    
            }
        }        
        else {
            if (functionp(temp)) {
//                num += evaluate(temp, command, indir, id_arg, ii_arg, args, 
//                    pattern);
                temp = evaluate(temp, command, indir, id_arg, ii_arg, args, 
                    pattern);
            }            
//            else {
            // If we have a string, then we're going to display it to the
            // player. 
            if (stringp(temp)) {
                write(temp);
                num++;
                add_succeeded_mess(({ "", "$N $V $D.\n" }));
            }
            else { 
                // If we got an integer instead, we have to return
                // num. 

                if ( intp( temp ) ) {
                    num += temp;
                }
            }
//            }

        }
    }    

    return num;
} /* command_control() */

/** @ignore yes */
mapping query_other_things() { return _other_things; }

/** @ignore yes */
mapping query_pattern() { return _pattern; }

/** @ignore yes */
string query_position_string(string pos) {
    int i;

    if (!_position_stuff) {
        return 0;
    }

    for (i = 0; i < sizeof(_cur_desc); i++) {
        if (_position_stuff[_cur_desc[i]]) {
            return _position_stuff[_cur_desc[i]][POSITION_DESC];
        }
    }
    return 0;
} /* query_position_string() */

/** @ignore yes */
int query_position_multiple(string pos) {
    int i;

    if (!_position_stuff) {
        return 0;
    }

    for (i = 0; i < sizeof(_cur_desc); i++) {
        if (_position_stuff[_cur_desc[i]]) {
            return _position_stuff[_cur_desc[i]][POSITION_MULT];
        }
    }
    return 0;
} /* query_position_multiple() */

/** @ignore yes */
mapping query_position_stuff() { return _position_stuff; }

int query_count()  {
   int i;
   int ret = 0;

   foreach (i in _cur_desc)  {
      ret += evaluate(_count[i]);
   }
   return ret;
}
