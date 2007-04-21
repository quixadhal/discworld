(bawl
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [on] <indirect:any-living>")
  (pattern "[on] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You bawl your eyes out.")
      (rest "$mcname$ bawl$s$ $mposs$ eyes out.")
    )
    (arguments
      (arguments  loudly, quietly, despairingly, forlornly, happily, 
                  uncontrollably, completely, unexpectedly, silently, 
                  cheekily, #)
      (self "You bawl your eyes out $arg$.")
      (rest "$mcname$ bawl$s$ $mposs$ eyes out $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  shoulder, chest, breast)
      (self "You bawl your eyes out on $mhcname$ $arg$.")
      (target "$mcname$ bawl$s$ $mposs$ eyes out on your $arg$.")
      (rest "$mcname$ bawl$s$ $mposs$ eyes out on $mhcname$ $arg$.")
    )
    (arguments
      (arguments  loudly, quietly, despairingly, forlornly, happily, 
                  uncontrollably, completely, unexpectedly, silently, 
                  cheekily, #)
      (self "You bawl your eyes out $arg$ on $mhcname$ shoulder.")
      (target "$mcname$ bawl$s$ $mposs$ eyes out $arg$ on your shoulder.")
      (rest "$mcname$ bawl$s$ $mposs$ eyes out $arg$ on $mhcname$ shoulder.")
    )
  )
)

