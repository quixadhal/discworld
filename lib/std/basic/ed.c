/* ok... This will be ye old standard inherit me if you want me editorS
 * There will be a cloneable version in /obj
 */
#include "ed.h"
int start_ed();

private string my_bit;

int begin_editing(string str) {
  if (str)
    my_bit = str;
  else
    my_bit = "";
  write("Write your text in the following lines.  A \"**\" on a line "+
        "by itself will save the text you have entered.  A \"~q\" "+
        "will abort your text and a \"~e\" will drop you into ed, "+
        "warning don't use ed unless you know what you are doing.\n");
  write("] ");
  input_to("ed_get_line");
}

int ed_get_line(string str) {
  if (!my_bit)
    my_bit = "";
  if (str == "**") {
    this_object()->ed_end_it_all(my_bit);
    my_bit = 0;
  } else if (str == "~q") {
    my_bit = 0;
    this_object()->ed_end_it_all(0);
  } else if (str == "~e")
    start_ed();
  else {
    my_bit += str + "\n";
    write("] ");
    input_to("ed_get_line");
  }
  return 1;
}

void exit_ed() {
  my_bit = read_file(TMP_FILE);
  if (!rm(TMP_FILE))
    log_file(LOG_FILE, "ed: couldn't rm "+TMP_FILE+"\n");
  write("Back into the text enterer.\n");
  write("] ");
  input_to("ed_get_line");
}

void start_ed() {
  write("Entering ed....  Use 'q' to quit, 'x' to save and exit, 'Q' to quit "+
        "without saveing changes and 'h' for help.\n");
  write_file(TMP_FILE, my_bit);
  ed(TMP_FILE, "exit_ed");
}
