(stare
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You stare into space.")
      (rest "$mcname$ stare$s$ into space.")
    )
    (arguments
      (arguments happily, sadly, worriedly, lazily, rudely, dazedly,
                 hungrily, macabrely, absentmindedly, fanatically,
                 lovingly, respectfully, womblely, froggily, desperately,
                 sternly, tentatively, politely, listlessly, jealously,
                 enviously, dreamily, intently, mindlessly, expectantly,
                 longingly, anxiously, fluffily, spongily, portablely, #)
      (self "You stare $arg$ into space.")
      (rest "$mcname$ stare$s$ $arg$ into space.")
    )
  )
  (targeted
    (no-arguments
      (self   "You stare at $hcname$.")
      (target "$mcname$ stare$s$ at you.")
      (rest  "$mcname$ stare$s$ at $hcname$.")
    )
    (arguments
      (arguments  happily, sadly, worriedly, lazily, rudely, dazedly,
                  hungrily, macabrely, absentmindedly, fanatically,
                  lovingly, respectfully, womblely, froggily, desperately,
                    sternly, tentatively, politely, listlessly, jealously,
                    enviously, dreamily, intently, mindlessly, romantically,
                    expectantly, longingly, continuously, annoyingly,
                    anxiously, fluffily, spongily, portablely, #)
      (self   "You stare $arg$ at $hcname$.")
      (target "$mcname$ stare$s$ $arg$ at you.")
      (rest  "$mcname$ stare$s$ $arg$ at $hcname$.")
     )
    (arguments
      (arguments eyes, mouth, nose, lips, ears, eyebrows, hands, fingers,
                 fingernails, arms, chest, waist, stomach, legs, 
                 ankles, feet, toenails)
      (self   "You stare at $hcname$'s $arg$.")
      (target "$mcname$ stare$s$ at your $arg$")
      (rest  "$mcname$ stare$s$ at $hcname$'s $arg$.")
    )
  )
)

