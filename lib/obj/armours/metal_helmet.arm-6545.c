#include "virtual.h"

void dest_me() { destruct(this_object()); }

void create() {
object clone;

  seteuid((string)"/secure/master"->creator_file(this_object()));
