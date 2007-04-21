(squeal
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
        (self "You squeal.")
        (rest "$mcname$ squeal$s$.")
    )
    (arguments
      (arguments in delight, in terror, in pain, in mortal fear, 
                 like a pig, like a rubber chew toy, #)
        (self "You squeal $arg$.")
        (rest "$mcname$ squeal$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
        (self   "You squeal at $hcname$.")
        (target "$mcname$ squeal$s$ at you.")
        (rest   "$mcname$ squeal$s$ at $hcname$.")
    )
    (arguments
      (arguments in delight, in terror, in pain, in mortal fear, 
                 like a pig, like a rubber chew toy, #)
        (self   "You squeal $arg$ at $hcname$.")
        (target "$mcname$ squeal$s$ $arg$ at you.")
        (rest   "$mcname$ squeal$s$ $arg$ at $hcname$.")
    )
  )
)

/* Given all the good options by Saffra, 1-31-01. */