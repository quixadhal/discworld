mapping m_delete(mapping map, mixed key) {
  map = map + ([ ]);
  map_delete(map, key);
  return map;
} /* m_delete() */
