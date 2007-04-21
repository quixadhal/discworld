
#include <db.h>
#include <config.h>

inherit "/w/taffyd/html_library";

#include <http.h>

#define MAKE_SQL_REQUEST( cmd, fp ) DB_HANDLER->make_sql_request( \
 "errors", CONFIG_DB_ERRORS_USER, "", (cmd), (fp) )

#define TYPO "Type = 'TYPO'" /*is a typo*/
#define BUG "Type = 'BUG'" /*is a bug*/
#define IDEA "Type = 'IDEA'" /*is an idea*/
#define FIXED "Status = 'FIXED'" /*is fixed*/
#define OPEN "Status = 'OPEN'" /*open bug*/
#define DENIED "Status = 'DENIED'" /*denied report*/
#define FIXER "Fixer is not NULL" /*has a fixer*/
#define FORWARDER_QUERY "Forwarder is not NULL"

#define RESULT_KEY "A"

int _doing_queries;
string _error_cache; 
function *_finished_funcs; 

mixed *_queries = ({ 
    ({ 
        ({ "Bug Fixers", "Fixer", "errors",  ({ FIXER, BUG, FIXED }) }),
        ({ "Bugs Fixed", "Reporter", "errors",  ({ FIXER, BUG, FIXED }) }),
        ({ "Open Bugs", "Reporter", "errors",  ({ OPEN, BUG }) }),
        ({ "Bugs Reported", "Reporter", "errors",  ({ BUG }) }),
    }),

    ({ 
        ({ "Typo Fixers", "Fixer", "errors",  ({ FIXER, TYPO, FIXED }) }),
        ({ "Typo Fixed", "Reporter", "errors",  ({ FIXER, TYPO, FIXED }) }),
        ({ "Open Typos", "Reporter", "errors",  ({ OPEN, TYPO }) }),
        ({ "Typos Reported", "Reporter", "errors",  ({ TYPO }) }),
    }),

    ({ 
        ({ "Idea \"Fixers\"", "Fixer", "errors",  ({ FIXER, IDEA, FIXED }) }),
        ({ "Ideas \"Fixed\"", "Reporter", "errors",  ({ FIXER, IDEA, FIXED }) }),
        ({ "Open Ideas ", "Reporter", "errors",  ({ OPEN, IDEA }) }),
        ({ "Ideas Reported", "Reporter", "errors",  ({ IDEA }) }),
    }),

    ({ 
        ({ "Total Fixers", "Fixer", "errors",  ({ FIXER, FIXED }) }),
        ({ "Total Fixed", "Reporter", "errors",  ({ FIXER, FIXED }) }),
        ({ "Open Reports", "Reporter", "errors",  ({ OPEN }) }),
        ({ "Reported", "Reporter", "errors",  ({ }) }),
    }),

    ({ 
        ({ "Denied Reports By Player", "Reporter", "errors",  ({ DENIED }) }),
        ({ "Denied Reports By Creator", "Fixer", "errors",  ({ DENIED }) }),
        ({ "Report Forwarders", "Forwarder", "forwards",  ({ FORWARDER_QUERY }) }),
        ({ "Commenter", "Commenter", "comments",  ({ }) }),
    }),
});

mixed *_finished_queries; 

void build_tables(int row, int column);
void build_error_stats();

void create() {
    html_library::create(); 
    
    set_title( "Bug Fixing Statistics" ); 
    build_error_stats();
} /* create() */ 

void build_error_stats() {
    int i, j; 
    int size; 

    _error_cache = 0; 
    _finished_queries = allocate( sizeof( _queries ) ); 
    _doing_queries = 0; 
    _finished_funcs = ({ }); 

    for ( i = 0; i < sizeof( _queries ); i++ ) {
        size = sizeof( _queries[ i ] );
        _doing_queries += size; 
        _finished_queries[ i ] = allocate( size ); 

        for ( j = 0; j < size; j++ ) {
            call_out( (: build_tables :), i, i, j); 
        }
    }
} /* build_error_stats() */ 

string make_table( string header, mixed *data ) {
    return "<table cellpadding=\"5\" cellspacing=\"0\" width=\"100%\">\n" 
        "<tr><th>" + header + "</th><th>Number</th></tr>\n" +
        implode( map( data, (: "<tr><td>" + $1[0] + "</td><td>" + 
            $1[1]  + "</td></tr>" :) ), "\n" ) + 
        "</table>\n";
} /* make_table() */ 

string build_full_sql_query(string who, string table, string select){
    return sprintf("SELECT %s, COUNT(*) AS " + RESULT_KEY + " FROM %s %s "
        "GROUP BY %s ORDER BY " + RESULT_KEY + " DESC LIMIT 10", who, table, 
        select, who);
} /* make_query() */ 

