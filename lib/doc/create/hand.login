The login handler lets you have a function called as a player logs in or out.
The function you setup will be called when the player logs in or out.  The
object will also give you a list of all the people who are logged in. This is
a by product of its method of operation rather than anything important.  It is
quite possibly more useful to call users().

int add_login_call(string name, string function, string object);
int remove_login_call(string name, string function, string object);
string *query_players_on();

