(clasp
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You clasp $hcname$.")
      (target "$mcname$ clasps you.")
      (rest  "$mcname$ clasps $hcname$.")
    )
    (arguments
      (arguments  tightly,loosely,wetly,happily,lovingly,passionately,prudently,seductively)
      (self   "You clasp $mhcname$ hand $arg$.")
      (target "$mcname$ clasps your hand $arg$.")
      (rest  "$mcname$ clasps $mhcname$ hand $arg$.")
    )
  )
)

