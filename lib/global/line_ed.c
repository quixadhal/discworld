#include "ed.h"
/*
 * Expansion of the editor.  We will now allow the player to use any
 * or three different editors.  One of them is the default (the old
 * one) one of them is ed and the other I am going to write in a
 * few secs....
 *
 * cute line editor thing.  Which even a player could use!
 */

static string *lines, end_func;
static object end_object;
static int insertion_point;
string editor; /* The editor to use... */

void main_bit(string str);
void editor_do_quit(string str);

void create() {
  editor = "menu";
} /* create() */

void editor_commands() {
  add_action("set_editor", "editor");
} /* edito_commands() */

int do_edit(string str, string end_f, object end_o) {

  if (!end_f) {
    write("Someone has stuffed up.\n");
    return 0;
  }
  end_func = end_f;
  if (!end_o)
    end_object = previous_object();
  else
    end_object = end_o;
  if (!str)
    str = "";
  switch (editor) {
    case "menu" :
      lines = explode(str, "\n") - ({ 0 });
      write("Ok, enter your text.  ** on a line by itself to exit.\n");
      insertion_point = sizeof(lines);
      printf("%-2d] ", insertion_point+1);
      input_to("editor_loop");
      return 1;
    case "ed" :
      if (stringp(str) && str != "")
        write_file(TMP_FILE, str);
      ed(TMP_FILE, "editor_finish_ed");
      return 1;
    case "command" :
      lines = explode(str, "\n") - ({ 0 });
      write("Ok, enter your text.  ** on a line by itself to quit.  ~h for help.\n");
      insertion_point = sizeof(lines);
      printf("%-2d] ", insertion_point+1);
      input_to("editor_loop");
      return 1;
  }
} /* do_edit() */

void main_bit(string str) {
  int i;
  string s;

  if (str != "") {
    while (str[0] == ' ')
      str = str[1..100];
    switch (str[0]) {
      case 'i' :
      case 'I' :
                write("Insert before line : ");
                input_to("editor_insert");
                return ;
      case 'd' :
      case 'D' :
                write("Delete (line no, or n..m ie 1..2) : ");
                input_to("editor_delete");
                return ;
      case 'l' :
      case 'L' :
                s = "";
                for (i=0;i<sizeof(lines);i++)
                  s += sprintf("%3d: %s\n", i+1, lines[i]);
                this_player()->set_finish_func("end_of_edit_more");
                this_player()->more_string(s);
                return ;
      case 'm' :
      case 'M' :
                write("Modify which line (line no, or n..m ie 1..2) : ");
                input_to("editor_modify");
                return ;
      case 'c' :
      case 'C' :
                write(sprintf("Ok, back into insertion mode, ** on a line by "+
                      "itself to exit.\n%-2d] ", insertion_point+1));
                insertion_point = sizeof(lines);
                input_to("editor_loop");
                return ;
      case 'e' :
      case 'E' :
                write("Entering ed....  Use 'q' to quit, 'x' to save and "+
                      "exit, 'Q' to quit without saveing changes and 'h' "+
                      "for help.\n");
                write_file(TMP_FILE, implode(lines,"\n"));
                ed(TMP_FILE, "editor_exit_ed");
                return ;
      case 's' :
      case 'S' :
                write("Quiting and saving.\n");
                editor_do_quit(implode(lines, "\n"));
                return ;
      case 'q' :
      case 'Q' :
                write("Are you sure you want to quit (Y/N)? ");
                input_to("editor_check_quit");
                return ;
      case 'h' :
      case 'H' :
                s = read_file(ED_HELP_FILE);
                this_player()->set_finish_func("end_of_edit_more");
                this_player()->more_string(s);
                return ;
       default :
                write("I do not understand you.  Try "+(editor == "command"?
                      "~h":"h")+" for help.\n");
                main_bit("");
                return ;
    }
  }
  if (editor == "menu") {
    write(sizeof(lines)+" lines - Choose from IDLMCESQ H for help. ");
    input_to("main_bit");
  } else {
    printf("%-2d] ", insertion_point+1);
    input_to("editor_loop");
  }
  return ;
} /* editor_loop() */

void end_of_edit_more() {
  this_player()->set_finish_func(0);
  main_bit("");
} /* end_of_edit_more() */

void editor_delete(string str) {
  int num1, num2, tmp;

  if (sscanf(str, "%d..%d", num1, num2) == 2) {
    if (num1 > num2) {
      tmp = num1;
      num1 = num2;
      num2 = tmp;
    }
    if (num1 < 1 || num2 > sizeof(lines)+1) {
      write("Out of bounds.\n");
      main_bit("");
    }
    write("Deleteing from line "+num1+" to line "+num2+".\n");
    lines = delete(lines, num1-1, num2-num1+1);
    write("Ok.\n");
    main_bit("");
    return ;
  }
  if (sscanf(str, "%d", num1) == 1) {
    if (num1 < 1 || num1 > sizeof(lines)) {
      write("Line number out of range.\n");
      main_bit("");
      return ;
    }
    write("Deleteing line "+num1+".\n");
    lines = delete(lines, num1-1, 1);
    write("Ok.\n");
    main_bit("");
    return ;
  }
  write("Error, invalid data "+str+".\n");
  main_bit("");
  return ;
} /* editor_delete() */

