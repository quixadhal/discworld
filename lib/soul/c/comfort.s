(comfort
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You comfort $hcname$.")
      (target "$mcname$ comforts you.")
      (rest   "$mcname$ comforts $hcname$.")
    )
    (arguments
      (arguments  sincerely, wholeheartedly, mischievously, halfheartedly, 
                  vaguely, eagerly, completely, lovingly, gently, tenderly, 
                  sadly, in earnest, with your frog, in the ear, in the leg, 
                  twice, anxiously, carefully, delicately, like someone else, #)
      (self   "You comfort $hcname$ $arg$.")
      (target "$mcname$ comforts you $arg$.")
      (rest   "$mcname$ comforts $hcname$ $arg$.")
    )
  )
)

