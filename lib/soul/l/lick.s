(lick
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You lick your lips.")
      (rest "$mcname$ lick$s$ $mposs$ lips.")
    )
    (arguments
      (arguments sensuously, seductively, expectantly, carefully, suggestively,
                 hungrily, #)
      (self "You lick your lips $arg$.")
      (rest "$mcname$ lick$s$ $mposs$ lips $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You lick $hcname$.")
      (target "$mcname$ lick$s$ you.")
      (rest   "$mcname$ lick$s$ $hcname$.")
    )
    (arguments
      (arguments  carefully, lustily, excitedly, slowly, quickly, happily,
                  silently, loudly, absentmindedly, hungrily, clean,
                  quietly, fondly, passionately, suggestively, #)
      (self   "You lick $hcname$ $arg$.")
      (target "$mcname$ lick$s$ you $arg$.")
      (rest  "$mcname$ lick$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  eye, eyebrow, nose, ear, lips, tongue, cheek, forehead, chin,
                  neck, nape, throat, arm, armpit, hand, back, tummy, elbow, 
                  wrist, palm, shoulder, finger, fingers, thigh, upper thigh, 
                  knee, legs, foot, feet, toe, toes, paws, calf,
                  strawberries, cucumbers, stamp, fruitbat, ice cream cone, 
                  apples, frog)
      (self   "You lick $mhcname$ $arg$.")
      (target "$mcname$ lick$s$ your $arg$.")
      (rest  "$mcname$ lick$s$ $mhcname$ $arg$.")
    )
  )
)
