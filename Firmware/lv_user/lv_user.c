#include "lv_user.h"
#include "lcd.h"
#include "demos/lv_demos.h"

#include "usbh_hid_keybd.h"
#include "usbh_hid_mouse.h"

/* 显示设备初始化函数 */
static void disp_init(void);

/* 显示设备刷新函数 */
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

/**
 * @brief       LCD加速绘制函数
 * @param       (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex - sx + 1) * (ey - sy + 1)
 * @param       color:要填充的颜色
 * @retval      无
 */
void lcd_draw_fast_rgb_color(int16_t sx, int16_t sy,int16_t ex, int16_t ey, uint16_t *color)
{
    uint16_t w = ex-sx+1;
    uint16_t h = ey-sy+1;

    lcd_set_window(sx, sy, w, h);
    uint32_t draw_size = w * h;
    lcd_write_ram_prepare();

    for(uint32_t i = 0; i < draw_size; i++)
    {
        lcd_wr_data(color[i]);
    }
}

/**
 * @brief       初始化并注册显示设备
 * @param       无
 * @retval      无
 */
void lv_port_disp_init(void)
{
    /*-------------------------
     * 初始化显示设备
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * 创建一个绘图缓冲区
     *----------------------------*/

    /**
     * LVGL 需要一个缓冲区用来绘制小部件
     * 随后，这个缓冲区的内容会通过显示设备的 `flush_cb`(显示设备刷新函数) 复制到显示设备上
     * 这个缓冲区的大小需要大于显示设备一行的大小
     *
     * 这里有3中缓冲配置:
     * 1. 单缓冲区:
     *      LVGL 会将显示设备的内容绘制到这里，并将他写入显示设备。
     *
     * 2. 双缓冲区:
     *      LVGL 会将显示设备的内容绘制到其中一个缓冲区，并将他写入显示设备。
     *      需要使用 DMA 将要显示在显示设备的内容写入缓冲区。
     *      当数据从第一个缓冲区发送时，它将使 LVGL 能够将屏幕的下一部分绘制到另一个缓冲区。
     *      这样使得渲染和刷新可以并行执行。
     *
     * 3. 全尺寸双缓冲区
     *      设置两个屏幕大小的全尺寸缓冲区，并且设置 disp_drv.full_refresh = 1。
     *      这样，LVGL将始终以 'flush_cb' 的形式提供整个渲染屏幕，您只需更改帧缓冲区的地址。
     */

    /* 单缓冲区示例) */
    static lv_disp_draw_buf_t draw_buf_dsc_1;

    static lv_color_t buf_1[MY_DISP_HOR_RES * 50];                                              /* 设置缓冲区的大小为 10 行屏幕的大小 */
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 50);                  /* 初始化显示缓冲区 */

    /* 双缓冲区示例) */
//    static lv_disp_draw_buf_t draw_buf_dsc_2;
//    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];                                            /* 设置缓冲区的大小为 10 行屏幕的大小 */
//    static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];                                            /* 设置另一个缓冲区的大小为 10 行屏幕的大小 */
//    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 10);             /* 初始化显示缓冲区 */

    /* 全尺寸双缓冲区示例) 并且在下面设置 disp_drv.full_refresh = 1 */
//    static lv_disp_draw_buf_t draw_buf_dsc_3;
//    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];                               /* 设置一个全尺寸的缓冲区 */
//    static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];                               /* 设置另一个全尺寸的缓冲区 */
//    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2, MY_DISP_HOR_RES * MY_DISP_VER_RES);/* 初始化显示缓冲区 */

    /*-----------------------------------
     * 在 LVGL 中注册显示设备
     *----------------------------------*/

    static lv_disp_drv_t disp_drv;                  /* 显示设备的描述符 */
    lv_disp_drv_init(&disp_drv);                    /* 初始化为默认值 */

    /* 建立访问显示设备的函数  */

    /* 设置显示设备的分辨率，通常所使用的屏幕大小是固定的，因此可以直接设置为屏幕的大小 */
    disp_drv.hor_res = lcddev.width;
    disp_drv.ver_res = lcddev.height;
    /* 支持软件轮换 */
    disp_drv.sw_rotate = 1;
    /* 用来将缓冲区的内容复制到显示设备 */
    disp_drv.flush_cb = disp_flush;

    /* 设置显示缓冲区 */
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /* 全尺寸双缓冲区示例)*/
    //disp_drv.full_refresh = 1

    /* 如果您有GPU，请使用颜色填充内存阵列
     * 注意，你可以在 lv_conf.h 中使能 LVGL 内置支持的 GPUs
     * 但如果你有不同的 GPU，那么可以使用这个回调函数。 */
    //disp_drv.gpu_fill_cb = gpu_fill;

    /* 注册显示设备 */
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * @brief       初始化显示设备和必要的外围设备
 * @param       无
 * @retval      无
 */
