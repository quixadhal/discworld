(bite
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You bite yourself.")
      (rest "$mcname$ bite$s$ $mobj$self.")
    )
  )
  (targeted
    (no-arguments
      (self   "You bite $hcname$.")
      (target "$mcname$ bite$s$ you.")
      (rest   "$mcname$ bite$s$ $hcname$.")
    )
    (arguments
      (arguments  hard, nastily, carefully, politely, fishily, lovingly,
                  refreshingly, happily, like a train, pointlessly, 
                  thoughtfully, objectively, sulkily, gently, brightly, 
                  excitedly, uncertainly, like a dog, wolfishly, mercilessly, 
                  ruthlessly, softly, sweetly, professionally, hungrily, 
                  mischievously, casually, like a vampire, #)
      (self   "You bite $hcname$ $arg$.")
      (target "$mcname$ bite$s$ you $arg$.")
      (rest   "$mcname$ bite$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments  nose, ear, lip, arm, hand, finger, leg, foot, toe, neck, 
                  shoulder, throat, vein, artery, face, back, hip, thigh, ankle,
                  heel, elbow, knee, shank, belly, stomach, ribcage, rib, 
                  collarbone, earlobe, mouth, tongue, calf, lips)
      (self   "You bite $hcname$ on the $arg$.")
      (target "$mcname$ bite$s$ you on the $arg$.")
      (rest   "$mcname$ bite$s$ $hcname$ on the $arg$.")
    )
  )
)

