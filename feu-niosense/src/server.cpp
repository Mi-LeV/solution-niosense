#include "server.h"

// Set web server port number to 80
WebServer server(PORT);
// Static adress
IPAddress apIP(192, 168, 1, 1);

/*** VARIABLES ***/
String index_html = "";
JsonDocument jsonDocument;
bool start = false;
bool stop = true;
bool reset = false;
int vitesse1 = 0, vitesse2 = 0, distance = 10;

struct run_time_t{
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
}run_time;

void init_server(void){
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(SSID, PASSWORD);
    delay(100);

    index_html = ouvrir_fichier(NOM_PAGE_WEB);

    server.on ("/", handle_webpage);
    server.on ("/start", HTTP_POST, handle_start);
    server.on ("/stop", HTTP_POST, handle_stop);
    server.on ("/reset", HTTP_POST, handle_reset);
    server.on ("/vit1", HTTP_POST, handle_vitesse1);
    server.on ("/vit2", HTTP_POST, handle_vitesse2);
    server.on ("/dist", HTTP_POST, handle_distance);
    server.onNotFound(handle_404);

    // Démarre le serveur web
    server.begin();
}

void handle_webpage(void){
    server.send(200, "text/html", index_html);
}

// From : https://microcontrollerslab.com/esp32-rest-api-web-server-get-post-postman/
void handle_start(void){
    if (server.hasArg("plain")) {
        start = true;
        stop = false;
        reset = false;
        Serial.print("Start : ");
        Serial.println(start);

        // Démarrer le timer
        TimerLib.setInterval_s(timer, 1);

        JsonDocument doc;
        String ligne = new_line(START);
        char response[100];

        doc["new_line"] = ligne;
        serializeJson(doc, response);
        server.send(200, "application/json", response);

        appendFile(NOM_FICHIER_LOG, ligne.c_str());
    }
}

void handle_stop(void){
    if (server.hasArg("plain")) {
        stop = true;
        start = false;
        reset = false;
        Serial.print("Stop : ");
        Serial.println(stop);

        // Arrêter le timer
        TimerLib.clearTimer();

        JsonDocument doc;
        String ligne = new_line(STOP);
        char response[100];

        doc["new_line"] = ligne;
        serializeJson(doc, response);
        server.send(200, "application/json", response);

        appendFile(NOM_FICHIER_LOG, ligne.c_str());
    }
}

void handle_reset(void){
    if (server.hasArg("plain")) {
        reset = true;
        start = false;
        stop = false;
        Serial.print("Reset : ");
        Serial.println(reset);

        // Arrêter le timer et remettre le compte à 0
        TimerLib.clearTimer();
        run_time.hours = 0;
        run_time.minutes = 0;
        run_time.seconds = 0;

        JsonDocument doc;
        String ligne = new_line(RESET);
        char response[100];

        doc["new_line"] = ligne;
        serializeJson(doc, response);
        server.send(200, "application/json", response);

        appendFile(NOM_FICHIER_LOG, ligne.c_str());
    }
}

void handle_vitesse1(void){
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        deserializeJson(jsonDocument, body);

        vitesse1 = jsonDocument["vit1"];
        Serial.print("Vitesse1 : ");
        Serial.println(vitesse1);

        JsonDocument doc;
        String ligne = new_line(VIT1_CHANGED);
        char response[100];

        doc["new_line"] = ligne;
        serializeJson(doc, response);
        server.send(200, "application/json", response);

        appendFile(NOM_FICHIER_LOG, ligne.c_str());
    }
}

void handle_vitesse2(void){
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        deserializeJson(jsonDocument, body);

        vitesse2 = jsonDocument["vit2"];
        Serial.print("Vitesse2 : ");
        Serial.println(vitesse2);

        JsonDocument doc;
        String ligne = new_line(VIT2_CHANGED);
        char response[100];

        doc["new_line"] = ligne;
        serializeJson(doc, response);
        server.send(200, "application/json", response);

        appendFile(NOM_FICHIER_LOG, ligne.c_str());
    }
}

void handle_distance(void){
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        deserializeJson(jsonDocument, body);

        distance = jsonDocument["dist"];
        Serial.print("Distance : ");
        Serial.println(distance);

        JsonDocument doc;
        String ligne = new_line(DIST_CHANGED);
        char response[100];

        doc["new_line"] = ligne;
        serializeJson(doc, response);
        server.send(200, "application/json", response);

        appendFile(NOM_FICHIER_LOG, ligne.c_str());
    }
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

void timer(void){
    if(++run_time.seconds >= 60){
        run_time.seconds = 0;
        if(++run_time.minutes >= 60){
            run_time.minutes = 0;
            if(++run_time.hours >= 99){
                run_time.hours = 0;
            }
        }
    }
    // pour test
    Serial.printf("%02d:%02d:%02d\n", run_time.hours, run_time.minutes, run_time.seconds);
}

String new_line(event_t event){
    String str;
    char str2[10];
    sprintf(str2, "%02d:%02d:%02d\t", run_time.hours, run_time.minutes, run_time.seconds);
    str = str2;
    switch(event){
        case START:
            str += "Début du test\n";
            break;
        case STOP:
            str += "Test mis sur pause\n";
            break;
        case RESET:
            str += "Remise à 0 du minuteur de test\n"; // ou peut-être fin du test?
            break;
        case VIT1_CHANGED:
            str += "Vitesse du camion 1 modifiée à : " + String(vitesse1) + "\n";
            break;
        case VIT2_CHANGED:
            str += "Vitesse du camion 2 modifiée à : " + String(vitesse2) + "\n";
            break;
        case DIST_CHANGED:
            str += "Distance modifiée à : " + String(distance) + "\n";
            break;
    }
    return str;
}