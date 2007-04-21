(ick
  (pattern "[at] <string>")
  (pattern "[at] <indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You ick.")
      (rest "$mcname$ ick$s$.")
    )
    (arguments
      (arguments disgustedly, frightenedly, happily, carefully, ickily, 
                 prissily, #)
      (self "You ick $arg$.")
      (rest "$mcname$ ick$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You ick at $hcname$.")
      (target "$mcname$ ick$s$ at you.")
      (rest  "$mcname$ ick$s$ at $hcname$.")
    )
    (arguments
      (arguments disgustedly, frightenedly, happily, carefully, ickily, 
                 prissily, #)
      (self   "You ick $arg$ at $hcname$.")
      (target "$mcname$ ick$s$ $arg$ at you.")
      (rest  "$mcname$ ick$s$ $arg$ at $hcname$.")
    )
  )
)
