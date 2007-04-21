(toast
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (arguments
      (arguments  happiness,two wombles,the future,#)
      (self "You propose a toast to $arg$.")
      (rest "$mcname$ propose$s$ a toast to $arg$")
	)
	)
  (targeted
    (no-arguments 
      (self   "You raise your glass and toast $hcname$.")
      (target "$mcname$ raise$s$ $mposs$ glass and toast$s$ you.")
      (rest  "$mcname$ raise$s$ $mposs$ glass and toast$s$ $hcname$.")
    )
  )
)
/* HOBBES, Suggested by Gototh 970823 */
