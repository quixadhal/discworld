/*
 * You add a property called goto_co_ord to the monster before setting
 * up this shadow.  It will then do a bunch of call outs and move the
 * npc to the co_ordinate.  Welll hopefully to the co-ordinate.  The
 * accessing function will be called when it gets there with 1 as
 * a argument or 0 as an argument if it fails.
 *
 * It will check two things for the end room.  It will check for
 * exact co-ordinates and a property.  You set the goto_property,
 * this is optional.
 */

mapping been;
int num_moves;
#define MAX_MOVES 300

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
    return ({ 10000, 10000, 10000 });
  if (bing)
    return bing;
  ob->calc_co_ord();
  return (mixed)ob->query_co_ord();
} /* get_co_ord() */

static int distance(mixed *co_ord, mixed *co_ord2) {
  int i, ret;

  if (!co_ord || !co_ord2)
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

  bit = (mixed *)this_object()->query_actions("goto_co_ord") + ({ });
  this_object()->remove_trigger("goto_co_ord");
  for (i=0;i<sizeof(bit);i+=2)
    call_other(bit[i+1][0], bit[i+1][1], bing);
  return ;
} /* finitio() */

/* sail away into the sunset */
void sail_away() {
  mixed co_ord, t_guild, *dirs;
  object env;
  int min_dist, i, k, dir_pos, val;
  string dir;
  object *obs;

  co_ord = get_co_ord(env = (object)this_object()->query_current_room());
  if (!pointerp(co_ord)) /* Eeeek! */
    return finitio(0);
  this_object()->set_move_after(0, 0);
                             /* we don't wante them randomly rushing around
                              * as we do this! */
  t_guild = (mixed *)this_object()->query_property("goto co-ordinates");
  if (!t_guild)
    return finitio(0);
  if (!distance(co_ord, t_guild))
    if (!(dir = (string)this_object()->query_property("goto property")) ||
        env->query_property(dir)) {
      finitio(1);
      return ;
    }
  dirs = (string *)env->query_dest_dir();
  if (num_moves++ > MAX_MOVES) {
    return finitio(0);
  }
  while (1) {
    min_dist = 1000000;
    if (sizeof(dirs) == 2) {
/* Choice?  whats that? */
      been[env]++;
/* Hmmm.  cant leave through the only exit.  Sad...  */
      catch (val = (int)this_object()->do_command(dirs[0]));
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
      if (catch(call_other(dirs[i+1], "??"))) {
        dirs = delete(dirs, i, 2);
        i -= 2;
        continue;
      }
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
