(hoot
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
    (single
      (no-arguments
       (self "You hoot like an owl.")
       (rest "$mcname$ hoot$s$ like an owl.")
     )
   )
   (targeted
    (arguments
       (arguments  like an owl,evilly,wickedly,insanely,mischievously,devilishly,impishly,
         cheekily,idiotically,wryly,sceptically,derisively,annoyingly,absentmindedly,
         whilst you work,distractedly,pointlessly,silently,loudly,rudely,with laughter,vaguely,
         obviously,tunelessly,#)
	      (self   "You hoot $arg$ at $hcname$.")
	      (target "$mcname$ hoot$s$ $arg$ at you.")
	      (rest  "$mcname$ hoot$s$ $arg$ at $hcname$.")
	    )
	  )
	)
/*Hobbes 970805*/
