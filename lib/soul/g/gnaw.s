(gnaw
  (pattern "[on] <indirect:any-living>")
  (pattern "<string>")
  (pattern "[on] <indirect:any-living> <string>")
  (single
    (arguments
	(arguments on a bone happily,#)
      (self "You gnaw $arg$.")
      (rest "$mcname$ gnaw$s$ $arg$.")
    )
  )
  (targeted
    (arguments
	(arguments
           leg,arm,bones,dinner,cat,keyboard,code,nose,frog,toe,ankles,finger,neck,crayons,#) 
      (self  "You gnaw on $mhcname$ $arg$.")
      (target "$mcname$ gnaw$s$ on your $arg$.")
      (rest  "$mcname$ gnaw$s$ on $mhcname$ $arg$.") 
    )
  )
)

