(prescribe
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (arguments
     (arguments dried frog pills, narcotics, bedrest, sanity, chill,#)
      (self "You prescribe a dose of $arg$ for $hcname$.")
      (target "$mcname$ prescribe$s$ a dose of $arg$ for $hcname$.")
      (rest "$mcname$ prescribe$s$ a dose of $arg$ for $hcname$".)
    )
  )
)
