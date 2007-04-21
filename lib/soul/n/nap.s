(nap
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "Zzz...")
      (rest "$mcname$ take$s$ a quick nap.")
      )
    )
  (targeted
    (no-arguments
      (self   "You take a quick nap in $mhcname$ arms.")
      (target "$mcname$ take$s$ a quick nap in your arms.")
      (rest  "$mcname$ take$s$ a quick nap in $mhcname$ arms.")
    )
    (arguments
      (arguments  lap,bed,#) 
      (self   "You take a quick nap in $mhcname$ $arg$.")
      (target "$mcname$ take$s$ a quick nap in your $arg$.")
      (rest  "$mcname$ take$s$ a quick nap in $mhcname$ $arg$.")
    )
  )
)

