#ifndef __SYS__WEAPON
#define __SYS__WEAPON

/**
 * These macros are used to index the array returned by query_attack_data() in
 * /std/weapon_logic.c
 */
#define W_ARRAY_SIZE 5
#define W_CHANCE     0
#define W_DAMAGE     1
#define W_TYPE       2
#define W_SKILL      3
#define W_FUNCTION   4

/**
 * The W_DAMAGE index of query_attack_data() can be an integer or a three
 * index array. These macros index that array.
 */
#define F_FIXED  0
#define F_NUM    1
#define F_DIE    2

/**
 * These macros index the armour_types data stored in /std/armour_logic.c
 * Quite why they're in weapon.h is beyond me -- Ceres.
 */
#define A_ARRAY_SIZE 2
#define A_NAME       0
#define A_TYPE       1
#define A_AC         0

/**
 * These macros are used to index the array returned by weapon_attacks() in
 * /std/weapon_logic.c
 */
#define AT_ARRAY_SIZE 4
#define AT_DAMAGE    0
#define AT_SKILL     1
#define AT_TYPE      2
#define AT_NAME      3
#endif /* __SYS__WEAPON */
