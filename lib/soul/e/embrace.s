(embrace
  (pattern "<indirect:any-living>")
  (pattern "<string> <indirect:any-living>")
  (pattern "<indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You embrace $hcname$.")
      (target "$mcname$ embrace$s$ you.")
      (rest  "$mcname$ embrace$s$ $hcname$.")
    )
    (arguments
      (arguments  tightly, warmly, gently, close, lustfully, 
                  touchy-feelingly, lovingly, affectionately, sweetly, 
                  mushily, schmoopily, carnally, adoringly, suddenly, 
                  like a huge bear, in a public display of affection, 
                  with pillows, with fluffy pillows, #)
      (self   "You embrace $hcname$ $arg$.")
      (target "$mcname$ embrace$s$ you $arg$.")
      (rest  "$mcname$ embrace$s$ $hcname$ $arg$.")
    )
  )
)

