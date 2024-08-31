#define SLINT_DEBUG_PERFORMANCE "console,overlay"
#define SLINT_STYLE "material"
#include <stdio.h>
#include <esp_err.h>
#include <slint-esp.h>
#include <esp_lcd_types.h>
#include <vector>
#include <driver/spi_common.h>
#include <esp_lcd_panel_io.h>
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_gc9a01.h"
#include "driver/gpio.h"

#if defined(BSP_LCD_DRAW_BUFF_SIZE)
#    define DRAW_BUF_SIZE BSP_LCD_DRAW_BUFF_SIZE
#else
#    define DRAW_BUF_SIZE (BSP_LCD_H_RES * CONFIG_BSP_LCD_DRAW_BUF_HEIGHT)
#endif

#include "mainwindow.h"

#define EXAMPLE_LCD_H_RES              240
#define EXAMPLE_LCD_V_RES              240

#define TAG "aaa"
#define LCD_HOST  SPI2_HOST

#define TFT_SCLK 4
#define TFT_MOSI 5
#define TFT_CS 7
#define TFT_DC 6
#define TFT_RST -1
#define TFT_MISO 7


extern "C" void app_main(void)
{
    gpio_set_direction(GPIO_NUM_8, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_8, 1);

//    ESP_LOGI(TAG, "Initialize SPI bus");
//    const spi_bus_config_t bus_config = GC9A01_PANEL_BUS_SPI_CONFIG(TFT_SCLK, TFT_MOSI,
//                                                                    EXAMPLE_LCD_H_RES * 80 * sizeof(uint16_t));

    const spi_bus_config_t bus_config = {
            .mosi_io_num = TFT_MOSI,
            .miso_io_num = -1,
            .sclk_io_num = TFT_SCLK,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = EXAMPLE_LCD_H_RES * 80 * sizeof(uint16_t)
    };


    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &bus_config, SPI_DMA_CH_AUTO));

//    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    const esp_lcd_panel_io_spi_config_t io_config = GC9A01_PANEL_IO_SPI_CONFIG(TFT_CS, TFT_DC, NULL, NULL);
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

/**
 * Uncomment these lines if use custom initialization commands.
 * The array should be declared as "static const" and positioned outside the function.
 */
// static const gc9a01_lcd_init_cmd_t lcd_init_cmds[] = {
// //  {cmd, { data }, data_size, delay_ms}
//     {0xfe, (uint8_t []){0x00}, 0, 0},
//     {0xef, (uint8_t []){0x00}, 0, 0},
//     {0xeb, (uint8_t []){0x14}, 1, 0},
//     ...
// };

//    ESP_LOGI(TAG, "Install GC9A01 panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    // gc9a01_vendor_config_t vendor_config = {  // Uncomment these lines if use custom initialization commands
    //     .init_cmds = lcd_init_cmds,
    //     .init_cmds_size = sizeof(lcd_init_cmds) / sizeof(gc9a01_lcd_init_cmd_t),
    // };
    const esp_lcd_panel_dev_config_t panel_config = {
            .reset_gpio_num = -1,      // Set to -1 if not use
//#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
//            .color_space = ESP_LCD_COLOR_SPACE_RGB,
//#else
            .rgb_endian = LCD_RGB_ENDIAN_RGB,
//#endif
            .bits_per_pixel = 16,                           // Implemented by LCD command `3Ah` (16/18)
            // .vendor_config = &vendor_config,            // Uncomment this line if use custom initialization commands
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_gc9a01(io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
//#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
//    ESP_ERROR_CHECK(esp_lcd_panel_disp_off(panel_handle, false));
//#else
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
//#endif

    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));
    /* Allocate a drawing buffer */
    std::vector<slint::platform::Rgb565Pixel> buffer(EXAMPLE_LCD_H_RES * EXAMPLE_LCD_H_RES);

    /* Initialize Slint's ESP platform support*/
    slint_esp_init(SlintPlatformConfiguration {
            .size = slint::PhysicalSize({ EXAMPLE_LCD_H_RES, EXAMPLE_LCD_H_RES }),
            .panel_handle = panel_handle,
//            .touch_handle = touch_handle,
            .buffer1 = buffer,

            .color_swap_16 = false });


    /* Instantiate the UI */
    auto ui = MainWindow::create();

    /* Show it on the screen and run the event loop */
    ui->run();
}