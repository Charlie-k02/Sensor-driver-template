/*
    demo_main.c

    Démo :
    - configure les HAL host (fake bus + time + log)
    - initialise le driver capteur
    - lit la température en boucle
*/

#include <stdio.h>
#include <stdint.h>

#include "sensor/sensor.h"
#include "hal/hal_bus_fake.h"
#include "hal/hal_time.h"
#include "hal/hal_log.h"

/*
    Ces fonctions sont implémentées dans :
    - src/hal/hal_time_fake.c
    - src/hal/hal_log_stdio.c

    Comme on n'a pas encore de .h dédiés, on déclare les prototypes ici.
    (Plus tard, on pourra créer hal_time_fake.h / hal_log_stdio.h si tu veux.)
*/
void hal_time_fake_init(hal_time_t *time);
void hal_log_stdio_init(hal_log_t *log);

int main(void)
{
    printf("=== Sensor Driver Demo (Simulated) ===\n");

    /* ---------------- HAL fake bus ---------------- */

    hal_bus_t bus;
    hal_bus_fake_ctx_t bus_ctx;

    /* Initialise le capteur simulé + configure bus.reg_read / bus.reg_write */
    hal_bus_fake_init(&bus_ctx, &bus);

    /* ---------------- HAL time ---------------- */

    hal_time_t time;
    hal_time_fake_init(&time);

    /* ---------------- HAL log ---------------- */

    hal_log_t log;
    hal_log_stdio_init(&log);

    /* ---------------- Driver capteur ---------------- */

    sensor_t sensor;

    /*
        dev_addr : adresse fictive (en fake, elle n'est pas utilisée)
        Elle est quand même utile pour montrer une API réaliste.
    */
    sensor_status_t st = sensor_init(&sensor, 0x50, &bus, &time, &log);
    if (st != SENSOR_OK) {
        printf("Sensor init failed (status=%d)\n", st);
        return 1;
    }

    printf("Sensor init OK\n");

    /* ---------------- Lecture en boucle ---------------- */

    while (1) {
        int16_t temp_centi = 0;

        if (sensor_read_temperature_centi(&sensor, &temp_centi) == SENSOR_OK) {
            /* temp_centi est en centi-degrés : 2534 => 25.34°C */
            printf("Temperature: %.2f °C\n", (float)temp_centi / 100.0f);
        } else {
            printf("Temperature read error\n");
        }

        /* Attendre 1 seconde */
        time.delay_ms(time.ctx, 1000);
    }

    return 0;
}
