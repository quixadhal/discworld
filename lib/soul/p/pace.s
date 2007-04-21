(pace
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You pace back and forth.")
      (rest "$mcname$ pace$s$ back and forth.")
    )
    (arguments
      (arguments quickly, loudly, carefully, excitedly, nervously, tiredly,
                 fervently, slowly, worriedly, thoughtfully, distractedly,
                 diligently, impatiently, patiently, smugly, heavily,
                 lightly, #)
      (self "You pace back and forth $arg$.")
      (rest "$mcname$ pace$s$ back and forth $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments quickly, loudly, carefully, excitedly, nervously, tiredly,
                 fervently, slowly, worriedly, thoughtfully, distractedly,
                 diligently, impatiently, patiently, smugly, heavily,
                 lightly, #)
      (self   "You pace back and forth $arg$ at $hcname$.")
      (target "$mcname$ pace$s$ back and forth $arg$ at you.")
      (rest  "$mcname$ pace$s$ back and forth $arg$ at $hcname$.")
    )
  )
)

/* Saffra Dec 27 2001 */