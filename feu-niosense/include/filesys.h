#ifndef FILESYS_H
#define FILESYS_H

#include <Arduino.h>
#include <LittleFS.h>

String ouvrir_fichier(String nom_fichier);
void appendFile(const char * name_file, const char * message);
void writeFile(const char * name_file, const char * message);
#endif