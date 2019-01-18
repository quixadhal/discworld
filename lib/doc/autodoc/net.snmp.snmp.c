.DT
snmp.c
Disk World autodoc help
snmp.c

.SH Includes
.SP 5 5
This class includes the following files /include/snmp.h, /include/socket.h and /include/socket_errors.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^SnmpMessage%^RESET%^
class SnmpMessage {
int version;
string community;
string SourceAddress;
int RequestType;
int * PacketID;
int ErrorStatus;
int ErrorIndex;
buffer * ObjectData;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^ParseObjects%^RESET%^
.EI
.SI 5
void ParseObjects(class SnmpMessage CurrentMessage)
.EI

.SI 3
* %^BOLD%^ReceiveData%^RESET%^
.EI
.SI 5
void ReceiveData(int fd, buffer message, string address)
.EI

.SI 3
* %^BOLD%^SendReply%^RESET%^
.EI
.SI 5
void SendReply(class SnmpMessage Reply)
.EI

.SI 3
* %^BOLD%^WriteBuffer%^RESET%^
.EI
.SI 5
void WriteBuffer(string what, buffer tbuf)
.EI


