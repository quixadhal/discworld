(scowl
  (pattern "[at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You scowl.")
      (rest "$mcname$ scowl$s$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You scowl at $hcname$.")
      (target "$mcname$ scowl$s$ at you.")
      (rest  "$mcname$ scowl$s$ at $hcname$.")
    )
  )
)

