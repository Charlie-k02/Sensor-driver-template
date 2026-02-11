#pragma once
/*
    hal_log.h

    Interface HAL pour les logs/debug.

    Permet d'afficher des messages sans
    forcer printf ou un UART spécifique.
*/

/*
    Niveaux de log standards.
*/
typedef enum {
    HAL_LOG_INFO,  // Information générale
    HAL_LOG_WARN,  // Avertissement
    HAL_LOG_ERR    // Erreur critique
} hal_log_level_t;

/*
    Structure HAL pour le logging.
*/
typedef struct {

    /*
        Contexte utilisateur.

        Peut contenir :
        - UART handle
        - fichier log
        - stdout
    */
    void *ctx;

    /*
        Fonction de log.

        fmt est une string formatée type printf.
    */
    void (*log)(
        void *ctx,
        hal_log_level_t level,
        const char *fmt,
        ...
    );

} hal_log_t;
