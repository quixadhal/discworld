#include <bit.h>
#include <corpse.h>
#include <move_failures.h>

/* 200/5*30 seconds == 20 minutes */
#define DECAY_BASE 200
#define PLAYER_DECAY_BASE 600
#define RANDOM( x ) x[ random( sizeof( x ) ) ]


inherit "/std/container";
inherit "/std/living/carrying";
inherit "/std/basic/virtual_quit_control";

private int _decay;
private int _corpse_id;
private string _owner;
private string _race_ob;
private string _race_name;
private string *_bits_gone;
private string *_removed;
private string *_permitted;
private object _weapon;
private object *_armours;
private object *_holding;
private string _start_pos;
private string _nationality;

void set_decay_messages();
void remove_creator_corpse( object ob );

void set_race_ob(string _race_ob);
int query_corpse() {
  return 1;
}

/**
 * This method returns the id associated with the corpse.
 * @return the corpse id
 */
int query_corpse_id() {
   return _corpse_id;
}

int query_decay() {
  return _decay;
}
void set_decay(int dec) {
  _decay = dec;
}

string query_owner() {
  return _owner;
}

string *query_permitted() {
  return _permitted;
}
void set_race_name(string str) {
  _race_name = str;
}

string query_race_name() {
  return _race_name;
}

object make_bit(string which_bit);
object *make_bits(string *what_bits);
void give_permission(string);

string query_name() {
  if (!::query_name())
    return "someone";
  return::query_name();
}                               /* query_name() */

string *remove_array_parts(string *a1,
                           string *a2) {
  int i;
  string a;

  if (sizeof(a2))
    foreach(a in a2) {
    for (i = 0; i < sizeof(a1); i++) {
      if (a == a1[i]) {
        a1 = delete(a1, i, 1);
        break;
      }
    }
  }
  return a1;
}

void setup() {
  _bits_gone = ({ });
  _removed = ({ });
  add_property("cureable", 1);
  add_property("determinate", "the ");
  _owner = "noone";
  _race_name = "womble";
  _decay = DECAY_BASE;
  add_plural("corpses");
  add_plural("bodies");
  add_plural("carcasses");
  add_alias(({ "corpse", "body", "carcass" }));
  set_short("corpse");
  set_long("A corpse, it looks dead.\n");
  set_weight(STD_CORPSE_WEIGHT);
  set_race_ob("/std/races/unknown");
  _permitted = ({ });
  _armours = ({ });
  _holding = ({ });
  add_extra_look(this_object());
}                               /* setup() */

/** @ignore yes */
string extra_look() {
  if(sizeof(_removed))
    return "It appears to be missing its " + query_multiple_short(_removed) +
      ".\n";
  return "";
}

void give_permission(string words) {
  _permitted += ({ words });
}


int get(mixed dest) {
  if (query_property("player") && dest) {
    if (!this_player())
      return::get(dest);

    if (member_array((string) this_player()->query_name(), _permitted) ==
        -1)
      return MOVE_INVALID_DEST;
  }
  return::get(dest);
}

/*
 * This checks if someone is removing something from their own, or someone
 * elses corpse. If it's someone elses and they're on the permit list then
 * it allows the removal.
 * If they aren't on the permit list it passes it to /std/container's checks
 * which will prevent removal if one or other is not a PK or allow it if
 * they're both PK and record a theft event.
 *
 */
int test_remove(object thing,
                int flag,
                mixed dest) {
  int i;

  if (base_name(environment()) == "/room/rubbish") {
    return 1;
  }

  if (thing->query_property("my corpse") == this_object()) {
    return 1;
  }

  if (!query_property("player")) {
    return 1;
  }

  if (!sizeof(_permitted) || !this_player()) {
    i =::test_remove(thing, flag, dest);
  } else if (member_array((string) this_player()->query_name(), _permitted) ==
             -1) {
    i =::test_remove(thing, flag, dest);
  } else {
    write((string) thing->the_short() + " $V$0=buzzes,buzz$V$ for a "
          "moment.\n");
    return 1;
  }

  // Record looting of quest weapons. This is useful for tracking purposes.
  if (i && this_player() && thing &&
      sizeof(thing->effects_matching("mudlib.owned.weapon")) &&
      this_player()->query_name() != thing->query_owner() &&
      member_array((string) this_player()->query_name(), _permitted) == -1) {
    log_file("LOOT", "%s %s took %s [%s] from %s.\n",
             ctime(time()), this_player()->query_cap_name(),
             thing->query_short(), thing->query_owner(),
             this_object()->query_owner());
  }

  if (i) {
     CORPSE_HANDLER->save_corpse(this_object());
  }
  return i;
}                               /* test_remove() */

