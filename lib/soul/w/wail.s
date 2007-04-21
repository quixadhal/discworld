(wail
  (pattern "<string> at <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> at <indirect:any-living>")
  (pattern "at <indirect:any-living> <string>")
 (pattern "[at] <indirect:any-living>")
  (single
    (no-arguments
      (self "You wail.")
      (rest "$mcname$ wail$s$.")
    )
    (arguments
      (arguments like a banshee, like a toothless banshee, manically, evilly,
                 wickedly, insanely,
                 impishly, cheekily, idiotically, repeatedly, in horror, 
                 in fright, in fear, in pain, in surprise, in frustration, 
                 in despair, in anger,#)
      (self "You wail $arg$.")
      (rest "$mcname$ wail$s$ $arg$.")
    )
  )
  (targeted
    (arguments
      (arguments like a banshee, like a toothless banshee,
       manically, evilly, wickedly, insanely,
                 impishly, cheekily, idiotically, repeatedly, in horror, 
                 in fright, in fear, in pain, in surprise, in frustration, 
                 in despair, in anger,#)
      (self   "You wail $arg$ at $hcname$.")
      (target "$mcname$ wail$s$ $arg$ at you.")
      (rest   "$mcname$ wail$s$ $arg$ at $hcname$.")
    )
      (arguments
       (arguments ear,face)
         (self   "You wail in $mhcname$ $arg$.")
         (target "$mcname$ wail$s$ in your $arg$.")
         (rest   "$mcname$ wail$s$ in $mhcname$ $arg$.")
      )
   )
)
/*Db  */

