#include "esp_i2c.h"

uint8_t i2c_driver_init(){

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_PIN,         // select GPIO specific to your project
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_SCL_PIN,         // select GPIO specific to your project
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,  // select frequency specific to your project
        // .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };

    uint8_t rslt =  i2c_param_config(I2C_NUM_1, &conf);
    if (rslt != ESP_OK){
        printf("Configuración de parámetros.\n");
        check_rslt(rslt);
    }
    
    rslt = i2c_driver_install(I2C_NUM_1, I2C_MODE_MASTER, 0, 0, ESP_INTR_FLAG_INTRDISABLED);
    if (rslt != ESP_OK){
        printf("Instalación de drivers.\n");
        check_rslt(rslt);
    }
    return rslt;
}

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

int i2c_send_data(uint8_t dev_addr, uint8_t *data, uint8_t data_size, uint32_t timeout){
    
    int rslt;
    i2c_cmd_handle_t cmd;

    // Creación del comando
    cmd = i2c_cmd_link_create();

    // Inicialización en modo máster
    rslt = i2c_master_start(cmd);
    if (rslt != ESP_OK){
        printf("Inicialización en modo máster.\n");
        check_rslt(rslt);
    }
    

    // Se escribe la dirección del esclavo    
    rslt = i2c_master_write_byte(cmd, dev_addr, true);
    if (rslt != ESP_OK){
        printf("Escritura de la dirección del esclavo.\n");
        check_rslt(rslt);
    }


    // Se escriben los datos que se deseen
    rslt = i2c_master_write(cmd, (uint8_t *) &data, data_size, true);
    if (rslt != ESP_OK){
        printf("Escritura de datos.\n");
        check_rslt(rslt);
    }

    // Envío de la señal de stop
    rslt = i2c_master_stop(cmd);
    if (rslt != ESP_OK){
        printf("Escritura del bit de stop.\n");
        check_rslt(rslt);
    } 
    
    // Se realiza el envío
    rslt = i2c_master_cmd_begin(I2C_NUM_1, cmd, timeout);
    if (rslt != ESP_OK){
        printf("Ejecución de los comandos.\n");
        check_rslt(rslt);
    }

    // Se finaliza la comunicación
    i2c_cmd_link_delete(cmd);

    return ESP_OK;
}

int i2c_recv_data(uint8_t dev_addr, uint8_t *data, uint8_t data_size, uint32_t timeout){
    int rslt;
    i2c_cmd_handle_t cmd;

    // Creación del comando
    cmd = i2c_cmd_link_create();

    // Inicialización en modo máster
    rslt = i2c_master_start(cmd);
    if (rslt != ESP_OK){
        printf("Inicialización en modo máster.\n");
        check_rslt(rslt);
    }

    // Se escribe la dirección del esclavo
    rslt = i2c_master_write_byte(cmd, dev_addr, true);
    if (rslt != ESP_OK){
        printf("Escritura de la dirección del esclavo.\n");
        check_rslt(rslt);
    }

    // Se leen los datos que se deseen (n - 1) con ACK
    if (data_size > 1){
        rslt = i2c_master_read(cmd,(uint8_t *)  &data, data_size-1, true);
        if (rslt != ESP_OK){
            printf("Lectura de datos con ACK.\n");
            check_rslt(rslt);
        }
    }

    // Se escriben los datos que se deseen sin ACK
    rslt = i2c_master_read(cmd, (uint8_t *) &data, 1, false);
    if (rslt != ESP_OK){
        printf("Lectura de datos sin ACK.\n");
        check_rslt(rslt);
    }

    // Envío de la señal de stop
    rslt = i2c_master_stop(cmd);
    if (rslt != ESP_OK){
        printf("Finalización del comando.\n");
        check_rslt(rslt);
    }
    
    // Se realiza el envío
    rslt = i2c_master_cmd_begin(I2C_NUM_1, cmd, timeout);
    if (rslt != ESP_OK){
        printf("Ejecución del comando.\n");
        check_rslt(rslt);
    }

    // Se finaliza la comunicación
    i2c_cmd_link_delete(cmd);

    return ESP_OK;
}