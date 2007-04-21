(sob
  (pattern "[on] <indirect:any-living>")
  (pattern "[on] <indirect:any-living> <string>")
  (pattern "<string> [on] <indirect:any-living>")
  (pattern "<string>")
  (single
    (arguments
      (arguments  quietly, loudly, despairingly, forlornly, happily, 
                  uncontrollably, completely, unexpectedly, silently, 
                  cheekily, messily, hysterically, woefully, sadly, #)
      (self "You sob $arg$.")
      (rest "$mcname$ sob$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You sob on $mhcname$ shoulder.")
      (target "$mcname$ sob$s$ on your shoulder.")
      (rest   "$mcname$ sob$s$ on $mhcname$ shoulder.")
    )
    (arguments
      (arguments  quietly, loudly, despairingly, forlornly, happily, 
                  uncontrollably, completely, unexpectedly, silently, 
                  cheekily, messily, hysterically, woefully, sadly, #)
      (self   "You sob $arg$ on $mhcname$ shoulder.")
      (target "$mcname$ sob$s$ $arg$ on your shoulder.")
      (rest   "$mcname$ sob$s$ $arg$ on $mhcname$ shoulder.")
    )
  )
)

