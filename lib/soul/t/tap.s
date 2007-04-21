(tap
  (pattern "<indirect:any-living> on the <string>")
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> [on] <string>")
  (single
    (arguments
      (arguments foot,head,left leg,right leg,fingers,nose,shoulder,teeth,arm)
      (self "You tap your $arg$ impatiently.")
      (rest "$mcname$ taps $mposs$ $arg$ impatiently.")
    )
  )
  (targeted
    (arguments
      (arguments  shoulder,head,arm,nose,leg,teeth,foot,ear,frog,other thing)
      (self   "You tap $hcname$ on the $arg$.")
      (target "$mcname$ taps you on the $arg$.")
      (rest  "$mcname$ taps $hcname$ on the $arg$.")
    )
  )
)

