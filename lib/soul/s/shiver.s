(shiver
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You shiver.")
      (rest "$mcname$ shiver$s$.")
    )
    (arguments
       (arguments horror, fear, revulsion, disappointment, ecstasy,
                 pleasure, orgasmic release,#)
      (self "You shiver in $arg$.")
      (rest "$mcname$ shiver$s$ in $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You shiver at $hcname$.")
      (target "$mcname$ shiver$s$ at you.")
      (rest  "$mcname$ shiver$s$ at $hcname$.")
    )
    (arguments
      (arguments horror, fear, revulsion, disappointment, ecstasy,
                 pleasure, orgasmic release,#)
      (self   "You shiver in $arg$ at $hcname$.")
      (target "$mcname$ shiver$s$ in $arg$ at you.")
      (rest  "$mcname$ shiver$s$ in $arg$ at $hcname$.")
    )
  )
)

