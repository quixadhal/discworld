(flirt
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You flirt with $hcname$.")
      (target "$mcname$ flirt$s$ with you.")
      (rest   "$mcname$ flirt$s$ with $hcname$.")
    )
    (arguments
      (arguments  softly, passionately, deeply, tenderly, gently, brightly, 
                  after a fashion, hopefully, lovingly, lengthily, fishily, 
                  carefully, demandingly, sensuously, longingly, hungrily, 
                  back, boringly, inattentively, absentmindedly, froggily, 
                  in an igloo, on a coffee table, in a shower, outrageously, 
                  slowly, suspiciously, calmly, like pinkfish, 
                  using a coffee table, #)
      (self   "You flirt with $hcname$ $arg$.")
      (target "$mcname$ flirt$s$ with you $arg$.")
      (rest   "$mcname$ flirt$s$ with $hcname$ $arg$.")
    )
  )
)

