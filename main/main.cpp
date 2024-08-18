#include <stdio.h>
#include <esp_err.h>
#include <slint-esp.h>
#include <esp_lcd_types.h>

#if defined(BSP_LCD_DRAW_BUFF_SIZE)
#    define DRAW_BUF_SIZE BSP_LCD_DRAW_BUFF_SIZE
#else
#    define DRAW_BUF_SIZE (BSP_LCD_H_RES * CONFIG_BSP_LCD_DRAW_BUF_HEIGHT)
#endif

//#include "main.h"

extern "C" void app_main(void)
{
//    /* Initialize display  */
//    esp_lcd_panel_io_handle_t io_handle = NULL;
//    esp_lcd_panel_handle_t panel_handle = NULL;
//    const bsp_display_config_t bsp_disp_cfg = {
//            .max_transfer_sz = DRAW_BUF_SIZE * sizeof(uint16_t),
//    };
//    bsp_display_new(&bsp_disp_cfg, &panel_handle, &io_handle);
//
//    /* Set display brightness to 100% */
//    bsp_display_backlight_on();
//
//    /* Initialize touch */
//    esp_lcd_touch_handle_t touch_handle = NULL;
//    const bsp_uitouch_config_t bsp_touch_cfg = {};
//    bsp_touch_new(&bsp_touch_cfg, &touch_handle);
//
//    /* Allocate a drawing buffer */
//    static std::vector<slint::platform::Rgb565Pixel> buffer(BSP_LCD_H_RES * BSP_LCD_V_RES);
//
//    /* Initialize Slint's ESP platform support*/
//    slint_esp_init(SlintPlatformConfiguration {
//            .size = slint::PhysicalSize({ BSP_LCD_H_RES, BSP_LCD_V_RES }),
//            .panel_handle = panel_handle,
//            .touch_handle = touch_handle,
//            .buffer1 = buffer,
//            .color_swap_16 = true });
//
//    /* Instantiate the UI */
//    auto ui = AppWindow::create();
//    /* Show it on the screen and run the event loop */
//    ui->run();
}