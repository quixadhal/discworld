/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
/**
 * Deletes an element from a mapping (obsolete).  Use of this
 * should be avoided.  It is far more expensive than the
 * map_delete() efun.
 * @see efun::map_delete()
 * @param map the mapping to remove an element from
 * @param key the element to remove
 */
mapping m_delete(mapping map, mixed key) {
  map = map + ([ ]);
  map_delete(map, key);
  return map;
} /* m_delete() */
