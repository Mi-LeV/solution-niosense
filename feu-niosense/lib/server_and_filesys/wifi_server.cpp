#include "wifi_server.h"
#include "comm_nrf24.h"

// Set web server port number to 80
WebServer server(PORT);
// Static adress
IPAddress apIP(192, 168, 1, 1);

/*** VARIABLES ***/
String index_html = "";
JsonDocument jsonDocument;
uint8_t etat_btn = STOP;
int vitesse1_desire = 0, vitesse2_desire = 0, distance_desire = 10;

// Déclarés dans le module NRF...
extern bool status1, status2;
extern int nb_deconnexions_1, nb_deconnexions_2;
extern SlavePayloadStruct slave_payload[NB_SLAVES];

//declare dans le main
extern uint8_t current_node;

struct run_time_t{
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
}run_time;

void init_server(void){
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(SSID_AP, PASSWORD);
    delay(100);

    if(!LittleFS.begin(1)){
        index_html = "An Error has occurred while mounting LittleFS";
    }else{
        index_html = ouvrir_fichier(PAGE_WEB);
    }

    server.on ("/", handle_webpage);
    server.on ("/start", HTTP_POST, handle_start);
    server.on ("/pause", HTTP_POST, handle_pause);
    server.on ("/stop", HTTP_POST, handle_stop);
    server.on ("/vit1", HTTP_POST, handle_vitesse1);
    server.on ("/vit2", HTTP_POST, handle_vitesse2);
    server.on ("/dist", HTTP_POST, handle_distance);
    server.on ("/clear", HTTP_POST, handle_clear);
    server.on ("/time", HTTP_GET, handle_time);
    server.on ("/data", HTTP_GET, handle_data);
    server.on ("/download", HTTP_GET, handle_download);
    server.onNotFound(handle_404);

    // Démarre le serveur web
    server.begin();
}

void handle_webpage(void){
    server.send(200, "text/html", index_html);
}

// Adapté de : https://microcontrollerslab.com/esp32-rest-api-web-server-get-post-postman/
void handle_start(void){
    // Démarrer le timer
    TimerLib.setInterval_s(update_timer, 1);

    JsonDocument doc;
    String ligne = new_line(START);
    char response[100];

    doc["new_line"] = ligne;
    serializeJson(doc, response);
    server.send(200, "application/json", response);

    etat_btn = START;
    Serial.printf("Etat : %d\n", etat_btn);
    appendFile(FICHIER_LOG, ligne.c_str());
}

void handle_pause(void){
    // Arrêter le timer
    TimerLib.clearTimer();

    JsonDocument doc;
    String ligne = new_line(PAUSE);
    char response[100];

    doc["new_line"] = ligne;
    serializeJson(doc, response);
    server.send(200, "application/json", response);

    etat_btn = PAUSE;
    Serial.printf("Etat : %d\n", etat_btn);
    appendFile(FICHIER_LOG, ligne.c_str());
}

void handle_stop(void){
    JsonDocument doc;
    String ligne = new_line(STOP);
    char response[100];

    doc["new_line"] = ligne;
    serializeJson(doc, response);
    server.send(200, "application/json", response);

    // Arrêter le timer et remettre le compte à 0
    TimerLib.clearTimer();
    run_time.hours = 0;
    run_time.minutes = 0;
    run_time.seconds = 0;

    etat_btn = STOP;
    Serial.printf("Etat : %d\n", etat_btn);
    appendFile(FICHIER_LOG, ligne.c_str());
}

void handle_clear(void){
    writeFile(FICHIER_LOG, "");
    Serial.println("Fichier log supprimé!");
    server.send(200, "application/json", "{}");
}

void handle_download(void){
    String fichier_log = ouvrir_fichier(FICHIER_LOG);
    server.send(200, "application/blob", fichier_log);
}

void handle_time(void){
    JsonDocument doc;
    char response[100];

    doc["hours"] = run_time.hours;
    doc["minutes"] = run_time.minutes;
    doc["seconds"] = run_time.seconds;
    doc["state"] = etat_btn;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}

