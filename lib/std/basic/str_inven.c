mixed query_strange_inventory(mixed arr) {
  mixed inv;
  object ob;
  mixed desc;
  int i,j, k;

  inv = ({ });
  for (k=0;k < sizeof(arr); k++) {
    ob = arr[k];
    if (!(desc = (string)ob->short()) || (desc == ""))
      continue;
    if ((i = member_array(desc, inv)) >= 0)
      inv[i+1] += ({ ob });
    else
      inv += ({ desc, ({ ob }) });
  }
  return inv;
}
