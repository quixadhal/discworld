(love
  (pattern "<indirect:any-living>" )
  (pattern "<indirect:any-living> <string>" )
  (pattern "[at] <indirect:any-living> <string>")
  (targeted
    (no-arguments
      (self   "You whisper sweet nothings into $mhcname$ ear.")
      (target "$mcname$ whisper$s$ sweet nothings in your ear.")
      (rest   "$mcname$ whisper$s$ to $hcname$.")
    )
    (arguments
    (arguments tenderly, softly, #)
      (self   "You whisper sweet nothings into $mhcname$ ear $arg$.")
      (target "$mcname$ whisper$s$ sweet nothings in your ear $arg$.")
      (rest   "$mcname$ whisper$s$to $hcname$ $arg$.")
    )
  )
)

