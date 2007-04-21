(yikes
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<string> [at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You go \"yikes!\"")
      (rest "$mcname$ go$s$ \"yikes!\"")
    )
    (arguments
      (arguments in surprise,in fright,in fear,in terror)
      (self "You go \"yikes!\" $arg$.")
      (rest "$mcname$ go$s$ \"yikes!\" $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You go \"yikes!\" at $hcname$.")
      (target "$mcname$ go$s$ \"yikes!\" at you.")
      (rest  "$mcname$ go$s$ \"yikes!\" at $hcname$.")
    )
    (arguments
      (arguments in surprise,in fright,in fear,in terror)
      (self   "You go \"yikes!\" $arg$ at $hcname$.")
      (target "$mcname$ go$s$ \"yikes!\" $arg$ at you.")
      (rest  "$mcname$ go$s$ \"yikes!\" $arg$ at $hcname$.")
    )
  )
)
