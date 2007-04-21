(sskip
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You skip around happily.")
      (rest "$mcname$ skip$s$ around happily.")
    )
  )
  (targeted
    (no-arguments
      (self   "You skip around $hcname$.")
      (target "$mcname$ skip$s$ around you.")
      (rest  "$mcname$ skip$s$ around $hcname$.")
    )
  )
)

