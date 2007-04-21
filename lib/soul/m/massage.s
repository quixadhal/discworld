(massage
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You give $hcname$ a massage.")
      (target "$mcname$ give$s$ you a massage.")
      (rest   "$mcname$ give$s$ $hcname$ a massage.")
    )
    (arguments
      (arguments good,bad,ugly,long,refreshing,pink,full body,
                 seductive,sensuous,strange,painful,deep,meaningful,
                 ergonomic,economic,painless,memorable,different,
                 unwanted,unwarranted,unprovoked,unknown,sneaky,
                 unnoticed,unwieldy,back,professional,unprofessional,
                 happy,sad,trained,untrained,train,virgin mary,
                 compact,quick,foot,leg,head,teeth,neck,
                 shoulder,nose, sensual, oiled, #)
      (self   "You give $hcname$ $article$ $arg$ massage.")
      (target "$mcname$ give$s$ you $article$ $arg$ massage.")
      (rest   "$mcname$ give$s$ $hcname$ $article$ $arg$ massage.")
    )
  )
)