static void disp_init(void)
{
    /*You code here*/
}

/**
 * @brief       将内部缓冲区的内容刷新到显示屏上的特定区域
 *   @note      可以使用 DMA 或者任何硬件在后台加速执行这个操作
 *              但是，需要在刷新完成后调用函数 'lv_disp_flush_ready()'
 *
 * @param       disp_drv    : 显示设备
 *   @arg       area        : 要刷新的区域，包含了填充矩形的对角坐标
 *   @arg       color_p     : 颜色数组
 *
 * @retval      无
 */
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /* LVGL 官方给出的一个打点刷新屏幕的例子，但这个效率是最低效的 */

//    int32_t x;
//    int32_t y;
//    for(y = area->y1; y <= area->y2; y++) {
//        for(x = area->x1; x <= area->x2; x++) {
//            /*Put a pixel to the display. For example:*/
//            /*put_px(x, y, *color_p)*/
//            color_p++;
//        }
//    }

//    /* 在指定区域内填充指定颜色块 */
//    lcd_color_fill(area->x1, area->y1, area->x2, area->y2, (uint16_t *)color_p);
    lcd_draw_fast_rgb_color(area->x1,area->y1,area->x2,area->y2,(uint16_t*)color_p);

    /* 重要!!!
     * 通知图形库，已经刷新完毕了 */
    lv_disp_flush_ready(disp_drv);
}

