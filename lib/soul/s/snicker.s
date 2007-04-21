(snicker
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You snicker.")
      (rest "$mcname$ snicker$s$.")
    )
    (arguments
      (arguments  mischievously, nastily, a bit, annoyingly, belatedly, 
                  sympathetically, happily, quietly, quietly from the corner, 
                  a private joke, #)
      (self "You snicker $arg$.")
      (rest "$mcname$ snicker$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You snicker at $hcname$.")
      (target "$mcname$ snicker$s$ at you.")
      (rest   "$mcname$ snicker$s$ at $hcname$.")
    )
    (arguments
      (arguments  mischievously, nastily, a bit, annoyingly) 
      (self   "You snicker $arg$ at $hcname$.")
      (target "$mcname$ snicker$s$ $arg$ at you.")
      (rest   "$mcname$ snicker$s$ $arg$ at $hcname$.")
    )
    (arguments
      (arguments  mischievously, nastily, a bit, annoyingly, belatedly, 
                  sympathetically, happily, quietly, quietly from the corner, 
                  #) 
      (self   "You snicker $arg$ at $hcname$.")
      (target "$mcname$ snicker$s$ $arg$ at you.")
      (rest   "$mcname$ snicker$s$ $arg$ at $hcname$.")
    )
    (arguments
      (arguments  at a private joke) 
      (self   "You snicker $arg$ with $hcname$.")
      (target "$mcname$ snicker$s$ $arg$ with you.")
      (rest   "$mcname$ snicker$s$ $arg$ with $hcname$.")
    )
  )
)

