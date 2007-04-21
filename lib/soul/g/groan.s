(groan
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You groan.")
      (rest "$mcname$ groan$s$.")
    )
    (arguments
      (arguments loudly, resignedly, tiredly, in disappointment, #)
      (self "You groan $arg$.")
      (rest "$mcname$ groan$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You groan at $hcname$.")
      (target "$mcname$ groan$s$ at you.")
      (rest  "$mcname$ groan$s$ at $hcname$.")
    )
    (arguments
      (arguments  loudly, resignedly, tiredly, in disappointment, #)
      (self   "You groan $arg$ at $hcname$.")
      (target "$mcname$ groan$s$ $arg$ at you.")
      (rest  "$mcname$ groan$s$ $arg$ at $hcname$.")
    )
  )
)

