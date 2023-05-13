#ifndef PROJETCPPQT_PLAYER_H
#define PROJETCPPQT_PLAYER_H

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <cmath>

#include "Mutes.h"
#include "Arrows.h"

#define VITESSE_ARROW 49
#define VITESSE_PERSO 50
#define BONUS_SEC 1000

class Player : public QGraphicsPixmapItem {

private :
    int LP = 3; // Life Points
    int speed; // vitesse pour le saut
    int lookin_r = 0; // utile pour le sprite du perso, savoir s'il regarde à gauche ou droite

    QVector<QGraphicsItem*> collList; // liste des éléments à collisions
    QVector<Mutes*> EcollList; // liste des ennemis à collisions
    QVector<Arrows*> AcollList; // liste des ennemis à collisions

    int jmpdr = 0;
    int jmpdl = 0;

    int crouched = 0; // allongé = 1, 0 = debout
    int otherFoot = 0; // flag pour savoir si le perso doit changer de pied, le drapeau est levé toutes les 0.2 secondes
    int footToSwap = 0; // pour savoir si le perso a changé de pied

    //Variables saut :
    int posx=200;
    int posy=0;
    const double g = 9.81;
    const double pi = 3.14;
    int v_init = 2;
    int angle_init = pi/3;
    int t = 0;
    double v_x = cos(angle_init)*v_init;
    double v_y = sin(angle_init)*v_init;
    // Saut grandement tiré de http://sdz.tdct.org/sdz/les-sauts-2d-en-c.html mais grandement repris et adapté à mes besoins


public :
    Player(QString imageFileName) : QGraphicsPixmapItem(QPixmap(imageFileName)) {
        this->setPos(2000,10);
        this->speed = VITESSE_PERSO;
    }
    void droite(); // bouge le sprite du perso
    void gauche();

    void toggleFoot();

    void crouch();
    void releve();
    int getCorD() const{return crouched;}

    void jump(); // initialise les variables de saut et fais le premier pas du saut pour décoler du sol
    void jumpboucle(); // effectue les mouvements du saut en fonction de la direction et de jmpdr et jmpdl
    int collision(QGraphicsItem* coll); // fonctionne avec collcheck
    void add_collList(QGraphicsItem* newcoll); // ajoute des éléments à collList

    int collcheck(); // methode pour verifier si on collidewith un element de la liste des collisions collList

    void stillr(); // reinitialise jmpdr quand D est laché
    void stilll(); // pareil pour jmpdl quand Q est laché

    // enemy part :
    int ecollcheck();
    int ecollision(Mutes* coll); // fonctionne avec ecollcheck
    void add_ecollList(Mutes* newcoll); // ajoute des éléments à ecollList
    const QVector<Mutes*> getEnemies(); // liste des éléments à collisions
    void deleteE(int* index);
    void hideE();
    void raiseOOB(int index);
    int perdVie();


    // arrow part :
    int acollcheck();
    int acollision(Arrows* coll,Mutes* ennemy); // fonctionne avec ecollcheck
    void add_acollList(Arrows* newcoll); // ajoute des éléments à ecollList
    const QVector<Arrows*> getArrows(); // liste des éléments à collisions
    void deleteA(int* index);
    void hideA();
    void AraiseOOB(int index);

    void aupdate() ;
    QGraphicsPixmapItem* generateurArrow();
    int IsItIB(QPointF posi); // Est-ce la flèche est dans la zone (IB in bound)
};

#endif //PROJETCPPQT_PLAYER_H
