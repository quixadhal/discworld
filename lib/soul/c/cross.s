(cross
  (pattern "[my] <string>")
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments fingers, legs, eyes, arms, toes, t's, #)
      (self "You cross your $arg$.")
      (rest "$mcname$ crosses $mposs$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments fingers, legs, eyes, arms, toes, t's, #)
      (self "You cross your $arg$ at $hcname$.")
      (target "$mcname$ crosses $mposs$ $arg$ at you.")
      (rest   "$mcname$ crosses $mposs$ $arg$ at $hcname$.")
    )
  )
)