string long(string words,
            int dark) {
  if (dark == 2 || dark == -2) {
    if (query_living_contents(0) != "") {
      return::long(words, dark) + "Carrying, wearing or holding some "
        "things you cannot make out.\n";
    }
  }
  return::long(words, dark) + query_living_contents(0);
}                               /* long() */

/**
 * This method initially sets up the corpse object.  Generally
 * speaking words should be 0 and the 'thing' variable should be
 * set to the object which just died.
 * @param words the owner of the object
 * @param thing the object to get the ownership data from
 */
void set_owner(string words, object thing) {
  string det;
  string *tmp;

  if (stringp(words)) {
    _owner = words;
  } else {
    det = (string) thing->query_property("determinate");
    if (stringp(det)) {
      _owner = det + (string) thing->short();
    } else {
      _owner = add_a((string) thing->short());
    }
  }
  if (thing && thing->query_property("player")) {
    set_decay(PLAYER_DECAY_BASE);
    // the owner should always be permitted.
    give_permission(thing->query_name());
    catch(_corpse_id = CORPSE_HANDLER->query_next_corpse_id());
    _start_pos = thing->query_start_pos();
    set_ownership(thing->query_name());
    add_property("player", 1);
  }

  set_name("corpse");
  set_short("corpse of " + _owner);
  add_adjective(({ "corpse", "of" }));
  tmp = explode(lower_case(_owner), " ");
  if (sizeof(tmp) > 1) {
    add_alias(tmp[<1]);
  }
  add_adjective(tmp);
  add_adjective(tmp[<1] + "'s");

  if (thing) {
     _nationality = thing->query_nationality(); 
    if(userp(thing)) {
      set_main_plural("corpses of " + thing->short());
    } else  if (thing->query_main_plural() && !userp(thing)) {
      set_main_plural("corpses of " + (string) thing->query_main_plural());
    } else {
      set_main_plural("corpses of " + pluralize((string) thing->short()));
    }
  } else {
    set_main_plural("corpses");
  }
  set_long("This is the dead body of " + _owner + ".\n");
  if (thing && thing->query_weight(1)) {
    set_weight((int) thing->query_weight(1));
  } else {
    set_weight(STD_CORPSE_WEIGHT);
  }

  if ( thing && ( thing->query_creator() || 
    thing->query_property( "test character" ) ) ) 
  remove_creator_corpse( thing );
 
  BIT_CONTROLLER->add_bit(this_object());
  if (thing && thing->query_property("player")) {
    catch(CORPSE_HANDLER->register_corpse(this_object()));
  }
}                               /* set_owner() */

//Fun - I think we still do that.
void remove_creator_corpse( object ob ) {
  call_out( (: move( "/room/morgue", "$N lands in the room with a thud. The kind of thud a "
      "sack of steaks dropped from a great height would make.\n", "" ) :), 6 ); 
  
  call_out( (: tell_room( environment( $(ob) ), "squeeek THUMP squeeeek THUMP "
    "squeeek THUMP squeeeek THUMP\n" ) :), 1 );

  call_out( (: tell_room( environment( $(ob) ), "An igor pushing an old and "
    "battered wheelbarrow appears from somewhere.\n" ) :), 2 );

  call_out( (: tell_room( environment( $(ob) ), "%^CYAN%^The igor says \"Well bugger "
    "me. I've been wanting another one "
    "of those. The mathter will be so pleased.\"%^RESET%^\n" ) :), 4 );

  call_out( (: tell_room( environment( $(ob) ), "Igor cuts something off the corpse"
    " and stuffs it in his pocket before throwing the remains of the corpse into "
  "his barrow and shuffling away. \n" ) :), 6 );

  call_out( (: CORPSE_HANDLER->save_corpse(this_object()) :), 8 );  
  return;
}

/**
 * This is called to decay the corpse.  It is done in a continuous
 * call out loop until all of the decay has been completed.
 */
void do_decay() {
  int rate;

  if (!environment()) {
    return;
  }

  rate = 5 + (int) (environment()->query_property("decay rate"));
  if (rate > 0) {
    _decay -= rate;
  }
  set_decay_messages();
  if (_decay > 0) {
    CORPSE_HANDLER->save_corpse(this_object());
  }
}                               /* do_decay() */

/**
 * This method sets up the various 'states; of the corpse to show 
 * how decayed it is.
 */
