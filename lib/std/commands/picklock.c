int pick(string str) {
  string dir;

  if(!sscanf(str, "pick %s", dir)) {
    notify_fail("pick <direction>");
    return 0;
  }
  if(member_array(dir, (string *)environment(this_player())->query_dest_dir()) == -1) {
    notify_fail("No door in that direction.\n");
    return 0;
  }
  return 1;
}
