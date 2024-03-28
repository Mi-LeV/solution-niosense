#ifndef SERVER_H
#define SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <uTimerLib.h>
#include "filesys.h"

#define SSID "ELE400_AP"
#define PASSWORD "vert_le_futur" // le mot de passe doit être d'au moins 8 caractères
#define PORT 80
#define NOM_PAGE_WEB "/index.htm"
#define NOM_FICHIER_LOG "/banc_essai.log"

// d'autres événements peuvent être ajoutés
typedef enum { START = 0, STOP, RESET, VIT1_CHANGED, VIT2_CHANGED, DIST_CHANGED } event_t;

void init_server(void);
void handle_webpage(void);
void handle_start(void);
void handle_stop(void);
void handle_reset(void);
void handle_vitesse1(void);
void handle_vitesse2(void);
void handle_distance(void);
void handle_404(void);
void handle_client(void);
void disconnect_server(void);
void timer(void);
String new_line(event_t event);

#endif