(blush
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You blush.")
      (rest "$mcname$ blush$s$.")
    )
    (arguments
      (arguments  in embarrassment,redly,comfortably,slightly,worriedly,#)
      (self "You blush $arg$.")
      (rest "$mcname$ blush$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You blush at $hcname$.")
      (target "$mcname$ blush$s$ at you.")
      (rest  "$mcname$ blush$s$ at $hcname$.")
    )
    (arguments
      (arguments  in embarrassment,redly,comfortably,slightly,worriedly,#)  
      (self   "You blush $arg$ at $hcname$.")
      (target "$mcname$ blush$s$ $arg$ at you.")
      (rest  "$mcname$ blush$s$ $arg$ at $hcname$.")
    )
  )
)

