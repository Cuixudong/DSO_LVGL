#include "lv_user.h"
#include "lcd.h"
#include "demos/lv_demos.h"

/* ��ʾ�豸��ʼ������ */
static void disp_init(void);

/* ��ʾ�豸ˢ�º��� */
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

/**
 * @brief       LCD���ٻ��ƺ���
 * @param       (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex - sx + 1) * (ey - sy + 1)
 * @param       color:Ҫ������ɫ
 * @retval      ��
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
 * @brief       ��ʼ����ע����ʾ�豸
 * @param       ��
 * @retval      ��
 */
void lv_port_disp_init(void)
{
    /*-------------------------
     * ��ʼ����ʾ�豸
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * ����һ����ͼ������
     *----------------------------*/

    /**
     * LVGL ��Ҫһ����������������С����
     * �����������������ݻ�ͨ����ʾ�豸�� `flush_cb`(��ʾ�豸ˢ�º���) ���Ƶ���ʾ�豸��
     * ����������Ĵ�С��Ҫ������ʾ�豸һ�еĴ�С
     *
     * ������3�л�������:
     * 1. ��������:
     *      LVGL �Ὣ��ʾ�豸�����ݻ��Ƶ����������д����ʾ�豸��
     *
     * 2. ˫������:
     *      LVGL �Ὣ��ʾ�豸�����ݻ��Ƶ�����һ����������������д����ʾ�豸��
     *      ��Ҫʹ�� DMA ��Ҫ��ʾ����ʾ�豸������д�뻺������
     *      �����ݴӵ�һ������������ʱ������ʹ LVGL �ܹ�����Ļ����һ���ֻ��Ƶ���һ����������
     *      ����ʹ����Ⱦ��ˢ�¿��Բ���ִ�С�
     *
     * 3. ȫ�ߴ�˫������
     *      ����������Ļ��С��ȫ�ߴ绺�������������� disp_drv.full_refresh = 1��
     *      ������LVGL��ʼ���� 'flush_cb' ����ʽ�ṩ������Ⱦ��Ļ����ֻ�����֡�������ĵ�ַ��
     */

    /* ��������ʾ��) */
    static lv_disp_draw_buf_t draw_buf_dsc_1;

    static lv_color_t buf_1[MY_DISP_HOR_RES * 50];                                              /* ���û������Ĵ�СΪ 10 ����Ļ�Ĵ�С */
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 50);                  /* ��ʼ����ʾ������ */

    /* ˫������ʾ��) */
//    static lv_disp_draw_buf_t draw_buf_dsc_2;
//    static lv_color_t buf_2_1[MY_DISP_HOR_RES * 10];                                            /* ���û������Ĵ�СΪ 10 ����Ļ�Ĵ�С */
//    static lv_color_t buf_2_2[MY_DISP_HOR_RES * 10];                                            /* ������һ���������Ĵ�СΪ 10 ����Ļ�Ĵ�С */
//    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 10);             /* ��ʼ����ʾ������ */

    /* ȫ�ߴ�˫������ʾ��) �������������� disp_drv.full_refresh = 1 */
//    static lv_disp_draw_buf_t draw_buf_dsc_3;
//    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];                               /* ����һ��ȫ�ߴ�Ļ����� */
//    static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];                               /* ������һ��ȫ�ߴ�Ļ����� */
//    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2, MY_DISP_HOR_RES * MY_DISP_VER_RES);/* ��ʼ����ʾ������ */

    /*-----------------------------------
     * �� LVGL ��ע����ʾ�豸
     *----------------------------------*/

    static lv_disp_drv_t disp_drv;                  /* ��ʾ�豸�������� */
    lv_disp_drv_init(&disp_drv);                    /* ��ʼ��ΪĬ��ֵ */

    /* ����������ʾ�豸�ĺ���  */

    /* ������ʾ�豸�ķֱ���
     * ����Ϊ����������ԭ�ӵĶ����Ļ�������˶�̬��ȡ�ķ�ʽ��
     * ��ʵ����Ŀ�У�ͨ����ʹ�õ���Ļ��С�ǹ̶��ģ���˿���ֱ������Ϊ��Ļ�Ĵ�С */
    disp_drv.hor_res = lcddev.width;
    disp_drv.ver_res = lcddev.height;
    /* ֧������ֻ� */
    disp_drv.sw_rotate = 1;
    /* �����������������ݸ��Ƶ���ʾ�豸 */
    disp_drv.flush_cb = disp_flush;

    /* ������ʾ������ */
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /* ȫ�ߴ�˫������ʾ��)*/
    //disp_drv.full_refresh = 1

    /* �������GPU����ʹ����ɫ����ڴ�����
     * ע�⣬������� lv_conf.h ��ʹ�� LVGL ����֧�ֵ� GPUs
     * ��������в�ͬ�� GPU����ô����ʹ������ص������� */
    //disp_drv.gpu_fill_cb = gpu_fill;

    /* ע����ʾ�豸 */
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * @brief       ��ʼ����ʾ�豸�ͱ�Ҫ����Χ�豸
 * @param       ��
 * @retval      ��
 */
