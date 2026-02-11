#pragma once
/*
    hal_log_stdio.h

    Implémentation "host" (macOS/PC) de la HAL log.

    Objectif :
    - fournir une fonction de log basée sur stdout/stderr
    - utilisable par le driver ou l'application
*/

#include "hal/hal_log.h"

/*
    Initialise une structure hal_log_t avec une implémentation stdio.

    Paramètres :
    - log : structure HAL log à remplir
*/
void hal_log_stdio_init(hal_log_t *log);
