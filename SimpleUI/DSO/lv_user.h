/**
 * @file lv_user.h
 *
 */

#ifndef LV_USER_H
#define LV_USER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
 
#include "stm32f4xx_hal.h"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

extern int enc_pos[2];
/**********************
 *      MACROS
 **********************/

uint8_t check_enc(uint8_t para);

void lv_user_init(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_USER_H*/
