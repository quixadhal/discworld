(snap
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "[my] <string> [at|at] <indirect:any-living>")
  (single
    (arguments
      (arguments fingers,leg,arm in a quick salute)
      (self "You snap your $arg$.")
      (rest "$mcname$ snap$s$ $mposs$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  fingers,leg,arm in a quick salute)
      (self   "You snap your $arg$ at $hcname$.")
      (target "$mcname$ snap$s$ $mposs$ $arg$ at you.")
      (rest  "$mcname$ snap$s$ $mposs$ $arg$ at $hcname$.")
    )
  )
)

