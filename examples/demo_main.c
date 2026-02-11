/*
    demo_main.c

    Programme de démonstration du driver capteur.

    Il montre :
    - initialisation des HAL fake
    - initialisation du capteur
    - lecture périodique de la température
*/

#include <stdio.h>

#include "sensor/sensor.h"
#include "hal/hal_bus_fake.h"
#include "hal/hal_time.h"
#include "hal/hal_log.h"

/*
    Déclarations des fonctions d'init
    définies dans les .c fake.
*/
void hal_time_fake_init(hal_time_t *time);
void hal_log_stdio_init(hal_log_t *log);

int main(void)
{
    printf("=== Sensor Driver Demo ===\n");

    /* --------- Création des HAL --------- */

    hal_bus_t bus;
    hal_bus_fake_ctx_t bus_ctx;

    hal_bus_fake_init(&bus_ctx, &bus);

    hal_time_t time;
    hal_time_fake_init(&time);

    hal_log_t log;
    hal_log_stdio_init(&log);

    /* --------- Création du capteur --------- */

    sensor_t sensor;

    if (sensor_init(&sensor, 0x50, &bus, &time, &log) != SENSOR_OK) {
        printf("Sensor init failed!\n");
        return -1;
    }

    printf("Sensor init OK\n");

    /* --------- Boucle de lecture --------- */

    while (1)
    {
        int16_t temp;

        if (sensor_read_temperature_centi(&sensor, &temp) == SENSOR_OK)
        {
            printf("Temperature: %.2f °C\n", temp / 100.0f);
        }
        else
        {
            printf("Read error\n");
        }

        time.delay_ms(time.ctx, 1000); // 1 seconde
    }

    return 0;
}
