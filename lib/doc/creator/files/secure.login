.DT
login
Discworld Creator help
login

Name
.SI 5
/secure/login.c - This is the thingie that handles part of the login process.
.EI

Description

.SP 5 5
This thingie handles part of the login process, and as a result of that,
it's also a convenient place to get information about players who aren't
logged in.
.EP
.SP 5 5
Note that all these functions are able to pull information out of saved playes,
so remember that players who are logged in often has changed values of some of
these, eg. the level can be quite different if the player has advanced
recently. :)
.EP

Functions to get information

.SI 5
int test_user( string name )
.EI
.SP 10 5
Returns 1 if there are a saved player named 'name'.
.EP

.SI 5
int test_creator( string name )
.EI
.SP 10 5
Returns 1 if name is a creator.
.EP

.SI 5
int test_gender( string name )
.EI
.SP 10 5
Returns the gender of the player called name, or 0 if there are
no saved players of that name.
.EP

.SI 5
int test_level( string name )
.EI
.SP 10 5
Returns the guild_level of the player called name, or 0 if there are
no saved players of that name or the saved player has no guild.
.EP

.SI 5
string test_guild( string name )
.EI
.SP 10 5
Returns the guild_ob of the player called name, or 0 if there are
no saved players of that name or the saved player has no guild.
.EP

.SI 5
string test_deity( string name )
.EI
.SP 10 5
Returns the name of the deity of the player called name, or 0 if there are
no saved players of that name or the saved player worships no deity.
.EP

.SI 5
string test_real_name(string str)
.EI
.SP 10 5
Returns the name the player called name wrote as real name in the finger info,
"Mrs X" if the player didn't specify it or 0 if there are no saved players of
that name.
.EP

.SI 5
string test_email(string str)
.EI
.SP 10 5
Returns the address the player called name wrote as email in the finger info 
or 0 if the player didn't specify it or there are no saved players of
that name.
.EP

.SI 5
mixed test_property( string name, string prop )
.EI
.SP 10 5
Returns the value of the propery prop in the player named name, or 0 if
that's not a player, or that property wasn't added to that player.
.EP

.SI 5
int test_age( string word )
.EI
.SP 10 5
Returns the age of the player named name, or 0 if
that's not a player.
.EP

.SI 5
int test_last( string word )
.EI
.SP 10 5
Returns the time the player named name last was on, this being the value of
time() if the player's on now, otherwise it's the walue of time() when the
player logged out.
.EP

example

  int test_level( string name ) {
      if ( find_player( name ) ) {
         return (int)find_player( name )->query_level();
      } else {
         return (int)"/secure/login"->test_level( name );
      }
   } /* test_creator() */

  this will return the guildlevel of a player whether logged in or not.

