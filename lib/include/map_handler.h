#define MH "/obj/handlers/map_helper"
#define MAP_HANDLER "/obj/handlers/map"
#define SIZE 0
#define TIME 1
#define LOAD 2

#define DESTINATION 0
#define EXIT_SIZE 1
#define EXIT_TYPE 2
#define OFFSET    3

/* tune defines
 */
#define CACHESIZE 60 
#define SKEWTIME 4

/* debug defines */
#undef LOGTO "olorin"
#undef HELPLOGTO "ceres"
#define COORDS 1
#undef PARANOID
#define LOG_EXITS 1
#define LOG_SHORTS 1

#ifdef LOGTO
#define LOG(X) \
if ( find_living( LOGTO ) ) \
  tell_object( find_living( LOGTO ), X )
#else
#define LOG(X)
#endif

