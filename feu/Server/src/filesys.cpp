#include "filesys.h"

String ouvrir_fichier(String nom_fichier){
  String str;
  if(!LittleFS.begin(1)){
    return "An Error has occurred while mounting LittleFS";
  }
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

