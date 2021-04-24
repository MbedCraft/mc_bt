#pragma once

/* ------------------------------------------------------------------------- *\
 * Espressif specific includes
 * ------------------------------------------------------------------------- */
#include "esp_a2dp_api.h"

/* ------------------------------------------------------------------------- *\
 * MbedCraft includes
 * ------------------------------------------------------------------------- */
#include "mc_bt.h"

#if CONFIG_MC_BT_A2DP_ENABLED

typedef void (* mc_bt_a2dp_config_cb_t)(esp_a2d_mcc_t *mcc);
typedef void (* mc_bt_a2dp_connection_cb_t)(bool connected);

mc_bt_err_t mc_bt_a2dp_init(void);
void mc_bt_a2dp_register_connection_cb(mc_bt_a2dp_connection_cb_t callback);
void mc_bt_a2dp_register_configuration_cb(mc_bt_a2dp_config_cb_t callback);
void mc_bt_a2dp_register_data_cb(esp_a2d_sink_data_cb_t callback);

#endif
