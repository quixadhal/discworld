(quack
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "<string> at <indirect:any-living>")
  (single
    (no-arguments
      (self "You quack.")
      (rest "$mcname$ quack$s$.")
    )
    (arguments
      (arguments like a duck,like a confused dog,happily,#)
      (self "You quack $arg$.")
      (rest "$mcname$ quack$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You quack at $hcname$.")
      (target "$mcname$ quack$s$ at you.")
      (rest  "$mcname$ quack$s$ at $hcname$.")
    )
    (arguments
      (arguments like a duck,like a confused dog,happily,#)
      (self   "You quack $arg$ at $hcname$.")
      (target "$mcname$ quack$s$ $arg$ at you.")
      (rest  "$mcname$ quack$s$ $arg$ at $hcname$.")
    )
  )
)

/* Hobbes, 970524 */
