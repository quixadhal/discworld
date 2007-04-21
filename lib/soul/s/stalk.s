(stalk
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You get the impression someone's stalking you.")
      (rest "$mcname$ feel$s$ stalked.")
    )
  )
  (targeted
    (no-arguments
      (self   "You stalk $hcname$.")
      (target "$mcname$ stalk$s$ you.")
      (rest  "$mcname$ stalk$s$ $hcname$.")
    )
    (arguments
      (arguments  slowly, silently, closely, tenderly, lovingly,
                  lovingly, like a crotch sniffing labrador, from a distance, carefully, demandingly,
                  continuously, longingly, like Taffyd, whilst going through your trash, back, boringly,
                  with a gang of penguins, absentmindedly, wearing rubber,
                  in an igloo, to the toilet, in the shower,
                  slowly, suspiciously, like a deranged guilds creator who's got no marbles left, calmly,#)
      (self   "You stalk $hcname$ $arg$.")
      (target "$mcname$ stalk$s$ you $arg$.")
      (rest  "$mcname$ stalk$s$ $hcname$ $arg$.")
    )s
  )
)

