(howl
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "HOOOOOWWWWWWWWWLLLLLL!!")
      (rest "$mcname$ howl$s$.")
    )
    (arguments
    (arguments like a lunatic, insolently, lustily, eerily, softly, with glee, 
               hungrily, up and down the scale, for peanut butter, unabashedly, like  
               Angua, #)
    (self "You HOOOOOWWWWWWWWWLLLLLL $arg$.")
    (rest "$mcname$ howl$s$ $arg$.")
    )
  )
 (targeted
    (no-arguments
      (self "You HOOOOOWWWWWWWWWLLLLLL at $hcname$.")
      (target "$mcname$ howl$s$ at you.")
      (rest "$mcname$ howl$s$ at $hcname$.")
    )
    (arguments
    (arguments like a lunatic, insolently, lustily, eerily, softly, with glee, 
	              hungrily, up and down the scale, for peanut butter, unabashedly,
	              like Angua, #)
    (self "You HOOOOOWWWWWWWWWLLLLLL $arg$ at $hcname$.")
    (target "$mcname$ howl$s$ $arg$ at you.")
    (rest "$mcname$ howl$s$ $arg$ at $hcname$.")
    )
  )
)
/* Modified by Saffra Oct 09 2001 */