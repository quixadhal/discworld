/**
 * Version Control Inheritable.
 * @author Terano
 * @started 16/7/00
 * Making things fun!
 */

nosave mapping version_control = ([ ]); 

void create() {
  version_control[ base_name( this_object() ) ] = ctime( time() );

  foreach( object temp in inherit_list( this_object() ) ) 
    version_control[ base_name( temp ) ] = temp->query_version();
}

void query_version_control() {
  write( "File:\tTime:\n" );
  foreach( string temp in keys( version_control ) ) 
    printf( "%s\t%s.\n", temp, version_control[ temp ] );
  return;
}

string query_version() {
  return version_control[ base_name( this_object() ) ];
}