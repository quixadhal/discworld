(gnash
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You gnash your teeth.")
      (rest "$mcname$ gnash$s$ $mposs$ teeth.")
    )
  )
  (targeted
    (no-arguments
      (self   "You gnash your teeth at $hcname$.")
      (target "$mcname$ gnash$s$ $mposs$ teeth at you.")
      (rest  "$mcname$ gnash$s$ $mposs$ teeth at $hcname$.")
    )
  )
)

