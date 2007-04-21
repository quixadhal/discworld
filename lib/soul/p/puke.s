(puke
  (pattern "[on|at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You brighten up the place.")
      (rest "$mcname$ do$s$ a technicolour yawn.")
    )
  )
  (targeted
    (no-arguments
      (self   "You puke on $hcname$.")
      (target "$mcname$ puke$s$ on you.")
      (rest  "$mcname$ puke$s$ on $hcname$.")
    )
  )
)

