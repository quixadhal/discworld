inherit "std/smart_log";

void logging_commands() {
  add_action("log_bug", "bug");
  add_action("log_typo", "typo");
  add_action("log_idea", "idea");
  add_action("log_soul", "soul");
}

int log_typo(string str) {
  if(!str || "" == str) {
    notify_fail("Usage : typo <text>\n");
    return 0;
  }
  smart_log("TYPO", this_player()->query_name() + "\n" + str + "\n\n");
  write("Ok.\n");
  return 1;
}

int log_bug(string str) {
  if(!str || "" == str) {
    notify_fail("Usage : bug <text>\n");
    return 0;
  }
  smart_log("BUG", this_player()->query_name() + "\n" + str + "\n\n");
  write("Ok.\n");
  return 1;
}

int log_idea(string str) {
  if(!str || "" == str) {
    notify_fail("Usage : idea <text>\n");
    return 0;
  }
  smart_log("IDEA", this_player()->query_name() + "\n" + str + "\n\n");
  write("Ok.\n");
  return 1;
}

int log_soul(string str) {
  if(!str || str == "") {
    notify_fail("Usage : soul <text>\n");
    return 0;
  }
  log_file("soulcommands", this_player()->query_name() + " " + str + "\n");
  write("Ok.\n");
  return 1;
}
