(slobber
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You slobber all over the place.")
      (rest "$mcname$ slobber$s$ all over the place.")
    )
  )
  (targeted
    (no-arguments
      (self   "You slobber all over $hcname$.")
      (target "$mcname$ slobber$s$ all over you.")
      (rest  "$mcname$ slobber$s$ all over $hcname$.")
    )
  )
)

