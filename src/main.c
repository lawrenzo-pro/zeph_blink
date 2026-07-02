#include "zephyr/sys/printk.h"
#include "zephyr/sys/util_macro.h"
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#define DURATION 1000
#define LED_NODE0 DT_ALIAS(led0)
#define LED_NODE1 DT_ALIAS(led1)
#define INT_LED DT_ALIAS(led2)
#define BTN_NODE DT_ALIAS(btn0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE0, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED_NODE1, gpios);
static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(BTN_NODE, gpios);
static const struct gpio_dt_spec int_led = GPIO_DT_SPEC_GET(INT_LED, gpios);
static struct gpio_callback callback_data;
void btn_handler(const struct device *port, struct gpio_callback *cb,gpio_port_pins_t pins){
    int ret;
    ret = gpio_pin_toggle_dt(&int_led);
    if(ret<0){
        printk("oops!");
        return;
    }
 //do sth later!!!
}
int main(void) {
    int ret;
    bool led_state = true;
    if(!(gpio_is_ready_dt(&led) && gpio_is_ready_dt(&led1) && gpio_is_ready_dt(&btn)) && ! gpio_is_ready_dt(&int_led)){
        return 0;
    }
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if(ret < 0){
        return 0;
    }
    ret = gpio_pin_configure_dt(&int_led, GPIO_OUTPUT_ACTIVE);
    if(ret < 0){
        return 0;
    }
    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    if(ret < 0){
        return 0;
    }
    ret = gpio_pin_configure_dt(&btn, GPIO_INPUT);
    if(ret < 0){
        return 0;
    }
    ret =gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_EDGE_TO_ACTIVE);
    if(ret < 0){
        return 0;
    }
    gpio_init_callback(&callback_data, btn_handler, BIT(btn.pin));
    gpio_add_callback(btn.port, &callback_data);

    while(1){
        ret = gpio_pin_toggle_dt(&led);
        if(ret < 0){
            return 0;
        }
        ret = gpio_pin_toggle_dt(&led1);
        if(ret < 0){
            return 0;
        }
        led_state = !led_state;
        printf("led state: %d \n", led_state);
        k_msleep(DURATION);
    }
    return 0;
}