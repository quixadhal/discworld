(condemn
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You condemn $hcname$.")
      (target "$mcname$ condemns you.")
      (rest  "$mcname$ condemns $hcname$.")
    )
    (arguments
      (arguments  for trapping frogs in traps baited with honey,#)
      (self   "You condemn $hcname$ $arg$.")
      (target "$mcname$ condemns you $arg$.")
      (rest  "$mcname$ condemns $hcname$ $arg$.")
    )
  )
)
/*Hobbes & Gruper 970425 */
