varargs mixed find_match(string str, mixed ob) {
mixed *array, test;
int i, num;
string nick, type;

  if (stringp(ob)) {
    ob -> dummy();
    ob = find_object(ob);
  }
  test = ({ });
  nick = (string)this_object()->expand_nickname(str);
  if (!nick && this_player())
    nick = (string)this_player()->expand_nickname(str);
  if (nick && nick!="")
    str = nick;
  parse_command(str,all_inventory(ob),"%i",test);
  if (!test || !sizeof(test))
    return ({ });
  if (test[0] == 0)
    return test[1..sizeof(test)];
  if (test[0]<0)
    return ({ test[-test[0]] });
  return filter_array(test[1..sizeof(test)], "query_simul_efun_id",
                         this_object(), ({ test[0], str }) );
}

int query_simul_efun_id(object ob, mixed *arr) { 
  return (int)ob->query_parse_id(arr);
}

/*
 * What the hell would need this???
 *
 * vowel(int i) {
 *     return (i == 'a' || i == 'e' || i == 'i' || i == 'o' || i == 'u');
 *     }
 */

/*
 * I want to find where this is called and change it.
 * when (if) I implement this the params will be the other way around!!
 * consistent with bindex!!
 *
 * int index(mixed *array, mixed match) {
 * int i;
 *     log_file("INDEX", "Called from "+file_name(previous_object()) + "\n");
 *     for (i=0;i<sizeof(array);i++)
 *     if (array[i] == match) return i;
 *         return -1;
 *     }
 */

/*
 * use print_object() ;)
 *
 * write_object_array(ob) {
 * int i;
 *     if (!pointerp(ob)) { write(ob); write(" "); return; }
 *     write("({ ");
 *     for (i=0;i<sizeof(ob);++i) write_object_array(ob[i]);
 *     write(" })\n");
 *     }
 */

/* 
 * I want to find what uses this code so I can change it!
 *
 * private static string *numbers;
 * 
 * string write_number(int i) {
 *     if (!sizeof(numbers))
 *         numbers = ({
 *             "Zero","One","Two","Three","Four","Five","Six",
 *             "Seven","Eight","Nine","Ten","Eleven","Twelve"
 *             });
 *     if (i>12) return "many";
 *     return numbers[i];
 *     }
 */
 
shout(string str, object avoid) {
object ob;
int i;
    ob = users();
    for (i=0;i<sizeof(ob);i++)
        if (ob[i] != avoid && !ob[i] -> query_earmuffs())
        tell_object(ob[i], str);
    }

mixed *delete(mixed *arr, int start, int len) {
  return arr[0..start-1]+arr[start+len..sizeof(arr)];
}

mixed *slice_array(mixed *arr, int start, int fin) {
  return arr[start..fin];
}

mixed *insert(mixed *arr, mixed el, int pos) {
  return arr[0..pos-1]+({ el })+arr[pos..sizeof(arr)];
}

int bindex(mixed el, mixed *array) {
int i, j, k;
 
    if (!pointerp(array)) return -1;
    k = sizeof(array)-1;
    if (el < array[0]) return -1;
    else if (el > array[k]) return -(k+2);
    do {
	i = (j+k)/2;
	if (array[i] == el) return i;
	else if (el>array[i]) j = i;
	else k = i;
	} while (j+1 < k);
    return -j-2;
    }
 
mixed *binsert(mixed el, mixed *array) {
int i;
    if (!pointerp(array)) return ({ el });
    i = bindex(el, array);
    if (i >= 0) array = insert(array, el, i);
    else array = insert(array, el, -i-1);
    return array;
    }
