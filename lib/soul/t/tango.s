(tango
   (pattern "<indirect:any-living>")
   (pattern "<string> <indirect:any-living>")
   (pattern "<indirect:any-living> <string>")
   (targeted
      (arguments
	(arguments a rose,a frog,a cat,a fish,Terry Pratchett,a missing item receipt,a cabbage,#)
         (self   "You put $arg$ in your mouth and do the forbidden dance with $hcname$.")
         (target "$mcname$ puts $arg$ in $mposs$ mouth and does the forbidden dance with you.")
         (rest   "$mcname$ puts $arg$ in $mposs$ mouth and does the forbidden dance with $hcname$.")
      )
   )
)

 /*  Originaly suggested by Landis, changed and implemented by Hobbes */


