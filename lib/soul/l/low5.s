(low5
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You low five.")
      (rest "$mcname$ low five$s$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You low five $hcname$.")
      (target "$mcname$ low five$s$ you.")
      (rest  "$mcname$ low five$s$ $hcname$.")
    )
  )
)

