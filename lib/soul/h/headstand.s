(headstand
  (pattern "[on] <indirect:any-living>")
  (single
    (no-arguments
      (self "You stand on your head.")
      (rest "$mcname$ stand$s$ on $mposs$ head.")
    )
  )
  (targeted
    (no-arguments
      (self   "You stand on $mhcname$ head.")
      (target "$mcname$ stand$s$ on your head.")
      (rest  "$mcname$ stand$s$ on $mhcname$ head.")
    )
  )
)

