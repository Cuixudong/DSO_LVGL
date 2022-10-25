/**
 * @file beep.h
 *
 */

#ifndef BEEP_H
#define BEEP_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
 
#include "main.h"

/**********************
 *      TYPEDEFS
 **********************/
#define BEEP_MAX_VOL    80U
#define BEEP_DEF_VOL    20U

typedef enum
{
    BEEP_DEF_5 = 5,
    BEEP_DEF_10 = 10,
    BEEP_DEF_20 = 20,
    BEEP_DEF_40 = 40,
    BEEP_DEF_50 = 50,
    BEEP_DEF_60 = 60,
    BEEP_DEF_70 = 70,
    BEEP_DEF_80 = 80,
}BEEP_VOL;

#define ENC_BEEP_ON_T   25U
#define ENC_BEEP_OPEN()   beep_on(ENC_BEEP_ON_T)

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/
 
void beep_init(void);
void beep_handle(void);
void beep_set_vol(uint8_t vol);
void beep_on(uint8_t t_ms);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*BEEP_H*/
