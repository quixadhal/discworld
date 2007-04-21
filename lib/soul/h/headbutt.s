(headbutt
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You headbutt $hcname$.")
      (target "$mcname$ headbutt$s$ you.")
      (rest  "$mcname$ headbutt$s$ $hcname$.")
    )
    (arguments
      (arguments hard, in the nose, on the ear, slowly, unexpectedly,
                 quickly, explosively, pointedly, #)
      (self   "You headbutt $hcname$ $arg$.")
      (target "$mcname$ headbutt$s$ you $arg$.")
      (rest  "$mcname$ headbutt$s$ $hcname$ $arg$.")
    )
  )
)

