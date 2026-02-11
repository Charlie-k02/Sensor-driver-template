#pragma once
/*
    hal_time_fake.h

    Implémentation "host" (macOS/PC) de la HAL time.

    Objectif :
    - fournir une fonction delay_ms() utilisable par le driver
    - sans dépendre d'un microcontrôleur

    Sur macOS, on utilise usleep() (microsecondes).
*/

#include "hal/hal_time.h"

/*
    Initialise une structure hal_time_t pour l'environnement PC/macOS.

    Paramètres :
    - time : structure HAL time à remplir (pointeurs de fonctions)
*/
void hal_time_fake_init(hal_time_t *time);
