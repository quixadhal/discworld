(niggle
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (targeted
    (arguments
      (arguments affectionately, softly, slowly, carefully, with a shaver,
                 like a cabbage, greenly, pinkly, fishily, dreamily,
                 sleepily, happily, sadly, quickly, with your hair,
                 backwards, briefly, professionaly, excitingly,
                 spiffily, #)
      (self   "You niggle $hcname$ $arg$.")
      (target "$mcname$ niggle$s$ you $arg$.")
      (rest   "$mcname$ niggle$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments shoulder, arm, ear, nose, chicken, frog,
                 leg, hand, finger, eyebrow, foot, ankle, armpit,
                 hair, chest, tummy, bottom, belly, thigh, forearm,
                 palm, back, calf, wrist, knee, oranges)
      (self   "You niggle $mhcname$ $arg$.")
      (target "$mcname$ niggle$s$ your $arg$.")
      (rest   "$mcname$ niggle$s$ $mhcname$ $arg$.")
    )
  )
)

