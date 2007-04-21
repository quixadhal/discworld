#include <db.h>
#include <config.h>

mapping *results;

void set_results( int flag, mapping *res, object who);

int cmd( string db, string query ) {
  DB_HANDLER->make_sql_request( db, CONFIG_DB_USER, "", query, 
  (: set_results( $1, $2, $(this_player()) ) :) );
  return 1;
}

void set_results( int flag, mapping *res, object who ) { 
  tell_creator( who, "flag: %O\n%O\n", flag, res );
  results = res; 
}

mapping *query_results() { return results; }

mixed query_patterns() {
  return ({ 
    "<word'db'> <string:quoted'query'>", 
    (: cmd( $4[0], $4[1] ) :) });
}
