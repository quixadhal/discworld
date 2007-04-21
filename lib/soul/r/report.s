(report
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self "You report $hcname$ to the Code Police.")
      (target "$mcname$ report$s$ you to the Code Police.")
      (rest "$mcname$ report$s$ $hcname$ to the Code Police.")
    )
    (arguments
      (arguments DrGoon, PinkFish, the Watch, the Thieves Guild, Dr Cruces,
                 Mustrum Ridcully, Granny Weatherwax, Veronica, Titania,
                 Samuel Vimes, Wuffles, Deutha, Signe, Sadistic Git,
                 Bravd the Hublander, the Luggage, Havelock Vetinari)
      (self "You report $hcname$ to $arg$.")
      (target "$mcname$ report$s$ you to $arg$.")
      (rest "$mcname$ report$s$ $hcname$ to $arg$.")
    )
    (arguments
      (arguments #)
      (self "You report $hcname$ $arg$.")
      (target "$mcname$ report$s$ you $arg$.")
      (rest "$mcname$ report$s$ $hcname$ $arg$.")
    )
  )
)
