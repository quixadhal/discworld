(riddle
  (pattern "<indirect:any-living> <string>")
  (pattern "<string>")
  (single
    (no-arguments
      (self "You riddle.")
      (rest "$mcname$ riddles.")
    )
    (arguments
      (arguments randomly,in haiku,like a sphinx,unsolvably,
                 like a narsty thing in a murky underground cavern,
                 obliquely,inscrutably, #)
        (self "You riddle $arg$.")
        (rest "$mcname$ riddle$s$ $arg$.")
    )
    (arguments
      (arguments and twiddle my thumbs)
        (self "You riddle and twiddle your thumbs.")
        (rest "$mcname$ riddle$s$ and twiddle$s$ $mposs$ thumbs.")
    )
  )
  (targeted
    (arguments
      (arguments with bullets,in haiku,like a sphinx,over a barrel,unsolvably,
                 like a narsty thing in a murky underground cavern,
                 obliquely,randomly,inscrutably, #)
        (self   "You riddle $hcname$ $arg$.")
        (target "$mcname$ riddle$s$ you $arg$.")
        (rest   "$mcname$ riddle$s$ $hcname$ $arg$.")
    )
    (arguments
      (arguments with one hand tied behind my back)
        (self   "You riddle $hcname$ with one hand tied behind your back.")
        (target "$mcname$ riddle$s$ you with one hand tied behind $mposs$ back.")
        (rest   "$mcname$ riddle$s$ $hcname$ with one hand tied behind $mposs$ back.")
    )
  )	
)
