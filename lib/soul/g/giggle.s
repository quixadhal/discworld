(giggle
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You giggle.")
      (rest "$mcname$ giggle$s$.")
    )
   (arguments
     (arguments  inanely, stupidly, snort, quietly, idiotically, 
                 fiendishly, insanely, slightly, carefully, cutely, 
                 sarcastically, happily, #)
      (self "You giggle $arg$.")
      (rest "$mcname$ giggle$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You giggle at $hcname$.")
      (target "$mcname$ giggle$s$ at you.")
      (rest  "$mcname$ giggle$s$ at $hcname$.")
    )
    (arguments
     (arguments  inanely, stupidly, snort, quietly, idiotically, 
                 fiendishly, insanely, slightly, carefully, cutely, 
                 sarcastically, happily, #)
      (self   "You giggle $arg$ at $hcname$.")
      (target "$mcname$ giggle$s$ $arg$ at you.")
      (rest  "$mcname$ giggle$s$ $arg$ at $hcname$.")
     )
   )
 )




