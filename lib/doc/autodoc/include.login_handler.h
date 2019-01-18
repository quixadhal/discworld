.DT
login_handler.h
Disk World autodoc help
login_handler.h

.SH Description
.SP 5 5

The include file for the login handler.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h.
.EP

.SH Defines
.SI 3
* %^BOLD%^COMPRESSED_FULL%^RESET%^
.EI

.SI 3
* %^BOLD%^DISCWORLD_IS_EMPTY%^RESET%^
.EI
.SP 7 5
Returned from is_discworld_full if Discworld is full. 
.EP

.SI 3
* %^BOLD%^DISCWORLD_IS_FULL%^RESET%^
.EI

.SI 3
* %^BOLD%^LOGIN%^RESET%^
.EI
.SP 7 5
The name of the login event.  Used in the static/dynamic login calls. 
.EP

.SI 3
* %^BOLD%^LOGIN_HANDLER%^RESET%^
.EI
.SP 7 5

The path to the login handler.

.EP

.SI 3
* %^BOLD%^LOGOUT%^RESET%^
.EI
.SP 7 5
The name of the logout event.  Used in the static/dynamic login calls. 
.EP

.SI 3
* %^BOLD%^MAX_COMPRESSED_PLAYERS%^RESET%^
.EI
.SP 7 5
The maximum number of compressed stream players. 
.EP

.SI 3
* %^BOLD%^MAX_PLAYERS%^RESET%^
.EI
.SP 7 5
The maximum number of players online at Discworld at once. 
.EP

.SI 3
* %^BOLD%^MAX_UNCOMPRESSED_PLAYERS%^RESET%^
.EI
.SP 7 5
The maximum number of uncompressed stream players.  
.EP

.SI 3
* %^BOLD%^NETDEATH%^RESET%^
.EI
.SP 7 5
The name of the netdeath event.  Used in the static/dynamic login calls. 
.EP

.SI 3
* %^BOLD%^RECONNECT%^RESET%^
.EI
.SP 7 5
The name of the reconnect event.  Used in the static/dynamic login calls. 
.EP

.SI 3
* %^BOLD%^UNCOMPRESSED_FULL%^RESET%^
.EI

.SI 3
* %^BOLD%^WITH_LOGIN_QUEUE%^RESET%^
.EI
.SP 7 5
Flag to the is_discworld_full method. 
.EP

.SI 3
* %^BOLD%^WITHOUT_ANY_LOGINS%^RESET%^
.EI
.SP 7 5
Flag to the is_discworld_full method. 
.EP

.SI 3
* %^BOLD%^WITHOUT_LOGIN_QUEUE%^RESET%^
.EI
.SP 7 5
Flag to the is_discworld_full method. 
.EP

.SI 3
* %^BOLD%^WITHOUT_LOGINS_NOT_IN_QUEUE%^RESET%^
.EI
.SP 7 5
Flag to the is_discworld_full method. 
.EP

