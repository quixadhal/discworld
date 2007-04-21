(caress
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You caress $hcname$.")
      (target "$mcname$ caress$s$ you.")
      (rest  "$mcname$ caress$s$ $hcname$.")
    )
    (arguments
      (arguments  gently, strangely, violently, firmly, seductively,
                  suggestively, slightly, sensually, reluctantly, politely,
                  lovingly, vigorously, eagerly, vivaciously)
      (self   "You caress $hcname$ $arg$.")
      (target "$mcname$ caress$s$ you $arg$.")
      (rest  "$mcname$ caress$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  ear, nose, cheek, hand, forehead, eyebrow,
                  finger, foot, toe, fingernail, arm, thigh,
                  upper thigh, calf, elbow, knee, fingers, neck,
                  shoulder, palm, toes, hair)
      (self   "You caress $mhcname$ $arg$.")
      (target "$mcname$ caress$s$ your $arg$.")
      (rest  "$mcname$ caress$s$ $mhcname$ $arg$.")
    )
  )
)