const uint8_t mouse_cursor_icon_map[] = {
#if LV_COLOR_DEPTH == 1 || LV_COLOR_DEPTH == 8
    /*Pixel format: Alpha 8 bit, Red: 3 bit, Green: 3 bit, Blue: 2 bit*/
    0x24, 0xb8, 0x24, 0xc8, 0x00, 0x13, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x49, 0xcc, 0xdb, 0xff, 0x49, 0xcc, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x49, 0xc8, 0xff, 0xff, 0xff, 0xff, 0x49, 0xe0, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x49, 0xcb, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0x6d, 0xf3, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x49, 0xcb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x92, 0xff, 0x00, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x49, 0xcb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x92, 0xff, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0xb6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x49, 0xcb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0xff, 0x24, 0xab, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x49, 0xcb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0xff, 0x24, 0xbb, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x49, 0xcc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0xff, 0x49, 0xd8, 0x00, 0x37, 0x00, 0x00, 0x00, 0x00,
    0x49, 0xcc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x6d, 0xef, 0x00, 0x4f, 0x00, 0x00,
    0x49, 0xcc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x92, 0xff, 0x00, 0x6b,
    0x49, 0xcc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0xff, 0x92, 0xf7, 0x92, 0xf8, 0x6e, 0xfb, 0x92, 0xf8, 0x6d, 0xff, 0x00, 0xb3,
    0x49, 0xcc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdb, 0xff, 0x24, 0xb7, 0x00, 0x1b, 0x00, 0x14, 0x00, 0x13, 0x00, 0x0c, 0x25, 0x07,
    0x49, 0xcc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x6d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x6e, 0xf0, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x49, 0xcc, 0xff, 0xff, 0xff, 0xff, 0x49, 0xd8, 0x00, 0x78, 0x92, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xdb, 0xff, 0x00, 0x8b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x6d, 0xd3, 0xff, 0xff, 0x6d, 0xef, 0x00, 0x34, 0x00, 0x00, 0x49, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x6d, 0xdc, 0x00, 0x14, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
    0x49, 0xe0, 0x6d, 0xff, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x57, 0x92, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0xff, 0x00, 0x78, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
    0x00, 0x68, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x49, 0xd0, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0x6d, 0xd8, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6f, 0xb7, 0xff, 0xff, 0xff, 0x92, 0xff, 0x49, 0xac, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x25, 0xd7, 0x49, 0xc7, 0x00, 0x47, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 0
    /*Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit*/
    0xc3, 0x18, 0xb8, 0xe4, 0x20, 0xc8, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x49, 0x4a, 0xcc, 0x96, 0xb5, 0xff, 0xc7, 0x39, 0xcc, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xe7, 0x39, 0xc8, 0xbf, 0xff, 0xff, 0xfb, 0xde, 0xff, 0x28, 0x42, 0xe0, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xe7, 0x39, 0xcb, 0x3d, 0xef, 0xff, 0xff, 0xff, 0xfc, 0x3d, 0xef, 0xff, 0xcb, 0x5a, 0xf3, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xe7, 0x39, 0xcb, 0x5d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0x8e, 0x73, 0xff, 0x00, 0x00, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xe8, 0x41, 0xcb, 0x5d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x51, 0x8c, 0xff, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd3, 0x9c, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xe8, 0x41, 0xcb, 0x5d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x14, 0xa5, 0xff, 0xa2, 0x10, 0xab, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x42, 0xcb, 0x5d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd7, 0xbd, 0xff, 0x04, 0x21, 0xbb, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x42, 0xcc, 0x5d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x59, 0xce, 0xff, 0xe8, 0x41, 0xd8, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x42, 0xcc, 0x5d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xe6, 0xff, 0xab, 0x5a, 0xef, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00,
    0x08, 0x42, 0xcc, 0x5d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbe, 0xf7, 0xff, 0xaf, 0x7b, 0xff, 0x00, 0x00, 0x6b,
    0x28, 0x42, 0xcc, 0x5d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7a, 0xd6, 0xff, 0x10, 0x84, 0xf7, 0xae, 0x73, 0xf8, 0x6e, 0x73, 0xfb, 0x8e, 0x73, 0xf8, 0xcb, 0x5a, 0xff, 0x61, 0x08, 0xb3,
    0x28, 0x42, 0xcc, 0x7d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x59, 0xce, 0xff, 0xa2, 0x10, 0xb7, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x14, 0x00, 0x00, 0x13, 0x00, 0x00, 0x0c, 0x45, 0x29, 0x07,
    0x29, 0x4a, 0xcc, 0x5d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xdb, 0xde, 0xff, 0xec, 0x62, 0xff, 0x1c, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0c, 0x63, 0xf0, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x29, 0x4a, 0xcc, 0xdf, 0xff, 0xff, 0x7d, 0xef, 0xff, 0x49, 0x4a, 0xd8, 0x00, 0x00, 0x78, 0x51, 0x8c, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x38, 0xc6, 0xff, 0x00, 0x00, 0x8b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xcb, 0x5a, 0xd3, 0xdb, 0xde, 0xff, 0xec, 0x62, 0xef, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0xe7, 0x39, 0xc7, 0x5d, 0xef, 0xff, 0xff, 0xff, 0xff, 0xbe, 0xf7, 0xff, 0xaa, 0x52, 0xdc, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0xe8, 0x41, 0xe0, 0xaa, 0x52, 0xff, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x57, 0x72, 0x94, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x96, 0xb5, 0xff, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x61, 0x08, 0x04, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x68, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x69, 0x4a, 0xd0, 0x7d, 0xef, 0xff, 0xff, 0xff, 0xfc, 0xbe, 0xf7, 0xff, 0xaa, 0x52, 0xd8, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe4, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6f, 0x75, 0xad, 0xff, 0xbf, 0xff, 0xff, 0x10, 0x84, 0xff, 0x86, 0x31, 0xac, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x41, 0x08, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x66, 0x31, 0xd7, 0xc7, 0x39, 0xc7, 0x00, 0x00, 0x47, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#endif
#if LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP != 0
    /*Pixel format: Alpha 8 bit, Red: 5 bit, Green: 6 bit, Blue: 5 bit  BUT the 2  color bytes are swapped*/
    0x18, 0xc3, 0xb8, 0x20, 0xe4, 0xc8, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x4a, 0x49, 0xcc, 0xb5, 0x96, 0xff, 0x39, 0xc7, 0xcc, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x39, 0xe7, 0xc8, 0xff, 0xbf, 0xff, 0xde, 0xfb, 0xff, 0x42, 0x28, 0xe0, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x39, 0xe7, 0xcb, 0xef, 0x3d, 0xff, 0xff, 0xff, 0xfc, 0xef, 0x3d, 0xff, 0x5a, 0xcb, 0xf3, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x39, 0xe7, 0xcb, 0xef, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0x73, 0x8e, 0xff, 0x00, 0x00, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x41, 0xe8, 0xcb, 0xef, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8c, 0x51, 0xff, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0xd3, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x41, 0xe8, 0xcb, 0xef, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x14, 0xff, 0x10, 0xa2, 0xab, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x42, 0x08, 0xcb, 0xef, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbd, 0xd7, 0xff, 0x21, 0x04, 0xbb, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x42, 0x08, 0xcc, 0xef, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xce, 0x59, 0xff, 0x41, 0xe8, 0xd8, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x42, 0x08, 0xcc, 0xef, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe6, 0xfc, 0xff, 0x5a, 0xab, 0xef, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00,
    0x42, 0x08, 0xcc, 0xef, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xbe, 0xff, 0x7b, 0xaf, 0xff, 0x00, 0x00, 0x6b,
    0x42, 0x28, 0xcc, 0xef, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd6, 0x7a, 0xff, 0x84, 0x10, 0xf7, 0x73, 0xae, 0xf8, 0x73, 0x6e, 0xfb, 0x73, 0x8e, 0xf8, 0x5a, 0xcb, 0xff, 0x08, 0x61, 0xb3,
    0x42, 0x28, 0xcc, 0xef, 0x7d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xce, 0x59, 0xff, 0x10, 0xa2, 0xb7, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x14, 0x00, 0x00, 0x13, 0x00, 0x00, 0x0c, 0x29, 0x45, 0x07,
    0x4a, 0x29, 0xcc, 0xef, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xde, 0xdb, 0xff, 0x62, 0xec, 0xff, 0xe7, 0x1c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x63, 0x0c, 0xf0, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x4a, 0x29, 0xcc, 0xff, 0xdf, 0xff, 0xef, 0x7d, 0xff, 0x4a, 0x49, 0xd8, 0x00, 0x00, 0x78, 0x8c, 0x51, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc6, 0x38, 0xff, 0x00, 0x00, 0x8b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x5a, 0xcb, 0xd3, 0xde, 0xdb, 0xff, 0x62, 0xec, 0xef, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x39, 0xe7, 0xc7, 0xef, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xbe, 0xff, 0x52, 0xaa, 0xdc, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x41, 0xe8, 0xe0, 0x52, 0xaa, 0xff, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x57, 0x94, 0x72, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb5, 0x96, 0xff, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x08, 0x61, 0x04, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x68, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x4a, 0x69, 0xd0, 0xef, 0x7d, 0xff, 0xff, 0xff, 0xfc, 0xf7, 0xbe, 0xff, 0x52, 0xaa, 0xd8, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xe4, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6f, 0xad, 0x75, 0xff, 0xff, 0xbf, 0xff, 0x84, 0x10, 0xff, 0x31, 0x86, 0xac, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x41, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x31, 0x66, 0xd7, 0x39, 0xc7, 0xc7, 0x00, 0x00, 0x47, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#endif
#if LV_COLOR_DEPTH == 32
    0x19, 0x19, 0x19, 0xb8, 0x1e, 0x1e, 0x1e, 0xc8, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x48, 0x48, 0x48, 0xcc, 0xb2, 0xb2, 0xb2, 0xff, 0x3a, 0x3a, 0x3a, 0xcc, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x0a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3b, 0x3b, 0x3b, 0xc8, 0xf6, 0xf6, 0xf6, 0xff, 0xdc, 0xdc, 0xdc, 0xff, 0x43, 0x43, 0x43, 0xe0, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x0a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3b, 0x3b, 0x3b, 0xcb, 0xe6, 0xe6, 0xe6, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xe5, 0xe5, 0xe5, 0xff, 0x59, 0x59, 0x59, 0xf3, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3c, 0x3c, 0x3c, 0xcb, 0xe9, 0xe9, 0xe9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf5, 0xf5, 0xf5, 0xff, 0x72, 0x72, 0x72, 0xff, 0x00, 0x00, 0x00, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3d, 0x3d, 0x3d, 0xcb, 0xe9, 0xe9, 0xe9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8a, 0x8a, 0x8a, 0xff, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0x99, 0x99, 0x00, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3e, 0x3e, 0x3e, 0xcb, 0xe9, 0xe9, 0xe9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa2, 0xa2, 0xa2, 0xff, 0x13, 0x13, 0x13, 0xab, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x3f, 0x3f, 0x3f, 0xcb, 0xe9, 0xe9, 0xe9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb7, 0xb7, 0xb7, 0xff, 0x1f, 0x1f, 0x1f, 0xbb, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x41, 0x41, 0x41, 0xcc, 0xea, 0xea, 0xea, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xca, 0xca, 0xca, 0xff, 0x3d, 0x3d, 0x3d, 0xd8, 0x00, 0x00, 0x00, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x41, 0x41, 0x41, 0xcc, 0xea, 0xea, 0xea, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xde, 0xde, 0xde, 0xff, 0x56, 0x56, 0x56, 0xef, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00,
    0x42, 0x42, 0x42, 0xcc, 0xea, 0xea, 0xea, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xf3, 0xf3, 0xff, 0x76, 0x76, 0x76, 0xff, 0x00, 0x00, 0x00, 0x6b,
    0x43, 0x43, 0x43, 0xcc, 0xea, 0xea, 0xea, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xce, 0xce, 0xce, 0xff, 0x80, 0x80, 0x80, 0xf7, 0x74, 0x74, 0x74, 0xf8, 0x6d, 0x6d, 0x6d, 0xfb, 0x72, 0x72, 0x72, 0xf8, 0x57, 0x57, 0x57, 0xff, 0x0c, 0x0c, 0x0c, 0xb3,
    0x44, 0x44, 0x44, 0xcc, 0xeb, 0xeb, 0xeb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xfb, 0xfb, 0xfb, 0xff, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc9, 0xc9, 0xc9, 0xff, 0x13, 0x13, 0x13, 0xb7, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x0c, 0x29, 0x29, 0x29, 0x07,
    0x45, 0x45, 0x45, 0xcc, 0xe8, 0xe8, 0xe8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd9, 0xd9, 0xd9, 0xff, 0x5e, 0x5e, 0x5e, 0xff, 0xe2, 0xe2, 0xe2, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x62, 0x62, 0x62, 0xf0, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x45, 0x45, 0x45, 0xcc, 0xf9, 0xf9, 0xf9, 0xff, 0xec, 0xec, 0xec, 0xff, 0x4a, 0x4a, 0x4a, 0xd8, 0x00, 0x00, 0x00, 0x78, 0x8a, 0x8a, 0x8a, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xc3, 0xc3, 0xff, 0x00, 0x00, 0x00, 0x8b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x58, 0x58, 0x58, 0xd3, 0xd9, 0xd9, 0xd9, 0xff, 0x5e, 0x5e, 0x5e, 0xef, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x3b, 0x3b, 0xc7, 0xe9, 0xe9, 0xe9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf4, 0xf4, 0xf4, 0xff, 0x54, 0x54, 0x54, 0xdc, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
    0x3e, 0x3e, 0x3e, 0xe0, 0x54, 0x54, 0x54, 0xff, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x57, 0x8e, 0x8e, 0x8e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb0, 0xb0, 0xb0, 0xff, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x4c, 0x4c, 0x4c, 0xd0, 0xec, 0xec, 0xec, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xf4, 0xf4, 0xf4, 0xff, 0x53, 0x53, 0x53, 0xd8, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x1e, 0x1e, 0x00, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6f, 0xab, 0xab, 0xab, 0xff, 0xf6, 0xf6, 0xf6, 0xff, 0x80, 0x80, 0x80, 0xff, 0x31, 0x31, 0x31, 0xac, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x09, 0x09, 0x09, 0x03, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x2e, 0x2e, 0x2e, 0xd7, 0x38, 0x38, 0x38, 0xc7, 0x00, 0x00, 0x00, 0x47, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
#endif
};

