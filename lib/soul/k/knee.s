(knee
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (targeted
    (no-arguments
      (self   "You knee $hcname$.")
      (target "$mcname$ knee$s$ you.")
      (rest  "$mcname$ knee$s$ $hcname$.")
    )
    (arguments
      (arguments  #)
      (self   "You knee $hcname$ $arg$.")
      (target "$mcname$ knee$s$ you $arg$.")
      (rest  "$mcname$ knee$s$ $hcname$ $arg$.")
    )
  )
)

