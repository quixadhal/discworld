(believe
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You believe.")
      (rest "$mcname$ believe$s$.")
    )
    (arguments
    (arguments  everything, nothing, some of it, most of it, all of it, 
                none of it, willingly, unwillingly, reluctantly, 
                completely, #)
      (self "You believe $arg$.")
      (rest "$mcname$ believe$s$ $arg$.")
    )
  )
  (targeted
  (no-arguments
    (self "You believe $hcname$.")
    (target "$mcname$ believe$s$ you.")
    (rest "$mcname$ believe$s$ $hcname$.")
  )
  (arguments
    (arguments  willingly, unwillingly, reluctantly, completely, 
                not at all, devotedly, till the ends of the earth, #)
    (self "You believe $hcname$ $arg$." )
    (target "$mcname$ believe$s$ you $arg$." )
    (rest "$mcname$ believe$s$ $hcname$ $arg$." )
    )
  )
)

/* Saffra 2-20-00 */

