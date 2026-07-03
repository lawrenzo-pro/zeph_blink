#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#define DURATION 1000

#define LED_NODE0 DT_ALIAS(led0)
#define LED_NODE1 DT_ALIAS(led1)
#define INT_LED DT_ALIAS(led2)
#define BTN_NODE DT_ALIAS(btn0)
#define STACK_SIZE 500
K_THREAD_STACK_DEFINE(my_stack,STACK_SIZE)
K_THREAD_STACK_DEFINE(my_second_stack, STACK_SIZE);
struct k_thread my_thread;
struct k_thread my_thread2;
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
void init(){
      int ret;
    if(!(gpio_is_ready_dt(&led) && gpio_is_ready_dt(&led1) && gpio_is_ready_dt(&btn)) && ! gpio_is_ready_dt(&int_led)){
        return;
    }
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if(ret < 0){
        return;
    }
    ret = gpio_pin_configure_dt(&int_led, GPIO_OUTPUT_ACTIVE);
    if(ret < 0){
        return;
    }
    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
    if(ret < 0){
        return;
    }
    ret = gpio_pin_configure_dt(&btn, GPIO_INPUT);
    if(ret < 0){
        return;
    }
    ret =gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_EDGE_TO_ACTIVE);
    if(ret < 0){
        return;
    }
    gpio_init_callback(&callback_data, btn_handler, BIT(btn.pin));
    gpio_add_callback(btn.port, &callback_data);
}
void blink_func(void *p1,void *p2,void *p3){
    while(1){
        gpio_pin_toggle_dt(&led);
        k_msleep(500);
    } 
}
void blink_func_two(void *p1,void *p2,void *p3){
    while(1){
        gpio_pin_toggle_dt(&led1);
        k_msleep(450);
    }
}
int main(void) {
    init();
    k_tid_t blink_thread= k_thread_create(&my_thread,my_stack,
        K_THREAD_STACK_SIZEOF(my_stack),
    blink_func,NULL,NULL,NULL, 5,0,K_NO_WAIT);
    k_tid_t blink_thread2 = k_thread_create(&my_thread2,my_second_stack,K_THREAD_STACK_SIZEOF(my_second_stack), 
    blink_func_two, NULL, NULL, NULL, 5, 0, K_NO_WAIT);
   
    k_thread_join(blink_thread, K_FOREVER);
    k_thread_join(blink_thread2, K_FOREVER);
    k_thread_stack_free(my_stack);
    k_thread_stack_free(my_second_stack);
    return 0;
}