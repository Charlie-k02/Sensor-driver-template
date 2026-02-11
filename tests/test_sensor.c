/*
    test_sensor.c

    Tests unitaires simples (sans framework externe).

    Objectifs :
    - vérifier que sensor_init() réussit quand WHO_AM_I est correct
    - vérifier que sensor_init() échoue quand WHO_AM_I est faux
    - vérifier que la lecture température renvoie une valeur cohérente

    On utilise :
    - hal_bus_fake (capteur simulé)
    - hal_time_fake / hal_log_stdio (implémentations host)

    Note :
    - On ne fait pas de tests "temps réel" : delay_ms n'est pas essentiel ici.
*/

#include <stdio.h>
#include <stdint.h>

#include "sensor/sensor.h"
#include "hal/hal_bus_fake.h"
#include "hal/hal_time.h"
#include "hal/hal_log.h"

/*
    Fonctions d'init (implémentées dans src/hal/*.c)
    Comme on n'a pas de headers dédiés, on déclare les prototypes ici.
*/
void hal_time_fake_init(hal_time_t *time);
void hal_log_stdio_init(hal_log_t *log);

/* Petit utilitaire : compteur de tests */
static int g_tests_run = 0;
static int g_tests_failed = 0;

/*
    Macro d'assertion minimaliste.
    - Si la condition est fausse : on marque le test en échec et on affiche un message.
*/
#define TEST_ASSERT(cond) do {                                      \
    g_tests_run++;                                                  \
    if (!(cond)) {                                                  \
        g_tests_failed++;                                           \
        printf("[FAIL] %s:%d: %s\n", __FILE__, __LINE__, #cond);    \
    }                                                               \
} while (0)

/*
    Adresses de registres utilisées dans le fake bus.
    Elles doivent correspondre à celles du driver + fake.
*/
#define REG_WHO_AM_I  0x00

/* Valeur attendue par le driver (EXPECTED_ID dans sensor.c) */
#define EXPECTED_ID   0x42

/*
    Test 1 : sensor_init doit réussir si WHO_AM_I est correct.
*/
static void test_init_ok(void)
{
    hal_bus_t bus;
    hal_bus_fake_ctx_t bus_ctx;
    hal_bus_fake_init(&bus_ctx, &bus);

    hal_time_t time;
    hal_time_fake_init(&time);

    hal_log_t log;
    hal_log_stdio_init(&log);

    sensor_t s;
    sensor_status_t st = sensor_init(&s, 0x50, &bus, &time, &log);

    TEST_ASSERT(st == SENSOR_OK);
}

/*
    Test 2 : sensor_init doit échouer avec SENSOR_BAD_ID si WHO_AM_I est faux.
*/
static void test_init_bad_id(void)
{
    hal_bus_t bus;
    hal_bus_fake_ctx_t bus_ctx;
    hal_bus_fake_init(&bus_ctx, &bus);

    /*
        On modifie directement le registre WHO_AM_I du capteur simulé
        pour simuler "mauvais composant branché".
    */
    bus_ctx.regs[REG_WHO_AM_I] = 0x00; // volontairement faux

    hal_time_t time;
    hal_time_fake_init(&time);

    hal_log_t log;
    hal_log_stdio_init(&log);

    sensor_t s;
    sensor_status_t st = sensor_init(&s, 0x50, &bus, &time, &log);

    TEST_ASSERT(st == SENSOR_BAD_ID);
}

/*
    Test 3 : lecture température doit renvoyer une valeur plausible.
    Notre fake démarre vers 25.00°C et augmente.
*/
static void test_read_temperature_plausible(void)
{
    hal_bus_t bus;
    hal_bus_fake_ctx_t bus_ctx;
    hal_bus_fake_init(&bus_ctx, &bus);

    /* S'assurer que l'ID est bien correct (normalement déjà le cas) */
    bus_ctx.regs[REG_WHO_AM_I] = EXPECTED_ID;

    hal_time_t time;
    hal_time_fake_init(&time);

    hal_log_t log;
    hal_log_stdio_init(&log);

    sensor_t s;
    sensor_status_t st = sensor_init(&s, 0x50, &bus, &time, &log);
    TEST_ASSERT(st == SENSOR_OK);

    int16_t temp = 0;
    st = sensor_read_temperature_centi(&s, &temp);
    TEST_ASSERT(st == SENSOR_OK);

    /*
        Température plausible :
        - on s'attend à être autour de 25°C => 2500 centi-degrés
        - on accepte une plage large pour éviter des tests trop fragiles
    */
    TEST_ASSERT(temp > 1500);  // > 15.00°C
    TEST_ASSERT(temp < 6000);  // < 60.00°C
}

int main(void)
{
    printf("=== Running sensor tests ===\n");

    test_init_ok();
    test_init_bad_id();
    test_read_temperature_plausible();

    printf("Tests run: %d\n", g_tests_run);
    printf("Tests failed: %d\n", g_tests_failed);

    /*
        Convention CI :
        - return 0 si succès
        - return 1 si au moins un test a échoué
    */
    return (g_tests_failed == 0) ? 0 : 1;
}
