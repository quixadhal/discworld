(sneeze
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<indirect:any-living> [at] <string>")
  (single
    (arguments
      (arguments violently,loudly,silently,quietly,suddenly,unexpectedly, #)
      (self "You sneeze $arg$.")
      (rest "$mcname$ sneeze$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  violently,loudly,silently,quietly,suddenly,unexpectedly, #)
      (self   "You sneeze $arg$ at $hcname$.")
      (target "$mcname$ sneeze$s$ $arg$ at you.")
      (rest  "$mcname$ sneeze$s$ $arg$ at $hcname$.")
    )
  )
)

