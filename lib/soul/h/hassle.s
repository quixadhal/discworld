(hassle
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> [about] <string>")
  (targeted
    (no-arguments
      (self   "You hassle $hcname$.")
      (target "$mcname$ hassle$s$ you.")
      (rest  "$mcname$ hassle$s$ $hcname$.")
    )
    (arguments
      (arguments  frogs,chickens,wombles,#)
      (self   "You hassle $hcname$ about $arg$.")
      (target "$mcname$ hassle$s$ you about $arg$.")
      (rest  "$mcname$ hassle$s$ $hcname$ about $arg$.")
    )
  )
)

