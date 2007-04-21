/**
 * The include file for the weather handler.
 * @author Pinkfish
 */
#ifndef __SYS__WEATHER
/** @ignore yes */
#define __SYS__WEATHER

/**
 * The location of the weather handler.
 */
#define WEATHER "/obj/handlers/weather"
/**
 * The location of the weather handler.
 */
#define WEATHER_HANDLER WEATHER

/**
 * The type of rain if it is not raining at all.
 */
#define WEATHER_NO_RAIN_TYPE 0
/**
 * The type of precipitation if it is raining.
 */
#define WEATHER_RAIN_TYPE 3
/**
 * The type of precipitation if it is sleeting.
 */
#define WEATHER_SLEET_TYPE 2
/**
 * The type of precipitation if it is snowing.
 */
#define WEATHER_SNOW_TYPE 1

/**
 * The standard set of climate types.
 */
#define CLIMATES ({ "Af", "Am", "Aw", "Bw", "Bs", "Cfa", "Cs", "Cfb", \
                    "Dw", "Ds", "Df", "Et", "Ef", "H" })
/**
 * Default climate
 */
#define DEFAULT_CLIMATE "Cfb"

#endif /* __SYS__WEATHER */
