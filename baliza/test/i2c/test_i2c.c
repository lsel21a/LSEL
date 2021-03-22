#include "unity.h"
#include "drivers.h"
#include "mock_stdio.h"
#include "mock_driver/esp_i2c.h"
#include "mock_freertos/FreeRTOS.h"

#define True 1
#define False 0

void setUp{

}

void tearDown{

}

void test_checkrslt_ReturnOKWhenRstlrIsESP_OK(){
    printf_ExpectAnyArgsAndReturn(True);

    check_rslt(ESP_OK);
}

void test_i2c_init(){

    i2c_param_config_ExpectAnyArgsAndReturn(True);
    TEST_ASSERT(i2c_init() == ESP_OK);

}

void test_i2c_detect(){

    i2c_param_config_ExpectAnyArgsAndReturn(True);

    i2c_driver_install_ExpectAnyArgsAndReturn(ESP_OK);
    printf_IgnoreAndReturn(True);
    fflush_IgnoreAndReturn(True);
    i2c_cmd_link_create_IgnoreAndReturn(0);
    i2c_master_start_IgnoreAndReturn(ESP_OK);
    i2c_master_write_byte_IgnoreAndReturn(ESP_OK);
    i2c_master_write_stop_IgnoreAndReturn(ESP_OK);
    i2c_master_cmd_begin_IgnoreAndReturn(ESP_OK);
    i2c_cmd_link_delete_IgnoreAndReturn(0);
    i2c_driver_delete_IgnoreAndReturn(0);

    TEST_ASSERT(i2c_detect() == ESP_OK);
}

void test_i2c_send_data_ReturnOK(){

    i2c_param_config_ExpectAnyArgsAndReturn(True);

    i2c_driver_install_ExpectAnyArgsAndReturn(ESP_OK);
    i2c_cmd_link_create_ExpectAnyArgsAndReturn(0);
    i2c_master_start_ExpectAnyArgsAndReturn(ESP_OK);
    i2c_master_write_byte_ExpectAnyArgsAndReturn(ESP_OK);
    i2c_master_write_ExpectAnyArgsAndReturn(ESP_OK);
    i2c_master_stop_ExpectAnyArgsAndReturn(ESP_OK);
    i2c_master_cmd_begin_ExpectAnyArgsAndReturn(ESP_OK);
    i2c_cmd_link_delete_ExpectAnyArgsAndReturn(0);
    i2c_driver_delete_ExpectAnyArgsAndReturn(0);

    uint8_t addr = 0x76;
    uint8_t data = 0x25;

    TEST_ASSERT(i2c_send_data(addr, &data, sizeof(data), 10) == ESP_OK);
}

void test_i2c_send_data_InformsErrorWhenSomethingFails(){

    i2c_param_config_ExpectAnyArgsAndReturn(True);

    i2c_driver_install_ExpectAnyArgsAndReturn(ESP_FAIL);
    printf_ExpectAnyArgsAndReturn(True);
    i2c_cmd_link_create_ExpectAnyArgsAndReturn(0);
    i2c_master_start_ExpectAnyArgsAndReturn(ESP_FAIL);
    printf_ExpectAnyArgsAndReturn(True);
    i2c_master_write_byte_ExpectAnyArgsAndReturn(ESP_FAIL);
    printf_ExpectAnyArgsAndReturn(True);
    i2c_master_write_IgnoreAndReturn(ESP_OK);
    i2c_master_stop_ExpectAnyArgsAndReturn(ESP_FAIL);
    printf_ExpectAnyArgsAndReturn(True);
    i2c_master_cmd_begin_ExpectAnyArgsAndReturn(ESP_FAIL);
    printf_ExpectAnyArgsAndReturn(True);
    i2c_cmd_link_delete_ExpectAnyArgsAndReturn(0);
    i2c_driver_delete_ExpectAnyArgsAndReturn(0);

    uint8_t addr = 0x76;
    uint8_t data = 0x25;

    i2c_send_data(addr, &data, sizeof(data), 10);
}

void test_i2c_recv_dataReturnOK(){
    
    i2c_param_config_ExpectAnyArgsAndReturn(True);

    i2c_driver_install_ExpectAnyArgsAndReturn(ESP_OK);
    i2c_cmd_link_create_ExpectAnyArgsAndReturn(0);
    i2c_master_start_ExpectAnyArgsAndReturn(ESP_OK);
    i2c_master_write_byte_ExpectAnyArgsAndReturn(ESP_OK);
    i2c_master_read_IgnoreAndReturn(ESP_OK);
    i2c_master_stop_ExpectAnyArgsAndReturn(ESP_OK);
    i2c_master_cmd_begin_ExpectAnyArgsAndReturn(ESP_OK);
    i2c_cmd_link_delete_ExpectAnyArgsAndReturn(0);
    i2c_driver_delete_ExpectAnyArgsAndReturn(0);

    uint8_t addr = 0x76;
    uint8_t data;

    TEST_ASSERT(i2c_recv_data(addr, &data, sizeof(data), 10) == ESP_OK);
}

void test_i2c_recv_dataInformsErrorWhenSomethingFails(){
    
    i2c_param_config_ExpectAnyArgsAndReturn(True);

    i2c_driver_install_ExpectAnyArgsAndReturn(ESP_FAIL);
    printf_ExpectAnyArgsAndReturn(True);
    i2c_cmd_link_create_ExpectAnyArgsAndReturn(0);
    i2c_master_start_ExpectAnyArgsAndReturn(ESP_FAIL);
    printf_ExpectAnyArgsAndReturn(True);
    i2c_master_write_byte_ExpectAnyArgsAndReturn(ESP_FAIL);
    printf_ExpectAnyArgsAndReturn(True);
    i2c_master_read_IgnoreAndReturn(ESP_OK);
    i2c_master_stop_ExpectAnyArgsAndReturn(ESP_OK);
    i2c_master_cmd_begin_ExpectAnyArgsAndReturn(ESP_FAIL);
    printf_ExpectAnyArgsAndReturn(True);
    i2c_cmd_link_delete_ExpectAnyArgsAndReturn(0);
    i2c_driver_delete_ExpectAnyArgsAndReturn(0);

    uint8_t addr = 0x76;
    uint8_t data;

    i2c_recv_data(addr, &data, sizeof(data), 10);
}
