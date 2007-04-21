(serenade
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You serenade everyone.")
      (rest "$mcname$ serenade$s$ everyone.")
    )
    (arguments
      (arguments  softly, musically, melodically, harmoniously,
                  romantically, seductively, lovingly, insistently, 
                  with a guitar, with love songs, under the moon, 
                  under the stars, in the rain, on a balcony,
                  in the warm still night, on a warm spring day,
                  like Casanunda, #)
      (self "You serenade everyone $arg$.")
      (rest "$mcname$ serenade$s$ everyone $arg$.")
    )
  )
  (targeted
  (no-arguments
    (self "You serenade $hcname$.")
    (target "$mcname$ serenade$s$ you.")
    (rest "$mcname$ serenade$s$ $hcname$.")
  )
  (arguments
    (arguments  softly, musically, melodically, harmoniously,
                romantically, seductively, lovingly, insistently, 
                with a guitar, with love songs, under the moon, 
                under the stars, in the rain, on a balcony,
                in the warm still night, on a warm spring day,
                like Casanunda, #)
    (self "You serenade $hcname$ $arg$." )
    (target "$mcname$ serenade$s$ you $arg$." )
    (rest "$mcname$ serenade$s$ $hcname$ $arg$." )
    )
  )
)
