/* Auth services for the Intermud 3 package.
 * By Turrican@Discworld, 20-3-96
 */

#define SERVICE_AUTH
#define TIMEOUT 10*60

private nosave mapping sessions = ([]);

void eventReceiveAuthRequest(mixed *packet) {
  int key;

  if (file_name(previous_object()) != INTERMUD_D) return;
  if (sizeof(packet) != 6) return;
  key = random(time()); /* XXX - Is this guaranteed to be unique ?? */
  if (!packet[2]) return;
  sessions[packet[2]] = key;
  call_out("eventTimeout", TIMEOUT, packet[2]);
  INTERMUD_D->eventExternWrite(({"auth-mud-reply", 5, mud_name(), 0,
			 packet[2], 0, key}));
}

void eventTimeout(string mudname) {
  if (sessions[mudname])
    map_delete(sessions, mudname);
}

void eventReceiveAuthReply(mixed *packet) {
  if (file_name(previous_object()) != INTERMUD_D) return;
  if (sizeof(packet) != 7) return;
  if (packet[6])
    sessions[mud_name()] = packet[6];
}

void eventSendAuthRequest(string other) {
  if (!other || other == "") return;
  INTERMUD_D->eventExternWrite(({"auth-mud-req", 5, mud_name(), 0, other, 0}));
}

mapping GetKeys() {
  return copy(sessions);
}

int GetKey(string mudname) {
  if (!sessions[mudname]) return 0;
  return sessions[mudname];
}
