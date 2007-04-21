
(huggle
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self   "You huggle yourself.  Aww!")
      (rest   "$mcname huggle$s$ $mobj$self.")
    )
  )
  (targeted
    (no-arguments
      (self   "You huggle $hcname$.")
      (target "$mcname$ huggle$s$ you.")
      (rest   "$mcname$ huggle$s$ $hcname$.")
    )
    (arguments
      (arguments  tightly, warmly, gently, close, under your jumper, #)
      (self   "You huggle $hcname$ $arg$.")
      (target "$mcname$ huggle$s$ you $arg$.")
      (rest   "$mcname$ huggle$s$ $hcname$ $arg$.")
    )
  )
)
