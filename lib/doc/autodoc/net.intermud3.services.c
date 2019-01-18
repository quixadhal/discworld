.DT
services.c
Disk World autodoc help
services.c

.SH Includes
.SP 5 5
This class includes the following files /net/intermud3/services/error.c, /net/intermud3/services/auth.c, /net/intermud3/services/locate.c, /include/socket_errors.h, /net/intermud3/services/tell.c, /net/intermud3/services/emoteto.c, /include/creator.h, /net/intermud3/services/finger.c, /include/localtime.h, /net/intermud3/services/who.c, /net/intermud3/services/channel.c, /include/network.h and /include/term.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^AddChannel%^RESET%^
.EI
.SI 5
int AddChannel(string channel, int privee)
.EI

.SI 3
* %^BOLD%^GetKey%^RESET%^
.EI
.SI 5
int GetKey(string mudname)
.EI

.SI 3
* %^BOLD%^GetKeys%^RESET%^
.EI
.SI 5
mapping GetKeys()
.EI

.SI 3
* %^BOLD%^GetLocalChannel%^RESET%^
.EI
.SI 5
string GetLocalChannel(string channel)
.EI

.SI 3
* %^BOLD%^GetServices%^RESET%^
.EI
.SI 5
mapping GetServices()
.EI

.SI 3
* %^BOLD%^RemoveChannel%^RESET%^
.EI
.SI 5
int RemoveChannel(string channel)
.EI

.SI 3
* %^BOLD%^addUrl%^RESET%^
.EI
.SI 5
varargs int addUrl(string match, int the_time, string channel, string user, string mud)
.EI

.SI 3
* %^BOLD%^clean_emote_string%^RESET%^
.EI
.SI 5
string clean_emote_string(string txt)
.EI

.SI 3
* %^BOLD%^close_call_back%^RESET%^
.EI
.SI 5
void close_call_back(int fd)
.EI

.SI 3
* %^BOLD%^deleteUrl%^RESET%^
.EI
.SI 5
int deleteUrl(string url)
.EI

.SI 3
* %^BOLD%^eventAdministerChannel%^RESET%^
.EI
.SI 5
int eventAdministerChannel(string channel, string * additions, string * subs)
.EI

