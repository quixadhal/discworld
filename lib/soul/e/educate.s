(educate
  (pattern "<indirect:any-living> [about] <string>")
  (pattern "<string> <indirect:any-living>")
  (targeted
    (arguments
      (arguments #)
      (self   "You educate $hcname$ about $arg$.")
      (target "$mcname$ educate$s$ you about $arg$.")
      (rest  "$mcname$ educate$s$ $hcname$ about $arg$.")
    )
  )
)

