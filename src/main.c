#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#define DURATION 1000
#define LED_NODE0 DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE0, gpios);
int main(void) {
    int ret;
    bool led_state = true;
    if(!gpio_is_ready_dt(&led)){
        return 0;
    }
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if(ret < 0){
        return 0;
    }
    while(1){
        ret = gpio_pin_toggle_dt(&led);
        if(ret < 0){
            return 0;
        }
        led_state = !led_state;
        printf("led state: %d \n", led_state);
        k_msleep(DURATION);
    }
    return 0;
}