void handle_vitesse1(void){
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        deserializeJson(jsonDocument, body);

        vitesse1_desire = jsonDocument["vit1"];
        Serial.print("Vitesse1 : ");
        Serial.println(vitesse1_desire);

        JsonDocument doc;
        String ligne = new_line(VIT1_CHANGED);
        char response[100];

        doc["new_line"] = ligne;
        serializeJson(doc, response);
        server.send(200, "application/json", response);

        appendFile(FICHIER_LOG, ligne.c_str());
    }
}

void handle_vitesse2(void){
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        deserializeJson(jsonDocument, body);

        vitesse2_desire = jsonDocument["vit2"];
        Serial.print("Vitesse2 : ");
        Serial.println(vitesse2_desire);

        JsonDocument doc;
        String ligne = new_line(VIT2_CHANGED);
        char response[100];

        doc["new_line"] = ligne;
        serializeJson(doc, response);
        server.send(200, "application/json", response);

        appendFile(FICHIER_LOG, ligne.c_str());
    }
}

void handle_distance(void){
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        deserializeJson(jsonDocument, body);

        distance_desire = jsonDocument["dist"];
        Serial.print("Distance : ");
        Serial.println(distance_desire);

        JsonDocument doc;
        String ligne = new_line(DIST_CHANGED);
        char response[100];

        doc["new_line"] = ligne;
        serializeJson(doc, response);
        server.send(200, "application/json", response);

        appendFile(FICHIER_LOG, ligne.c_str());
    }
}

void handle_data(void){
    JsonDocument doc;
    char response[100];

    /*doc["status1"] = status1;
    doc["status2"] = status2;
    doc["nb_decon_1"] = nb_deconnexions_1;
    doc["nb_decon_2"] = nb_deconnexions_2;*/
    doc["status1"] = slave_payload[0].connection_status;
    doc["status2"] = slave_payload[1].connection_status;
    doc["nb_decon_1"] = nb_deconnexions_1;
    doc["nb_decon_2"] = nb_deconnexions_2;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}

void handle_404(void) {
    String message =   "<!DOCTYPE html>\
                        <html>\
                            <title>Banc d'essai</title>\
                            <body style=\"color:white; background-color:black\">\
                                <h1 align=\"center\">404 not found!</h1>\
                            </body>\
                        </html>";
    server.send(404, "text/html", message);
}

void handle_client(void){
    // Gère toutes les requêtes entrantes.
    server.handleClient();
}

void disconnect_server(void){
    server.stop();
}

void update_timer(void){
    if(++run_time.seconds > 59){
        run_time.seconds = 0;
        if(++run_time.minutes > 59){
            run_time.minutes = 0;
            if(++run_time.hours > 99){
                run_time.hours = 0;
            }
        }
    }
    // pour test
    Serial.printf("%02d:%02d:%02d\n", run_time.hours, run_time.minutes, run_time.seconds);
}

String new_line(event_t event){
    String str;
    char temps[10];
    sprintf(temps, "%02d:%02d:%02d\t", run_time.hours, run_time.minutes, run_time.seconds);
    str = temps;
    switch(event){
        case START:
            if(etat_btn == PAUSE)
                str += "Reprise du test\n";
            else
                str += "Début du test\n";
            break;
        case PAUSE:
            str += "Test mis sur pause\n";
            break;
        case STOP:
            str += "Fin du test\n"; // ou peut-être fin du test?
            break;
        case VIT1_CHANGED:
            str += "Vitesse du camion 1 modifiée à : " + String(vitesse1_desire) + "%\n";
            break;
        case VIT2_CHANGED:
            str += "Vitesse du camion 2 modifiée à : " + String(vitesse2_desire) + "%\n";
            break;
        case DIST_CHANGED:
            str += "Distance modifiée à : " + String(distance_desire) + "cm\n";
            break;
        case CAMION_BY_FEU:
            str += "Le camion " + String(current_node) + "est passe par le feu\n";
            break;
    }
    return str;
}