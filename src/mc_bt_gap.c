/* ------------------------------------------------------------------------- *\
 * Espressif specific includes
 * ------------------------------------------------------------------------- */
#include "esp_gap_bt_api.h"
#include "esp_log.h"

/* ------------------------------------------------------------------------- *\
 * MbedCraft includes
 * ------------------------------------------------------------------------- */
#include "mc_bt_gap.h"

static mc_bt_gap_cfm_req_cb_t __cfm_req_cb = NULL;

/* ------------------------------------------------------------------------- *\
 * Private function definitions
 * ------------------------------------------------------------------------- */
static void __gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);

/* ------------------------------------------------------------------------- *\
 * Public function implementations
 * ------------------------------------------------------------------------- */
mc_bt_err_t mc_bt_gap_init(void) {
    esp_bt_gap_register_callback(__gap_cb);

    /* set discoverable and connectable mode, wait to be connected */
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);

    /* Set default parameters for Secure Simple Pairing */
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_IO;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));

    /*
     * Set default parameters for Legacy Pairing
     * Use fixed pin code
     */
    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_FIXED;
    esp_bt_pin_code_t pin_code;
    pin_code[0] = '1';
    pin_code[1] = '2';
    pin_code[2] = '3';
    pin_code[3] = '4';
    esp_bt_gap_set_pin(pin_type, 4, pin_code);

    return MC_BT_OK;
}

void mc_bt_gap_register_cfm_req_cb(mc_bt_gap_cfm_req_cb_t callback) {
    __cfm_req_cb = callback;
}

/* ------------------------------------------------------------------------- *\
 * Private function implementations
 * ------------------------------------------------------------------------- */
static void __gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    switch (event) {
        case ESP_BT_GAP_AUTH_CMPL_EVT:
            if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS) {
                ESP_LOGI(__func__, "authentication success: %s", param->auth_cmpl.device_name);
                esp_log_buffer_hex(__func__, param->auth_cmpl.bda, ESP_BD_ADDR_LEN);
            } else {
                ESP_LOGE(__func__, "authentication failed, status:%d", param->auth_cmpl.stat);
            }
            break;

#if (CONFIG_BT_SSP_ENABLED == true)
        case ESP_BT_GAP_CFM_REQ_EVT:
            if (NULL != __cfm_req_cb) {
                bool accept = __cfm_req_cb(param->cfm_req.num_val);
                esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, accept);
            } else {
                esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, false);
            }
            break;
        case ESP_BT_GAP_KEY_NOTIF_EVT:
            ESP_LOGI(__func__, "ESP_BT_GAP_KEY_NOTIF_EVT passkey:%d", param->key_notif.passkey);
            break;
        case ESP_BT_GAP_KEY_REQ_EVT:
            ESP_LOGI(__func__, "ESP_BT_GAP_KEY_REQ_EVT Please enter passkey!");
            break;
#endif

        default:
            ESP_LOGI(__func__, "event: %d", event);
            break;
    }
    return;
}