lv_img_dsc_t mouse_cursor_icon =
{
    .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
    .header.always_zero = 0,
    .header.reserved = 0,
    .header.w = 14,
    .header.h = 20,
    .data_size = 280 * LV_IMG_PX_SIZE_ALPHA_BYTE,
    .data = mouse_cursor_icon_map,
};

static void mouse_init(void);
static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool mouse_is_pressed(void);
static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y);

lv_indev_t * indev_mouse;       // 鼠标
lv_indev_t * indev_keypad;      // 键盘

/**
 * @brief       初始化并注册输入设备
 * @param       无
 * @retval      无
 */
void lv_port_indev_init(void)
{
    /**
     * 
     * 在这里你可以找到 LittlevGL 支持的出入设备的实现示例:
     *  - 触摸屏
     *  - 鼠标 (支持光标)
     *  - 键盘 (仅支持按键的 GUI 用法)
     *  - 编码器 (支持的 GUI 用法仅包括: 左, 右, 按下)
     *  - 按钮 (按下屏幕上指定点的外部按钮)
     *
     *  函数 `..._read()` 只是示例
     *  你需要根据具体的硬件来完成这些函数
     */

    static lv_indev_drv_t indev_drv;

    /*------------------
     * 鼠标
     * -----------------*/

    /* 初始化鼠标(如果有) */
    mouse_init();

    /* 注册鼠标输入设备 */
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = mouse_read;
    indev_mouse = lv_indev_drv_register(&indev_drv);

    /* 设置光标，为了简单起见，现在设置为一个 HOME 符号 */
    lv_obj_t * mouse_cursor = lv_img_create(lv_scr_act());
    lv_img_set_src(mouse_cursor, &mouse_cursor_icon);
    lv_indev_set_cursor(indev_mouse, mouse_cursor);

    /*------------------
     * 键盘
     * -----------------*/

//    /* 初始化键盘(如果有) */
//    keypad_init();

//    /* 注册键盘输入设备 */
//    lv_indev_drv_init(&indev_drv);
//    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
//    indev_drv.read_cb = keypad_read;
//    indev_keypad = lv_indev_drv_register(&indev_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#include "usb_host.h"
extern USBH_HandleTypeDef hUsbHostFS;
/*------------------
 * 鼠标
 * -----------------*/

/**
 * @brief       初始化鼠标
 * @param       无
 * @retval      无
 */
static void mouse_init(void)
{
    /*Your code comes here*/
}

/**
 * @brief       图形库的鼠标读取回调函数
 * @param       indev_drv   : 鼠标设备
 *   @arg       data        : 输入设备数据结构体
 * @retval      无
 */
static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    /* 获取当前的 x、y 坐标 */
    mouse_get_xy(&data->point.x, &data->point.y);

    /* 获取是否按下或释放鼠标按钮 */
    if(mouse_is_pressed()) {
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

/**
 * @brief       获取鼠标设备是否被按下
 * @param       无
 * @retval      返回鼠标设备是否被按下
 */
static bool mouse_is_pressed(void)
{
    /*Your code comes here*/
    #if 0
    HID_MOUSE_Info_TypeDef * mouse_data;
    mouse_data = USBH_HID_GetMouseInfo(&hUsbHostFS);
    if(mouse_data != NULL)
    {
        if(mouse_data->buttons[0])
        return true;
    }
    #else
    if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == GPIO_PIN_SET)
    {
        return true;
    }
    else
    #endif
    
    return false;
}

/**
 * @brief       当鼠标被按下时，获取鼠标的 x、y 坐标
 * @param       x   : x坐标的指针
 *   @arg       y   : y坐标的指针
 * @retval      无
 */
static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y)
{
//    static int xx = 100,yy = 100;
    
//    uint8_t enc1,enc2;
//    enc1 = check_enc(1);
//    enc2 = check_enc(2);
//    if(enc1 == 1)
//    {
//        xx -= 5;
//    }
//    else if(enc1 == 2)
//    {
//        xx += 5;
//    }
//    
//    if(enc2 == 1)
//    {
//        yy -= 5;
//    }
//    else if(enc2 == 2)
//    {
//        yy += 5;
//    }
    /*Your code comes here*/
    #if 0
    HID_MOUSE_Info_TypeDef * mouse_data;
    mouse_data = USBH_HID_GetMouseInfo(&hUsbHostFS);
    if(mouse_data != NULL)
    {
        (*x) = mouse_data->x;
        (*y) = mouse_data->y;
    }
    else
    {
        //(*x) = ((lv_tick_get() / 20) % 200) + 140;
        //(*y) = ((lv_tick_get() / 30) % 200) + 60;
    }
    #else
//    (*x) = ((lv_tick_get() / 20) % 200) + 140;
//    (*y) = ((lv_tick_get() / 30) % 200) + 60;
    (*x) = enc_pos[0];
    (*y) = enc_pos[1];
    #endif
}

