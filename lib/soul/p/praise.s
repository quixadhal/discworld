(praise
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You praise $hcname$.")
      (target "$mcname$ praise$s$ you.")
      (rest  "$mcname$ praise$s$ $hcname$.")
    )
    (arguments
      (arguments  to the skies,obsequiously,loudly,and asks for more money,#)
      (self   "You praise $hcname$ $arg$.")
      (target "$mcname$ praise$s$ you $arg$.")
      (rest  "$mcname$ praise$s$ $hcname$ $arg$.")
    )
  )
)
/* Hobbes */
