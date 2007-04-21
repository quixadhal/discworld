(raspberry
  (pattern "[at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You blow a raspberry.")
      (rest "$mcname$ blow$s$ a raspberry.")
    )
  )
  (targeted
    (no-arguments
      (self   "You blow a raspberry at $hcname$.")
      (target "$mcname$ blow$s$ a raspberry at you.")
      (rest  "$mcname$ blow$s$ a raspberry at $hcname$.")
    )
  )
)

