(strangle
  (pattern "<indirect:any-living>")
  (single
    (no-arguments
      (self "You strangle yourself.")
      (rest "$mcname$ strangle$s$ $mobj$self.")
    )
  )
  (targeted
    (no-arguments
      (self   "You strangle $hcname$.")
      (target "$mcname$ strangle$s$ you.")
      (rest   "$mcname$ strangle$s$ $hcname$.")
    )
  )
)

