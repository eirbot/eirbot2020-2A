# Eirbium - Coupe de France de Robotique

## Présentation

Ce projet contient les sources en `C++` du code développé par l'équipe de
première année de l'association de robotique **Eirbot** de Septembre 2018 à
Juin 2019. Il est destiné à fonctionner sur une carte `Nucléo F429ZI` et
contrôler le robot au moyen de deux cartes de puissances.

## Le projet

Le code se divise en plusieurs briques. Au plus bas niveau, le fichier
*position.cpp* s'occupe de traiter les signaux en provenance des encodeurs afin
d'établir une odométrie pour le robot.

Sur la couche au dessus, le fichier *pid.cpp* permet de modeliser simplement un
asservissement. Nous avons implémenté un asservissement en position, en angle et
en vitesse.

Au plus haut niveau, le fichier *navigateur.cpp* s'occupe de permettre au robot
de se déplacer en fournissant les bonnes consignes aux moteurs.
