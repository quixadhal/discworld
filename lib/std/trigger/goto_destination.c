/*
 * This is similar to the goto_co_ord function...  except with this one
 * you just specify a destination room.  The room can be an object or
 * a string.  You use the property 'goto destination' to set it.
 */

#define MAX_MOVES 300
mapping been;
int num_moves;

void create() {
  string str;
  int i;

  seteuid("monster");
  if (sscanf(file_name(this_object()), "%s#%d", str, i) != 2)
    return ;
  call_out("sail_away", 0);
  been = ([ ]);
} /* create() */

static mixed get_co_ord(mixed ob) {
  mixed bing;
  if (catch(bing = (mixed)ob->query_co_ord()))
/* Way out of bounds hopefully */
    return ({ 10000, 10000, 10000 });
  if (bing)
    return bing;
  ob->calc_co_ord();
  return (mixed)ob->query_co_ord();
} /* get_co_ord() */

static int distance(mixed *co_ord, mixed *co_ord2) {
  int i, ret;

  if (!pointerp(co_ord) || !pointerp(co_ord2))
    return 1000000;
  i = co_ord[0] - co_ord2[0]; 
  ret = (i<0?-i:i);
  i = co_ord[1] - co_ord2[1]; 
  ret += (i<0?-i:i);
  i = co_ord[2] - co_ord2[2]; 
  ret += (i<0?-i:i);
  return ret;
} /* distance() */

static void finitio(int bing) {
  mixed *bit;
  int i;

  bit = (mixed *)this_object()->query_actions("goto_destination") + ({ });
  this_object()->remove_trigger("goto_destination");
  for (i=0;i<sizeof(bit);i+=2)
    call_other(bit[i+1][0], bit[i+1][1], bing);
  return ;
} /* finitio() */

/* sail away into the sunset */
void sail_away() {
  mixed co_ord, t_guild, *dirs;
  mixed env, dest;
  int min_dist, i, k, dir_pos, val;
  string dir;
  object *obs;

  co_ord = get_co_ord(env = (object)this_object()->query_current_room());
  if (!pointerp(co_ord)) /* Eeeek! */
    return finitio(0);
  this_object()->set_move_after(0, 0);
                             /* we don't wante them randomly rushing around
                              * as we do this! */
  dest = (object)this_object()->query_property("goto destination");
  if (stringp(dest)) {
    call_other(dest, "??");
    dest = find_object(dest);
  }
  t_guild = get_co_ord(dest);
  if (!t_guild)
    return finitio(0);
  if (stringp(env))
    env = find_object(env);
  if (!distance(co_ord, t_guild))
    if (env == dest) {
      return finitio(1);
      return ;
    }
  dirs = (string *)env->query_dest_dir();
  if (num_moves++ > MAX_MOVES) {
    return finitio(0);
    return ;
  }
  while (1) {
    min_dist = 1000000;
    if (sizeof(dirs) == 2) {
/* Choice?  whats that? */
/* Hmmm.  cant leave through the only exit.  Sad...  */
      been[env]++;
      catch(val = this_object()->do_command(dirs[0]));
      if (!val)
        return finitio(0);
      else
        call_out("sail_away", 2+random(10));
/*
    this_object()->set_move_after(10, 10);
 */
      return ;
    }
    for (i=0;i<sizeof(dirs);i+=2) {
      catch(call_other(dirs[i+1], "??"));
      if (objectp(dirs[i+1]))
        k = been[dirs[i+1]]*50;
      else /* if (member_array(find_object(dirs[i+1]), from) != -1) */
        k = been[find_object(dirs[i+1])]*50;
      if ((k=(distance(t_guild, get_co_ord(dirs[i+1]))+k)) < min_dist) {
        dir = dirs[i];
        dir_pos = i;
        min_dist = k;
      }
    }
    if (!dir) {/* Argghh.  we have already been through them all!  give up.  lost */
      finitio(0);
      return ;
    }
    catch (val = (int)this_object()->do_command(dir));
    if (val || env != (object)this_object()->query_current_room()) {
      been[env]++;
      call_out("sail_away", 2+random(10));
      return ;
    } else
      dirs = delete(dirs, dir_pos, 2);
  }
} /* sail_away() */

mapping query_been_array() { return been; }
