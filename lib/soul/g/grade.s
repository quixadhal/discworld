(grade
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> [in] <string>")
  (targeted
    (no-arguments
      (self   "You give $hcname$ an 'F'.")
      (target "$mcname$ give$s$ you an 'F'.")
      (rest  "$mcname$ give$s$ $hcname$ an 'F'.")    
    )
    (arguments
      (arguments Geography, History, Mathematics, English, Chemistry, Physics, #)
      (self   "You give $hcname$ an 'F' in $arg$.")
      (target "$mcname$ give$s$ you an 'F' in $arg$.")
      (rest  "$mcname$ give$s$ $hcname$ an 'F' in $arg$.")
    )
  )
)
