inherit "/std/key";

void set_lock_number(int num) {
   set_key("small furniture", "furniture_lock_" + num);
   set_long("A small key, it looks like it would be "
            "suitable for a piece of furniture.\n");
   set_value(10);
} /* set_lock_number() */

// This is the func name other keys and the shops use...
void set_key_number( int num ){
  set_lock_number(num);
}

mapping query_static_auto_load() {
   return int_query_static_auto_load();
} /* query_static_auto_load() */

/**
 * Allows the keys to be duplicated if it returns 1.
 * @return always returns 1
 */
int query_duplicatable_key() {
   return 1;
}
