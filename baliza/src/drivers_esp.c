#include "drivers_esp.h"

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
    };
};

uint32_t micros()
{
    return (uint32_t) (esp_timer_get_time());   // Devuelve los microsegundos desde el encendido del chip
}