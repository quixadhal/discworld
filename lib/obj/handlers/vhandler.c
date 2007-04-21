/*
   Saved Variable handler - very basic atm - will tidy up given time.
   Written by Sojan [sojan@ukcis.demon.co.uk] - 27/12/95

   Instructions for Use ..
	call set_variable_value with a string parameter identifying the variable and a
		second mixed value which is the value you want to store
	to query it simply call query_variable_status with a string parameter that 
		Identifies the variable

    Things I know thats wrong with it atm ...

	*NO checking for existing variables so you can overwrite other peoples - use your
		name as part of the identifier or something atm ....

	*It DOESN'T clear up variables that are no longer required - nor currently has code
		to do so.

    Both these things will be fixed in a few days - I wrote this off the cuff for a creator
    With an unprounoucable name but I guess other people will find it useful.

    Any comments/criticisms to me ....

    Sojan
*/


mapping stored_variables;

void create()
{
  seteuid(geteuid(this_object()));
  stored_variables=([ ]);
  restore_object(file_name(this_object()));
}

mixed query_variable_status(string vname)
{
  return stored_variables[vname];
}

int set_variable_status(string vname, mixed vvalue)
{
  stored_variables[vname]=vvalue;
  save_object(file_name(this_object()));
}

