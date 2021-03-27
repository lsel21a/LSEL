#include "drivers_bme.h"

// #define COMM_I2C
#define COMM_SPI

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

    return i2c_send_data(dev_addr, (uint8_t *) data2send, (uint16_t)(len+1), 20) != ESP_OK;
}

BME68X_INTF_RET_TYPE bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr){
    // Se contruye la trama que hay que enviar al sensor
    // Para más información consultar datasheet de BME680
    uint8_t dev_addr = *(uint8_t*)intf_ptr;
    return i2c_read_data(dev_addr, reg_addr, reg_data, (uint16_t)len, 20) != ESP_OK;
}

BME68X_INTF_RET_TYPE bme68x_spi_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr){
    spi_device_handle_t *spi = (spi_device_handle_t *)intf_ptr;
    uint8_t rslt = 0;

    int i = 0;
    for (i=0; i<=len; i++){
        // Leer el datasheet para más info
        uint8_t data = 0x00 | (reg_addr>>1);
        // Se envía el registro y el dato a escribir N veces, siendo N el número de bytes que se quieren escribir definido por len
        rslt += spi_send_data(spi, (&data) + i, 1);
        rslt += spi_send_data(spi, (uint8_t *)(reg_data + i), 1);
    }
    return rslt != ESP_OK;
}

BME68X_INTF_RET_TYPE bme68x_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr){
    spi_device_handle_t *spi = (spi_device_handle_t *)intf_ptr;
    uint8_t rslt = 0;

    // Leer el datasheet para más info

    int i = 0;
    for (i=0; i<=len; i++){
        uint8_t data = 0xFF | (reg_addr>>1);
        rslt += spi_send_data(spi, (&data) + i, 1);
        rslt += spi_recv_data(spi, reg_data+i, 1);
    }
    
    return  rslt != ESP_OK;
}

void delay_us(uint32_t timeout_us) {
    
    uint32_t us_act = micros();

    if(timeout_us){
        uint32_t wait = (us_act + timeout_us);
        if(us_act > wait){ // Overflow
            while(micros() > timeout_us){
                // Espera hasta que pase el tiempo ( 0 + timeout)
            }
        }
        while(micros() < wait){
            // Espera hasta que pase el tiempo deseado (act + timeout)
        }
    }
}

void bme68x_delay_us(uint32_t period, void *intf_ptr)
{
	delay_us(period);
    // vTaskDelay(5 / portTICK_RATE_MS);
}

uint8_t dev_addr;
int8_t bme68x_interface_init(struct bme68x_dev *bme, uint8_t intf){
    int8_t rslt = BME68X_OK;
    
    #ifdef COMM_SPI
    static spi_device_handle_t spi;
    #endif

    if (bme != NULL)
    {
        /* Bus configuration : I2C */
        if (intf == BME68X_I2C_INTF)
        {
            dev_addr = BME68X_I2C_ADDR_LOW;
            bme->read = bme68x_i2c_read;
            bme->write = bme68x_i2c_write;
            bme->intf = BME68X_I2C_INTF;
        }
        else if(intf == BME68X_SPI_INTF){
            bme->read = bme68x_spi_read;
            bme->write = bme68x_spi_write;
            bme->intf = BME68X_SPI_INTF;
        }

        bme->delay_us = bme68x_delay_us;
        #ifdef COMM_I2C
        bme->intf_ptr = &dev_addr;
        i2c_init();
        #endif
        #ifdef COMM_SPI
        bme->intf_ptr = &spi;
        spi_init(&spi);
        #endif
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