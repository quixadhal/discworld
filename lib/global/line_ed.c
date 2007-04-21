/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: line_ed.c,v 1.23 2001/08/25 01:52:19 ceres Exp $
 */

#include "ed.h"
#include <player_handler.h>

/**
 * This allows a piece of text to be placed into the players own
 * line editor (which ever one they wish to use).
 * <p>
 * Expansion of the editor.  We will now allow the player to use any
 * of three different editors.  One of them is the default (the old
 * one) one of them is ed and the other I am going to write in a
 * few secs....
 * <p>
 * cute line editor thing.  Which even a player could use!
 * @author Pinkfish
 */

/* September '97 - modified by Presto to allow players to use the 'magic'
 *                 editor and to allow cres to use it as their default file
 *                 editor.  Also cleaned up the code a bit cos it was painful
 *                 to look at.  Is this file a bunch of crocky code or what?
 */
private nosave string *_lines;
private nosave mixed _end_func;
//private nosave object _end_object;
private nosave object _ed_ob = 0;
//private nosave mixed *_other;
private nosave int _insertion_point;
private string editor; /* The editor to use... */
private nosave string _filename;

private nosave int _in_editor;

private void main_bit(string str);
int set_editor(string name);
private void editor_do_quit(string str);
private void editor_loop(string str);
private void editor_insert(string str);
private void editor_delete(string str);
private void editor_modify(string str);
private void editor_check_quit(string str);
private void editor_modify2(string str, int range1, int range2);
private void editor_modify3(string str, int range1, int range2,
                            string modify_string);

protected void create()  {
  editor = "menu";
} /* create() */

/** @ignore yes */
protected void editor_commands()  {
  add_command("editor", "{menu|magic|ed|command}", (: set_editor($4[0]) :));
} /* edito_commands() */

/**
 * Edit a string.  This will edit the string or file name and then
 * call the end function and end object at the end.  It will pas in
 * the string if the edit is successful, or 0 if it is not.
 * @param str the string to edit
 * @param end_f the function to call
 * @param end_o the object to call it on (defaul: previous_object())
 * @param fname a specific filename to edit
 * @param extra an array of any parameters you need to carry into the end function
 * @return 0 on failure.
 */
varargs int do_edit(string str, mixed end_f, object end_o, string fname,
                    mixed extra)  {
  if (!end_f) {
    printf("Someone has stuffed up - there is no end function.\n");
    return 0;
  } else {
    _end_func = end_f;
  }
/*
  if(extra) {
    _other = extra;
  }
  if ( !end_o ) {
    _end_object = previous_object();
  } else {
    _end_object = end_o;
  }
 */
  if (!end_o) {
    end_o = previous_object();
  }

  if (!str) {
    str = "";
  }

  if (!strlen(fname)) {
    _filename = TMP_FILE;
  } else {
    _filename = fname;
  }

  if (!functionp(end_f)) {
    if (extra) {
      _end_func = (: call_other($(end_o), $(end_f), $1, $(extra)) :);
    } else {
      _end_func = (: call_other($(end_o), $(end_f), $1) :);
    }
  } else {
    if (extra) {
      _end_func = (: evaluate($(end_f), $1, $(extra)) :);
    } else {
      _end_func = (: evaluate($(end_f), $1) :);
    }
  }

  switch (editor) {
  case "menu":
    _lines = explode(str, "\n") - ({ 0 });
    printf("Enter your text.  Use ** on a line by itself to exit.\n");
    _insertion_point = sizeof(_lines);
    printf("%-2d] ", _insertion_point + 1);
    _in_editor = 1;
    input_to((: editor_loop :));
    return 1;
  case "ed":
    if (strlen(str) && (_filename == TMP_FILE))
      write_file(_filename, str);
    _in_editor = 1;
    ed(_filename, "editor_finish_ed", !this_object()->query_creator());
    return 1;
  case "magic":
    if (strlen(str) && (_filename == TMP_FILE))
      write_file(_filename, str);
    _in_editor = 1;
    _ed_ob = new(PRESTOS_ED, _filename, _filename != TMP_FILE);
    return 1;
  case "command":
    _lines = explode(str, "\n") - ({ 0 });
    printf("Enter your text.  Use ** on a line by itself to exit or "
           "~h for help.\n");
    _insertion_point = sizeof(_lines);
    printf("%-2d] ", _insertion_point + 1);
    _in_editor = 1;
    input_to((: editor_loop :));
    return 1;
  }
} /* do_edit() */

