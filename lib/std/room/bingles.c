mixed query_door(mixed dest) {
  mixed bing;

  if ((bing = door_control[dest]))
    if (!pointerp(bing))
      return bing;
  if (objectp(dest))
    if ((bing = door_control[file_name(dest)]))
      if (!pointerp(bing))
        return bing;
  return 0;
}