void set_decay_messages() {
  if (!_race_name) {
    _race_name = (string) _race_ob->query_name();
  }
  switch (_decay) {
  case 101..PLAYER_DECAY_BASE:
    break;
  case 51..100:
    if (!find_player(query_name())) {
      set_short("somewhat decayed remains of " + add_a(query_name()));
      set_main_plural("somewhat decayed remains of " +
                      pluralize(query_name()));
    }
    break;
  case 1..50:
    set_short("decayed remains of " + add_a(_race_name));
    set_main_plural("decayed remains of " + pluralize(_race_name));
    set_long("This is the dead body of " + add_a(_race_name) + ".\n");
    break;
  default:
    CORPSE_HANDLER->deregister_corpse(this_object());
    set_ownership(0);
    all_inventory()->move(environment());
    move("/room/rubbish");
  }
}                               /* do_decay() */

void set_race_ob(string s) {
  _race_ob = s;
}

string query_race_ob() {
  return _race_ob;
}

string query_bit_left(string s) {
  string *bits;

  bits = _race_ob->query_possible_bits(s);
  if (!bits || !sizeof(bits)) {
    return 0;
  }
  bits = remove_array_parts(bits, _bits_gone);

  if (!sizeof(bits)) {
    return 0;
  }

  return bits[0];
}                               /* query_bit_left() */

string *query_bit_left_pl(string s) {
  string *bits;

  bits = _race_ob->query_possible_plural_bits(s);
  if (!bits || !sizeof(bits)) {
    return 0;
  }

  bits = remove_array_parts(bits, _bits_gone);
  if (!sizeof(bits)) {
    return 0;
  }

  return bits;
}                               /* query_bit_left() */

string *query_edible_bits_left() {
  string *bits;
  string *inedible;

  bits = _race_ob->query_possible_bits();
  inedible = _race_ob->query_all_inedible();
  if (!bits || !inedible) {
    return ({ });
  }
  return remove_array_parts(bits, _bits_gone) - inedible;
}                               /* query_edible_bits_left() */

varargs object *find_inv_match(string s,
                               object looker) {
  string bit;
  string *bits;
  object *weap;
  object wep;
  int cut;

  if (undefinedp(s)) {
    return all_inventory();
  }
  bit = query_bit_left(s);
  bits = query_bit_left_pl(s);
  if (!bit && !sizeof(bits)) {
    return all_inventory();
  }
  cut = 0;
  if (looker) {
    weap = looker->query_weapons();
  }
  if (sizeof(weap)) {
    foreach(wep in weap) {
      if (wep->id("dagger") || wep->id("knife")) {
        cut = 1;
      }
    }
  }
  if (bit) {
    if (cut || _race_ob->query_pluckable(bit)) {
      return ({ make_bit(bit) });
    }
    if (sizeof(weap)) {
      tell_object(looker, "You can only cut things from a corpse "
                  "with a knife or dagger.\n");
    } else {
      tell_object(looker, "You can't cut bits from a corpse with your "
                  "bare hands.\n");
    }
    return ({ });
  }
  if (sizeof(bits)) {
    if (cut) {
      if (sizeof(bits) > 5) {
        return make_bits(bits[0..4]);
      } else {
        return make_bits(bits);
      }
    }
    foreach(bit in bits) {
      if (!((string) _race_ob->query_pluckable(bit))) {
        bits -= ({ bit });
      }
    }
    if (sizeof(bits)) {
      if (sizeof(bits) > 5) {
        return make_bits(bits[0..4]);
      } else {
        return make_bits(bits);
      }
    }
    if (sizeof(weap)) {
      tell_object(looker, "You can only cut things from a corpse "
                  "with a knife or dagger.\n");
    } else {
      tell_object(looker, "You can't cut bits from a corpse with your "
                  "bare hands.\n");
    }
    return ({ });
  }
}                               /* find_inv_match() */

object make_bit(string which_bit) {
  mixed *bit;
  object bitobj;

  bit = _race_ob->query_bit(which_bit);
  if ((sizeof(bit[2][2]) > 1) && stringp(bit[2][2][0])) {
    bitobj = clone_object(bit[2][2][0]);
  } else {
    bitobj = clone_object("/std/bit");
  }
  bitobj->set_race_ob(_race_ob);
  if (_race_name) {
    bitobj->set_race_name(_race_name);
  } else {
    bitobj->set_race_name(_race_ob->query_name());
  }
  bitobj->set_corpse_weight(query_weight());
  if (!_race_ob->query_eat(bit[BIT_NAME])) {
    bitobj->set_bit(bit[0], 0);
  } else {
    bitobj->set_bit(bit[0], (_decay * 2) / 3);
  }

