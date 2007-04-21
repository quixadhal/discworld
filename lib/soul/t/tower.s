(tower
  (pattern "[over] <indirect:any-living>")
  (pattern "[over] <indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You tower over $hcname$.")
      (target "$mcname$ tower$s$ over you.")
      (rest  "$mcname$ tower$s$ over $hcname$.")
    )
    (arguments
      (arguments  ominously,threateningly)
      (self   "You tower over $hcname$ $arg$.")
      (target "$mcname$ tower$s$ over you $arg$.")
      (rest  "$mcname$ tower$s$ over $hcname$ $arg$.")
    )
  )
)

