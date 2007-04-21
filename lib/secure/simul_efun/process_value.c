/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: process_value.c,v 1.2 1998/03/07 13:49:40 pinkfish Exp $
 * $Log: process_value.c,v $
 * Revision 1.2  1998/03/07 13:49:40  pinkfish
 * Fixes because of the new inherit structor of the simul_efun object.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
/*
 * process_value - a replacement for the (now missing) process_value
 *                 efun.  I was hoping that function pointers would
 *                 make this unnecessary, but there's no way to store
 *                 a function pointer with the autoloader.
 *
 * Initial release: 16-Nov-96
 */

string *pattern = ({
  "\\|", " *[0-9]+ *", ".*"
});

#define PV_BAR 2
#define PV_INT 3
#define PV_STR 4
#define PV_QUOTED 5

int *token = ({ 2, 3, 4 });

/**
 * This method replaces the process_value efun which is now missing.
 * @param s the string to process
 * @return the processed string
 */
mixed process_value(string s) {
  string func, file, arg;
  mixed assoc, params;
  object ob;
  int i;

  if (sscanf(s, "%s|%s", func, arg) == 2) {
    // Call has arguments
  } else {
    // No arguments
    func = s;
    arg = 0;
  }
  if (sscanf(func, "%s:%s", func, file) == 2) {
    if (!objectp(ob = find_object(file)))
      return 0;
  } else {
    ob = this_object();
  }
  //printf("func:   %O\n", func);
  //printf("file:   %O\n", file);
  params = ({ func });
  if (stringp(arg)) {
    assoc = reg_assoc(arg, pattern, token);
    // First, get rid of the separators
    for (i = 0; i < sizeof(assoc[0]); i++) {
      if ((assoc[0][i] == "") && (assoc[1][i] == 0)) {
        assoc[0] = assoc[0][0..i  - 1] + assoc[0][i + 1..];
        assoc[1] = assoc[1][0..i  - 1] + assoc[1][i + 1..];
      } else if (assoc[1][i] == 0) {
        // Got unanticipated pattern; dunno what to do here
        write("process_value: syntax error: " + assoc[0][i] + "\n");
        return 0;
      }
    }
    // Check for escaped special characters
    for (i = 0; i < sizeof(assoc[0])-1; i++) {
      if ((assoc[1][i+1] == PV_BAR)
          && (strsrch(assoc[0][i], "\\", -1) == strlen(assoc[0][i])-1)) {
        assoc[0][i] = assoc[0][i][0..sizeof(assoc[0][i])-2];
        if (i+2 < sizeof(assoc[0])) {
          //printf("i=%d; assoc=%O\n", i, assoc);
          assoc[0][i] += assoc[0][i+1] + assoc[0][i+2];
          assoc[0] = assoc[1][0..i] + assoc[1][i + 2..];
        } else {
          assoc[0][i] += assoc[0][i+1];
          assoc[0] = assoc[1][0..i] + assoc[1][i + 2..];
        }
      }
    }
    for (i = 0; i < sizeof(assoc[0]); i++) {
      switch (assoc[1][i]) {
      case PV_BAR:
        //printf("bar:    %O\n", assoc[0][i]);
        break;
      case PV_INT:
        //printf("int:    %O\n", to_int(assoc[0][i]));
        params += ({ to_int(assoc[0][i]) });
        break;
      case PV_STR:
      case PV_QUOTED:
        //printf("string: %O\n", assoc[0][i]);
        params += ({ assoc[0][i] });
        break;
      default:
        //if (assoc[0][i] != "")
          //printf("OTHER:  %O\n", assoc[0][i]);
      }
    }
  }
  return call_other( ob, params );
}

/*
 * And I'll just put process_string in here also, so that it's all in
 * one place.
 */

string process_string(string s) {
  string *ss, proc;
  int i;

  ss = explode(s, "@@");
  if (strsrch(s, "@@") == 0)
    ss = ({ "" }) + ss;
  for (i = 1; i < sizeof(ss); i += 2) {
    proc = process_value(ss[i]);
    if (stringp(proc))
      ss[i] = proc;
  }
  return implode(ss, "");
}
