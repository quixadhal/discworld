(crash
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern " <string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string> ")
  (single
    (no-arguments
      (self "You crash.")
      (rest "$mcname$ crash.")
    )
    (arguments
      (arguments onto the ground, onto the floor, into the wall, into the 
                 crowd, into the Disc, like a caffeine addict, like a bugge, 
                 disasterously, with an explosion, # )
      (self "You crash $arg$.")
      (rest "$mcname$ crash $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You crash into $hcname$.")
      (target "$mcname$ crashes into you.")
      (rest "$mcname$ crashes into $hcname$.")
    )
    (arguments
      (arguments onto the ground, onto the floor, into the wall, into the 
                 crowd, into the Disc, into a tree, into a house)
      (self   "You crash $hcname$ $arg$.")
      (target "$mcname$ crashes you $arg$.")
      (rest   "$mcname$ crashes $hcname$ $arg$.")
    )
    (arguments
      (arguments like a caffeine addict, like a bugge, like the Disc,   
                 disasterously, with an explosion, #)
      (self   "You crash into $hcname$ $arg$.")
      (target "$mcname$ crashes into you $arg$.")
      (rest   "$mcname$ crashes into $hcname$ $arg$.")
    )

  )
)
/* Saffra Mar 30 2003 */
