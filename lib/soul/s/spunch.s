(spunch
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You punch yourself.")
      (rest "$mcname$ punch$s$ $mobj$self.")
    )
  )
  (targeted
    (no-arguments
      (self   "You punch $hcname$.")
      (target "$mcname$ punch$s$ you.")
      (rest  "$mcname$ punch$s$ $hcname$.")
    )
  )
)

