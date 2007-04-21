/**
 * This handler manages reports about alleged misdemeanors and other
 * Sherlock Holmes type stuff.  It's main uses are holding cases so
 * they are not overlooked and forgotten about, recording decisions
 * on various offences and allowing a case history of a player to
 * be retrieved.
 *
 * @author Gototh
 */
// Gototh 22/02/98

#include <report_handler.h>

inherit "/std/object";

void move_to_recent_cases(mixed *thing);

mixed *new_cases, *recent_cases;

/**
 * @ignore yes
 */
void setup() {
  set_short("report handler");
  add_property("determinate", "the ");
  set_long("This is a rather tacky statue of a woman holding "
    "a sword and some wonky scales.\n");
  if(file_size(NEWCASES) > -1) {
    new_cases = restore_variable(read_file(NEWCASES));
  }
  if(file_size(RECENT) > -1) {
    recent_cases = restore_variable(read_file(RECENT));
  }
} /* setup() */

// New cases.

/**
 * @ignore yes
 */
void save_new_cases() {
  unguarded((: rm, NEWCASES :));
  unguarded((: write_file, NEWCASES, save_variable(new_cases)+"\n" :));
} /* save_me() */

/**
 * @ignore yes
 */
mixed query_new() {
  return new_cases;
} /* query_new() */

/**
 * @ignore yes
 */
int sizeof_new_cases() {
  if(!new_cases) {
    new_cases = restore_variable(read_file(NEWCASES));
  }
  return sizeof(new_cases);
} /* sizeof_new_cases() */

/**
 * @ignore yes
 */
string query_new_cases() {
  int i, j;
  string reports = "";
  for(i = 0; i < sizeof(new_cases); i++) {
    if(i < 10) {
      reports += "   "+(i + 1)+": ";
    } else {
      reports += "  "+(i + 1)+": ";
    }
    reports += "%^CYAN%^Player%^RESET%^: "+capitalize(new_cases[i][NAME]);
    for(j = 0; j < (12 - sizeof(new_cases[i][NAME])); j++) {
      reports += " ";
    }
    reports += "%^CYAN%^Category%^RESET%^: "+new_cases[i][CATEGORY]+"\n";
  }
  return reports;
} /* query_new_cases() */

/**
 * @ignore yes
 */
string query_new_case(int i) {
  string report = "";
  mixed *stuff;
  if(!new_cases) {
    new_cases = restore_variable(read_file(NEWCASES));
  }
  if(i < 1 || i > sizeof(new_cases)) {
    return 0;
  }
  stuff = new_cases[i - 1];
  report += "  %^CYAN%^Suspect%^RESET%^: "+capitalize(stuff[NAME])+"\n";
  if(!sizeof(stuff[CHARS])) {
    report += "  %^CYAN%^Other Characters%^RESET%^: Unknown.\n";
  } else {
    report += "  %^CYAN%^Other Characters%^RESET%^: "
      "$I$5="+query_multiple_short(stuff[CHARS])+"$I$0=\n";
  }
  report += "  %^CYAN%^Creator%^RESET%^: "+stuff[CREATOR]+"\n"
    "  %^CYAN%^Time%^RESET%^: "+stuff[TIME]+"\n"
    "  %^CYAN%^Category%^RESET%^: "+stuff[CATEGORY]+"\n"
    "  %^CYAN%^Report%^RESET%^: $I$5="+stuff[REPORT]+"$I$0=";
  return report;
} /* query_new_case() */

/**
 * @ignore yes
 */
void add_report(string *stuff) {
  if(!new_cases) {
    new_cases = restore_variable(read_file(NEWCASES));
  }
  new_cases += ({ stuff });
  save_new_cases();
} /* add_report() */

/**
 * @ignore yes
 */
void deal_with_it(int num, string name, string decision) {
  mixed *thing;
  num--;
  thing = new_cases[num];
  thing += ({ name });
  thing += ({ decision });
  new_cases -= ({new_cases[num]});
  move_to_recent_cases(thing);
} /* deal_with_it() */

/**
 * @ignore yes
 */
void save_recent_cases() {
  unguarded((: rm, RECENT :));
  unguarded((: write_file, RECENT, save_variable(recent_cases)+"\n" :));
} /* save_me() */

/**
 * @ignore yes
 */
void move_to_recent_cases(mixed *thing) {
  int i, j;
  mixed *c_record, *chars;
  for(i = sizeof(recent_cases) - NO_RECENT; i > -1; i--) {
    chars = ({recent_cases[i][NAME]});
    if(arrayp(recent_cases[i][CHARS])) {
      chars += recent_cases[i][CHARS];
    }
    uniq_array(chars);
    for(j = 0; j < sizeof(chars); j++) {
      if(file_size((SAVEPATH+chars[j]+".o")) > -1) {
        c_record = restore_variable(read_file((SAVEPATH+chars[j]+".o")));
        c_record += ({recent_cases[j]});
      } else {
        c_record = ({recent_cases[j]});
      }
      unguarded((: rm, (SAVEPATH+chars[j]+".o") :));
      unguarded((: write_file, (SAVEPATH+chars[j]+".o"),
        save_variable(c_record)+"\n" :));
    }
    recent_cases -= ({recent_cases[i]});
  }
  recent_cases += ({ thing });
  save_new_cases();
  save_recent_cases();
} /* move_to_recent_cases() */

/**
 * @ignore yes
 */
mixed query_recent() {
  if(!recent_cases) {
    recent_cases = restore_variable(read_file(RECENT));
  }
  return recent_cases;
} /* query_recent() */

