(chase
  (pattern "<indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You chase $hcname$.")
      (target "$mcname$ chase$s$ you.")
      (rest   "$mcname$ chase$s$ $hcname$.")
    )
    (arguments
      (arguments  around the room, around the Disc, into a corner, up a tree,
                  to the Counterweight Continent and back, off to bed, #)
     (self   "You chase $hcname$ $arg$.")
      (target "$mcname$ chase$s$ you $arg$.")
      (rest   "$mcname$ chase$s$ $hcname$ $arg$.")
    )
  )
)


