(plot
  (pattern "<string>")
  (pattern "[against] <indirect:any-living>")
  (pattern "<string> [with] <indirect:any-living>")
  (pattern "[with] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You plot.")
      (rest "$mcname$ plot$s$.")
    )
    (arguments
      (arguments  to overthrow the Disc, to downgrape spells, 
                  another skills cut, a mutiny, a strategic alliance, 
                  a rebellion, a cunning plan, more cunning plans, 
                  vicious things, horrible tortures, like a deranged frog, 
                  to make someone's day, cunningly, madly, and scheme, 
                  thoughtfully, a diversion, like a mad scientist, 
                  like the Scarlet Pimpernel, like Robespierre, 
                  like a Guild Master, like a Magistrate, like a High Lord, like Pinky and the Brain, #)
      (self "You plot $arg$.")
      (rest "$mcname$ plot$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments  against)
      (self   "You plot $arg$ $hcname$.")
      (target "$mcname$ plot$s$ $arg$ you.")
      (rest   "$mcname$ plot$s$ $arg$ $hcname$.")
    )
    (arguments
      (arguments  with)
      (self   "You plot with $hcname$.")
      (target "$mcname$ plot$s$ with you.")
      (rest   "$mcname$ plot$s$ with $hcname$.")
    )
    (arguments
      (arguments  to overthrow the Disc, to downgrape spells, 
                  another skills cut, a mutiny, a strategic alliance, 
                  a rebellion, a cunning plan, more cunning plans, 
                  vicious things, horrible tortures, like a deranged frog, 
                  to make someone's day, cunningly, madly, and scheme, 
                  thoughtfully, a diversion, like a mad scientist, 
                  like the Scarlet Pimpernel, like Robespierre, 
                  like a Guild Master, like a Magistrate, like a High Lord, like Pinky and the Brain, #)
      (self   "You plot $arg$ with $hcname$.")
      (target "$mcname$ plot$s$ $arg$ with you.")
      (rest   "$mcname$ plot$s$ $arg$ with $hcname$.")
    )
  )
)

