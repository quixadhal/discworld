(lust
  (pattern "[at] <indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You lust.")
      (rest "$mcname$ lust$s$.")
    )
    (arguments
      (arguments
       happily, sadly, nervously, wryly, broadly, crookedly, stupidly,
       idiotically, condescendingly, ironically, patiently, brightly,
       slightly, nastily, excitedly, strangely, with forced patience,
       plasticly, carefully, sadistically, lopsidedly, fishily, cattishly,
       faintly, like a train, angrily, paranoically, innocently, froggily,
       slyly, weakly, humbly, paradoxically, sarcastically, viciously, 
       garishly, sweetly, innocently, lovingly, dreamily, radiantly,
       enthusiastically, insolently, quirkily, completely, unconsciously,
       drunkenly, insanely, cutely, maliciously, smugly, absentmindedly,
       curtly, disturbingly, unbelievably, quietly, loudly, differently,
       winningly, in triumph, seductively, tightly, smugly,
       gosh there are lots of arguments, softly, lividly, demonically,
       warmly, grimly, indolently, ingeniously, wickedly, contentedly,
       statistically, spasmodically, incandescently, pleasantly,
       with menace, manically, warily, sardonically, lazily, serenely,
       disappointedly, coyly, spazmodically, sheepishly, enigmatically,
       friperally, wryly, hopefully, knowingly, politely, kindly,
      coffee table like, shyly, #)
      (self "You lust $arg$.")
      (rest "$mcname$ lust$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You lust after $hcname$.")
      (target "$mcname$ lust$s$ after you.")
      (rest  "$mcname$ lust$s$ after $hcname$.")
    )
    (arguments
      (arguments happily, sadly, nervously, wryly, broadly, crookedly,
                 stupidly, idiotically, condescendingly, ironically,
                 patiently, brightly, slightly, nastily, excitedly,
                 strangely, with forced patience, plasticly, carefully,
                 sadistically, lopsidedly, fishily, catishly, faintly,
                 like a train, angrily, paranoically, innocently, froggily,
                 slyly, weakly, humbly, paradoxically, sarcastically,
                 viciously, garishly, sweetly, innocently, lovingly,
                 dreamily, radiantly, enthusiastically, insolently,
                 quirkily, completely, unconsciously, drunkenly, insanely,
                 cutely, maliciously, smugly, absentmindedly, curtly,
                 disturbingly, unbelievably, quietly, loudly, differently,
                 winningly, in triumph, seductively, tightly, smugly,
                 gosh there are lots of arguments, softly, lividly,
                 demonically, warmly, grimly, indolently, ingeniously,
                 wickedly, contentedly, statistically, spasmodically,
                 incandescently, pleasantly, with menace, manically,
                 warily, sardonically, lazily, serenely, disappointedly,
                 coyly, spazmodically, sheepishly, enigmatically,
                 friperally, wryly, hopefully, knowingly, politely,
                   kindly, coffee table like, shyly, #)
      (self   "You lust $arg$ after $hcname$.")
      (target "$mcname$ lust$s$ $arg$ after you.")
      (rest  "$mcname$ lust$s$ $arg$ after $hcname$.")
    )
  )
)
