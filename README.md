
# Sensor Driver Template (C) — HAL + Simulated Sensor

Projet pédagogique en C qui montre comment structurer un **driver de capteur** de manière propre et portable grâce à une **HAL (Hardware Abstraction Layer)**.  
Le capteur est **simulé** via un bus fake “register-based”, ce qui permet de développer et tester **sans matériel**.

## Objectifs

- Séparer proprement **driver** et **dépendances hardware** (bus, temps, logs)
- Pouvoir exécuter le driver sur **macOS/PC** grâce à une implémentation **host**
- Fournir une base “portfolio-ready” (structure, build, demo, tests)

## Architecture

Le driver ne connaît pas la plateforme. Il dépend uniquement d’interfaces HAL :

- **HAL Bus** : lecture/écriture de registres (I²C/SPI abstrait)
- **HAL Time** : `delay_ms()`
- **HAL Log** : logs (INFO/WARN/ERR)

Pour exécuter sans capteur réel, on fournit :

- **Fake Bus** : tableau de registres simulés + valeur de température qui évolue

## Structure du projet

