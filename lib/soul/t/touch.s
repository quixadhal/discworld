(touch
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You feel touched.")
      (rest "$mcname$ feel$s$ touched.")
    )
  )
  (targeted
    (no-arguments
      (self   "You touch $hcname$.")
      (target "$mcname$ touch$s$ you.")
      (rest  "$mcname$ touch$s$ $hcname$.")
    )
    (arguments
      (arguments  softly, passionately, deeply, tenderly, gently,
                  lovingly, lengthily, fishily, carefully, demandingly,
                  sensuously, longingly, hungrily, back, boringly,
                  inattentively, absentmindedly, froggily,
                  in an igloo, on a coffee table, in a shower,
                  slowly, suspiciously, calmly,#)
      (self   "You touch $hcname$ $arg$.")
      (target "$mcname$ touch$s$ you $arg$.")
      (rest  "$mcname$ touch$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  ear, nose, cheek, hand, forehead, eyebrow,
                  finger, foot, toe, fingernail, arm, thigh,
                  upper thigh, calf, elbow, knee, fingers, neck,
                  shoulder, palm, toes)
      (self   "You touch $mhcname$ $arg$.")
      (target "$mcname$ touch$s$ your $arg$.")
      (rest  "$mcname$ touch$s$ $mhcname$ $arg$.")
    )
    (arguments
      (arguments  girly air, rabbity, scruffy, cabbagey)
      (self   "You give $hcname$ a $arg$ touch on the cheek.")
      (target "$mcname$ give$s$ you a $arg$ touch on the cheek.")
      (rest  "$mcname$ give$s$ $hcname$ a $arg$ touch on the cheek.")
    )
    (arguments
      (arguments  exciting)
      (self   "You give $hcname$ an $arg$ touch on the cheek.")
      (target "$mcname$ give$s$ you an $arg$ touch on the cheek.")
      (rest  "$mcname$ give$s$ $hcname$ an $arg$ touch on the cheek.")
    )
  )
)