/** @ignore yes */
private void main_bit(string str) {
  int i;
  string s;

  if (str != "") {
    while (str[0] == ' ') {
      str = str[1..];
    }
    switch (str[0]) {
    case 'i':
    case 'I':
      printf("Insert before line: ");
      input_to((: editor_insert :));
      return;
    case 'd':
    case 'D':
      printf("Delete (line no, or n..m ie 1..2): ");
      input_to((: editor_delete :));
      return;
    case 'l':
    case 'L':
      if (!sizeof(_lines))  {
        main_bit("");
      } else {
        s = "";
        for (i = 0; i < sizeof(_lines); i++)
          s += sprintf("%3d: %s\n", i + 1, _lines[i]);
        this_object()->set_finish_func("end_of_edit_more");
        this_object()->more_string(s);
      }
      return;
    case 'm':
    case 'M':
      printf("Modify which line (line no, or n..m ie 1..2): ");
      input_to((: editor_modify :));
      return;
    case 'c':
    case 'C':
      _insertion_point = sizeof(_lines);
      printf("Okay, back into insertion mode.  Use ** on a line "
             "by itself to exit.\n%-2d] ", _insertion_point + 1);
      input_to((: editor_loop :));
      return;
    case 'e':
    case 'E':
      printf("Entering ed... Use \"q\" to quit, \"x\" to save and "
             "quit, \"Q\" to quite without saving changes and \"h\" "
             "for help.\n");
      write_file(_filename, implode(_lines, "\n"));
      ed(_filename, "editor_exit_ed", !this_object()->query_creator());
      return;
    case 's':
    case 'S':
      printf("Quitting and saving.\n");
      editor_do_quit(implode(_lines, "\n"));
      return;
    case 'q':
    case 'Q':
      printf("Are you sure you want to quit? (Y/N) ");
      input_to((: editor_check_quit :));
      return;
    case 'h':
    case 'H':
      s = read_file(ED_HELP_FILE);
      this_object()->set_finish_func("end_of_edit_more");
      this_object()->more_string(s);
      return;
    default:
      printf("I do not understand you.  Try %s for help.\n",
             (editor == "command" ? "~h" : "h" ));
      main_bit("");
      return;
    }
  }
  if (editor == "menu") {
    printf(sizeof(_lines) +" lines - Choose from IDLMCESQ or H for help.");
    input_to((: main_bit :));
  }
  else {
    printf("%-2d] ", _insertion_point + 1);
    input_to((: editor_loop :));
  }
  return;
} /* main_bit() */

/** @ignore yes */
void end_of_edit_more()  {
  this_object()->set_finish_func(0);
  main_bit("");
} /* end_of_edit_more() */

/** @ignore yes */
private void editor_delete(string str)  {
  int num1, num2, tmp;

  if (sscanf(str, "%d..%d", num1, num2) == 2)  {
    if (num1 > num2)  {
      tmp = num1;
      num1 = num2;
      num2 = tmp;
    }
    if ((num1 < 1) || (num2 > (sizeof(_lines) + 1))) {
      printf("Out of bounds.\n");
      main_bit("");
    }
    printf("Deleting from line %d to line %d.\n", num1, num2);
    _lines = delete(_lines, num1 - 1, num2 - num1 + 1);
    printf("Okay.\n");
    main_bit("");
    return;
  }
  if (sscanf(str, "%d", num1) == 1)  {
    if ((num1 < 1) || (num1 > (sizeof(_lines) + 1)))  {
      printf("Line number out of range.\n");
      main_bit("");
      return;
    }
    printf("Deleting line %d.\n", num1);
    _lines = delete(_lines, num1 - 1, 1);
    printf("Okay.\n");
    main_bit("");
    return;
  }
  printf("Error: invalid data %s.\n", str);
  main_bit("");
  return;
} /* editor_delete() */

