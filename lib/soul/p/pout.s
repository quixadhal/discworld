(pout
  (pattern "<string>")
  (pattern "[at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You pout.")
      (rest "$mcname$ pout$s$.")
    )
    (arguments
      (arguments childishly,silently,resignedly, #)
      (self "You pout $arg$.")
      (rest "$mcname$ pout$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You pout at $hcname$.")
      (target "$mcname$ pout$s$ at you.")
      (rest  "$mcname$ pout$s$ at $hcname$.")
    )
    (arguments
      (arguments childishly,silently,resignedly, #)
      (self   "You pout $arg$ at $hcname$.")
      (target "$mcname$ pout$s$ $arg$ at you.")
      (rest  "$mcname$ pout$s$ $arg$ at $hcname$.")
    )
  )
)
