inherit "std/object";
#define BRIGHTNESS 50 

int amount_of_fuel;
status is_lighted;
 
void setup() {
  amount_of_fuel = 2000; 
  set_name("torch"); 
  set_main_plural("torches");
  set_short("Torch");
  set_long("A small torch.\n");
  is_lighted = 0;
  set_weight(50);
  set_value(50);
} /* setup() */

string short(int i) {
    if (is_lighted)
        return ::short(0) + " (lit)";
    return ::short(0);
} /* short() */
 
string pretty_plural() {
  if (is_lighted)
    return ::pretty_plural() + " (lit)";
  return ::pretty_plural();
} /* pretty_plural() */

void set_fuel(int f) { amount_of_fuel = f; }
 
void init() {
  this_player()->add_command("light", this_object());
  this_player()->add_command("extinguish", this_object());
  this_player()->add_command("dowse", this_object());
} /* init() */
 
int do_light() {
  if (is_lighted) {
/*
    write(capitalize(short(0))+" is already lit.\n");
*/
    return 0;
  }
  if (amount_of_fuel <= 0) {
    write(capitalize(short(0))+" is burnt out.\n");
    return 0;
  }
  is_lighted = 1;
  call_out("out_of_fuel", amount_of_fuel * 2);
  amount_of_fuel = 0;
  set_light(BRIGHTNESS);
  return 1;
} /* do_light() */
 
void out_of_fuel() {
/* perhaps we can something clever here with events. */
  is_lighted = 0;
  set_light(0);
  say("The " + short(0) + " goes out.\n");
  write("The " + short(0) + " goes out.\n");
  set_long("The remains of a torch.  It doesn't look like it has anything "+
"left to burn on it.\n");
} /* out_of_fuel() */
 
int do_extinguish(object *indir, string s1, string s2, string prep) {
  int i;

  if (!is_lighted) {
    write(capitalize(short(0))+" is not lit.\n");
    return 0;
  }
  i = remove_call_out("out_of_fuel");
  amount_of_fuel = i/2;
  is_lighted = 0;
  set_light(0);
  return 1;
} /* do_extenguish() */

void dest_me() {
  set_light(0);
  ::dest_me();
} /* dest_me() */

mixed query_dynamic_auto_load() {
  return ([ "::" : ::query_dynamic_auto_load(),
            "amount of fuel" : amount_of_fuel,
            "is lit" : is_lighted,
          ]);
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping arg) {
  amount_of_fuel = arg["amount of fuel"];
  if (arg["is lit"])
    do_light();
} /* init_dynamic_arg() */
