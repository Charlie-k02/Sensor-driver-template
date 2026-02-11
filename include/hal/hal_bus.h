#pragma once
/*
    hal_bus.h

    Ce fichier définit l'interface HAL pour un bus de communication
    (typiquement I2C ou SPI).

    Le driver capteur utilisera cette interface sans connaître
    le matériel réel derrière.

    -> Cela permet de changer de microcontrôleur ou de simuler
       un capteur sans modifier le driver.
*/

#include <stdint.h>
#include <stddef.h>

/*
    Codes de retour standard de la HAL.
    Ils indiquent si une opération s'est bien passée.
*/
typedef enum {
    HAL_OK = 0,        // Tout s'est bien passé
    HAL_ERR = -1,      // Erreur générique
    HAL_TIMEOUT = -2   // Timeout de communication
} hal_status_t;

/*
    Structure représentant un bus HAL.

    Le driver ne sait pas ce qu'il y a dedans,
    il appelle seulement les fonctions.
*/
typedef struct {

    /*
        Pointeur de contexte utilisateur.

        Peut contenir par exemple :
        - handle I2C STM32
        - structure fake pour simulation
        - etc.

        Le driver ne l'utilise jamais directement.
    */
    void *ctx;

    /*
        Lecture de registres.

        Paramètres :
        - ctx      : contexte HAL
        - dev_addr : adresse du capteur (I2C par exemple)
        - reg      : registre à lire
        - data     : buffer de sortie
        - len      : nombre d'octets à lire
    */
    hal_status_t (*reg_read)(
        void *ctx,
        uint8_t dev_addr,
        uint8_t reg,
        uint8_t *data,
        size_t len
    );

    /*
        Écriture de registres.

        Paramètres :
        - ctx      : contexte HAL
        - dev_addr : adresse du capteur
        - reg      : registre à écrire
        - data     : données à écrire
        - len      : taille des données
    */
    hal_status_t (*reg_write)(
        void *ctx,
        uint8_t dev_addr,
        uint8_t reg,
        const uint8_t *data,
        size_t len
    );

} hal_bus_t;
