(drool
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You start to drool.")
      (rest "$mcname$ start$s$ to drool.")
    )
    (arguments
      (arguments manically, hungrily, slobberingly, messily, #)
      (self "You drool $arg$.")
      (rest "$mcname$ drool$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You drool all over $hcname$.")
      (target "$mcname$ drool$s$ all over you.")
      (rest   "$mcname$ drool$s$ all over $hcname$.")
    )
    (arguments
      (arguments meal, dinner, lunch, breakfast, tea, brunch, snack, sandwich, 
                 pork chops, potatoes, candy, chocolate, cabbage, 
                 banana daiquiri, scumble, #)
      (self   "You drool all over $mhcname$ $arg$.")
      (target "$mcname$ drool$s$ all over your $arg$.")
      (rest   "$mcname$ drool$s$ all over $mhcname$ $arg$.")
    )
  )
)
