(flip
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (arguments
      (arguments  head over heels, backwards, forwards, sideways, around, 
                  carefully, slowly, twice, doggily, awkwardly, disturbingly, 
                  quickly, slowly, dangerously, spectacularly, gracefully, 
                  over the moon, like a pancake, like a playing card, 
                  like an acrobat, 
                  around in a really complicated double back roll flip, #)
      (self "You flip $arg$.")
      (rest "$mcname$ flip$s$ $arg$.")
    )
    (arguments
      (arguments  half way around)
      (self "You flip $arg$ and land on your head.")
      (rest "$mcname$ flip$s$ $arg$ and lands on $mposs$ head.")
    ) 
  )
  (targeted
    (arguments
      (arguments  head over heels, off, into the garbage bin, viciously, 
                  backwards, forwards, sideways, around, carefully, twice, 
                  doggily, awkwardly, disturbingly, quickly, slowly, gracefully,
                  dangerously, spectacularly, over the moon, like a pancake, 
                  like a playing card, like an acrobat, 
                  around in a really complicated double back roll flip, #)
      (self   "You flip $hcname$ $arg$.")
      (target "$mcname$ flip$s$ you $arg$.")
      (rest   "$mcname$ flip$s$ $hcname$ $arg$.")
    )
  )
)

