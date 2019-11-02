#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/adc.h"

#define ECHO_TEST_TXD  (GPIO_NUM_4)
#define ECHO_TEST_RXD  (GPIO_NUM_5)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE (1024)


adc2_channel_t pins2[] = {ADC2_CHANNEL_4, ADC2_CHANNEL_5, ADC2_CHANNEL_6, ADC2_CHANNEL_7, ADC2_CHANNEL_9, ADC2_CHANNEL_8};
adc1_channel_t pins1[] = {ADC1_CHANNEL_5, ADC1_CHANNEL_4, ADC1_CHANNEL_7, ADC1_CHANNEL_6, ADC1_CHANNEL_3, ADC1_CHANNEL_0};


static void read_sensors(){
  for(int i=0;i<6;i++){
    adc2_config_channel_atten( pins2[i], ADC_ATTEN_11db );
    adc1_config_channel_atten( pins1[i], ADC_ATTEN_11db );
  }
  adc1_config_width(ADC_WIDTH_BIT_12);

  int read_raw;
  while(1){
    for(int i=0;i<12;i++){
      if(i<6){
        esp_err_t r = adc2_get_raw( pins2[i], ADC_WIDTH_12Bit, &read_raw);
        if ( r == ESP_OK ) {
            read_raw = read_raw;
        } else if ( r == ESP_ERR_TIMEOUT ) {
            read_raw = -1;
        }
      }else{
        read_raw = adc1_get_raw(pins1[i-6]);
      }

      printf("%d%s", read_raw,i<11?", ":"\n");
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void echo_task(){
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    while (1) {
        // Read data from the UART
        int len = uart_read_bytes(UART_NUM_1, data, BUF_SIZE, 20 / portTICK_RATE_MS);
        // Write data back to the UART
        uart_write_bytes(UART_NUM_1, (const char *) data, len);
    }
}

extern "C" void app_main(){
  //xTaskCreate(read_sensors, "read_sensors", 1024, NULL, 10, NULL);
  xTaskCreate(&echo_task, "uart_echo_task", 1024, NULL, 10, NULL);
}
