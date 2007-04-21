(envy
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You envy $hcname$.")
      (target "$mcname$ envy$s$ you.")
      (rest  "$mcname$ envy$s$ $hcname$.")
    )
    (arguments
      (arguments  frog,#)
      (self   "You envy $mhcname$ $arg$.")
      (target "$mcname$ envy$s$ your $arg$.")
      (rest  "$mcname$ envy$s$ $mhcname$ $arg$.")
    )
  )
)