#define ENC1_READ (((HAL_GPIO_ReadPin(EC1A_GPIO_Port,EC1A_Pin) == GPIO_PIN_SET) ? 1 : 0) << 1) + ((HAL_GPIO_ReadPin(EC1B_GPIO_Port,EC1B_Pin) == GPIO_PIN_SET) ? 1 : 0)
#define ENC2_READ (((HAL_GPIO_ReadPin(EC2A_GPIO_Port,EC2A_Pin) == GPIO_PIN_SET) ? 1 : 0) << 1) + ((HAL_GPIO_ReadPin(EC2B_GPIO_Port,EC2B_Pin) == GPIO_PIN_SET) ? 1 : 0)
__IO uint8_t g_enc_sta[2];
__IO uint8_t g_enc_old_sta[2];
int enc_pos[2] = {100,100};
__IO uint8_t ENC_TAB[4] = {0,2,3,1};

uint8_t check_enc(uint8_t para)
{
    uint8_t res = 0;
    uint8_t enc_status;
    if(para == 1)
    {
        enc_status = ENC1_READ;
        if(((enc_status == ENC_TAB[0]) && (g_enc_sta[0] == ENC_TAB[1]))||\
           ((enc_status == ENC_TAB[1]) && (g_enc_sta[0] == ENC_TAB[2]))||\
           ((enc_status == ENC_TAB[2]) && (g_enc_sta[0] == ENC_TAB[3]))||\
           ((enc_status == ENC_TAB[3]) && (g_enc_sta[0] == ENC_TAB[0])))
        {
            res = 1;
        }
        else if(((enc_status == ENC_TAB[0]) && (g_enc_sta[0] == ENC_TAB[3]))||\
                ((enc_status == ENC_TAB[1]) && (g_enc_sta[0] == ENC_TAB[0]))||\
                ((enc_status == ENC_TAB[2]) && (g_enc_sta[0] == ENC_TAB[1]))||\
                ((enc_status == ENC_TAB[3]) && (g_enc_sta[0] == ENC_TAB[2])))
        {
            res = 2;
        }
        
        g_enc_sta[0] = enc_status;
    }
    else if(para == 2)
    {
        enc_status = ENC2_READ;
        if(((enc_status == ENC_TAB[0]) && (g_enc_sta[1] == ENC_TAB[1]))||\
           ((enc_status == ENC_TAB[1]) && (g_enc_sta[1] == ENC_TAB[2]))||\
           ((enc_status == ENC_TAB[2]) && (g_enc_sta[1] == ENC_TAB[3]))||\
           ((enc_status == ENC_TAB[3]) && (g_enc_sta[1] == ENC_TAB[0])))
        {
            res = 1;
        }
        else if(((enc_status == ENC_TAB[0]) && (g_enc_sta[1] == ENC_TAB[3]))||\
                ((enc_status == ENC_TAB[1]) && (g_enc_sta[1] == ENC_TAB[0]))||\
                ((enc_status == ENC_TAB[2]) && (g_enc_sta[1] == ENC_TAB[1]))||\
                ((enc_status == ENC_TAB[3]) && (g_enc_sta[1] == ENC_TAB[2])))
        {
            res = 2;
        }
        g_enc_sta[1] = enc_status;
    }
    return res;
}

