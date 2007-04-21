(fika
  (pattern "<string>")
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>")
  (pattern "<string> <indirect:any-living>")
  (single
    (no-arguments
      (self "You fika.")
      (rest "$mcname$ fika$s$.")
    )
    (arguments
      (arguments  on a coffee table, like Wix, madly, passionately, wildly, 
                  without precautions, with cream, without cream, with two sugars, 
                  in a cafe, with milk, with non-dairy creamer, with foam, 
                  with a bowl of cream and a plate of strawberry shortbread, #)
      (self "You fika $arg$.")
      (rest "$mcname$ fika$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You fika $hcname$.")
      (target "$mcname$ fika$s$ you.")
      (rest   "$mcname$ fika$s$ $hcname$.")
    )
    (arguments
      (arguments  on a coffee table, like Wix, madly, passionately, wildly, 
                  without precautions, with cream, without cream, with two sugars, 
                  in a cafe, with milk, with non-dairy creamer, with foam, 
                  with a bowl of cream and a plate of strawberry shortbread, #)
      (self   "You fika $hcname$ $arg$." )
      (target "$mcname$ fika$s$ you $arg$." )
      (rest   "$mcname$ fika$s$ $hcname$ $arg$." )
    )
  )
)
