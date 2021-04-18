set(srcs
        "src/mc_bt.c"
        "src/mc_bt_gap.c")

if(CONFIG_MC_BT_A2DP_ENABLED)
    list(APPEND srcs "src/mc_bt_a2dp.c")
endif()

idf_component_register( SRCS "${srcs}"
                        INCLUDE_DIRS "inc"
                        PRIV_REQUIRES
                            # MbedCraft components
                            mc_assert
                            # ESP specifics components
                            bt
                        )


