(threaten
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> with <string>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You threaten yourself.")
      (rest "$mcname$ threatens $mposs$self... how odd.")
    )
  )
  (targeted
    (no-arguments
      (self   "You threaten $hcname$.")
      (target "$mcname$ threatens you.")
      (rest  "$mcname$ threatens $hcname$.")
    )
    (arguments
      (arguments a big sharp pointy stick, a tax return, a threatening face,
                 a broomstick, smiting, a warrant for your arrest,
                 handcuffs, coffee tables, a cabbage, a frog,
                 a bad connection, marriage, divorce, a family reunion,
                 being gagged, being meteored, being fired, being fined,
                 deletion, interesting times,
                 being suspended, being grounded, no allowance, extra chores,
                 a cream pie, purple things, a tight tshirt, extra anchovies,
                 delight, fear, sadism, happiness, cupcakes,
                 total perspective vortex, running away, standing your ground,
                 eating quiche, #)
      (self   "You threaten $hcname$ with $arg$.")
      (target "$mcname$ threatens you with $arg$.")
      (rest  "$mcname$ threatens $hcname$ with $arg$.")
    )
    (arguments
      (arguments back, slowly, quickly, dispassionately, passionately,
                around the clock, gracefully, admirably, disgustingly,
                excitedly, in waves)
      (self   "You threaten $hcname$ $arg$.")
      (target "$mcname$ threatens you $arg$.")
      (rest  "$mcname$ threatens $hcname$ $arg$.")
    )
  )
)

