mixed *delete(mixed *arr, int start, int len) {
  return arr[0..start-1]+arr[start+len..sizeof(arr)];
} /* delete() */

mixed *slice_array(mixed *arr, int start, int fin) {
  return arr[start..fin];
} /* slice_array() *.

mixed *insert(mixed *arr, mixed el, int pos) {
  return arr[0..pos-1]+({ el })+arr[pos..sizeof(arr)];
} /* insert() */
