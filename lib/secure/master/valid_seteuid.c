int valid_seteuid(object ob, string euid) {
  string crea;

  if (euid == "tmp") {
      return 1;
  }
  crea = creator_file(file_name(ob));
  if (crea == "Root" || crea == "Room") {
      return 1;
  }
  return (euid == crea) || !euid;
} /* valid_seteuid() */
