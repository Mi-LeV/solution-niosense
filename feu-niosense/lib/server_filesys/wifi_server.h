#ifndef WIFI_SERVER_H
#define WIFI_SERVER_H

//#include <Arduino.h>
#include "includes.h"

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <uTimerLib.h>
#include "filesys.h"


#define SSID_AP "ELE400_AP"
#define PASSWORD "vert_le_futur" // le mot de passe doit être d'au moins 8 caractères
#define PORT 80
#define PAGE_WEB "/index.htm"
#define FICHIER_LOG "/banc_essai.log"


void init_server(void);
void handle_webpage(void);
void handle_start(void);
void handle_pause(void);
void handle_stop(void);
void handle_vitesse1(void);
void handle_vitesse2(void);
void handle_distance(void);
void handle_data(void);
void handle_clear(void);
void handle_time(void);
void handle_download(void);
void handle_404(void);
void handle_client(void);
void disconnect_server(void);
void update_timer(void);
void handle_algo(void);
String new_line(event_t event);

#endif