  _bits_gone |= bit[BIT_EXTRA][3..] + ({ bit[BIT_NAME] });
  _removed += ({ bit[BIT_NAME] });
  if (which_bit == "head") {
    //set_long(query_long() + "It is decapitated.\n");
    set_short("decapitated corpse of " + _owner);
    add_adjective("decapitated");
  }
  if (bitobj->move(this_object()) != MOVE_OK) {
    if (environment()) {
      bitobj->move(environment());
    }
  }
  bitobj->add_property("my corpse", this_object());
  return bitobj;
}                               /* make_bit() */

object *make_bits(string *what_bits) {
  string bit;
  object *bits = ({
  });

  foreach(bit in what_bits) {
    bits += ({ make_bit(bit) });
  }
  return bits;
}                               /* make_bits() */

string *query_bits_gone() {
  return _bits_gone;
}

mixed *add_bit_gone(string bit) {
  string *poss_bits;
  string tempbit;
  mixed *bit_details;

  poss_bits =
    remove_array_parts(_race_ob->query_possible_bits(bit), _bits_gone);
  if (!sizeof(poss_bits)) {
    return 0;
  }
  bit_details = _race_ob->query_bit(poss_bits[0]);
  _bits_gone += ({ bit_details[BIT_NAME] });
  foreach(tempbit in
          bit_details[BIT_EXTRA][3..sizeof(bit_details[BIT_EXTRA])]) {
    _bits_gone += ({ tempbit });
  }
  return bit_details;
}                               /* add_bit_gone() */

void set_bits_gone(string *bits) {
  int i;

  _bits_gone = ({ });
  for (i = 0; i < sizeof(bits); i++) {
    add_bit_gone(bits[i]);
  }
}                               /* set_bits_gone() */

string *query_bits_left() {
  int i;
  int j;
  string *all_bits;
  mixed *bits;

  bits = _race_ob->query_bits();
  all_bits = ({ });
  for (i = 0; i < sizeof(bits); i += 3) {
    if (bits[i + 2][2]) {
      for (j = 0; j < bits[i + 2][2][1]; j++) {
        all_bits += ({ bits[i] });
      }
    }
  }
  return remove_array_parts(all_bits, _bits_gone);
}                               /* query_bits_left */

/* this for formatting of objects sake */
object *query_armours() {
  int i;

  _armours -= ({ 0 });
  for (i = 0; i < sizeof(_armours); i++) {
    if ((object) _armours[i]->query_worn_by() != this_object()) {
      _armours = delete(_armours, i, 1);
      i--;
    }
  }
  return _armours + ({ });
}                               /* query_armours() */

object *query_wearing() {
  return query_armours();
}

void set_armours(object * things) {
  int i;

  _armours = ({ });
  for (i = 0; i < sizeof(things); i++) {
    if (things[i]->query_no_limbs()) {
      _holding += ({ things[i] });
    } else {
      _armours += ({ things[i] });
    }
  }
}                               /* set_armours() */

void remove_armour(object arm) {
  _armours -= ({ arm });
}

object query_weapon() {
  return _weapon;
}

void set_weapon(object wpn) {
  _weapon = wpn;
}

void unwield_weapon(object wpn) {
  if (_weapon == wpn)
    _weapon = 0;
}

object *query_holding() {
  return _holding;
}

void set_holding(object * hold) {
  _holding += hold;
}

int *set_unhold(object ob) {
  int pos;

  if ((pos = member_array(ob, _holding)) == -1) {
    return ({ });
  }
  if (!ob->set_holder(0)) {
    return ({ });
  }
  _holding = _holding - ({ ob });
  return ({ pos });
}                               /* set_hold() */

int *set_hold(object ob,
              int pos) {
  if (member_array(ob, _holding) != -1) {
    return ({ });
  }
  _holding += ({ ob });
  return ({ pos });
}                               /* set_hold() */

int move_or_destruct(object dest) {
  if (objectp(dest)) {
    move_object(dest);
  } else {
    move("/room/void");
  }
  return 1;
}

/*
 * This was added so that people dont loose stuff just in case it gets
 * dested for weird reasons.
 * Pinkfish 3rd may 1993
 */
void dest_me() {
  mixed *xp;
  object ob;

  // This hands out the rest of the death Xp when the object is destructed
  // hopefully by burial or ventisepelating or whatever.
  xp = query_property("XP");
  if (xp && sizeof(xp) == 2) {
    foreach(ob in xp[0]) {
      if (ob) {
        ob->adjust_xp(xp[1], 1);
      }
    }
  }

  BIT_CONTROLLER->remove_bit(this_object());
  if (environment()) {
    all_inventory()->move(environment());
  }
  ::dest_me();
}                               /* dest_me() */

