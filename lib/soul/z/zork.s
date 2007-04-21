(zork
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You zork.")
      (rest "$mcname$ zork$s$.")
    )
    (arguments
    (arguments  like a Swedish chef, with meatballs, #)
      (self "You zorks $arg$.")
      (rest "$mcname$ zork$s$ $arg$.")
    )
  )
  (targeted
  (no-arguments
    (self "You zork at $hcname$.")
    (target "$mcname$ zork$s$ at you.")
    (rest "$mcname$ zork$s$ at $hcname$.")
  )
  (arguments
    (arguments  like a Swedish chef, with meatballs, #)
    (self "You zork at $hcname$ $arg$." )
    (target "$mcname$ zork$s$ at you $arg$." )
    (rest "$mcname$ zork$s$ $hcname$ $arg$." )
    )
  )
)
