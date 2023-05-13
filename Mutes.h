#ifndef PROJETCPPQT_MUTES_H
#define PROJETCPPQT_MUTES_H
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QRandomGenerator>
#define VITESSE_ENNEMI 5

class Mutes : public QGraphicsPixmapItem {
private:
    int OutOfBound = 0; // pour savoir si cet ennemi est dans la zone de jeu (actualisé à chaque update)
    int GoinR = 0; // si set, alors l'ennemi est en train d'avancer vers la droite
    int GoinL = 0; // pareil pour la gauche
    int vitesse = 0;
    int hauteur = 0;

public:
    Mutes(QString imageFileName, int R, int vitesse, int hauteur) : QGraphicsPixmapItem(QPixmap(imageFileName)), vitesse(vitesse), hauteur(hauteur) {
        if ( R == 1 ) { // ici R veut dire Right, si l'ennemi apparait à droite alors il va vers la gauche
            GoinL = 1;
            this->setPos(5760+307,+1613-300*hauteur); // ici on utilise des valeurs absolues adaptées aux dimensions de l'environnement dans lequel le personnage évolue
        }
        else {
            GoinR = 1;
            this->setPos(0-307,+1613-300*hauteur);
        }
    }

    void LeftToRight();
    void RightToLeft();

    // getters & setters :
    int getOOB();
    void setOOB(int boule);

    int getGoinR();
    int getGoinL();
};

#endif //PROJETCPPQT_MUTES_H
