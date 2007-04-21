/* OOB service for the Intermud3 protocol. */
 * By Turrican@Discworld, 20-3-96
 */

#define SERVICE_OOB

void eventReceiveOobRequest(mixed *packet) {
  if (file_name(previous_object()) != INTERMUD_D) return;
  if (sizeof(packet) != 6) return;
  if (!packet[2]) return;
  sessions[packet[2]] = -1; /* Special value for non auth services. */
  call_out(eventTimeout, 600, packet[2]);
  OOB_D->eventListen();
}

