# Code MCU feu

4 etats possibles : 

1. Attendre debut test
2. Initialisation du test
3. Boucle du test
4. Fin du test

## 1. Attendre debut test
- Gerer l'entree du scenario + parametres
- Gerer les commandes utilisateur

## 2. Debut test 
- Recuperer+ stocker le scenario + parametres
- Initialiser une entree de journal

- Envoyer message de debut d'initialisation aux MCUs camions
- Attendre pour toutes les messages de fin d'initialisation des MCUs camions

## 3. Boucle du test
- Lire l'etape du scenario

- Calculer la presence d'interferences et la distance feu-camions
- SI la communication est possible :
  Envoyer un ping aux camions concernes
  Attendre le ping des camions concernes
  
- Calculer l'etat du feu
- Allumer les LEDS
- Envoyer l'etat logique du feu aux camions
- Attendre la position des camions

- Envoyer les variables utiles dans le journal

- Gerer les commandes utilisateur (STOP, PAUSE)

## 4. Arret du test
- Envoyer message arret aux camions

- Terminer l'entree de journal
Calculer le reultat a partir des logs
Presenter le resultat a l'utilisateur
