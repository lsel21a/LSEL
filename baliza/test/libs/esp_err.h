#ifndef ESP_ERR_H
#define ESP_ERR_H

#include <stdint.h>

typedef int32_t esp_err_t;

#ifndef ESP_OK
#define ESP_OK 0
#endif

#ifndef ESP_FAIL
#define ESP_FAIL -1
#endif

#ifndef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x) do {             \
        esp_err_t __err_rc = (x);           \
        (void) sizeof(__err_rc);            \
    } while(0);
#endif

#endif /* ESP_ERR_H */
