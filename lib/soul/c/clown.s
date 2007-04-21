(clown
(pattern "[at] <indirect:any-living>")
(pattern "<string>")
(pattern "<string> [at] <indirect:any-living>")
(pattern "[at] <indirect:any-living> <string>")
(single
(no-arguments
(self "You clown around like Bozo the clown.")
(rest "$mcname$ clowns around like Bozo the clown.")
)
(arguments
(arguments like a newbie,as if there is no tomorrow, in big pants, wearing
 a red nose, with huge shoes on, like a liaison, like a Lord, like someone 
 with baggy pants and unreasonably large shoes and an orange wig who's just 
 been hit in the face with a giant custard pie, # )
(self "You clown around $arg$.")
(rest "$mcname$ clowns around $arg$.")
)
)
(targeted
(no-arguments
(self   "You clown $hcname$ like Bozo the clown.")
(target "$mcname$ clowns around you like Bozo the clown.")
(rest  "$mcname$ clowns around $hcname$ like Bozo the clown.")
)
(arguments
(arguments like a newbie,as if there is no tomorrow, in big pants, wearing
 a red nose, with huge shoes on, like a liaison, like a Lord, like someone 
 with baggy pants and unreasonably large shoes and an orange wig who's just 
 been hit in the face with a giant custard pie, # )
(self   "You clown around $hcname$ $arg$.")
(target "$mcname$ clowns around you $arg$.")
(rest  "$mcname$ clowns around $hcname$ $arg$.")
)
)
)
