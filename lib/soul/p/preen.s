(preen
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You preen yourself.")
      (rest "$mcname$ preen$s$ $mobj$self.")
    )
    (arguments
    (arguments  proudly, happily, graciously, flirtatiously, 
                successfully, limply, wonderfully, amazingly, brightly, #)
      (self "You preen yourself $arg$.")
      (rest "$mcname$ preen$s$ $mobj$self $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You preen yourself at $hcname$.")
      (target "$mcname$ preen$s$ $mobj$self at you.")
      (rest   "$mcname$ preen$s$ $mobj$self at $hcname$.")
    )
    (arguments
    (arguments  proudly, happily, graciously, flirtatiously, 
                successfully, limply, wonderfully, amazingly, brightly, #)
      (self   "You preen yourself at $hcname$.")
      (target "$mcname$ preen$s$ $mobj$self at you.")
      (rest   "$mcname$ preen$s$ $mobj$self at $hcname$.")
    )
  )
)
