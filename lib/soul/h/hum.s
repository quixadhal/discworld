(hum
  (pattern "<string>")
    (pattern "[to] <indirect:any-living>")
  (pattern "<string> [to] <indirect:any-living>")
  (pattern "[to] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You hum.")
      (rest "$mcname$ hum$s$.")
    )
    (arguments
      (arguments  a merry tune, tunelessly, annoyingly, vaguely, cutely, 
                  absentmindedly, whilst you work, distractedly, pointlessly, 
                  silently, loudly, rudely, #)
      (self "You hum $arg$.")
      (rest "$mcname$ hum$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self   "You hum to $hcname$.")
      (target "$mcname$ hum$s$ to you.")
      (rest   "$mcname$ hum$s$ to $hcname$.")    
    )
    (arguments
      (arguments  a merry tune, tunelessly, annoyingly, vaguely, cutely, 
                  absentmindedly, whilst you work, distractedly, pointlessly, 
                  silently, loudly, rudely, #)
      (self   "You hum $arg$ to $hcname$.")
      (target "$mcname$ hum$s$ $arg$ to you.")
      (rest   "$mcname$ hum$s$ $arg$ to $hcname$.")    
    )
  )
)

