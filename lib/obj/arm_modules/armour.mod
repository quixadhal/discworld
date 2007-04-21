void set_wear_remove_func(string ob, string func) {
  wear_remove_func = ({ ob, func });
} /* set_wear_remove_func() */

void set_wear_effect( string effect_name ) {
  wear_effect = effect_name;
} /* set_wear_effect() */

string query_wear_effect() { return wear_effect; }
 
int query_armour() { return 1; }
 
int query_ac( string type, int amount ) {
  int i, arm_class, damage;
  arm_class = ::query_ac( type, amount );
  if ( arm_class > 100 )
    arm_class = max_ac;
  else
    arm_class = ( arm_class * max_ac ) / 100;
  arm_class += ( arm_class * enchanted ) / query_max_enchant() + enchanted;
  if ( member_array( type, immune_to ) == -1 )
    adjust_cond( ( -2 * random( damage_chance ) * amount ) / 1000 );
  return ( arm_class * cond ) / max_cond;
} /* query_ac() */
 
void setup_armour(int c) {
  set_cond(c);
  set_max_cond(c);
  set_lowest_cond(c);
} /* setup_armour() */
 
void set_damage_chance(int i) { damage_chance = i; }
 
int set_worn_by( object thing ) {
  if ( sizeof( wear_remove_func ) ) {
    if ( thing != worn_by ) {
      if ( worn_by )
        call_other( wear_remove_func[ 0 ], wear_remove_func[ 1 ], 0 );
      if ( thing )
        call_other( wear_remove_func[ 0 ], wear_remove_func[ 1 ], thing );
    }
  }
  if ( ( thing != worn_by ) && thing && wear_effect )
    thing->add_effect( wear_effect );
  if ( ( thing != worn_by ) && worn_by && wear_effect )
    worn_by->taken_off( this_object(), wear_effect );
  worn_by = thing;
  return 1;
} /* set_worn_by() */
 
int query_damage_chance() { return damage_chance; }
object query_worn_by() { return worn_by; }
 
int query_worn() { return worn_by != 0; }
string *query_immune_to() { return immune_to; }
 
void add_immune_to(mixed arr) {
  int i;
 
  if (pointerp(arr))
    for (i=0;i<sizeof(arr);i++)
      add_immune_to(arr[i]);
  else
    if (member_array(arr, immune_to) == -1)
      immune_to += ({ arr });
} /* add_immune_to() */ 
 
void remove_immune_to(mixed arr) {
  int i;
 
  if (pointerp(arr))
    for (i=0;i<sizeof(arr);i++)
      remove_immune_to(arr[i]);
  else
    if ((i=member_array(arr, immune_to)) != -1)
      immune_to = delete(immune_to, i, 1);
} /* remove_immune_to() */
 
void set_type(string str) {
  if (type)
    remove_alias(type);
  switch (str) {
    case "armour" :
      max_ac = 40;
      break;
    case "shield" :
      max_ac = 20;
      break;
    case "boot" :
      max_ac = 10;
      break;
    case "helmet" :
      max_ac = 10;
      break;
    case "cloak" :
      max_ac = 10;
      break;
    case "glove" :
      max_ac = 10;
      break;
    case "ring" :
      max_ac = 10;
      break;
  }
  add_alias(str);
  type = str;
} /* set_type() */
 
string query_type() { return type; }
 
int query_value() {
  int val;
 
  val = ::query_value();
  return (val*10)/100 + (val*query_cond()*90)/(100*query_max_cond());
} /* query_value() */

int query_full_value() { return ::query_value(); }
 
mixed *query_money_array() {
  return (mixed *)MONEY_HAND->create_money_array(query_value());
} /* query_money_array() */
 
int query_money(string type) {
  int i;
  mixed *m_a;
 
  m_a = (mixed *)MONEY_HAND->create_money_array(query_value()); 
  if ((i=member_array(type, m_a)) == -1)
    return 0;
  return m_a[i+1];
} /* query_money() */

int query_no_limbs() { return no_limbs; }
void set_no_limbs(int i) { no_limbs = i; }

int set_holder(object ob, int pos) {
  held_in = pos;
  return 1;
} /* set_holder() */
 
void player_wear() {
  if (!environment()) return ;
  if (no_limbs)
    environment()->set_hold(this_object(), held_in);
  else
    environment()->wear_armour(this_object());
} /* player_wear() */

void break_me() {
  if ( !worn_by ) /* Don't know how this could happen, but... */
    return ::break_me();
  tell_object( worn_by, "%^RED%^Your "+ short( 0 ) +" breaks into "+
      "pieces!%^RESET%^\n" );
  tell_room( environment( worn_by ),
      capitalize( (string)worn_by->the_short() ) +"'s "+ short( 0 ) +
      " breaks into pieces!\n", worn_by );
  ::break_me();
} /* break_me() */
 
