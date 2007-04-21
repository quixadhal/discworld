(slaver
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You slaver all over the place.")
      (rest "$mcname$ slaver$s$ all over the place.")
    )
  )
  (targeted
    (no-arguments
      (self   "You slaver all over $hcname$.")
      (target "$mcname$ slaver$s$ all over you.")
      (rest  "$mcname$ slaver$s$ all over $hcname$.")
    )
  )
)

