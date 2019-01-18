.DT
geometry.c
Disk World autodoc help
geometry.c

.SH Description
.SP 5 5

This does a few geometric calculations to make life easier for
us.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Apr  5 15:47:05 PST 2002

.EP

.SH Classes

.SI 3
* %^BOLD%^point%^RESET%^
class point {
int x;
int y;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^distance_between_two_line_segments%^RESET%^
.EI
.SI 5
int distance_between_two_line_segments(int x1_1, int y1_1, int x2_1, int y2_1, int x1_2, int y1_2, int x2_2, int y2_2)
.EI
.SP 7 5

This method finds the minimum distance between two line segments.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x1_1 - the start of the first line
.EP
.SP 9 5
y1_1 - the start of the first line
.EP
.SP 9 5
x2_1 - the end of the first line
.EP
.SP 9 5
y2_1 - the end of the first line
.EP
.SP 9 5
x1_2 - the start of the second line
.EP
.SP 9 5
y1_2 - the start of the second line
.EP
.SP 9 5
x2_2 - the end of the second line
.EP
.SP 9 5
y2_2 - the end of the second line
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the minimum distance

.EP

.SI 3
* %^BOLD%^distance_point_to_line_segment%^RESET%^
.EI
.SI 5
int distance_point_to_line_segment(int x1, int y1, int x2, int y2, int point_x, int point_y)
.EI
.SP 7 5

Finds the distance from a point to a line segment.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x1 - the start of the line segment
.EP
.SP 9 5
y1 - the start of the line segment
.EP
.SP 9 5
x2 - the end of the line segment
.EP
.SP 9 5
y2 - the end of the line segment
.EP
.SP 9 5
point_x - the point
.EP
.SP 9 5
point_y - the point
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the distance

.EP

.SI 3
* %^BOLD%^intersection_of_line_and_segment%^RESET%^
.EI
.SI 5
class point intersection_of_line_and_segment(int lx1, int ly1, int lx2, int ly2, int sx1, int sy1, int sx2, int sy2)
.EI
.SP 7 5

Return the intersection of a line and a line segment.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lx1 - the line x top
.EP
.SP 9 5
ly1 - the line y top
.EP
.SP 9 5
lx2 - the line x bottom
.EP
.SP 9 5
ly2 - the line y bottom
.EP
.SP 9 5
sx1 - the line segement x start
.EP
.SP 9 5
sy1 - the line segement y start
.EP
.SP 9 5
sx2 - the line segement x end
.EP
.SP 9 5
sy2 - the line segement y end
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the intersection point

.EP


