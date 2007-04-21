(hit
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You hit yourself.")
      (rest "$mcname$ hit$s$ $mobj$self.")
    )
  )
  (targeted
    (no-arguments
      (self   "You hit $hcname$.")
      (target "$mcname$ hit$s$ you.")
      (rest  "$mcname$ hit$s$ $hcname$.")
    )
  )
)

