(pounce
  (pattern "[on] <indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "[on] <indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You pounce on $hcname$.")
      (target "$mcname$ pounce$s$ on you.")
      (rest  "$mcname$ pounce$s$ on $hcname$.")
    )
    (arguments
      (arguments  excitedly, seductively, madly, carefully, appreciatively,
                  selectively, with a wooden spoon, at near light speed,
                  while showing some cleavage, # )
      (self   "You pounce on $hcname$ $arg$.")
      (target "$mcname$ pounce$s$ on you $arg$.")
      (rest  "$mcname$ pounce$s$ on $hcname$ $arg$.")
    )
  )
)

