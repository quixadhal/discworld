(timewarp
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You do the Time Warp.")
      (rest "$mcname$ does the Time Warp.")
    )
    (arguments
      (arguments again, with a jump to the left, with a step to the right, 
                 with your hands on your hips, with your knees in tight, with a    
                 pelvic thrust, in tap shoes, with a red feather boa, with an 
                 axe, in a freezer, in an elevator, on the table, in the
                 floorshow in a strange house, in the middle of the night, in a 
                 late-night double feature picture show, in the velvet darkness 
                 of the blackest night, in another dimension, with voyeuristic 
                 intention, by the light of the night, #)
      (self "You do the Time Warp $arg$.")
      (rest "$mcname$ does the Time Warp $arg$.")
    )
  )
  (targeted
  (no-arguments
    (self   "You do the Time Warp with $hcname$.")
    (target "$mcname$ does the Time Warp with you.")
    (rest   "$mcname$ does the Time Warp with $hcname$.")
  )
    (arguments
      (arguments again, with a jump to the left, with a step to the right, 
                 with your hands on your hips, with your knees in tight, with a    
                 pelvic thrust, in tap shoes, with a red feather boa, with an 
                 axe, in a freezer, in an elevator, on the table, in the
                 floorshow in a strange house, in the middle of the night, in a 
                 late-night double feature picture show, in the velvet darkness 
                 of the blackest night, in another dimension, with voyeuristic 
                 intention, by the light of the night, #)
      (self   "You do the Time Warp with $hcname$ $arg$.")
      (target "$mcname$ does the Time Warp with you $arg$.")
      (rest   "$mcname$ does the Time Warp with $hcname$ $arg$.")
    )
  )
)
/* Saffra 5-14-00 */
