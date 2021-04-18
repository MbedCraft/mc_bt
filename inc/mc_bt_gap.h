#pragma once

#include "mc_bt.h"

typedef bool (* mc_bt_gap_cfm_req_cb_t)(uint32_t num_val);

mc_bt_err_t mc_bt_gap_init(void);
void mc_bt_gap_register_cfm_req_cb(mc_bt_gap_cfm_req_cb_t callback);
