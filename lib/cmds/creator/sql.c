/**
 * sql interface command
 * @author Wodan
 */
#include <config.h>
#include <db.h>

inherit "/cmds/base";

private mapping fd = ([]), com = ([]);
#define FD fd[this_player()]
#define COM com[this_player()]

void prompt(string db, string user, string pass, object player){
   if(!strlen(COM)) {
      tell_object(player, "sql> ");
   } else {
      tell_object(player, "  -> ");
   }
   input_to("main", 0, db, user, pass);
}

int cmd(string arg, string user, string passwd){
   string err;

   if(!passwd){
      passwd = "";
   }
   if(!user){
      user = CONFIG_DB_USER;
   }

   if (stringp(err)){
      return notify_fail(err);
   }
  
   write("Welcome to the " + mud_name() + " SQL monitor.  Commands end with ; .\n\nType 'help' for help."
        "\n\n");
   COM = "";
   prompt(arg, user, passwd, this_player());
   return 1;
}

void finish_query(int result, mixed params, string db, string user,
                  string passwd, object player) {
   mapping row;
   string *keys;
   string key;
   if (result != DB_SUCCESS) {
      tell_object(player, "Failed: " + params + ".\n");
      return ;
   }
   foreach (row in params) {
      if (!keys) {
         keys = keys(row);
         foreach (key in keys) {
            tell_object(player, key + "   ");
         }
      }
      foreach (key in keys) {
         tell_object(player, row[key] + "   ");
      }
      tell_object(player, "\n");
   }
   prompt(db, user, passwd, player);
} /* finish_quer() */

void main(string input, string db, string user, string passwd){
   if(strlen(input)) {
      while(input[<1] == ' ') {
         input = input[0..<2];
      }
      while(input[0] == ' ') {
         input = input[1..];
      }
   }

  switch(input){
  case "clear":
    COM = "";
    return prompt(db, user, passwd, this_player());
  case "edit":
    this_player()->do_edit(COM, "edfin", this_object(), 0, ({ db, user, passwd }));
    return;
  case "quit":
    printf("Use 'exit' to quit.\n");
    return prompt(db, user, passwd, this_player());
  case "exit":
    //db_close(FD);
    map_delete(fd, this_player());
    map_delete(com, this_player());
    return;
  case "help":
    printf(@END
'help' prints this help.
'clear' clears the command you are working on.
'edit' enters the editor so you can edit your query.
'exit' quits sql.
END);
    return prompt(db, user, passwd, this_player());
  default:
    if(strlen(input) && input[<1] == ';'){

      COM += input[0..<2] + "\n";
      //
      // Do the request.
      //
      DB_HANDLER->make_sql_request(db, user, passwd, input,
                                  (: finish_query($1, $2, $(db), $(user), $(passwd), $(this_player())) :));
      input_to("main", db, user, passwd);
      COM="";
     tell_creator ("drakkos", "Bing %s.\n", input);
      return ;
/*
      if(stringp(rows)){
        printf("%s\n", rows);
        return prompt(db, user, passwd);
      }
      for(i = 0; i <= rows; i++){
        printf("%s\n", implode(db_fetch(FD, i), (:$1 + " " + $2:), ""));
      }
 */
    } else {
      COM += input + "\n";
    }
    prompt(db, user, passwd, this_player());
  }
}

void edfin(string input, string* args){
  COM=input;
  prompt(args[0], args[1], args[2], this_player());
}

mixed *query_patterns() { 
   return ({ "<word'database'>", (:cmd($4[0], CONFIG_DB_USER, 0) :),   
            "<word'database'> -u <word'user'>", (:cmd($4[0], $4[1], 0):),
            "<word'database'> -u <word'user'> -p <word'password'>",
                                    (:cmd($4[0], $4[1], $4[2]):)});
}
