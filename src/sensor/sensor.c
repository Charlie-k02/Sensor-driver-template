/*
    sensor.c

    Implémentation du driver capteur simulé.

    Ce driver communique avec un capteur registre-based
    via la HAL bus.

    Il ne dépend d'aucun hardware spécifique.
*/

#include "include/sensor/sensor.h"

/*
    Définition des registres du capteur.

    Ces adresses sont fictives mais
    ressemblent à de vrais capteurs.
*/
#define REG_WHO_AM_I  0x00
#define REG_TEMP_MSB  0x10
#define REG_TEMP_LSB  0x11

/*
    ID attendu du capteur.

    Permet de vérifier qu'on parle
    au bon composant.
*/
#define EXPECTED_ID 0x42

/*
    Lecture de l'ID capteur.
*/
sensor_status_t sensor_get_id(
    sensor_t *s,
    uint8_t *id_out
)
{
    // Vérifications de sécurité
    if (!s || !id_out || !s->bus || !s->bus->reg_read)
        return SENSOR_ERR;

    uint8_t id = 0;

    // Lecture du registre WHO_AM_I
    if (s->bus->reg_read(
            s->bus->ctx,
            s->dev_addr,
            REG_WHO_AM_I,
            &id,
            1
        ) != HAL_OK)
    {
        return SENSOR_ERR;
    }

    *id_out = id;
    return SENSOR_OK;
}

/*
    Initialisation du capteur.

    Vérifie l'ID et prépare la structure.
*/
sensor_status_t sensor_init(
    sensor_t *s,
    uint8_t dev_addr,
    const hal_bus_t *bus,
    const hal_time_t *time,
    const hal_log_t *log
)
{
    // Vérifications de sécurité
    if (!s || !bus || !bus->reg_read || !bus->reg_write)
        return SENSOR_ERR;

    // Stocker les dépendances HAL
    s->dev_addr = dev_addr;
    s->bus = bus;
    s->time = time;
    s->log = log;

    // Lire ID capteur
    uint8_t id = 0;
    if (sensor_get_id(s, &id) != SENSOR_OK)
        return SENSOR_ERR;

    // Vérifier ID
    if (id != EXPECTED_ID)
        return SENSOR_BAD_ID;

    // Petit délai après init (comme en vrai)
    if (s->time && s->time->delay_ms)
        s->time->delay_ms(s->time->ctx, 10);

    return SENSOR_OK;
}

/*
    Lecture de la température.

    Format :
    valeur en centi-degrés Celsius
    ex: 2534 = 25.34°C
*/
sensor_status_t sensor_read_temperature_centi(
    sensor_t *s,
    int16_t *temp_centi_out
)
{
    if (!s || !temp_centi_out || !s->bus || !s->bus->reg_read)
        return SENSOR_ERR;

    uint8_t buf[2] = {0};

    // Lire MSB + LSB
    if (s->bus->reg_read(
            s->bus->ctx,
            s->dev_addr,
            REG_TEMP_MSB,
            buf,
            2
        ) != HAL_OK)
    {
        return SENSOR_ERR;
    }

    // Reconstruction valeur 16 bits
    int16_t raw =
        ((int16_t)buf[0] << 8) |
        buf[1];

    *temp_centi_out = raw;

    return SENSOR_OK;
}
