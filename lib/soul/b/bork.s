(bork
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You bork.")
      (rest "$mcname$ bork$s$.")
    )
    (arguments
    (arguments  like a Swedish chef, with meatballs, #)
      (self "You bork$s$ $arg$.")
      (rest "$mcname$ bork$s$ $arg$.")
    )
  )
  (targeted
  (no-arguments
    (self "You bork at $hcname$.")
    (target "$mcname$ bork$s$ at you.")
    (rest "$mcname$ bork$s$ at $hcname$.")
  )
  (arguments
    (arguments  like a Swedish chef, with meatballs, #)
    (self "You bork at $hcname$ $arg$." )
    (target "$mcname$ bork$s$ at you $arg$." )
    (rest "$mcname$ bork$s$ $hcname$ $arg$." )
    )
  )
)