void enc_handle(void)
{
    #define ENC_MOVE_POS 5
    #define ENC_NUM     2
    #define ENC_F_NUM   3
    static uint8_t enc[ENC_NUM * ENC_F_NUM];//滤波编码
    static uint8_t p[ENC_NUM];              //滤波位置
    static uint8_t cnt = 0;
    
    int i,j,k;
    
    enc[(p[0])                 % (ENC_NUM * ENC_F_NUM)] = ENC1_READ;//编码器1
    enc[(p[1] + ENC_F_NUM * 1) % (ENC_NUM * ENC_F_NUM)] = ENC2_READ;//编码器2
    
    for(i=0;i<ENC_NUM;i++)
    {
        p[i]++;
        p[i] = p[i] % ENC_F_NUM;
    }
    
    for(i=0;i<ENC_NUM;i++)//两个编码器
    {
        k = 0;
        for(j=0;j<ENC_F_NUM-1;j++)//滤波次数
        {
            if((enc[i * ENC_F_NUM + j]) == (enc[i * ENC_F_NUM + j + 1]))//滤波匹配
            {
                k ++;
            }
        }
        if(k == ENC_F_NUM-1)//滤波全匹配
        {
            g_enc_sta[i] = enc[i * ENC_F_NUM];//更新有效编码
        }
    }
    
    //if(cnt ++ > 10)
    {
        cnt = 0;
        if(g_enc_old_sta[0] != g_enc_sta[0])
        {
            if(((g_enc_old_sta[0] == ENC_TAB[0]) && (g_enc_sta[0] == ENC_TAB[1]))||\
               ((g_enc_old_sta[0] == ENC_TAB[1]) && (g_enc_sta[0] == ENC_TAB[2]))||\
               ((g_enc_old_sta[0] == ENC_TAB[2]) && (g_enc_sta[0] == ENC_TAB[3]))||\
               ((g_enc_old_sta[0] == ENC_TAB[3]) && (g_enc_sta[0] == ENC_TAB[0])))
            {
                enc_pos[0] += ENC_MOVE_POS;
            }
            else if(((g_enc_old_sta[0] == ENC_TAB[0]) && (g_enc_sta[0] == ENC_TAB[3]))||\
                    ((g_enc_old_sta[0] == ENC_TAB[1]) && (g_enc_sta[0] == ENC_TAB[0]))||\
                    ((g_enc_old_sta[0] == ENC_TAB[2]) && (g_enc_sta[0] == ENC_TAB[1]))||\
                    ((g_enc_old_sta[0] == ENC_TAB[3]) && (g_enc_sta[0] == ENC_TAB[2])))
            {
                enc_pos[0] -= ENC_MOVE_POS;
            }
            g_enc_old_sta[0] = g_enc_sta[0];
        }
        if(g_enc_old_sta[1] != g_enc_sta[1])
        {
            if(((g_enc_old_sta[1] == ENC_TAB[0]) && (g_enc_sta[1] == ENC_TAB[1]))||\
               ((g_enc_old_sta[1] == ENC_TAB[1]) && (g_enc_sta[1] == ENC_TAB[2]))||\
               ((g_enc_old_sta[1] == ENC_TAB[2]) && (g_enc_sta[1] == ENC_TAB[3]))||\
               ((g_enc_old_sta[1] == ENC_TAB[3]) && (g_enc_sta[1] == ENC_TAB[0])))
            {
                enc_pos[1] += ENC_MOVE_POS;
            }
            else if(((g_enc_old_sta[1] == ENC_TAB[0]) && (g_enc_sta[1] == ENC_TAB[3]))||\
                    ((g_enc_old_sta[1] == ENC_TAB[1]) && (g_enc_sta[1] == ENC_TAB[0]))||\
                    ((g_enc_old_sta[1] == ENC_TAB[2]) && (g_enc_sta[1] == ENC_TAB[1]))||\
                    ((g_enc_old_sta[1] == ENC_TAB[3]) && (g_enc_sta[1] == ENC_TAB[2])))
            {
                enc_pos[1] -= ENC_MOVE_POS;
            }
            g_enc_old_sta[1] = g_enc_sta[1];
        }
    }
}

void lv_user_init(void)
{
    g_enc_sta[0] = ENC1_READ;
    g_enc_sta[1] = ENC2_READ;
    g_enc_old_sta[0] = g_enc_sta[0];
    g_enc_old_sta[1] = g_enc_sta[1];
    
    lv_init();              /* lvgl系统初始化 */
    lv_port_disp_init();    /* lvgl显示接口初始化,放在lv_init()的后面 */
    lv_port_indev_init();
    //lv_demo_widgets();
    //lv_demo_benchmark();
    //lv_demo_music();
    lv_demo_keypad_encoder();
}
