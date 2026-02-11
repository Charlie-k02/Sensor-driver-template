/*
    hal_bus_fake.c

    Implémentation du bus simulé.

    Ce fake bus :
    - stocke des registres dans ctx->regs
    - permet au driver de lire/écrire des registres comme s'il parlait à un vrai capteur
    - met à jour la température à chaque lecture pour "faire vivant"
*/

#include "hal/hal_bus_fake.h"
#include <string.h> // memset

/*
    IMPORTANT :
    Ces adresses doivent correspondre à celles utilisées dans sensor.c
*/
#define REG_WHO_AM_I  0x00
#define REG_TEMP_MSB  0x10
#define REG_TEMP_LSB  0x11

/*
    ID capteur simulé : doit correspondre à EXPECTED_ID du driver (sensor.c)
*/
#define FAKE_SENSOR_ID 0x42

/*
    Met à jour la température simulée.

    Ici on fait simple :
    - on augmente la température de 0.05°C à chaque lecture
    - puis on recopie la valeur dans les registres TEMP_MSB/TEMP_LSB
*/
static void fake_update_temperature(hal_bus_fake_ctx_t *ctx)
{
    // +5 centi-degrés = +0.05°C
    ctx->fake_temp_centi += 5;

    // Stocker en format 16 bits big-endian dans 2 registres
    ctx->regs[REG_TEMP_MSB] = (uint8_t)((ctx->fake_temp_centi >> 8) & 0xFF);
    ctx->regs[REG_TEMP_LSB] = (uint8_t)(ctx->fake_temp_centi & 0xFF);
}

/*
    Lecture de registres simulée.

    Paramètres (mêmes que l'interface HAL) :
    - context : pointeur vers hal_bus_fake_ctx_t
    - dev_addr : ignoré ici (on simule un seul capteur)
    - reg : registre de départ à lire
    - data : buffer où écrire les octets lus
    - len : nombre d'octets à lire
*/
static hal_status_t fake_reg_read(
    void *context,
    uint8_t dev_addr,
    uint8_t reg,
    uint8_t *data,
    size_t len
)
{
    (void)dev_addr; // pas utilisé dans la simulation

    // Vérifications basiques
    if (!context || !data) {
        return HAL_ERR;
    }

    hal_bus_fake_ctx_t *ctx = (hal_bus_fake_ctx_t *)context;

    // On met à jour la température AVANT de répondre,
    // pour que chaque lecture renvoie une valeur qui évolue.
    fake_update_temperature(ctx);

    // Copie des registres vers data[]
    for (size_t i = 0; i < len; i++) {
        data[i] = ctx->regs[(uint8_t)(reg + i)];
    }

    return HAL_OK;
}

/*
    Écriture de registres simulée.

    Permet au driver d'écrire dans des registres de config, etc.
*/
static hal_status_t fake_reg_write(
    void *context,
    uint8_t dev_addr,
    uint8_t reg,
    const uint8_t *data,
    size_t len
)
{
    (void)dev_addr; // pas utilisé

    // Vérifications basiques
    if (!context || !data) {
        return HAL_ERR;
    }

    hal_bus_fake_ctx_t *ctx = (hal_bus_fake_ctx_t *)context;

    // Copier data[] vers les registres
    for (size_t i = 0; i < len; i++) {
        ctx->regs[(uint8_t)(reg + i)] = data[i];
    }

    return HAL_OK;
}

/*
    Fonction publique d'initialisation.

    Elle prépare le contexte et configure la structure hal_bus_t.
*/
void hal_bus_fake_init(hal_bus_fake_ctx_t *ctx, hal_bus_t *bus)
{
    // Vérifications
    if (!ctx || !bus) {
        return;
    }

    // Reset complet des registres et de la température
    memset(ctx, 0, sizeof(*ctx));

    // Température initiale : 25.00°C
    ctx->fake_temp_centi = 2500;

    // Régler WHO_AM_I pour que sensor_init() réussisse
    ctx->regs[REG_WHO_AM_I] = FAKE_SENSOR_ID;

    // Régler les registres temp au départ
    ctx->regs[REG_TEMP_MSB] = (uint8_t)((ctx->fake_temp_centi >> 8) & 0xFF);
    ctx->regs[REG_TEMP_LSB] = (uint8_t)(ctx->fake_temp_centi & 0xFF);

    // Configurer l'interface HAL bus
    bus->ctx = ctx;
    bus->reg_read = fake_reg_read;
    bus->reg_write = fake_reg_write;
}
