(bap
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You bap to the beat.")
      (rest "$mcname$ bap$s$ to the beat.")
    )
  )
  (targeted
    (no-arguments
      (self   "You bap $hcname$ on the head.")
      (target "$mcname$ bap$s$ you on the head.")
      (rest  "$mcname$ bap$s$ $hcname$ on the head.")
    )
  )
)

