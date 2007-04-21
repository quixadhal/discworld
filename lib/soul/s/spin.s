(spin
  (pattern "<indirect:any-living>")
  (pattern "<string>")
  (pattern "<string> [at] <indirect:any-living>")
  (pattern "[at] <indirect:any-living> <string>")
  (single
    (no-arguments
      (self "You spin.")
      (rest "$mcname$ spin$s$.")
    )
    (arguments
      (arguments around, like a top, about, deliriously, dizzily, 
                 suddenly, quickly, slowly, dizzily, nauseatingly, 
                 in a panic, like a tornado, like a blender, in a spiral, 
                 in a holding pattern, clockwise, counterclockwise, 
                 anti-clockwise, hubwards, deosil, widdershins, turnwise, 
                 in your grave, a yarn, a tale, right round, like a record, #)
      (self "You spin $arg$.")
      (rest "$mcname$ spin$s$ $arg$.")
    )
  )
  (targeted
    (no-arguments
      (self "You spin $hcname$ around.")
      (target "$mcname$ spin$s$ you around.")
      (rest "$mcname$ spin$s$ $hcname$ around.")
    )
    (arguments
      (arguments like a top, about, deliriously, dizzily, 
                 suddenly, quickly, slowly, dizzily, nauseatingly, 
                 in a panic, like a tornado, like a blender, in a spiral, 
                 in a holding pattern, clockwise, counterclockwise, 
                 anti-clockwise, hubwards, deosil, widdershins, turnwise, 
                 a yarn, a tale, right round, like a record, #)
      (self "You spin $hcname$ $arg$.")
      (target "$mcname$ spin$s$ you $arg$.")
      (rest "$mcname$ spin$s$ $hcname$ $arg$.")
    )
  )
)
