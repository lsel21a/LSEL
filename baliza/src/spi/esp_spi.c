#include "esp_spi.h"

uint8_t spi_init(spi_device_handle_t *spi){

    spi_bus_config_t buscfg={
        .miso_io_num=SPI_MISO_PIN,
        .mosi_io_num=SPI_MOSI_PIN,
        .sclk_io_num=SPI_SCLK_PIN,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=32
    };

    spi_device_interface_config_t devcfg={
        .clock_speed_hz=SPI_MASTER_FREQ_HZ,           
        .mode=0,                                
        .spics_io_num=SPI_CS_PIN,               
        .queue_size=1,                     
    };
    
    uint8_t rslt;
    rslt = spi_bus_initialize(BME_HOST, &buscfg, DMA_CHAN);
    if (rslt != ESP_OK){
        printf("Inicialización del bus SPI.\n");
        check_rslt(rslt);
    };

    rslt = spi_bus_add_device(BME_HOST, &devcfg, spi);
    if (rslt != ESP_OK){
        printf("Añadiendo dispositivo al bus SPI.\n");
        check_rslt(rslt);
    };

    return rslt;
};

uint8_t spi_send_data(spi_device_handle_t *spi, uint8_t *data, uint8_t data_size){
    uint8_t rslt;

    // Transmisión completada
    if (data_size == 0){
        return ESP_OK;
    }

    // Se rellena la estrcutura para enviar la trama
    spi_transaction_t t;
    // memset(&t, 0, sizeof(t));       
    t.length = data_size * 8;                 
    t.tx_buffer = data;               
    t.rxlength = 0;  
    t.flags = 0;
    t.rx_buffer = NULL;
    t.user = (void*)1;

    rslt = spi_device_polling_transmit(*spi, &t);
    if (rslt != ESP_OK){
        printf("Envío del dato.\n");
        check_rslt(rslt);
    }
    return rslt;       
};

uint8_t spi_recv_data(spi_device_handle_t *spi, uint8_t *data, uint8_t data_size){
    uint8_t rslt;

    // Transmisión completada
    if (data_size == 0) return ESP_OK;

    // Se rellena la estrcutura para enviar la trama
    spi_transaction_t t;
    // memset(&t, 0, sizeof(t));
    t.rxlength = data_size * 8;  
    t.flags = 0;
    t.length = data_size * 8;
    t.tx_buffer = NULL;
    t.rx_buffer = data;
    t.user = (void*)1;

    rslt = spi_device_polling_transmit(*spi, &t);
    if (rslt != ESP_OK){
        printf("Recepción del dato.\n");
        check_rslt(rslt);
    }
    return rslt;
};