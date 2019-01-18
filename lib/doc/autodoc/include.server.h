.DT
server.h
Disk World autodoc help
server.h

.SH Classes

.SI 3
* %^BOLD%^buff%^RESET%^
class buff {
int Type;
int Size;
int Pos;
string Filename;
mixed Value;
}

.EI

.SI 3
* %^BOLD%^server%^RESET%^
class server {
int Descriptor;
int Blocking;
int Closing;
class buff * Buffer;
}

.EI


.SH Defines
.SI 3
* %^BOLD%^BLOCK_SIZE%^RESET%^
.EI

.SI 3
* %^BOLD%^FILE%^RESET%^
.EI

.SI 3
* %^BOLD%^STRING%^RESET%^
.EI

