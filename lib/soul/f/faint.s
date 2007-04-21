(faint
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (single
    (no-arguments
      (self "You faint.")
      (rest "$mcname$ faint$s$.")
    )
    (arguments
      (arguments tiredly,out of boredom)
      (self "You faint $arg$.")
      (rest "$mcname$ faint$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You faint on $hcname$.")
      (target "$mcname$ faint$s$ on you.")
      (rest  "$mcname$ faint$s$ on $hcname$.")
    )
  )
)

