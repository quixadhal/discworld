(snigger
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You snigger.")
      (rest "$mcname$ snigger$s$.")
    )
    (arguments
      (arguments mischievously,nastily,a bit,annoyingly,belatedly,#)
      (self "You snigger $arg$.")
      (rest "$mcname$ snigger$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You snigger at $hcname$.")
      (target "$mcname$ snigger$s$ at you.")
      (rest  "$mcname$ snigger$s$ at $hcname$.")
    )
    (arguments
      (arguments mischievously,nastily,a bit,annoyingly,belatedly,#) 
      (self   "You snigger $arg$ at $hcname$.")
      (target "$mcname$ snigger$s$ $arg$ at you.")
      (rest  "$mcname$ snigger$s$ $arg$ at $hcname$.")
    )
  )
)

