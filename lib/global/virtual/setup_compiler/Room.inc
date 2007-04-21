mixed *reset_funcs;
mixed *dest_funcs;
mixed *enter_funcs;
mixed *exit_funcs;
mixed *death_funcs;
 /* the above are stored as ({ ob1 ,"func1",ob2,"func2", ... })  */
mixed *reset_obs;
 /* the above stored as ({ ob1, instance1, ob2, instance2 })  */


create() 
{
reset_funcs = ({});
dest_funcs = ({});
enter_funcs = ({});
exit_funcs = ({});
death_funcs = ({});
reset_obs = ({});
 ::create();
} 
setup()
{
	  set_short("A room");
    set_long("A room");
    set_light(100);
}
 
object FindObject( mixed obj )
{
  mixed ob;
  ob = obj;	
  if( stringp(obj) ) {
	ob = find_object( obj);
        if( !objectp(ob) ) {
	  (void) obj->load_please();
	  ob = find_object( obj);
	}
  }
  if( !objectp( ob ) ) {
	printf("FindObject failed in add_func: %O\n",obj);
	throw("UnRecoverable error\n");
	return 0;
  }
  return ob;
}
 
int AddFuncsToMixed( mixed *array, object obj, mixed func )
{
  object ob;
  int pos, found;
  mixed *array2;
  ob = FindObject( obj );
  pos = 0;

  if( sizeof(array) ) 
  {
	array = ({ ob, func }); return 1;
  }

  array2 = array;
  while( pos < sizeof(array) )
  {
    pos = member_array(ob,array2);
    if( pos < 0 ) break;
    if( array2[pos+1] == func )  return 1;
    array2 = array2[pos+2..10000];
  }

  array += ({ ob, func });
  return 1;
}	

RunFuncsOnObjects( mixed *array , object arg )
{	
   int i;
   for( i = 0 ; i < sizeof(array) ; i +=2 ) 
	call_other( array[i], array[i+1], ({ arg })  );
}


void add_reset_func( mixed ob, string func )
{
	AddFuncsToMixed( reset_funcs, ob, func );
}

void add_dest_func( mixed ob, string func )
{
        AddFuncsToMixed( dest_funcs, ob, func );
}

void add_enter_func( mixed ob, string func )
{
	AddFuncsToMixed( enter_funcs, ob, func );
}

void add_exit_func( mixed ob, string func )
{
	AddFuncsToMixed( exit_funcs, ob, func );
}


void add_death_func( mixed ob, string func )
{
	AddFuncsToMixed( death_funcs, ob, func );
}


void add_clone_on_reset( string fname )
{
   if( sizeof(reset_obs) )  reset_obs += ({ fname , 0 });
   else                     reset_obs =  ({ fname , 0 });
}
QueryResetObjects() { printf("Reset OBS: %O\n",reset_obs); }


reset()
{
   int i;

   for(i=0; i < sizeof( reset_obs ); i += 2 )
   {
      if( reset_obs[i+1] ) continue;
      reset_obs[i+1] = clone_object( reset_obs[i] );
        ( reset_obs[i+1] )->move( this_object() );
   }

   if( sizeof( reset_funcs ) )
	RunFuncsOnObjects( reset_funcs  , 0 );

}

dest_me()
{
   int i;

   for( i = 0; i < sizeof( reset_obs ); i +=2 )
       if( reset_obs[i+1] )  (reset_obs[i+1])->dest_me();

   if( sizeof( reset_funcs ) )
         RunFuncsOnObjects( reset_funcs  , 0 );

   ::dest_me();
}


event_death( object ob )
{
   if( sizeof( death_funcs ) )
          RunFuncsOnObjects( death_funcs  , ob );
}

event_enter( object ob )
{
   if( sizeof( enter_funcs ) )
          RunFuncsOnObjects( enter_funcs  , ob );
}

void event_exit( object ob )
{
   if( sizeof( exit_funcs ) )
          RunFuncsOnObjects( exit_funcs  , ob );
}
