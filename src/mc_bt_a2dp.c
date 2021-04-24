/* ------------------------------------------------------------------------- *\
 * Espressif specific includes
 * ------------------------------------------------------------------------- */
#include "esp_gap_bt_api.h"
#include "esp_log.h"

/* ------------------------------------------------------------------------- *\
 * MbedCraft includes
 * ------------------------------------------------------------------------- */
#include "mc_bt_a2dp.h"

/* ------------------------------------------------------------------------- *\
 * Private variable definitions
 * ------------------------------------------------------------------------- */
static mc_bt_a2dp_connection_cb_t __connection_cb = NULL;

/* ------------------------------------------------------------------------- *\
 * Private variable definitions
 * ------------------------------------------------------------------------- */
static const char *s_a2d_conn_state_str[] = {
    "Disconnected",
    "Connecting",
    "Connected",
    "Disconnecting"
};

static const char *s_a2d_audio_state_str[] = {
    "Suspended",
    "Stopped",
    "Started"
};

static mc_bt_a2dp_config_cb_t __config_cb = NULL;

/* ------------------------------------------------------------------------- *\
 * Private function definitions
 * ------------------------------------------------------------------------- */
static void __a2dp_cb(esp_a2d_cb_event_t event, esp_a2d_cb_param_t *param);

/* ------------------------------------------------------------------------- *\
 * Public function implementations
 * ------------------------------------------------------------------------- */
mc_bt_err_t mc_bt_a2dp_init(void) {
    /* initialize A2DP sink */
    esp_a2d_register_callback(&__a2dp_cb);
    esp_a2d_sink_init();

   return MC_BT_OK;
}

void mc_bt_a2dp_register_connection_cb(mc_bt_a2dp_connection_cb_t callback) {
    __connection_cb = callback;
}

void mc_bt_a2dp_register_configuration_cb(mc_bt_a2dp_config_cb_t callback) {
    __config_cb = callback;
}

void mc_bt_a2dp_register_data_cb(esp_a2d_sink_data_cb_t callback) {
    esp_a2d_sink_register_data_callback(callback);
}

/* ------------------------------------------------------------------------- *\
 * Private function implementations
 * ------------------------------------------------------------------------- */
static void __a2dp_cb(esp_a2d_cb_event_t event, esp_a2d_cb_param_t *param) {
    uint8_t *bda = param->conn_stat.remote_bda;

    ESP_LOGD(__func__, "%s evt %d", __func__, event);
    switch (event) {
        case ESP_A2D_CONNECTION_STATE_EVT:
            ESP_LOGI(__func__, "A2DP connection state: %s, [%02x:%02x:%02x:%02x:%02x:%02x]",
                    s_a2d_conn_state_str[param->conn_stat.state], bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]);
            if (param->conn_stat.state == ESP_A2D_CONNECTION_STATE_DISCONNECTED) {
                if (NULL != __connection_cb) {
                    __connection_cb(false);
                }
                esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
            } else if (param->conn_stat.state == ESP_A2D_CONNECTION_STATE_CONNECTED){
                if (NULL != __connection_cb) {
                    __connection_cb(true);
                }
                esp_bt_gap_set_scan_mode(ESP_BT_NON_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
            }
            break;
        case ESP_A2D_AUDIO_STATE_EVT:
            ESP_LOGI(__func__, "A2DP audio state: %s", s_a2d_audio_state_str[param->audio_stat.state]);
            break;
        case ESP_A2D_AUDIO_CFG_EVT:
            ESP_LOGI(__func__, "A2DP audio stream configuration, codec type %d", param->audio_cfg.mcc.type);
            if (NULL != __config_cb) {
                __config_cb(&param->audio_cfg.mcc);
            }
            break;
        default:
            ESP_LOGE(__func__, "%s unhandled evt %d", __func__, event);
            break;
    }
}
