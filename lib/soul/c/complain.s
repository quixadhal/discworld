(complain
   (pattern "[about] <string>")
   (pattern "[about] <string> [to] <indirect:any-living>")
   (single
      (arguments
         (arguments the weather, the country you live in,
                    a country you do not live in,
                    the quality of help these days,
                    the good old days, chickens eating your food,
                    snow, rain, heat, cold, frogs, ice, drinks,
                    chocolate, smoking, fur, cats, dogs,
                    yesterday, today, pinkfish, allana,
                    too many souls, rubber boots, holes in your jeans,
                    hobbes, love, not getting any, #)
         (self "You complain about $arg$.")
         (rest "$mcname$ complains about $arg$.")
      )
   )
   (targeted
     (arguments
         (arguments the weather, the country you live in,
                    a country you do not live in,
                    the quality of help these days,
                    the good old days, chickens eating your food,
                    snow, rain, heat, cold, frogs, ice, drinks,
                    chocolate, smoking, fur, cats, dogs,
                    yesterday, today, pinkfish, allana,
                    too many souls, rubber boots, holes in your jeans,
                    hobbes, love, not getting any, #)
         (self "You complain about $arg$ to $hcname$.")
         (target "$mcname$ complains about $arg$ to you.")
         (rest "$mcname$ complains about $arg$ to $hcname$.")
      )
    )
  )
)