mixed build_sql_query(string who, string table, string *select) {
    string query;

    if ( sizeof(select) ) { 
        query = sprintf("WHERE %s", implode(select, " AND "));
    }
    else {
        query = "";
    }

    return build_full_sql_query(who, table, query);
} /* build_sql_query() */ 

string build_final_tables() {
    string txt;
    int i, j;

    txt = "";

    for ( i = 0; i < sizeof( _finished_queries ); i++ ) {
        txt += "<table cellpadding=\"5\" cellspacing=\"0\" width=\"100%\">\n"
            "<tr>";
        for ( j = 0; j < sizeof( _finished_queries[ i ] ); j++ ) {
            txt += "<td>" + _finished_queries[i][j] + "</td>";
        }

        txt += "</tr>"
            "</table>\n<hr>";
    }

    txt += "\n<div style=\"text-align: center;font-size: 10pt;\">\n"
        "<em>Statistics are generated once a reboot.</em>\n"
        "</div>\n";

    return construct_html_document(txt); 
} /* build_final_tables() */

void check_finished() {
    if ( !_doing_queries ) {
        _error_cache = build_final_tables(); 
        
        foreach( function func in _finished_funcs ) { 
            if ( functionp(func) ) { 
                evaluate( func, _error_cache ); 
            }
        }

        _finished_funcs = ({ }); 
    }
} /* check_finished() */ 

void finished_sql_query(int status, mixed *data, string key, string title, int row, int column) {

    mixed *results; 

    if ( status != DB_SUCCESS ) {
        _doing_queries--;
        check_finished(); 
        return;
    }

/* 0: ({
  ([ 
    "A" : 1905,
    "Reporter" : "hagi",
  ]),
  ([ 
    "A" : 837,
    "Reporter" : "lanfear",
  ]),
*/ 
    results = ({ });

    foreach( mapping result in data ) { 
        results += ({ ({ result[ key ], result[ RESULT_KEY ] }) });
    }
    
    _finished_queries[ row ][ column ] = make_table( title, results ); 

    _doing_queries--;

    // tell_creator( "taffyd", "%s, %d\n", _finished_queries[ row ][ column ], _doing_queries ); 
    
    check_finished(); 
} /* finished_sql_query() */

void build_tables(int row, int column) {
    mixed *bits; 
    string query;
    bits = _queries[ row ][ column ]; 
    
    query = build_sql_query( bits[1], bits[2], bits[3] );

    MAKE_SQL_REQUEST(query, (: finished_sql_query($1, $2, $(bits)[1], $(bits)[0], $(row), $(column)) :)); 

    tell_creator( "taffyd", "%s\n", query );
} /* build_tables() */ 

mixed www_delayed( function func, string str, mapping args, class http_request req ) {
    if ( _doing_queries ) {
        _finished_funcs += ({ func }); 
        return 1; 
    }
    
    return 0; 
} /* www_delayed() */ 

string www_request(string, mapping args, class http_request req) {
    if ( _error_cache ) {
        return _error_cache;
    }

    return construct_html_document( "No error statistics are available "
        "at the moment.\n");
} /* www_request() */

#ifdef OLD_CODE_JESUS_CHRIST_MY_EYES
  sixth = cont {
  rows = do_query("select Reporter, EntryDate, Directory, Filename, Status "
                  "from errors where Status != 'FIXED' and Status != "
                  "'DENIED' and Type = 'BUG' order by EntryDate",
              fix(({"select Reporter", "EntryDate", "Directory", "Filename",
                      "Status"}), {

  ret += make_table(rows, "Oldest bugs", "reporter", "date", "directory",
                    "filename", "status");
  
  ret += "<br clear=left>";
  /* 
  rows = db_exec(db, "select Reporter, EntryDate, Directory, Filename, Status "
                 "from errors where Status != 'FIXED' and Status != "
                 "'DENIED' and Type = 'IDEA' order by EntryDate");

  ret += make_table(rows, "Oldest ideas", "reporter", "date", "directory",
                    "filename", "status");

  ret += "<br clear=left>";

  
  rows = db_exec(db, "select Reporter, EntryDate, Directory, Filename, Status "
                 "from errors where Status != 'FIXED' and Status != "
                 "'DENIED' and Type = 'TYPO' order by EntryDate");
  
  ret += make_table(rows, "Oldest typos", "reporter", "date", "directory",
                    "filename", "status");

  ret += "<br clear=left>";
    
  rows = db_exec(db, "select Filename, count(*) as bing from errors where "
                 OPEN " and " BUG " group by filename order by bing desc");
  ret += make_table(rows, "Files with most bug reports", "file", "reports");
    
  rows = db_exec(db, "select Filename, count(*) as bing from errors where "
                 OPEN " and " IDEA " group by filename order by bing desc");
  ret += make_table(rows, "Files with most idea reports", "file", "reports");
  */
#endif