/*
 * Added in save code - Tue Feb 25 00:57:06 WST 1997
 * to do wombly stuff by Pinkfish.
 * Only does a bit of wombley stuff though.
 */
mapping query_static_auto_load() {
  return int_query_static_auto_load();
}                               /* query_static_auto_load() */

mapping query_dynamic_auto_load() {
  mapping tmp;
  string pos;

  tmp = ([ "::" : ::query_dynamic_auto_load(),
           "decay" : _decay,
           "owner" : _owner,
           "race ob" : _race_ob,
           "id" : _corpse_id,
           "last pos" : pos,
           "start pos" : _start_pos,
           "nationality" : _nationality,
           "ownership" : query_ownership(),
           "race name" : _race_name, "bits gone" : _bits_gone, ]);
  return tmp;
}                               /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping map, object) {
  if (map["::"]) {
    ::init_dynamic_arg(map["::"]);
  }
  if (map["id"]) {
    _corpse_id = map["id"];
  }
  if (map["decay"]) {
    _decay = map["decay"];
  }
  if (map["owner"]) {
    _owner = map["owner"];
  }
  if (map["race ob"]) {
    _race_ob = map["race ob"];
  }
  _nationality = map["nationality"];
  set_ownership(map["ownership"]);
  if (map["race name"]) {
    _race_name = map["race name"];
  }
  if (map["bits gone"]) {
    _bits_gone = map["bits gone"];
  }
  if (map["start pos"]) {
    _start_pos = map["start pos"];
  }
  set_decay_messages();
}                               /* init_dynamic_arg() */

/**
 * This is used by the corpse handler to get the data used to save
 * the corpse.
 */
string query_save_data() {
   string pos;
   object env;

   env = environment();
   while (env && environment(env)) {
      if (living(env) && env->query_property("player")) {
         return CORPSE_IN_PLAYER;
      }
      env = environment(env);
   }
   // First see if we are inside a living or something.
   pos = find_start_pos( this_object(), env );
   return save_variable(({ query_dynamic_auto_load(), query_static_auto_load(), pos }));
}

/**
 * This method is called by the corpse handler to setup the corpse
 * properly after it loads.
 */
void setup_corpse_from_save(string str_data, string name, int id) {
  mixed data;
  int res;

  if (str_data == CORPSE_IN_PLAYER) {
     return ;
  }

  data = restore_variable(str_data);

  // Restore the data.
  init_static_arg(data[1]);
  init_dynamic_arg(data[0], 0);

  // Make sure we have decay called on us
  BIT_CONTROLLER->add_bit(this_object());

  // Move the thing to the right spot.
  res = MOVE_NO_DROP;
  tell_creator("pinkfish", "Trying to restore to location %O [%O]\n", data[2], this_object());
  catch(res = this_object()->move(data[2],
                         "$N looks confused and pops out of the ground.\n",
                         "$N looks confused and pops out of the ground.\n"));
  if (res != MOVE_OK) {
     tell_creator("pinkfish", "Trying to restore to location %O [%O]\n", _start_pos, this_object());
     catch(res = this_object()->move(_start_pos,
                         "$N looks confused and pops out of the ground.\n",
                         "$N looks confused and pops out of the ground.\n"));
  }
  if (res != MOVE_OK) {
     tell_creator("pinkfish", "Trying to restore to location %O [%O]\n", "/room/rubbish", this_object());
     move("/room/rubbish");
  }
}

mixed *stats() {
  return::stats() +
    ({ ({ "decay", _decay, }), ({ "owner", _owner, }),
         ({ "race ob", _race_ob, }), ({ "race name", _race_name, }) });
}                               /* stat() */

/**
 * @ignore yes
 * This is added so that it acts like a living object and
 * things like non-movable signs cannot be added to it.
 */
int test_add(object ob,
             int flag) {
  // Allow bits in at any time...
  if (ob->query_bit_data()) {
    return 1;
  }

  return !flag;
}                               /* test_add() */

/**
 * @ignore yes
 * Stop people form getting stuff out of containers in corpses.
 */
int can_find_match_reference_inside_object(object thing,
                                           object player) {

  if (!query_property("player")) {
    return 1;
  }

  if (member_array(player->query_name(),
                   _permitted + ({ lower_case(_owner) })) == -1)
    {
      return 0;
    }

  return 1;
}                               /* can_find_match_reference_inside_object() */

int can_find_match_recurse_into(object player) {
  return 0;
} /* can_find_match_recurse_into() */

