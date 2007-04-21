(swear
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You swear.")
      (rest "$mcname$ swear$s$.")
    )
    (arguments
      (arguments violently,on your life,on the bible,on your mother's grave,loudly,#)
      (self "You swear $arg$.")
      (rest "$mcname$ swear$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You swear at $hcname$.")
      (target "$mcname$ swear$s$ at you.")
      (rest  "$mcname$ swear$s$ at $hcname$.")
    )
    (arguments
      (arguments  violently,on your life,on the bible,on your mother's grave,loudly,#)
      (self   "You swear $arg$ at $hcname$.")
      (target "$mcname$ swear$s$ $arg$ at you.")
      (rest  "$mcname$ swear$s$ $arg$ at $hcname$.")
    )
  )
)

