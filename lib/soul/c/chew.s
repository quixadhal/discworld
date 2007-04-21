(chew
  (pattern "[at] <indirect:any-living>")
  (pattern "[in] <string>")
  (pattern "[in] <string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> [in] <string>")
  (single
    (arguments
	(arguments your cheek thoughtfully,#)
      (self "You chew $arg$.")
      (rest "$mcname$ chews $arg$.")
    )
  )
  (targeted
    (arguments
	(arguments
leg,arm,bones,dinner,cat,keyboard,code,nose,frog,toe,ankles,finger,neck,#) 
      (self   "You chew on $mhcname$ $arg$.")
      (target "$mcname$ chews on your $arg$.")
      (rest  "$mcname$ chews on $mhcname$ $arg$.") 
    )
  )
)

