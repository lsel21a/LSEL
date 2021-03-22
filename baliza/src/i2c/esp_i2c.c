#include "esp_i2c.h"

void check_rslt(int rslt){
    switch (rslt){
        case ESP_OK:
            printf("Todo correcto.\n");
            break;
        case ESP_FAIL:
            printf("Error producido, la operación ha fallado.\n");
            break;
        case ESP_ERR_NO_MEM:
            printf("No hay memoria suficiente.\n");
            break;
        case ESP_ERR_INVALID_STATE:
            printf("Estado inválido.\n");
            break;
        case ESP_ERR_INVALID_SIZE:
            printf("Tamaño inválido.\n");
            break;
        case ESP_ERR_NOT_FOUND:
            printf("Elemeneto no encontrado.\n");
            break;
        case ESP_ERR_NOT_SUPPORTED:
            printf("Operación no esperada.\n");
            break;
        case ESP_ERR_TIMEOUT:
            printf("Timeout producido.\n");
            break;
        case ESP_ERR_INVALID_RESPONSE:
            printf("Espuesta inválida.\n");
            break;
        case ESP_ERR_INVALID_CRC:
            printf("Checksum no válida.\n");
            break;
        case ESP_ERR_INVALID_VERSION:
            printf("Versión incorrecta.\n");
            break;
        case ESP_ERR_INVALID_MAC:
            printf("MAC inválido.\n");
            break;
        case ESP_ERR_WIFI_BASE:
            printf("WIFI inválido.\n");
            break;
        case ESP_ERR_MESH_BASE:
            printf("MESH inválido.\n");
            break;
        case ESP_ERR_INVALID_ARG:
            printf("Argumento inválido.\n");
            break;
        default: 
            printf("Error inesperado.\n");
    }
};

uint8_t i2c_init(){

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_PIN,         // select GPIO specific to your project
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_SCL_PIN,         // select GPIO specific to your project
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,  // select frequency specific to your project
        // .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };

    uint8_t rslt =  i2c_param_config(I2C_PORT, &conf);
    if (rslt != ESP_OK){
        printf("Configuración de parámetros.\n");
        check_rslt(rslt);
    }
    
    return rslt;
}

uint8_t i2c_detect(){

    // Inicialización del I2C
    i2c_init();

    int rslt;
    rslt = i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
    if (rslt != ESP_OK){
        printf("Instalación de drivers.\n");
        check_rslt(rslt);
    }
    uint8_t address;
    printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");
    for (int i = 0; i < 128; i += 16) {
        printf("%02x: ", i);
        for (int j = 0; j < 16; j++) {
            fflush(stdout);
            address = i + j;
            i2c_cmd_handle_t cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
            i2c_master_stop(cmd);
            esp_err_t ret = i2c_master_cmd_begin(I2C_PORT, cmd, 50 / portTICK_RATE_MS);
            i2c_cmd_link_delete(cmd);
            if (ret == ESP_OK) {
                printf("%02x ", address);
            } else if (ret == ESP_ERR_TIMEOUT) {
                printf("UU ");
            } else {
                printf("-- ");
            }
        }
        printf("\r\n");
    }

    i2c_driver_delete(I2C_PORT);
    return 0;
}

uint8_t i2c_send_data(uint8_t dev_addr, uint8_t *data, uint8_t data_size, uint32_t timeout){
    
    int rslt;
    i2c_cmd_handle_t cmd;

    // Inicialización del I2C
    i2c_init();

    // Instalación de driver
    rslt = i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
    if (rslt != ESP_OK){
        printf("Instalación de drivers[send].\n");
        check_rslt(rslt);
    }

    // Creación del comando
    cmd = i2c_cmd_link_create();

    // Inicialización en modo máster
    rslt = i2c_master_start(cmd);
    if (rslt != ESP_OK){
        printf("Inicialización en modo máster[send].\n");
        check_rslt(rslt);
    }
    

    // Se escribe la dirección del esclavo    
    rslt = i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_WRITE , true);
    if (rslt != ESP_OK){
        printf("Escritura de la dirección del esclavo[send].\n");
        check_rslt(rslt);
    }

    // Se escriben los datos que se deseen
    rslt = i2c_master_write(cmd, (uint8_t *) data, data_size, true);
    if (rslt != ESP_OK){
        printf("Escritura de datos[send].\n");
        check_rslt(rslt);
    }

    // Envío de la señal de stop
    rslt = i2c_master_stop(cmd);
    if (rslt != ESP_OK){
        printf("Escritura del bit de stop[send].\n");
        check_rslt(rslt);
    } 
    
    // Se realiza el envío
    rslt = i2c_master_cmd_begin(I2C_PORT, cmd, timeout/portTICK_PERIOD_MS);
    if (rslt != ESP_OK){
        printf("Ejecución de los comandos[send].\n");
        check_rslt(rslt);
    }

    // Se finaliza la comunicación
    i2c_cmd_link_delete(cmd);

    // Se elimina el driver
    i2c_driver_delete(I2C_PORT);

    return ESP_OK;
}

uint8_t i2c_recv_data(uint8_t dev_addr, uint8_t *data, uint8_t data_size, uint32_t timeout){
    int rslt;
    i2c_cmd_handle_t cmd;

    // Inicialización del I2C
    i2c_init();

    // Instalación de driver
    rslt = i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0);
    if (rslt != ESP_OK){
        printf("Instalación de drivers[recv].\n");
        check_rslt(rslt);
    }

    // Creación del comando
    cmd = i2c_cmd_link_create();

    // Inicialización en modo máster
    rslt = i2c_master_start(cmd);
    if (rslt != ESP_OK){
        printf("Inicialización en modo máster[recv].\n");
        check_rslt(rslt);
    }

    // Se escribe la dirección del esclavo
    rslt = i2c_master_write_byte(cmd, (dev_addr << 1) | I2C_MASTER_READ, true);
    if (rslt != ESP_OK){
        printf("Escritura de la dirección del esclavo[recv].\n");
        check_rslt(rslt);
    }

    // Se leen los datos que se deseen (n - 1) con ACK
    if (data_size > 1){
        rslt = i2c_master_read(cmd, (uint8_t *) data, data_size-1, true);
        if (rslt != ESP_OK){
            printf("Lectura de datos con ACK[recv].\n");
            check_rslt(rslt);
        }
    }

    // Se escriben los datos que se deseen sin ACK
    rslt = i2c_master_read_byte(cmd, (uint8_t *) &data[data_size-1], false);
    if (rslt != ESP_OK){
        printf("Lectura de datos sin ACK[recv].\n");
        check_rslt(rslt);
    }

    // Envío de la señal de stop
    rslt = i2c_master_stop(cmd);
    if (rslt != ESP_OK){
        printf("Finalización del comando[recv].\n");
        check_rslt(rslt);
    }
    
    // Se realiza el envío
    rslt = i2c_master_cmd_begin(I2C_PORT, cmd, timeout/portTICK_PERIOD_MS);
    if (rslt != ESP_OK){
        printf("Ejecución del comando[recv].\n");
        check_rslt(rslt);
    }

    // Se finaliza la comunicación
    i2c_cmd_link_delete(cmd);

    // Se elimina el driver
    i2c_driver_delete(I2C_PORT);

    return ESP_OK;
}