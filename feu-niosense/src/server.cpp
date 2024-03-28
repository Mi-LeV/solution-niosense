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

    index_html = ouvrir_fichier(NOM_FICHIER);

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

        server.send(200, "application/json", "{}");
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

        server.send(200, "application/json", "{}");
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

        server.send(200, "application/json", "{}");
    }
}

void handle_vitesse1(void){
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        deserializeJson(jsonDocument, body);

        int vit1 = jsonDocument["vit1"];
        Serial.print("Vitesse1 : ");
        Serial.println(vit1);

        server.send(200, "application/json", "{}");
    }
}

void handle_vitesse2(void){
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        deserializeJson(jsonDocument, body);

        int vit2 = jsonDocument["vit2"];
        Serial.print("Vitesse2 : ");
        Serial.println(vit2);

        server.send(200, "application/json", "{}");
    }
}

void handle_distance(void){
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        deserializeJson(jsonDocument, body);

        int dist = jsonDocument["dist"];
        Serial.print("Distance : ");
        Serial.println(dist);

        server.send(200, "application/json", "{}");
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