(snog
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You snog $hcname$.")
      (target "$mcname$ snog$s$ you.")
      (rest  "$mcname$ snog$s$ $hcname$.")
    )
    (arguments
      (arguments  wetly, sweetly, hungrily, forcefully, fiercely,
                  with lots of slobber, sloppily, with a cotton mouth,
                  with honey, like a dying slug, with whiskers, tenderly,
                  softly, deeply, with a mouthful of fur, #)
      (self   "You snog $hcname$ $arg$.")
      (target "$mcname$ snog$s$ you $arg$.")
      (rest  "$mcname$ snog$s$ $hcname$ $arg$.")
    )
  )
)