/**
 * @ignore yes
 */
string query_recent_cases() {
  int i, j;
  string reports = "";
  for(i = 0; i < sizeof(recent_cases); i++) {
    if(i < 10) {
      reports += "   "+(i + 1)+": ";
    } else {
      reports += "  "+(i + 1)+": ";
    }
    reports += "%^CYAN%^Suspect%^RESET%^: "+capitalize(recent_cases[i][NAME]);
    for(j = 0; j < (12 - sizeof(recent_cases[i][NAME])); j++) {
      reports += " ";
    }
    reports += "%^CYAN%^Category%^RESET%^: "+recent_cases[i][CATEGORY]+"\n";
  }
  return reports;
} /* query_recent_cases() */

/**
 * @ignore yes
 */
string query_recent_case(int i) {
  string report = "";
  mixed *stuff;
  if(!recent_cases) {
    recent_cases = restore_variable(read_file(RECENT));
  }
  if(i < 1 || i > sizeof(recent_cases)) {
    return 0;
  }
  stuff = recent_cases[i - 1];
  report += "  %^CYAN%^Suspect%^RESET%^: "+capitalize(stuff[NAME])+"\n";
  if(!sizeof(stuff[CHARS])) {
    report += "  %^CYAN%^Other Characters%^RESET%^: Unknown.\n";
  } else {
    report += "  %^CYAN%^Other Characters%^RESET%^: "
      "$I$5="+query_multiple_short(stuff[CHARS])+"$I$0=\n";
  }
  report += "  %^CYAN%^Creator%^RESET%^: "+capitalize(stuff[CREATOR])+"\n"
    "  %^CYAN%^Time%^RESET%^: "+stuff[TIME]+"\n"
    "  %^CYAN%^Offence%^RESET%^: "+stuff[CATEGORY]+"\n"
    "  %^CYAN%^Report%^RESET%^: $I$5="+stuff[REPORT]+"$I$0="
    "  %^YELLOW%^High Lord%^RESET%^: "+stuff[HIGHLORD]+"\n"
    "  %^YELLOW%^High Lord Decision%^RESET%^: "+stuff[DECISION];
  for(i = 8; i < sizeof(stuff); i += 2) {
    report += "  %^YELLOW%^Comment from "+stuff[i]+"%^RESET%^: "
      +stuff[i + 1];
  }
  return report;
} /* query_recent_cases() */

/**
 * @ignore yes
 */
string case_history(string name) {
  int i, j;
  mixed *history, *cases, *stuff, *all_cases, *check_case;
  string report, *other_chars = ({ });
  history = get_dir(SAVEPATH+lower_case(name)+".o");
  all_cases = get_dir(SAVEPATH);
  all_cases -= ({"recent.o", "new.o"});
  for(i = 0; i < sizeof(all_cases); i++) {
    check_case = restore_variable(read_file((SAVEPATH+all_cases[i])));
    for(j = 0; j < sizeof(check_case); j++) {
      if(arrayp(check_case[j][CHARS])) {
        if(member_array(capitalize(name), check_case[j][CHARS]) != -1) {
          other_chars += check_case[j][CHARS];
          other_chars += ({check_case[j][NAME]});
        }
      }
    }
  }
  if(sizeof(other_chars)) {
    uniq_array(other_chars);
    for(i = 0; i < sizeof(other_chars); i++) {
      other_chars[i] = capitalize(other_chars[i]);
    }
    other_chars -= ({capitalize(name)});
    other_chars = uniq_array(other_chars);
    write(capitalize(name)+" is also "
      +query_multiple_short(other_chars)+".\n");    
  }
  if(!sizeof(history)) {
    return 0;
  }
  write("%^CYAN%^Case History for "+capitalize(name)+"%^RESET%^:\n");
  report = "";
  cases = restore_variable(read_file(SAVEPATH+lower_case(name)+".o"));
  for(i = 0; i < sizeof(cases); i++) {
    stuff = cases[i];
    report += "  %^CYAN%^Suspect%^RESET%^: "+capitalize(stuff[NAME])+"\n";
    if(!sizeof(stuff[CHARS])) {
      report += "  %^CYAN%^Other Characters%^RESET%^: Unknown.\n";
    } else {
      report += "  %^CYAN%^Other Characters%^RESET%^: "
        "$I$5="+query_multiple_short(stuff[CHARS])+"$I$0=\n";
    }
    report +=  "  %^CYAN%^Creator%^RESET%^: "+capitalize(stuff[CREATOR])+"\n"
      "  %^CYAN%^Time%^RESET%^: "+stuff[TIME]+"\n"
      "  %^CYAN%^Offence%^RESET%^: "+stuff[CATEGORY]+"\n"
      "  %^CYAN%^Report%^RESET%^: $I$5="+stuff[REPORT]+"$I$0="
      "  %^YELLOW%^High Lord%^RESET%^: "+stuff[HIGHLORD]+"\n"
      "  %^YELLOW%^High Lord Decision%^RESET%^: "+stuff[DECISION];
    for(j = 8; j < sizeof(stuff); j += 2) {
      report += "  %^YELLOW%^Comment from "+stuff[j]+"%^RESET%^: "
        +stuff[j + 1];
    }
  }
  return report;
} /* case_history() */

/**
 * @ignore yes
 */
void add_comment(int i, string name, string comment) {
  i--;
  recent_cases[i] += ({ capitalize(name), comment });
  save_recent_cases();
} /* add_comment() */

