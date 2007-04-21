(throw
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (targeted
    (no-arguments
      (self   "Hiyyyyaaa.")
      (target "$mcname$ throws you to the floor.")
      (rest  "$mcname$ throws $hcname$ to the floor in a happy embrace.")
      
      (position
        (default "lie")
        (silent  "yes")
      )
    )
    (arguments
      (arguments  arms, your arms, your arms around, arms around)
      (self   "You throw your arms around $hcname$.")
      (target "$mcname$ throws $mposs$ arms around you.")
     (rest "$mcname$ throws $mposs$ arms around $hcname$.") 
    )
    (arguments
      (arguments  your arms around and shout)
      (self   "You throw your arms around $hcname$ and shout $hposs$ name to the blue summer sky.")
      (target "$mcname$ throws $mposs$ arms around you and shouts your name to the blue summer sky.")
      (rest  "$mcname$ throws $mposs$ arms around $hcname$ and shouts $hposs$ name to the blue summer sky.")
    )
  )
)

