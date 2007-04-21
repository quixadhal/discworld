(hush
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You hush $hcname$.")
      (target "$mcname$ hush$s$ you.")
      (rest  "$mcname$ hush$s$ $hcname$.")
    )
    (arguments
      (arguments  quickly,carefully,threateningly,nervously,
      for using such language,#)
      (self   "You hush $hcname$ $arg$.")
      (target "$mcname$ hush$s$ you $arg$.")
      (rest  "$mcname$ hush$s$ $hcname$ $arg$.")
    )
  )
)