void editor_insert(string str) {
  int num;

  if (sscanf(str, "%d", num) != 1) {
    write("ERROR! Must be a number.\n");
    main_bit("");
    return ;
  }
  if (num < 1 || num > sizeof(lines)+1) {
    write("Number out of bounds.\n");
    main_bit("");
    return ;
  }
  insertion_point = num -1;
  write("Inserting before line "+num+".  Entering insertion mode, ** on a "+
        "line by itself to exit\n] ");
  input_to("editor_loop");
  return ;
} /* editor_insert() */

void editor_loop(string str) {
  if (strlen(str) > 1 && str[0] == '~' && editor == "command") {
    main_bit(str[1..1000]);
    return ;
  }
  if (str == "**") {
    if (editor == "menu")
      main_bit("");
    else if (sizeof(lines))
      editor_do_quit(implode(lines, "\n"));
    else
      editor_do_quit(0);
    return ;
  }
  lines = lines[0..insertion_point-1]+({ str })+lines[insertion_point..1000];
  insertion_point++;
  printf("%-2d] ", insertion_point+1);
  input_to("editor_loop");
  return ;
} /* editor_loop() */

static int range1, range2;
static string modify_string;

void editor_modify(string str) {
  int num1, num2, tmp;

  if (sscanf(str, "%d..%d", num1, num2) == 2) {
    if (num1 > num2) {
      tmp = num1;
      num1 = num2;
      num2 = tmp;
    }
    if (num1 < 1 || num2 > sizeof(lines)+1) {
      write("Out of bounds.\n");
      main_bit("");
    }
    write("Modifying from line "+num1+" to line "+num2+".\n");
    write("Text to change ? ");
    range1 = num1;
    range2 = num2;
    input_to("editor_modify2");
    return ;
  }
  if (sscanf(str, "%d", num1) == 1) {
    if (num1 < 1 || num1 > sizeof(lines)) {
      write("Line number out of range.\n");
      main_bit("");
      return ;
    }
    write("Modifying line "+num1+".\n");
    write("Text to change ? ");
    input_to("editor_modify2");
    range1 = range2 = num1;
    return ;
  }
  write("Error, invalid data "+str+".\n");
  main_bit("");
  return ;
} /* editor_modify() */

void editor_modify2(string str) {
  if (!str || str == "") {
    write("Aborting.\n");
    main_bit("");
    return ;
  }
  write("Change to ? ");
  input_to("editor_modify3");
  modify_string = str;
  return ;
} /* editor_modify2() */

void editor_modify3(string str) {
  int i;
  string s1, s2, s3;

  write("Changing all occurances of '"+modify_string+"' to '"+str+"' "+
        "from "+range1+" to "+range2+".\n");
  for (i=range1-1; i<range2;i++)
    if (sscanf(lines[i], "%s"+modify_string+"%s", s1, s2) == 2) {
      s1 += str;
      while (sscanf(s2, "%s"+modify_string+"%s", s3, s2) == 2)
        s1 += s3+str;
      lines[i] = s1 + s2;
      write(sprintf("%3d: %s\n", i+1, lines[i]));
    }
  write("Done.\n");
  main_bit("");
  return ;
} /* editor_modify3() */

void editor_exit_ed() {
  string str;

  str = read_file(TMP_FILE);
  if (!rm(TMP_FILE))
    log_file(LOG_FILE, "ed: couldn't rm "+TMP_FILE+"\n");
  lines = explode("#"+str, "\n");
  lines[0] = lines[0][1..1000];
/*
  lines[sizeof(lines)-1] = lines[sizeof(lines)-1][0..
                                 strlen(lines[sizeof(lines)-1])-2];
 */
  main_bit("");
  return ;
} /* editor_exit_ed() */

void editor_finish_ed() {
  string str;

  str = read_file(TMP_FILE);
  if (!rm(TMP_FILE))
    log_file(LOG_FILE, "ed: couldn't rm "+TMP_FILE+"\n");
  if (!str) {
    write("Aborted.\n");
    editor_do_quit(0);
    return ;
  }
  lines = explode(str, "\n");
  if (sizeof(lines))
    editor_do_quit(str);
  else
    editor_do_quit(0);
  return ;
} /* editor_finish_ed() */

void editor_do_quit(string str) {
  lines = ({ });
  call_other(end_object, end_func, str);
  end_object = 0;
} /* editor_do_quit() */

void editor_check_quit(string str) {
  if (str != "" && str[0] != 'y' && str[0] != 'Y') {
    write("Returing to the editor.\n");
    main_bit("");
    return ;
  }
  write("Quiting.\n");
  editor_do_quit(0);
  end_object = 0;
  return ;
} /* editor_check_quit() */

int set_editor(string str) {
  if (member_array(str, ({ "menu", "command", "ed" }) ) == -1) {
    notify_fail("Your editor can be set to one of the following menu, "+
                "ed and command.  Please do not set your editor to ed "+
                "unless you know what you are doing.  The menu editor "+
                "is the default and the command editor is based on "+
                "using ~ escape characters.\n");
    return 0;
  }
  editor = str;
  write("Editor set to "+str+".\n");
  return 1;
} /* set_editor() */

string append_signature() {
  mapping sig;

  sig = (mapping)this_player()->query_aliases();
  if (sig[".signature"]) {
    return "\n--\n"+replace((string)"/secure/finger"->make_string(
                            sig[".signature"]), ";", "\n")+"\n";
  }
  return "";
} /* append_signature() */

void editor_check_do_quit() {
  if (end_object) {
/* Force them to sve the thingy they are editing. */
    write("Saveing what you are editing.\n");
    if (editor != "ed")
      editor_do_quit(implode(lines, "\n"));
    else
      editor_finish_ed();
  }
} /* editor_check_do_quit() */
