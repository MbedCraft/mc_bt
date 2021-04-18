#pragma once

typedef enum {
    MC_BT_OK,
    MC_BT_INIT_FAILED,
} mc_bt_err_t;

mc_bt_err_t mc_bt_init(const char * const device_name);
