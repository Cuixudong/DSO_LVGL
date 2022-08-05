#include "lv_user.h"
#include "lcd.h"
#include "demos/lv_demos.h"

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

    /* 设置显示设备的分辨率
     * 这里为了适配正点原子的多款屏幕，采用了动态获取的方式，
     * 在实际项目中，通常所使用的屏幕大小是固定的，因此可以直接设置为屏幕的大小 */
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
    lv_img_set_src(mouse_cursor, LV_SYMBOL_HOME);
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
    (*x) = ((lv_tick_get() / 20) % 200) + 140;
    (*y) = ((lv_tick_get() / 30) % 200) + 60;
    #endif
}

void lv_user_init(void)
{
    lv_init();              /* lvgl系统初始化 */
    lv_port_disp_init();    /* lvgl显示接口初始化,放在lv_init()的后面 */
    lv_port_indev_init();
    lv_demo_widgets();
    //lv_demo_benchmark();
    //lv_demo_music();
}