/** @ignore yes */
private void editor_insert(string str)  {
  int num;

  if (sscanf(str, "%d", num) != 1)  {
    printf("Error: must be a number.\n");
    main_bit("");
    return;
  }
  if ((num < 1) || (num > (sizeof(_lines) + 1))) {
    printf("Error: number out of bounds.\n");
    main_bit("");
    return;
  }
  _insertion_point = num - 1;
  printf("Inserting before line %d.  Entering insertion mode.  Use ** on a "
         "line by itself to exit\n%-2d] ", num, num);
  input_to((: editor_loop :));
  return;
} /* editor_insert() */

/** @ignore yes */
private void editor_loop(string str) {
  if ((strlen(str) > 1) && (str[0] == '~') && (editor == "command")) {
    main_bit(str[1..]);
    return;
  }
  if (str == "**") {
    if (editor == "menu") {
      main_bit("");
    } else if (sizeof(_lines)) {
      editor_do_quit(implode(_lines, "\n"));
    } else {
      editor_do_quit(0);
    }
    return;
  }
  _lines = _lines[0.._insertion_point - 1] + ({ str }) +
    _lines[_insertion_point..];
  ++_insertion_point;
  printf("%-2d] ", _insertion_point + 1);
  input_to((: editor_loop :));
  return;
} /* editor_loop() */

/** @ignore yes */
private void editor_modify(string str)  {
  int num1, num2, tmp;

  if (sscanf(str, "%d..%d", num1, num2) == 2)  {
    if (num1 > num2)  {
      tmp = num1;
      num1 = num2;
      num2 = tmp;
    }
    if ((num1 < 1) || (num2 > (sizeof(_lines) + 1))) {
      printf("Error: number out of bounds.\n");
      main_bit("");
    }
    printf("Modifying from line %d to line %d.\n", num1, num2);
    printf("Text to change? ");
    input_to((: editor_modify2 :), 0, num1, num2);
    return;
  }
  if (sscanf(str, "%d", num1) == 1)  {
    if ((num1 < 1) || (num1 > (sizeof(_lines) + 1)))  {
      printf("Error: line number out of range.\n");
      main_bit("");
      return;
    }
    printf("Modifying line %d.\n", num1);
    printf("Text to change? ");
    input_to((: editor_modify2 :), 0, num1, num1);
    return;
  }
  printf("Error: invalid data %s.\n", str);
  main_bit("");
  return;
} /* editor_modify() */

/** @ignore yes */
private void editor_modify2(string str, int range1, int range2)  {
  if (!strlen(str))  {
    printf("Aborting.\n");
    main_bit("");
    return;
  }
  printf("Change to: ");
  input_to((: editor_modify3 :), 0, range1, range2, str);
  return;
} /* editor_modify2() */

/** @ignore yes */
private void editor_modify3(string str, int range1, int range2,
                            string modify_string) {
  int i;

  printf("Changing all occurrences of \"%s\" to \"%s\" from line %d to "
         "line %d.\n", modify_string, str, range1, range2);
  for (i = range1 - 1; (i < range2) && (i < sizeof(_lines)); i++) {
    _lines[i] = replace_string(_lines[i], modify_string, str);
      printf("%3d: %s\n", i + 1, _lines[i]);
  }
  printf("Done.\n");
  main_bit("");
  return;
} /* editor_modify3() */