static void disp_init(void)
{
    /*You code here*/
}

/**
 * @brief       ���ڲ�������������ˢ�µ���ʾ���ϵ��ض�����
 *   @note      ����ʹ�� DMA �����κ�Ӳ���ں�̨����ִ���������
 *              ���ǣ���Ҫ��ˢ����ɺ���ú��� 'lv_disp_flush_ready()'
 *
 * @param       disp_drv    : ��ʾ�豸
 *   @arg       area        : Ҫˢ�µ����򣬰����������εĶԽ�����
 *   @arg       color_p     : ��ɫ����
 *
 * @retval      ��
 */
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /* LVGL �ٷ�������һ�����ˢ����Ļ�����ӣ������Ч�������Ч�� */

//    int32_t x;
//    int32_t y;
//    for(y = area->y1; y <= area->y2; y++) {
//        for(x = area->x1; x <= area->x2; x++) {
//            /*Put a pixel to the display. For example:*/
//            /*put_px(x, y, *color_p)*/
//            color_p++;
//        }
//    }

//    /* ��ָ�����������ָ����ɫ�� */
//    lcd_color_fill(area->x1, area->y1, area->x2, area->y2, (uint16_t *)color_p);
    lcd_draw_fast_rgb_color(area->x1,area->y1,area->x2,area->y2,(uint16_t*)color_p);

    /* ��Ҫ!!!
     * ֪ͨͼ�ο⣬�Ѿ�ˢ������� */
    lv_disp_flush_ready(disp_drv);
}

static void mouse_init(void);
static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool mouse_is_pressed(void);
static void mouse_get_xy(lv_coord_t * x, lv_coord_t * y);

lv_indev_t * indev_mouse;       // ���
lv_indev_t * indev_keypad;      // ����

/**
 * @brief       ��ʼ����ע�������豸
 * @param       ��
 * @retval      ��
 */
void lv_port_indev_init(void)
{
    /**
     * 
     * ������������ҵ� LittlevGL ֧�ֵĳ����豸��ʵ��ʾ��:
     *  - ������
     *  - ��� (֧�ֹ��)
     *  - ���� (��֧�ְ����� GUI �÷�)
     *  - ������ (֧�ֵ� GUI �÷�������: ��, ��, ����)
     *  - ��ť (������Ļ��ָ������ⲿ��ť)
     *
     *  ���� `..._read()` ֻ��ʾ��
     *  ����Ҫ���ݾ����Ӳ���������Щ����
     */

    static lv_indev_drv_t indev_drv;

    /*------------------
     * ���
     * -----------------*/

    /* ��ʼ�����(�����) */
    mouse_init();

    /* ע����������豸 */
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = mouse_read;
    indev_mouse = lv_indev_drv_register(&indev_drv);

    /* ���ù�꣬Ϊ�˼��������������Ϊһ�� HOME ���� */
    lv_obj_t * mouse_cursor = lv_img_create(lv_scr_act());
    lv_img_set_src(mouse_cursor, LV_SYMBOL_HOME);
    lv_indev_set_cursor(indev_mouse, mouse_cursor);

    /*------------------
     * ����
     * -----------------*/

//    /* ��ʼ������(�����) */
//    keypad_init();

//    /* ע����������豸 */
//    lv_indev_drv_init(&indev_drv);
//    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
//    indev_drv.read_cb = keypad_read;
//    indev_keypad = lv_indev_drv_register(&indev_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * ���
 * -----------------*/

/**
 * @brief       ��ʼ�����
 * @param       ��
 * @retval      ��
 */
static void mouse_init(void)
{
    /*Your code comes here*/
}

/**
 * @brief       ͼ�ο������ȡ�ص�����
 * @param       indev_drv   : ����豸
 *   @arg       data        : �����豸���ݽṹ��
 * @retval      ��
 */
static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    /* ��ȡ��ǰ�� x��y ���� */
    mouse_get_xy(&data->point.x, &data->point.y);

    /* ��ȡ�Ƿ��»��ͷ���갴ť */
    if(mouse_is_pressed()) {
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

/**
 * @brief       ��ȡ����豸�Ƿ񱻰���
 * @param       ��
 * @retval      ��������豸�Ƿ񱻰���
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
 * @brief       ����걻����ʱ����ȡ���� x��y ����
 * @param       x   : x�����ָ��
 *   @arg       y   : y�����ָ��
 * @retval      ��
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
    lv_init();              /* lvglϵͳ��ʼ�� */
    lv_port_disp_init();    /* lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ��� */
    lv_port_indev_init();
    lv_demo_widgets();
    //lv_demo_benchmark();
    //lv_demo_music();
}
