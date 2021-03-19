#include "drivers_bme.h"

BME68X_INTF_RET_TYPE bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr){

    uint8_t dev_addr = *(uint8_t*)intf_ptr;

    // Se contruye la trama que hay que enviar al sensor
    // Para más información consultar datasheet de BME680
    uint8_t data2send[len+1];
    data2send[0] = reg_addr;
    int i;
    for(i=1;i<=len;i++){
    	data2send[i] = reg_data[i-1];
    };

    return i2c_send_data(dev_addr, (uint8_t *) &data2send, (uint16_t)(len+1), 10) != ESP_OK;
}

BME68X_INTF_RET_TYPE bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr){

    // Se contruye la trama que hay que enviar al sensor
    // Para más información consultar datasheet de BME680
    uint8_t dev_addr = *(uint8_t*)intf_ptr;
    i2c_send_data(dev_addr, (uint8_t *) &reg_addr, (uint16_t)len, 10);
    return i2c_recv_data(dev_addr, reg_data, (uint16_t) len, 10)!= ESP_OK;
}

uint32_t micros()
{
    return (uint32_t) (esp_timer_get_time());
}

void delay_us (uint32_t timeout_us) {
    
    uint32_t us_act = micros();
    uint8_t rslt;

    if(timeout_us){
        uint32_t wait = (us_act + timeout_us);
        if(us_act > wait){ // Overflow
            while(micros() > timeout_us){
                // Espera hasta que pase el tiempo ( 0 + timeout)
                rslt = esp_task_wdt_reset();
                if (rslt != ESP_OK){
                    printf("Reset del watchdog.\n");
                    check_rslt(rslt);
                }
            }
        }
        while(micros() < wait){
            // Espera hasta que pase el tiempo deseado (act + timeout)
            rslt = esp_task_wdt_reset();
            if (rslt != ESP_OK){
                printf("Reset del watchdog.\n");
                check_rslt(rslt);
            }
        }
    }
}

void bme68x_delay_us(uint32_t period, void *intf_ptr)
{
	delay_us(period);
}

uint8_t dev_addr;
int8_t bme68x_interface_init(struct bme68x_dev *bme, uint8_t intf){
    int8_t rslt = BME68X_OK;

    if (bme != NULL)
    {
        /* Bus configuration : I2C */
        if (intf == BME68X_I2C_INTF)
        {
            dev_addr = BME68X_I2C_ADDR_LOW;
            bme->read = bme68x_i2c_read;
            bme->write = bme68x_i2c_write;
            bme->intf = BME68X_I2C_INTF;
            i2c_init();
        }

        bme->delay_us = bme68x_delay_us;
        bme->intf_ptr = &dev_addr;
        bme->amb_temp = 20; /* The ambient temperature in deg C is used for defining the heater temperature */
    }
    else
    {
        rslt = BME68X_E_NULL_PTR;
    }

    return rslt;
}

void bme68x_check_rslt(const char api_name[], int8_t rslt){
    switch (rslt)
    {
        case BME68X_OK:

            /* Do nothing */
            break;
        case BME68X_E_NULL_PTR:
            printf("Error [%d] : Null pointer\r\n", rslt);
            break;
        case BME68X_E_COM_FAIL:
            printf("Error [%d] : Communication failure\r\n", rslt);
            break;
        case BME68X_E_INVALID_LENGTH:
            printf("Error [%d] : Incorrect length parameter\r\n", rslt);
            break;
        case BME68X_E_DEV_NOT_FOUND:
            printf("Error [%d] : Device not found\r\n", rslt);
            break;
        default:
            printf("Error [%d] : Unknown error code\r\n", rslt);
            break;
    }
}