/** @ignore yes */
void editor_exit_ed()  {
  string str;

  str = read_file(_filename);
  if ((file_size(_filename) >= 0) && !rm(_filename))
    log_file(LOG_FILE, "ed: couldn't rm %s\n", _filename);
  _lines = explode("#" + str, "\n");
  _lines[0] = _lines[0][1..];
/*
  _lines[sizeof(_lines)-1] = _lines[sizeof(_lines)-1][0..
                                 strlen(_lines[sizeof(_lines)-1])-2];
 */
  main_bit("");
  return;
} /* editor_exit_ed() */

/** @ignore yes */
void editor_finish_ed()  {
  string str;

  str = read_file(_filename);
  if ((file_size(_filename) >= 0) && (_filename == TMP_FILE) &&
      !rm(_filename)) {
    log_file(LOG_FILE, "ed: couldn't rm %s\n", _filename);
  }
  if (!str)  {
    printf("Aborted.\n");
    editor_do_quit(0);
    return;
  }
  if (_filename == TMP_FILE)  {
    editor_do_quit(str);
  } else {
    editor_do_quit(0);
  }
  return;
} /* editor_finish_ed() */

/** @ignore yes */
void editor_do_quit(string str)  {
  //object prev_end_object;

  _lines = ({ });
/*
  if (str) str = replace(str, "@@", "@ @ ");
*/
  if ((editor == "magic") && (_filename == TMP_FILE) &&
      (file_size(_filename) >= 0) && !rm(_filename))  {
    printf("magic: couldn't remove %s\n" , _filename);
    str = 0;
  }
  _in_editor = 0;
/*
  prev_end_object = _end_object;
  _end_object = 0;
  if (functionp(_end_func)) {
    if(_other) {
      evaluate(_end_func, str, _other);
    } else {
      evaluate(_end_func, str);
    }
  } else {
    if(_other) {
      call_other(prev_end_object, _end_func, str, _other);
    } else {
      call_other(prev_end_object, _end_func, str);
    }
  }
 */
  if (_ed_ob) {
    destruct(_ed_ob);
  }
  evaluate(_end_func, str);
} /* editor_do_quit() */

/** @ignore yes */
private void editor_check_quit(string str)  {
  if ((str != "") && (str[0] != 'y') && (str[0] != 'Y')) {
    printf("Returning to the editor.\n");
    main_bit("");
    return;
  }
  printf("Quitting.\n");
  editor_do_quit(0);
  //_end_object = 0;
  return;
} /* editor_check_quit() */

/** @ignore yes */
int set_editor(string str)  {
/*
  if ( member_array( str, ({ "magic", "menu", "command", "ed" }) ) == -1 )  {
    notify_fail( "Your editor can be set to one of the following menu, "
                 "ed, command, and magic.  Please do not set your editor "
                 "to ed unless you know what you are doing.  The menu "
                 "editor is the default, the command editor is based on "
                 "using ~ escape characters and the magic editor is one "
                 "of Presto's concoctions.\n" );
    return 0;
  }
 */
  editor = str;
  printf("Editor set to %s.\n", str);
  return 1;
} /* set_editor() */

/**
 * This method will create the signature to append to whatever needs
 * a signature.
 * @return the players signature
 */
string append_signature() {
  return PLAYER_HANDLER->query_signature(this_object()->query_name());
} /* append_signature() */

/** @ignore yes */
void editor_check_do_quit()  {
  if (_in_editor)  {
    /* Force them to sve the thingy they are editing. */
    printf("Saving what you are editing.\n");
    if (editor != "ed") {
      editor_do_quit(implode(_lines, "\n"));
    } else {
      editor_finish_ed();
    }
  }
} /* editor_check_do_quit() */

/**
 * This method will return the players current editor setting.
 * @return the current editor setting.
 */
string query_editor() {
  return editor;
}

/**
 * This method returns true if the player is currently in the editor.
 * @return 1 if the player is in the editor
 */
int query_in_editor() {
  return _in_editor;
} /* query_in_editor() */
