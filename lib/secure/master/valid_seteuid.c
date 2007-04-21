/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: valid_seteuid.c,v 1.4 2000/06/19 04:28:27 ceres Exp $
 */
int valid_euid(string str) {
  string *domains;

  switch (str) {
  case "all":
  case "Handlers":
  case "Mailer":
  case "Network":
  case "Room":
  case "Spell":
  case "WWW":
    return 1;
  default:
    domains = unguarded((: get_dir, "/d/" :));
    domains -= ({ "lost+found" });
    domains = map(domains, (: capitalize($1) :));
    if (member_array(str, domains) != -1) {
      return 1;
    }
    return 0;
  }
}

int valid_seteuid(object ob, string euid) {
  string crea;

  if (euid == "tmp") {
      return 1;
  }
   if ( !objectp( ob ) )
      return 0;
  crea = creator_file(file_name(ob));
  if (crea == "Root" || crea == "Room") {
      return 1;
  }
  return (euid == crea) || !euid;
} /* valid_seteuid() */
