#pragma once
/*
    hal_time.h

    Interface HAL pour la gestion du temps.

    Permet au driver d'utiliser des délais
    sans dépendre d'une plateforme spécifique.
*/

#include <stdint.h>

/*
    Structure HAL pour le temps.
*/
typedef struct {

    /*
        Contexte utilisateur.

        Peut être :
        - handle RTOS
        - timer système
        - NULL en simulation
    */
    void *ctx;

    /*
        Fonction de délai en millisecondes.

        Le driver peut appeler ceci pour :
        - attendre après un reset capteur
        - respecter timing datasheet
    */
    void (*delay_ms)(
        void *ctx,
        uint32_t ms
    );

} hal_time_t;
