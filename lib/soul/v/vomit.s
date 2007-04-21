(vomit
  (pattern "[on|at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You brighten up the place.")
      (rest "$mcname$ do$s$ a technicolour yawn.")
    )
  )
  (targeted
    (no-arguments
      (self   "You vomit on $hcname$.")
      (target "$mcname$ vomit$s$ on you.")
      (rest  "$mcname$ vomit$s$ on $hcname$.")
    )
  )
)

