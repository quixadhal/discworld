(shoot
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You shoot yourself.")
      (rest "$mcname$ shoot$s$ $mobj$self.")
    )
  )
  (targeted
    (no-arguments
      (self   "You shoot $hcname$ with a gonne.")
      (target "$mcname$ shoot$s$ you with a gonne.")
      (rest  "$mcname$ shoot$s$ $hcname$ with a gonne.")
    )
    (arguments
      (arguments  butt,head,eye,ear,leg,arm,toe,foot,hand,mouth,neck,#)
      (self   "You shoot $hcname$ in the $arg$ with a gonne.")
      (target "$mcname$ shoot$s$ you in the $arg$ with a gonne.")
      (rest  "$mcname$ shoot$s$ $hcname$ in the $arg$ with a gonne.")
    )
  )
)

