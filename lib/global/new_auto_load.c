#define MAGIC_NUMBER 31415
#define VERSION_NUMBER 1
#define MY_LOG "AUTOLOAD"
/* autoloading is important enough (and buggy enough) to warrant its
   own log file */
 
/* structure of autoload string:
 
   ({ "file_name",
      this_object_data1,
      this_object_data2,
      ...
      this_object_datan
   })
  */
 
mixed create_unique_auto_bing(object ob, int load) {/* terminology?  Whats that? */
   mixed bing, *al;
   int i;
   
   al = (mixed *)ob->query_init_data(load);
   bing = ([ ]);
   
   if (!pointerp(al) || sizeof(al) % 3) {
      /* log it somewhere */
      log_file(MY_LOG, "Bad return from 'query_init_data' on " + 
         file_name(ob) + "\n");
      return 0;
   }
   
   for (i = 0; i < sizeof(al); i+=3) 
      bing[al[i]] = ({ al[i+1], al[i+2] });
   
   return bing;
}
 
mixed *object_save(object ob)
/* hmmmm, interesting concept, could we do the entire player file as a
   part of the auto_load string?  :) :) :)  So to save the player,
   you do something like write_file(thingy->create_auto_load(this_player()));
 */
{
   mixed *aload;
   string s;
   int dummy;
   mixed bing, bong;
   
   s = file_name(ob);
   if (!sscanf(s, "%s#%d", s, dummy)) {
      log_file(MY_LOG, "Player has class " + file_name(ob) + "!\n");
      return (mixed *)0;
   }
 
   aload = ({ s });
   
   bing = create_unique_auto_bing(ob, 0);
   if (!bing) return aload;   /* icky icky */
 
   bong = m_values(bing);
   for (dummy = 0; dummy < sizeof(bong); dummy++)
      if (bong[dummy][1]) aload += ({ bong[dummy][0] });
 
   return aload;
}
 
mixed *create_auto_load(object *obs) {
   int i;
   mixed *bing;
 
   bing = ({ MAGIC_NUMBER + VERSION_NUMBER });
 
   for (i = 0; i < sizeof(obs); i++)
      bing += ({ object_save(obs[i]) });
   
   return bing;
}
 
void call_otherv(object ob, string func, mixed *argv)
/* call_other with a vector for arguments */
{
   switch (sizeof(argv)) {
   case 0:
      call_other(ob, func);      return;
   case 1:
      call_other(ob, func, argv[0]); return;
   case 2:
      call_other(ob, func, argv[0], argv[1]); return;
   case 3:
      call_other(ob, func, argv[0], argv[1], argv[2]); return;
   case 4:
      call_other(ob, func, argv[0], argv[1], argv[2], argv[3]); return;
   default:
      log_file(MY_LOG, "object " + file_name(ob) + " needs more args\n");
      return;
   }
}
 
void do_auto_func_call(object ob, string funcspec, mixed arg)
{
   string *bits, func;
   mixed *args;
   int i, j, conditional, multi_arg;
   
   conditional = 0;
   multi_arg = -1;
   
   bits = explode(funcspec, "/");	/* something nice and simple */
   func = bits[0];
   args = ({ });
   
   for (i = 1; i < sizeof(bits); i++) {
      switch (bits[i][0]) {
      case '?':
         conditional = 1;
         break;
      case '#':
         sscanf(bits[i], "#%d", j);
         args += ({ j });
         break;
      case 'p':
         args += ({ arg });
         break;
      case 'P':
         if (bits[i] == "P*") { 
            multi_arg = sizeof(args); 
            args += ({ 0 }); 
         } else {
            sscanf(bits[i], "P%d", j);
            if (pointerp(arg) && j < sizeof(arg)) args += ({ arg[j] });
         }
         break;
      case 'n':
         args += ({ sizeof(arg) });
         break;
      case '\'':
         args += ({ bits[i][1..10000] });
         break;
      default:
         log_file(MY_LOG, "Bad arg type " + bits[i] + " in object " +
            file_name(ob) + "\n");
         break;
      }
   }
   
   if (conditional && !arg) return;
   
   if (multi_arg < 0) 
      call_otherv(ob, func, args);
   else {
      for (i = 0; i < sizeof(arg); i++) {
         args[multi_arg] = arg[i];
         call_otherv(ob, func, args);
      }
   }
   return;
}
 
object object_restore(mixed *auto)
{
   int dummy;
   object ob;
   int next;
   mixed bing, bong;
   
   if (catch(auto[0]->load_please())) {
      log_file(MY_LOG, "Broken object " + auto[0] + "\n");
      return (object)0;
   }
   
   ob = (object)"/global/cloner"->clone(auto[0]);
   
   bing = create_unique_auto_bing(ob, 1);
   bong = m_values(bing);
   
   next = 1;
   for (dummy = 0; dummy < sizeof(bong); dummy++)
      if (bong[dummy][1])
         do_auto_func_call(ob, bong[dummy][1], auto[next++]);
   
   return ob;
}
 
object *load_auto_load(mixed *auto, object dest) {
   object *obs;
   int i;
   
   obs = allocate(sizeof(auto) - 1);
 
   if (auto[0] != MAGIC_NUMBER + VERSION_NUMBER) {
      return (object *)"/global/old_auto_load"->load_auto_load(auto, dest);
   }
   
   for (i = 1; i < sizeof(auto); i++) {
      obs[i-1] = object_restore(auto[i]);
      if (obs[i-1] && dest) 
         obs[i-1]->move(dest);
   }
   
   return obs;
}
 
/* now what is this birthday junk doing in /global/auto_load??? */
 
void birthday_gifts()
{
  if(!sizeof(find_match("birthday card", this_object())))
    if(this_object()->query_is_birthday_today())
      if(!catch(call_other("/obj/b_day/card", "??")) &&
         !catch(call_other("/obj/b_day/demon", "??"))) {
        call_out("card_arrives", 5);
      }
}
 
void card_arrives()
{
    object card;
    
    card = (object)"/global/cloner"->clone("/obj/b_day/card");
    card->move(this_object());
    tell_room(environment(), "You hear a rumbling in the distance. " +
        "Then, suddenly, a malformed goblin wizzes past you, " +
        "frantically pedalling a fire-engine red tricycle!\n");
    write("You feel something thrust into your hand by a greener, " +
        "wartier one.\n");
}
