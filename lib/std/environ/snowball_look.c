#include "snowball.h";
inherit "/std/object";

int *snowballarr;

void setup() {
  set_name("whatsie");
  add_adjective("snow");
  add_alias(SNOW_THING);
  set_short(0);
  snowballarr = ({ });
}

void snowball(int i) {
  snowballarr += ({ i });
  set_heart_beat(1);
}

string extra_look() {
  int num,
      i;
  string snowy;

  num = 0;
  if (sizeof(snowballarr)>0)
    for (i=0;i<sizeof(snowballarr);i++)
      if (snowballarr[i]>0)
        num++;
  if (num>0) {
    if (num>7)
      snowy = "Is plastered in snowballs.\n";
    else
      snowy = "Has "+ ({ "one","two","three","four","five","six","seven"
                      })[num- 1]+" snowballs on "+
                      environment()->query_possessive()+" face.\n";
  } else
    snowy = "";
  return snowy;
}

void heart_beat() {
  int i;

  for (i=0;i<sizeof(snowballarr);i++)
    if ((--snowballarr[i]) <= 0)
      snowballarr = delete(snowballarr, i--, 1);
  if (!sizeof(snowballarr))
    dest_me();
}

move(object dest, string msgin, string msgout) {
  int i;
  if (environment())
    environment()->remove_extra_look(this_object());
  i = ::move(dest, msgin, msgout);
  if (environment())
    environment()->add_extra_look(this_object());
  return i;
}

void dest_me() {
  if (environment())
    environment()->add_extra_look(this_object());
  ::dest_me();
}
