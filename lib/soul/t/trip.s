(trip
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You trip up.")
      (rest "$mcname$ trip$s$ up.")
      (position
        (standing "sit")
      )
    )
  )
  (targeted
    (no-arguments
      (self   "You trip $hcname$.")
      (target "$mcname$ trip$s$ you.")
      (rest  "$mcname$ trip$s$ $hcname$.")
      (position
        (standing "sit")
      )
    )
  )
)

