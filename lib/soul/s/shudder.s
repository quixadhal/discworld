(shudder
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You shudder.")
      (rest "$mcname$ shudder$s$.")
    )
    (arguments
      (arguments horror, fear, revulsion, disappointment, ecstasy,
                 pleasure, orgasmic release)
      (self "You shudder in $arg$.")
      (rest "$mcname$ shudder$s$ in $arg$.")
    )
    (arguments
      (arguments like they were having a shower,#) 
      (self   "You shudder $arg$.")
      (rest  "$mcname$ shudder$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You shudder at $hcname$.")
      (target "$mcname$ shudder$s$ at you.")
      (rest   "$mcname$ shudder$s$ at $hcname$.")
    )
    (arguments
      (arguments horror, fear, revulsion, disappointment, ecstasy,
                 pleasure, orgasmic release)
      (self   "You shudder in $arg$ at $hcname$.")
      (target "$mcname$ shudder$s$ in $arg$ at you.")
      (rest  "$mcname$ shudder$s$ in $arg$ at $hcname$.")
    )
    (arguments
      (arguments like they were having a shower,#) 
      (self   "You shudder $arg$ at $hcname$.")
      (target "$mcname$ shudder$s$ $arg$ at you.")
      (rest  "$mcname$ shudder$s$ $arg$ at $hcname$.")
    )
  )
)

