/*
 * This will shut the mud down... Hopefully at the right time.
 */

#define DOWN_TIME 16*60*60

void create() {
  int tim;

  tim = time()%24*60*60;
  tim = DOWN_TIME - tim;
  if (tim < 0)
    tim += 24*60*60;
  call_out("do_shut_down", tim);
}

void do_shut_down() {
  "/obj/shut"->shut(10);
}
