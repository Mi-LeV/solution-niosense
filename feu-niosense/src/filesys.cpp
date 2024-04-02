#include "filesys.h"

String ouvrir_fichier(String nom_fichier){
  String str;
  File file = LittleFS.open(nom_fichier);
  if(!file || file.isDirectory()){
    return "Failed to open file " + nom_fichier + " for reading";
  }
  //tant qu'on n'a pas atteint la fin du fichier
  while(file.available()){
    //lire le fichier
    str += (char)file.read();
  }
  //fermer le fichier
  file.close();
  return str;
}

void appendFile(const char * name_file, const char * message){
    Serial.printf("Appending to file: %s\r\n", name_file);

    File file = LittleFS.open(name_file, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    file.close();
}

void writeFile(const char * name_file, const char * message){
    Serial.printf("Writing file: %s\r\n", name_file);

    File file = LittleFS.open(name_file, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}