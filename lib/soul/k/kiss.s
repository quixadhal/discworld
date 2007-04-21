(kiss
  (pattern "<string>")
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self   "You pucker up.")
      (rest   "$mcname$ pucker$s$ up.")
    )
    (arguments
      (arguments cute, adorable, sensuous, prissy, icky, #)
      (self   "You make $article$ $arg$ kissy face.")
      (rest   "$mcname$ make$s$ $article$ $arg$ kissy face.")
    )
  )
  (targeted
    (no-arguments
      (self   "You kiss $hcname$.")
      (target "$mcname$ kiss$s$ you.")
      (rest  "$mcname$ kiss$s$ $hcname$.")
    )
    (arguments
      (arguments  softly, passionately, deeply, tenderly, gently,
                  lovingly, lengthily, fishily, carefully, demandingly,
                  sensuously, longingly, hungrily, back, boringly,
                  inattentively, absentmindedly, froggily,
                  in an igloo, on a coffee table, in a shower,
                  slowly, suspiciously, calmly, #)
      (self   "You kiss $hcname$ $arg$.")
      (target "$mcname$ kiss$s$ you $arg$.")
      (rest  "$mcname$ kiss$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  ear, nose, cheek, hand, forehead, eyebrow, eyebrows,
                  finger, foot, feet, toe, fingernail, arm, thigh, thighs,
                  upper thigh, calf, elbow, elbows, knee, knees, fingers, neck, 
                  eyelashes, shoulder, palm, toes, lips, mouth, tongue, leg, 
                  teeth, bottom, throat, nape, back)
      (self   "You kiss $mhcname$ $arg$.")
      (target "$mcname$ kiss$s$ your $arg$.")
      (rest  "$mcname$ kiss$s$ $mhcname$ $arg$.")
    )
    (arguments
      (arguments  girly air, rabbity, scruffy, cabbagey, prissy, pure, polite)
      (self   "You give $hcname$ a $arg$ kiss on the cheek.")
      (target "$mcname$ give$s$ you a $arg$ kiss on the cheek.")
      (rest  "$mcname$ give$s$ $hcname$ a $arg$ kiss on the cheek.")
    )
  )
)
