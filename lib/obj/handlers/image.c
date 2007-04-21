/*
 * image.c - handler for loading and storing 2D images.
 */

/**
 * Images can be used to efficiently store per-room area information.
 * Two current uses are for height maps (ie, third coordinate) and
 * exit maps (eg, a maze).  Along a row represents W-E (left to right),
 * and along a column represents N-S (top to bottom).  (0,0) in the image
 * corresponds to the northwestern-most room of the area.<p>
 * Currently, the only supported image type is RAW.
 */

#include <image.h>

/**
 * This method reads in the given image file, and returns a two-dimensional
 * array of integers representing the data in the file.  The 'rle'
 * flag is currently unused; in the future, it will be used to return
 * the array in a "run-length encoded" format, where each group of identical
 * elements in a row will be represented as a count/value pair.  This
 * may be more efficient for large arrays with sparse data.
 * @param image_file the pathname of the file containing the image
 * @rle optional (currently unused)
 * @return a 2-D array of ints, representing the image
 */
varargs class IMAGE_INFO load_image(string image_file, int rle) {
  int i, j, n, size_x, size_y;
  string image_type, l;
  mixed vals;
  class IMAGE_INFO ret;

  image_type = read_file(image_file, 1, 1);
  image_type = replace_string(image_type, " ", "");
  image_type = replace_string(image_type, "\n", "");
  if (image_type != "RAW") {
    error("Only image type RAW is currently supported.\n");
  }
  l = read_file(image_file, 2, 1);
  if (sscanf(l, "%d%d", size_x, size_y) != 2) {
    error("Image size(s) not found.\n");
  }
  ret = new(class IMAGE_INFO);
  ret->size = ({ size_x, size_y });
  // Read the rest of the file.
  vals = read_file(image_file, 3);
  // Take out newlines & explode into number strings
  //vals = explode(replace_string(vals, "\n", " "), " ");
  vals = reg_assoc(vals, ({ "[0-9]+" }), ({ 1 }));
  n = 0;
  while (vals[1][n] != 1) n++;
  // Save in image as ints.
  ret->image = allocate(size_x);
  // TODO: put the run-length encoding stuff in here, both on reading
  // and on storing in image.
  for (i = 0; i < size_x; i++) {
    ret->image[i] = allocate(size_y);
    for (j = 0; j < size_y; j++) {
      if (n >= sizeof(vals[0])) {
        error("Ran out of values for image at " + i + ", " + j + ".\n");
      }
      ret->image[i][j] = to_int(vals[0][n++]);
      while ((n < sizeof(vals[0])) && (vals[1][n] != 1))
        n++;
    }
  }
  return ret;
} /* load_image() */

/**
 * @ignore
 */
// Debugging function
void print_image(class IMAGE_INFO im) {
  int i, j;

  printf("size: %d x %d, %s, ", im->size[0], im->size[1],
         (im->rle?"rle":"no rle"));
  if (arrayp(im->nw_coord))
    printf("@ ({ %d, %d, %d })\n", im->nw_coord[0], im->nw_coord[1],
           im->nw_coord[2]);
  else
    printf("(not placed)\n");
  printf("Image:\n    ");
  for (i = 0; i < sizeof(im->image); i++) {
    for (j = 0; j < sizeof(im->image[i]); j++) {
      printf("% 4d", im->image[i][j]);
    }
    printf("\n    ");
  }
  printf("\n");
} /* print_image() */
