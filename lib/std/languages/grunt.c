/*
 * The language everyone can speak.  But no one understands.
 */

string long() {
  return "Consists of grunting noises.\n";
}

mixed garble_say(string start, string rest) {
  string nam, loud;

  sscanf(start, "%s %s:", nam, start);
  if (sscanf(start, "%s loudly", start) == 1)
    loud = " loudly";
  else
    loud = "";
  switch (start) {
    case "asks" :
       nam = nam+" quizzically grunts"+loud;
       break;
    case "exclaims" :
       nam = nam+" pointedly grunts"+loud;
       break;
    default :
       nam = nam+" grunts"+loud;
       break;
  }
  return ({ nam+".", "" });
}
