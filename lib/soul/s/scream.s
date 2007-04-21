(scream
  (pattern "<string> at <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> at <indirect:any-living>")
  (pattern "at <indirect:any-living> <string>")
(pattern "[at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You scream.")
      (rest "$mcname$ scream$s$.")
    )
    (arguments
      (arguments loudly, happily, manically, evilly, wickedly, insanely,
                 impishly, cheekily, idiotically, repeatedly, in horror, 
                 in fright, in fear, in pain, in surprise, in frustration, 
                 in despair, in anger,#)
      (self "You scream $arg$.")
      (rest "$mcname$ scream$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments loudly, happily, manically, evilly, wickedly, insanely,
                 impishly, cheekily, idiotically, repeatedly, in horror, 
                 in fright, in fear, in pain, in surprise, in frustration, 
                 in despair, in anger,#)
      (self   "You scream $arg$ at $hcname$.")
      (target "$mcname$ scream$s$ $arg$ at you.")
      (rest   "$mcname$ scream$s$ $arg$ at $hcname$.")
    )
      (arguments
       (arguments ear,face)
         (self   "You scream in $mhcname$ $arg$.")
         (target "$mcname$ scream$s$ in your $arg$.")
         (rest   "$mcname$ scream$s$ in $mhcname$ $arg$.")
      )
   )
)
/*Hobbes 970730 */

