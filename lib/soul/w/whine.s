(whine
  (pattern "[at|to] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at|to] <indirect:any-living>")
  (pattern "[at|to] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You whine.")
      (rest "$mcname$ whines.")
    )
    (arguments
      (arguments pathetically,unconvincingly,like Klahn,like a lost puppy, #)
      (self "You whine $arg$.")
      (rest "$mcname$ whine$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You whine at $hcname$.")
      (target "$mcname$ whine$s$ at you.")
      (rest   "$mcname$ whine$s$ at $hcname$.")
    )
    (arguments
      (arguments pathetically,unconvincingly,like Klahn,like a lost puppy,#)
      (self   "You whine $arg$ at $hcname$.")
      (target "$mcname$ whine$s$ $arg$ at you.")
      (rest  "$mcname$ whine$s$ $arg$ at $hcname$.")
    )
  )
)

