(mosh
  (pattern "<string>" )
  (pattern "<indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (pattern "<string> [at] <indirect:any-living>") 
  (single
    (arguments
      (arguments violently, angrily, carefully, thoughtfully, wonderingly,
                 vigorously, insanely, loudly, like Trent, clumsily, 
                 drunkenly,  in combat boots, #)
      (self "You mosh $arg$ around the room.")
      (rest "$mcname$ mosh$s$ $arg$ around the room.")
    )
  )
  (targeted
    (arguments
      (arguments violently, angrily, carefully, thoughtfully, wonderingly,
                 vigorously, insanely, loudly, like Trent, clumsily,  
                 drunkenly, in combat boots, #)
      (self   "You mosh up against $hcname$ $arg$.")
      (target "$mcname$ mosh$s$ up against you $arg$.")
      (rest   "$mcname$ mosh$s$ up against $hcname$ $arg$.")
    )
  )
)
