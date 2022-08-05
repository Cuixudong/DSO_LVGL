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

/*If "lv_conf.h" is available from here try to use it later.*/
#ifdef __has_include
#  if __has_include("lvgl.h")
#    ifndef LV_LVGL_H_INCLUDE_SIMPLE
#      define LV_LVGL_H_INCLUDE_SIMPLE
#    endif
#  endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "../lvgl/lvgl.h"
#endif

/*********************
 *      DEFINES
 *********************/
/*Test  lvgl version*/
#if LV_VERSION_CHECK(8, 0, 0) == 0
#error "lv_demo: Wrong lvgl version"
#endif

#define MY_DISP_HOR_RES (480)   /* ÆÁÄ»¿í¶È */
#define MY_DISP_VER_RES (320)   /* ÆÁÄ»¸ß¶È */


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/

void lv_port_disp_init(void);
void lv_user_init(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_USER_H*/
