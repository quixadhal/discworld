/*
 * image.h - definitions for handler to load and save 2D images
 */

class IMAGE_INFO {
  mixed image; // Array of image
  int rle;     // Image is run-length encoded along the rows (unimplemented)
  int *size;   // Array size (only 2D implemented right now)
  int *nw_coord; // Coordinate of the room at (0,0) (for calculating indices)
} /* IMAGE_INFO */

// This is just temporary for testing...
#define IMAGE_HANDLER "/d/sur/beta/topography/image"
