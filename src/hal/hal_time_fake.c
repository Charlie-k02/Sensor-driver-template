/*
    hal_time_fake.c

    Implémentation macOS/PC de delay_ms.

    On utilise usleep() qui attend un nombre de microsecondes.
*/

#include "hal/hal_time_fake.h"
#include <unistd.h> // usleep

/*
    Fonction delay_ms pour host.

    - ctx : non utilisé ici (mais gardé pour compatibilité)
    - ms  : durée en millisecondes
*/
static void host_delay_ms(void *ctx, uint32_t ms)
{
    (void)ctx; // ctx inutilisé sur host

    /*
        usleep prend des microsecondes.
        1 ms = 1000 microsecondes.
    */
    usleep((useconds_t)(ms * 1000u));
}

/*
    Initialise la HAL time (host).

    Remplit :
    - ctx (ici NULL car pas besoin)
    - delay_ms (pointeur vers host_delay_ms)
*/
void hal_time_fake_init(hal_time_t *time)
{
    if (!time) {
        return;
    }

    time->ctx = NULL;
    time->delay_ms = host_delay_ms;
}
