(sigh
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You sigh.")
      (rest "$mcname$ sigh$s$.")
    )
   (arguments 
     (arguments deeply, silently, desperately, tiredly, sadly, slowly, dreamily,
                happily, melodramatically, wistfully, dramatically, heavily,
                suggestively, sarcastically, in relief, with relief,
                contentedly, seductively, softly, resignedly, #)
      (self "You sigh $arg$.")
      (rest "$mcname$ sigh$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You sigh at $hcname$.")
      (target "$mcname$ sigh$s$ at you.")
      (rest  "$mcname$ sigh$s$ at $hcname$.")      
    )
    (arguments
     (arguments deeply, silently, desperately, tiredly, sadly, slowly, dreamily,
                happily, melodramatically, wistfully, dramatically, heavily,
                suggestively, sarcastically, in relief, with relief,
                contentedly, seductively, softly, resignedly, #)
      (self   "You sigh $arg$ at $hcname$.")
      (target "$mcname$ sigh$s$ $arg$ at you.")
      (rest  "$mcname$ sigh$s$ $arg$ at $hcname$.")
     )
   )
 )
