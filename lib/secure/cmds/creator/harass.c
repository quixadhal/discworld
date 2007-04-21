string info = "";
mapping busy = ([]);

void create(){
  seteuid("Root");
}

void receive_snoop(string bing){
  info += bing;
}

string query_result(){ return info; }

void remind(){
  if(busy[this_player()]){
    write("Type harass again to stop logging.\n");
    call_out((:remind:), 60);
  }
}

mixed *query_patterns(){
  return ({"", function(){
      if(busy[this_player()]){
        string result = busy[this_player()]->query_result();
        unguarded((:write_file,sprintf("/d/liaison/secure/harass/%s%d", 
                                       this_player()->query_name(), time()),
                   $(result):));
        destruct(busy[this_player()]);
        map_delete(busy, this_player());
        write("Log written.\n");
        return 1;
      } else {
        busy[this_player()] = clone_object(__FILE__);
        if(efun::snoop(busy[this_player()], this_player())){
          write("Started logging.\nType harass again to stop.\n");
          call_out((:remind:), 60);
          return 1;
        } else {
          destruct(busy[this_player()]);
          map_delete(busy, this_player());
          write("Failed.\n");
          return 1;
        }
      }
    }
  });
}
