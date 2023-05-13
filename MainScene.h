#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QRectF>

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QVector>
#include <QCoreApplication>
#include <cstring>
#include <QApplication>

//Sound support :
#include <QSound>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "Player.h"
#include "Mutes.h"
#include "Arrows.h"

#define INVINCIBILITE 3000
// 3000 = 3 sec

#define ARROW_RATE 500
// 500 = une flèche toutes les demi secondes

class MainScene : public QGraphicsScene {

Q_OBJECT

private :

    QTimer* timer;
    Player* perso;

    int NoCrouchWhileFlying = 0; // fix pour le bug qui faisait traverser le sol quand on maintenait S en sautant
    int CantCrouch = 0;

    char* salt = (char*)__TIME__; //élément en + pour le stockage du highscore

    char createTimehold[32]; // endroit où stocker la valeur de retour de createTime

    int fWon = 0; // drapeau levé quand la partie est gagnée (pour continuer le timer et faire clignoter  le highscore)

    int highscore = 0; // contient la valeur du highscore venant du fichier

    QGraphicsSimpleTextItem* decompte; // décompte avant victoire
    QGraphicsSimpleTextItem* printhigh; // highscore avant victoire
    QGraphicsSimpleTextItem* restart; // highscore avant victoire
    QGraphicsSimpleTextItem* quit; // highscore avant victoire
    QFont* chronof;

    QString applicationPath = qApp->applicationDirPath();
    QMediaPlayer* SoundTrack;

    int timeout = 0; // timestamp d'une touche par un ennemi

    int xdiff = 0; // l'abscisse du perso si elle varie beaucoup
    int ymoy = 0; // hauteur moyenne du perso
    QVector<QPointF> histPos;

    // images
    QPixmap background;
    QPixmap gover;

    QGraphicsPixmapItem* backgroundP;
    QGraphicsPixmapItem* over;
    QGraphicsPixmapItem* win;
    QGraphicsPixmapItem* full;
    QGraphicsPixmapItem* heart;

    // figures géométriques faisant office de sol sur lequel marcher
    QGraphicsRectItem* bottomsol;
    QGraphicsRectItem* solg;
    QGraphicsRectItem* sold;
    QGraphicsRectItem* mid;

    // ennemis
    int deltaT = 0;
    int t=0; // temps en ms
    int niveau=1; // niveau des ennemis (quotient de génération de nouveaux ennemis)

    // fleches
    int lastSent = 0;

public:
    MainScene();
    void drawBackground(QPainter* painter, const QRectF& rect);
    virtual ~MainScene();
    void generateur(); // fait apparaitre les ennemis
    void eupdate(); // et les fait se déplacer
    void generateurArrow(); // envoie une flèche
    void aupdate(); // met à jour la position des flèches
    int IsItIB(QPointF posi); // return 1 si l'objet est dans le background, sinon 0
    void GameStatusHandler(int playerLP); // Gestionnaire de changement de musique, d'écran de fin et des points de vie, et bruitages
    void GameWin(); //
    QString createTime(int sec, int cent);
    void WriteHigh(); // writes highscore to .highscore
    void Ristarte(); // envoie le signal triggered
    void Kwouite(); // envoie le signal quitsig

public slots :
    void update();
signals :
    void triggered(); // signal pour restart la partie
    void quitsig(); // signal pour la quitter
protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

};

#endif // MYSCENE_H
