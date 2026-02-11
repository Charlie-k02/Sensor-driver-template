#pragma once
/*
    hal_bus_fake.h

    Fake (simulation) du bus capteur pour exécuter le driver sur PC,
    sans I2C/SPI réel.

    Principe :
    - on simule un capteur "register-based" avec un tableau de 256 registres
    - le driver appelle bus->reg_read / bus->reg_write
    - ici, on lit/écrit dans le tableau regs[]
*/

#include "hal/hal_bus.h"
#include <stdint.h>

/*
    Contexte interne du fake bus.

    regs[256] :
      représente la mémoire de registres du capteur simulé.

    fake_temp_centi :
      température simulée en centi-degrés (ex: 2500 = 25.00°C)
*/
typedef struct {
    uint8_t regs[256];
    int16_t fake_temp_centi;
} hal_bus_fake_ctx_t;

/*
    Initialise le fake bus et le contexte.

    Paramètres :
    - ctx : contexte fake (doit être alloué par l'utilisateur, par ex. en variable locale)
    - bus : structure hal_bus_t que le driver utilisera

    Effet :
    - reset regs
    - met WHO_AM_I à la valeur attendue par le driver
    - initialise une température de départ
    - configure les pointeurs de fonctions reg_read/reg_write
*/
void hal_bus_fake_init(hal_bus_fake_ctx_t *ctx, hal_bus_t *bus);