.SI 3
* %^BOLD%^eventReceiveAuthReply%^RESET%^
.EI
.SI 5
void eventReceiveAuthReply(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveAuthRequest%^RESET%^
.EI
.SI 5
void eventReceiveAuthRequest(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveChannelEmote%^RESET%^
.EI
.SI 5
void eventReceiveChannelEmote(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveChannelMessage%^RESET%^
.EI
.SI 5
void eventReceiveChannelMessage(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveChannelUserRequest%^RESET%^
.EI
.SI 5
void eventReceiveChannelUserRequest(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveChannelWhoReply%^RESET%^
.EI
.SI 5
void eventReceiveChannelWhoReply(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveChannelWhoRequest%^RESET%^
.EI
.SI 5
void eventReceiveChannelWhoRequest(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveEmote%^RESET%^
.EI
.SI 5
void eventReceiveEmote(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveError%^RESET%^
.EI
.SI 5
void eventReceiveError(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveFingerReply%^RESET%^
.EI
.SI 5
void eventReceiveFingerReply(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveFingerRequest%^RESET%^
.EI
.SI 5
void eventReceiveFingerRequest(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveLocateReply%^RESET%^
.EI
.SI 5
void eventReceiveLocateReply(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveLocateRequest%^RESET%^
.EI
.SI 5
void eventReceiveLocateRequest(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveTell%^RESET%^
.EI
.SI 5
void eventReceiveTell(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveWhoReply%^RESET%^
.EI
.SI 5
void eventReceiveWhoReply(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventReceiveWhoRequest%^RESET%^
.EI
.SI 5
void eventReceiveWhoRequest(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventRegisterChannels%^RESET%^
.EI
.SI 5
void eventRegisterChannels(mapping list)
.EI

.SI 3
* %^BOLD%^eventSendAuthRequest%^RESET%^
.EI
.SI 5
void eventSendAuthRequest(string other)
.EI

.SI 3
* %^BOLD%^eventSendChannel%^RESET%^
.EI
.SI 5
varargs void eventSendChannel(string who, string ch, string msg, int emote, string target, string targmsg)
.EI

.SI 3
* %^BOLD%^eventSendChannelWhoRequest%^RESET%^
.EI
.SI 5
void eventSendChannelWhoRequest(string channel, string mud)
.EI

.SI 3
* %^BOLD%^eventSendEmote%^RESET%^
.EI
.SI 5
void eventSendEmote(string who, string where, string msg)
.EI

.SI 3
* %^BOLD%^eventSendFingerRequest%^RESET%^
.EI
.SI 5
void eventSendFingerRequest(string who, string where)
.EI

.SI 3
* %^BOLD%^eventSendLocateRequest%^RESET%^
.EI
.SI 5
void eventSendLocateRequest(string who)
.EI

.SI 3
* %^BOLD%^eventSendTell%^RESET%^
.EI
.SI 5
void eventSendTell(string who, string where, string msg)
.EI

.SI 3
* %^BOLD%^eventSendWhoRequest%^RESET%^
.EI
.SI 5
void eventSendWhoRequest(string mud, string who)
.EI

.SI 3
* %^BOLD%^eventShutdown%^RESET%^
.EI
.SI 5
void eventShutdown(int secs)
.EI

.SI 3
* %^BOLD%^eventTimeout%^RESET%^
.EI
.SI 5
void eventTimeout(string mudname)
.EI

.SI 3
* %^BOLD%^filter_listeners%^RESET%^
.EI
.SI 5
int filter_listeners(object ob, string chan)
.EI

.SI 3
* %^BOLD%^getChannelColor%^RESET%^
.EI
.SI 5
string getChannelColor(string ch)
.EI

.SI 3
* %^BOLD%^getColorChannelName%^RESET%^
.EI
.SI 5
varargs string getColorChannelName(string ch, string prefix, string suffix)
.EI

.SI 3
* %^BOLD%^getColorDate%^RESET%^
.EI
.SI 5
varargs string getColorDate(string prefix, string suffix, int the_time, int use_local)
.EI

.SI 3
* %^BOLD%^getColorDayTime%^RESET%^
.EI
.SI 5
varargs string getColorDayTime(string prefix, string suffix, int the_time, int use_local)
.EI

.SI 3
* %^BOLD%^getColorSpeakerName%^RESET%^
.EI
.SI 5
varargs string getColorSpeakerName(string speaker, string prefix, string suffix)
.EI

.SI 3
* %^BOLD%^getColorTimestamp%^RESET%^
.EI
.SI 5
varargs string getColorTimestamp(string prefix, string suffix, int the_time, int use_local)
.EI

.SI 3
* %^BOLD%^getDate%^RESET%^
.EI
.SI 5
varargs string getDate(int the_time, int use_local)
.EI

.SI 3
* %^BOLD%^getDayTime%^RESET%^
.EI
.SI 5
varargs string getDayTime(int the_time, int use_local)
.EI

.SI 3
* %^BOLD%^getRawTimeBits%^RESET%^
.EI
.SI 5
varargs mixed * getRawTimeBits(int the_time, int use_local)
.EI

.SI 3
* %^BOLD%^getSpeakerColor%^RESET%^
.EI
.SI 5
string getSpeakerColor(string who)
.EI

.SI 3
* %^BOLD%^getUrlData%^RESET%^
.EI
.SI 5
mapping getUrlData()
.EI

.SI 3
* %^BOLD%^mapSpeakerColors%^RESET%^
.EI
.SI 5
mapping mapSpeakerColors()
.EI

.SI 3
* %^BOLD%^oldGetLocalChannel%^RESET%^
.EI
.SI 5
string oldGetLocalChannel(string channel)
.EI

.SI 3
* %^BOLD%^old_eventReceiveChannelMessage%^RESET%^
.EI
.SI 5
void old_eventReceiveChannelMessage(mixed * packet)
.EI

.SI 3
* %^BOLD%^query_status%^RESET%^
.EI
.SI 5
string query_status(object ob)
.EI

.SI 3
* %^BOLD%^read_call_back%^RESET%^
.EI
.SI 5
void read_call_back(int fd, mixed mess)
.EI

.SI 3
* %^BOLD%^renameChatter%^RESET%^
.EI
.SI 5
int renameChatter(string who, string to, int keep_source_color)
.EI

.SI 3
* %^BOLD%^setSpeakerColor%^RESET%^
.EI
.SI 5
int setSpeakerColor(string who, string color)
.EI

.SI 3
* %^BOLD%^showSpeakerColors%^RESET%^
.EI
.SI 5
varargs string showSpeakerColors(string who)
.EI

.SI 3
* %^BOLD%^timestamp%^RESET%^
.EI
.SI 5
varargs string timestamp(int the_time, int use_local)
.EI

.SI 3
* %^BOLD%^write_call_back%^RESET%^
.EI
.SI 5
void write_call_back(int fd)
.EI


