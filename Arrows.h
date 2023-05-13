#ifndef PROJETCPPQT_ARROWS_H
#define PROJETCPPQT_ARROWS_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QRandomGenerator>

class Arrows : public QGraphicsPixmapItem {
private:
    int OutOfBound = 0; // pour savoir si cette flèche est dans la zone de jeu (actualisé à chaque update)
    int GoinR = 0; // si set, alors la flèche est en train d'avancer vers la droite
    int GoinL = 0; // pareil pour la gauche
    int vitesse = 0;
    QPointF posJ;

public:
    Arrows(QString imageFileName, int R, int vitesse, QPointF posJ) : QGraphicsPixmapItem(QPixmap(imageFileName)), vitesse(vitesse), posJ(posJ) {
        this->setPos(posJ.x(),posJ.y()+200); // la fleche part de la position du joueur
        ( R == 1 ) ? GoinL = 1 : GoinR = 1;
    }

    void LeftToRight();
    void RightToLeft();

    // getters & setters :
    int getOOB(); // OOB pour Out Of Bound
    void setOOB(int boule);

    int getGoinR();
    int getGoinL();


};

#endif //PROJETCPPQT_ARROWS_H
