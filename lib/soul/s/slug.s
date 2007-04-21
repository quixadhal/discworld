(slug
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You slug yourself.")
      (rest "$mcname$ slug$s$ $mobj$self.")
    )
  )
  (targeted
    (no-arguments
      (self   "You slug $hcname$.")
      (target "$mcname$ slug$s$ you.")
      (rest  "$mcname$ slug$s$ $hcname$.")
    )
  )
)

