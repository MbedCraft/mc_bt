/* ------------------------------------------------------------------------- *\
 * Standard Includes
 * ------------------------------------------------------------------------- */
#include <stdio.h>

/* ------------------------------------------------------------------------- *\
 * Espressif specific includes
 * ------------------------------------------------------------------------- */
#include "esp_bt.h"
#include "esp_bt_device.h"
#include "esp_bt_main.h"
#include "esp_log.h"

/* ------------------------------------------------------------------------- *\
 * MbedCraft includes
 * ------------------------------------------------------------------------- */
#include "mc_assert.h"
#include "mc_bt.h"
#include "mc_bt_a2dp.h"
#include "mc_bt_gap.h"

/* ------------------------------------------------------------------------- *\
 * Type definitions
 * ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- *\
 * Private variable definitions
 * ------------------------------------------------------------------------- */
const char * __device_name;

/* ------------------------------------------------------------------------- *\
 * Private function definitions
 * ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- *\
 * Public function implementations
 * ------------------------------------------------------------------------- */
mc_bt_err_t mc_bt_init(const char * const device_name) {
    esp_err_t esp_err;
    mc_bt_err_t mc_bt_err;

    __device_name = device_name;

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    esp_err = esp_bt_controller_init(&bt_cfg);
    ASSERTE_RET(ESP_OK == esp_err, MC_BT_INIT_FAILED,
            "initialize controller failed: %s", esp_err_to_name(esp_err));

    esp_err = esp_bt_controller_enable(bt_cfg.mode);
    ASSERTE_RET(ESP_OK == esp_err, MC_BT_INIT_FAILED,
            "enable controller failed: %s", esp_err_to_name(esp_err));

    esp_err = esp_bluedroid_init();
    ASSERTE_RET(ESP_OK == esp_err, MC_BT_INIT_FAILED,
            "initialize bluedroid failed: %s", esp_err_to_name(esp_err));

    esp_err = esp_bluedroid_enable();
    ASSERTE_RET(ESP_OK == esp_err, MC_BT_INIT_FAILED,
            "enable bluedroid failed: %s", esp_err_to_name(esp_err));

    /* Bluetooth device name, connection mode and profile set up */
    esp_bt_dev_set_device_name(__device_name);

    mc_bt_err = mc_bt_gap_init();
    ASSERTE_RET(MC_BT_OK == mc_bt_err, MC_BT_INIT_FAILED,
            "MbedCraft GAP initialization failed");

#if CONFIG_MC_BT_A2DP_ENABLED
    mc_bt_err = mc_bt_a2dp_init();
    ASSERTE_RET(MC_BT_OK == mc_bt_err, MC_BT_INIT_FAILED,
            "MbedCraft A2DP initialization failed");
#endif

    return MC_BT_OK;
}
