.DT
image.c
Disk World autodoc help
image.c

.SH Description
.SP 5 5

Images can be used to efficiently store per-room area information.
Two current uses are for height maps (ie, third coordinate) and
exit maps (eg, a maze).  Along a row represents W-E (left to right),
and along a column represents N-S (top to bottom).  (0,0) in the image
corresponds to the northwestern-most room of the area.

Currently, the only supported image type is RAW. 
.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/image.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^load_image%^RESET%^
.EI
.SI 5
varargs class IMAGE_INFO load_image(string image_file, int rle)
.EI
.SP 7 5

This method reads in the given image file, and returns a two-dimensional
array of integers representing the data in the file.  The 'rle'
flag is currently unused; in the future, it will be used to return
the array in a "run-length encoded" format, where each group of identical
elements in a row will be represented as a count/value pair.  This
may be more efficient for large arrays with sparse data.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
image_file - the pathname of the file containing the image
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a 2-D array of ints, representing the image

.EP


