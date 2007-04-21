(wait
  (pattern "[for] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [for] <indirect:any-living>")
  (pattern "[for] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "Time passes.")
      (rest "$mcname$ wait$s$.")
    )
    (arguments
      (arguments impatiently,for ever,angrily,happily,idiotically,
      for something to happen,#)
      (self "You wait $arg$.")
      (rest "$mcname$ wait$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You wait for $hcname$.")
      (target "$mcname$ wait$s$ for you.")
      (rest  "$mcname$ wait$s$ for $hcname$.")
    )
    (arguments
      (arguments impatiently,for ever,angrily,happily,idiotically,
       for something to happen,#)
      (self   "You wait $arg$ for $hcname$.")
      (target "$mcname$ wait$s$ $arg$ for you.")
      (rest  "$mcname$ wait$s$ $arg$ for $hcname$.")
    )
  )
)



