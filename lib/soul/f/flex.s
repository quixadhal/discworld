(flex
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You flex your muscles.")
      (rest "$mcname$ flex$s$ $mposs$ muscles.")
    )
    (arguments
      (arguments  muscles, biceps, triceps, legs, brain, nose, ears, 
                  fingers, tongue, influence, #)
      (self "You flex your $arg$.")
      (rest "$mcname$ flex$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You flex your muscles at $hcname$.")
      (target "$mcname$ flex$s$ $mposs$ muscles at you.")
      (rest   "$mcname$ flex$s$ $mposs$ muscles at $hcname$.")
    )
    (arguments
      (arguments  muscles, biceps, triceps, legs, brain, nose, ears, 
                  fingers, tongue, influence, #)
      (self   "You flex your $arg$ at $hcname$.")
      (target "$mcname$ flex$s$ $mposs$ $arg$ at you.") 
      (rest   "$mcname$ flex$s$ $mposs$ $arg$ at $hcname$.")
    )
  )
)

