int valid_override(string file, string func) {
  string *bing;

  bing = explode(file, "/") - ({ "", "." });
  switch (bing[0]) {
    case "secure" :
      return 1;
    case "std" :
    case "obj" :
    case "simul_efun" :
    case "global" :
      return (func != "snoop");
    default :
      return 0;
  }
} /* valid_override() */
