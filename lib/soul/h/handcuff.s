(handcuff
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> [to] <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You handcuff yourself to a coffee table.")
      (rest "$mcname$ $verb$handcuffs,handcuff$ $mobj$self to a coffee table.")
    )
    (arguments  
      (arguments  a coffee table, a refrigerator, a bicycle, a motorcycle,
              a bedpost, a desk, a chair, a sofa, a mailbox, a drainpipe,
              a cop, a cabbage, a womble, a computer, a shower, a frog, a spoon,
              a door handle, a green door, a thought bubble,
              a stick of dynamite, an ideal, a dentist, a cute girl,
              a cute guy,
              a High Lord, #)
      (self "You handcuff yourself to $arg$.")
      (rest "$mcname$ $verb$handcuffs,handcuff$ $mobj$self to $arg$.")
    )
    (arguments  
      (arguments  #)
      (self "You handcuff yourself to $arg$.")
      (rest "$mcname$ $verb$handcuffs,handcuff$ $mobj$self to $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You handcuff yourself to $hcname$.")
      (target "$mcname$ $verb$handcuffs,handcuff$ $mobj$self to you.")
      (rest "$mcname$ $verb$handcuffs,handcuff$ $mobj$self to $hcname$.")
    )
    (arguments
      (arguments  a coffee table, a refrigerator, a bicycle, a motorcycle, 
              a bedpost, a desk, a chair, a sofa, a mailbox, a drainpipe, 
              a cop, a cabbage, a womble, a computer, a frog, a spoon,
              a door handle, a green door, a thought bubble,
              a stick of dynamite, an ideal, a dentist, a cute girl,
              a cute guy,
              a High Lord, #)
      (self "You handcuff $hcname$ to $arg$." )
      (target "$mcname$ $verb$handcuffs,handcuff$ you to $arg$." )
      (rest "$mcname$ $verb$handcuffs,handcuff$ $hcname$ to $arg$.")
    )
    (arguments
      (arguments  #)
      (self "You handcuff $hcname$ to $arg$." )
      (target "$mcname$ $verb$handcuffs,handcuff$ you to $arg$." )
      (rest "$mcname$ $verb$handcuffs,handcuff$ $hcname$ to $arg$.")
    )
  )
)
