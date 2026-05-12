#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/sys/printk.h>

/* TX Power: +0 dBm */
#define ADV_TX_POWER 0x00

/* Advertising data — yalnız flags */
static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS,
        BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
    BT_DATA_BYTES(BT_DATA_TX_POWER, ADV_TX_POWER),
};

/* Scan response — device name */
static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_NAME_COMPLETE,
        CONFIG_BT_DEVICE_NAME,
        sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

/* Advertising parametrləri — 500ms interval */
static const struct bt_le_adv_param adv_param = {
    .options      = BT_LE_ADV_OPT_CONN,
    .interval_min = 0x0320,   /* 500ms = 800 × 0.625ms */
    .interval_max = 0x0320,
};

int main(void)
{
    int err;

    /* 1. BT stack-i işə sal — STM32-dəki aci_gap_init() ekvivalenti */
    err = bt_enable(NULL);
    if (err) {
        printk("BT enable failed (err %d)\n", err);
        return 0;
    }
    printk("Bluetooth initialized\n");

    /* 2. Advertising başlat — STM32-dəki aci_gap_set_discoverable() ekvivalenti */
    err = bt_le_adv_start(&adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err) {
        printk("Advertising failed (err %d)\n", err);
        return 0;
    }
    printk("Advertising started: %s\n", CONFIG_BT_DEVICE_NAME);

    /* 3. Sonsuz gözlə — heç bir service, heç bir connection handling yoxdur */
    while (1) {
        k_sleep(K_FOREVER);
    }

    return 0;
}