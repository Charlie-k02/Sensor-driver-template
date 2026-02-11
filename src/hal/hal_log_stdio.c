/*
    hal_log_stdio.c

    Logger simple basé sur stdio (printf/fprintf).

    - INFO/WARN vont sur stdout
    - ERR va sur stderr
*/

#include "hal/hal_log_stdio.h"
#include <stdio.h>   // printf, fprintf, vfprintf
#include <stdarg.h>  // va_list, va_start, va_end

/*
    Convertit le niveau de log en texte (préfixe lisible).
*/
static const char *level_to_str(hal_log_level_t level)
{
    switch (level) {
        case HAL_LOG_INFO: return "INFO";
        case HAL_LOG_WARN: return "WARN";
        case HAL_LOG_ERR:  return "ERR";
        default:           return "LOG";
    }
}

/*
    Fonction de log "host".

    Signature compatible avec hal_log_t->log:
      void (*log)(void *ctx, hal_log_level_t level, const char *fmt, ...);

    - ctx : non utilisé ici
    - level : niveau du message
    - fmt, ... : message formaté (comme printf)
*/
static void host_log(void *ctx, hal_log_level_t level, const char *fmt, ...)
{
    (void)ctx; // pas utilisé sur host

    /*
        Choix du flux :
        - erreurs -> stderr
        - le reste -> stdout
    */
    FILE *out = (level == HAL_LOG_ERR) ? stderr : stdout;

    // Préfixe [INFO], [WARN], [ERR]
    fprintf(out, "[%s] ", level_to_str(level));

    // Gérer la liste d'arguments variadiques
    va_list args;
    va_start(args, fmt);
    vfprintf(out, fmt, args);
    va_end(args);

    // Retour ligne pour que ce soit lisible
    fprintf(out, "\n");
}

/*
    Initialise la HAL log stdio.

    Remplit :
    - ctx (NULL)
    - log (pointeur vers host_log)
*/
void hal_log_stdio_init(hal_log_t *log)
{
    if (!log) {
        return;
    }

    log->ctx = NULL;
    log->log = host_log;
}
