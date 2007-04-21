(fun
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You hope everyone has fun.")
      (rest "$mcname$ hope$s$ everyone has fun.")
    )
  )
  (targeted
    (no-arguments
      (self   "You hope $hcname$ has fun.")
      (target "$mcname$ hope$s$ you have fun.")
      (rest  "$mcname$ hope$s$ $hcname$ has fun.")
    )
  )
)

