#pragma once
/*
    sensor.h

    API publique du driver capteur.

    Ce fichier décrit CE QUE le driver offre,
    mais pas COMMENT il fonctionne.

    -> Important : seule cette API doit être
       utilisée par l'utilisateur.
*/

#include <stdint.h>
#include "hal/hal_bus.h"
#include "hal/hal_time.h"
#include "hal/hal_log.h"

/*
    Codes de retour du driver capteur.
*/
typedef enum {
    SENSOR_OK = 0,       // Succès
    SENSOR_ERR = -1,     // Erreur générique
    SENSOR_BAD_ID = -2   // Mauvais capteur détecté
} sensor_status_t;

/*
    Structure contexte du capteur.

    Elle contient toutes les dépendances HAL.
*/
typedef struct {

    /*
        Adresse du capteur sur le bus.
        (ex : adresse I2C)
    */
    uint8_t dev_addr;

    /*
        Interfaces HAL utilisées par le driver.
        Elles sont fournies par l'utilisateur.
    */
    const hal_bus_t  *bus;
    const hal_time_t *time;
    const hal_log_t  *log;

} sensor_t;

/*
    Initialise le capteur.

    Vérifie l'ID du capteur et prépare
    la structure sensor_t.
*/
sensor_status_t sensor_init(
    sensor_t *s,
    uint8_t dev_addr,
    const hal_bus_t *bus,
    const hal_time_t *time,
    const hal_log_t *log
);

/*
    Lit l'identifiant du capteur.

    Permet de vérifier qu'on parle
    au bon composant.
*/
sensor_status_t sensor_get_id(
    sensor_t *s,
    uint8_t *id_out
);

/*
    Lit la température en centi-degrés Celsius.

    Exemple :
    2534 -> 25.34°C
*/
sensor_status_t sensor_read_temperature_centi(
    sensor_t *s,
    int16_t *temp_centi_out
);
