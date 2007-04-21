(bang
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You bang your head against a wall.")
      (rest "$mcname$ bang$s$ $mposs$ head against a wall.")
    )
    (arguments
      (arguments like Ceres)
      (self "You bang your head against a wall $arg$.")
      (rest "$mcname$ bang$s$ $mposs$ head against a wall $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You bang $mhcname$ head against a wall.")
      (target "$mcname$ bang$s$ your head against a wall.")
      (rest  "$mcname$ bang$s$ $mhcname$ head against a wall.")
    )
    (arguments
      (arguments like Ceres)
      (self   "You bang $mhcname$ head against a wall $arg$.")
      (target "$mcname$ bang$s$ your head against a wall $arg$.")
      (rest  "$mcname$ bang$s$ $mhcname$ head against a wall $arg$.")
    )
  )
)
