(smirk
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You smirk.")
      (rest "$mcname$ smirk$s$.")
    )
    (arguments
      (arguments  smugly, smartly, cruelly, #)
      (self "You smirk $arg$.")
      (rest "$mcname$ smirk$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You smirk at $hcname$.")
      (target "$mcname$ smirk$s$ at you.")
      (rest  "$mcname$ smirk$s$ at $hcname$.")
    )
    (arguments
      (arguments  smugly, smartly, cruelly, #)
      (self   "You smirk $arg$ at $hcname$.")
      (target "$mcname$ smirk$s$ $arg$ at you.")
      (rest  "$mcname$ smirk$s$ $arg$ at $hcname$.")
    )
  )
)
