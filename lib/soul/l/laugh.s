(laugh
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "[with] <indirect:any-living>")
  (single
    (no-arguments
      (self "You laugh.")
      (rest "$mcname$ laugh$s$.")
    )
    (arguments
      (arguments  nastily, nicely, quietly, uncontrollably, politely, happily,
                  magically, ecstatically, demonically, out loud, maniacally,
                  hysterically, evilly, mischievously, #)
      (self "You laugh $arg$.")
      (rest "$mcname$ laugh$s$ $arg$.")
    )
    (arguments
     (arguments fall)
     (self "You fall down laughing.")
     (rest "$mcname$ fall$s$ down laughing.")
    )
   )
  (targeted
    (no-arguments
      (self   "You laugh at $hcname$.")
      (target "$mcname$ laugh$s$ at you.")
      (rest   "$mcname$ laugh$s$ at $hcname$.")
    )
    (arguments
      (arguments  nastily, nicely, quietly, uncontrollably, politely, happily,
                  magically, ecstatically, demonically, out loud, maniacally,
                  hysterically, evilly, mischievously)
      (self   "You laugh $arg$ at $hcname$.")
      (target "$mcname$ laugh$s$ $arg$ at you.")
      (rest   "$mcname$ laugh$s$ $arg$ at $hcname$.")
    )
      (arguments
        (arguments  with, #)
        (self   "You laugh $arg$ $hcname$.")
        (target "$mcname$ laugh$s$ $arg$ you.")
        (rest   "$mcname$ laugh$s$ $arg$ $hcname$.")
      )
    )
  )
)

