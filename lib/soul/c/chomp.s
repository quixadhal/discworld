(chomp
  (pattern "[on] <indirect:any-living>")
  (pattern "<string>")
  (pattern "[on] <indirect:any-living> <string>")
  (single
    (arguments
	(arguments on a bone happily,#)
      (self "You chomp $arg$.")
      (rest "$mcname$ chomps $arg$.")
    )
  )
  (targeted
    (arguments
	(arguments
           leg,arm,bones,dinner,cat,keyboard,code,nose,frog,toe,ankles,finger,neck,crayons,#) 
      (self  "You chomp on $mhcname$ $arg$.")
      (target "$mcname$ chomps on your $arg$.")
      (rest  "$mcname$ chomps on $mhcname$ $arg$.") 
    )
  )
)

