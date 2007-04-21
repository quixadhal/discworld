(pity
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You pity $hcname$.")
      (target "$mcname$ pity$s$ you.")
      (rest  "$mcname$ pity$s$ $hcname$.")
    )
    (arguments
      (arguments #)
      (self   "You pity $hcname$ $arg$.")
      (target "$mcname$ pity$s$ you $arg$.")
      (rest  "$mcname$ pity$s$ $hcname$ $arg$.")
    )
  )
)

