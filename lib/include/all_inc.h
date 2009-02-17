#ifndef __ALL_INC_H
#define __ALL_INC_H

#define NEW_DRIVER

#define DISTRIBUTION_LIB

//optimize and save_types are off because they can crash us

#pragma strict_types
#pragma warnings
#pragma no_save_types
#pragma no_optimize
#pragma show_error_context

#define slice_array(a,b,c) (a[b..c])
#define m_indices(map) (keys(map))
#define m_sizeof(map) (sizeof(map))
#define m_values(map) (values(map))
//#define mapp(x) (mapp(x))

#ifndef __SENSIBLE_MODIFIERS__
#define nosave static
#define protected static
#endif

#endif
