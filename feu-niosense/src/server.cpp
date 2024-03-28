#include "server.h"

// Set web server port number to 80
WebServer server(PORT);
// Static adress
IPAddress apIP(192, 168, 1, 1);

String index_html = "";

void init_server(void){
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(SSID, PASSWORD);
    delay(100);

    index_html = ouvrir_fichier(NOM_FICHIER);

    server.on ("/", handle_webpage);
    server.onNotFound(handle_404);

    // Démarre le serveur web
    server.begin();
}

void handle_webpage(void){
    server.send(200, "text/html", index_html);
}

void handle_404(void) {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void handle_client(void){
    //Gère toutes les requêtes entrantes.
    server.handleClient();
}

void disconnect_server(void){
    server.stop();
}