#include "Player.h"
#include <QPointF>

void Player::droite(){

    if (footToSwap) {
        if(otherFoot) {
            this->setPixmap(QPixmap(":images/rbworkright.png"));
            footToSwap = 0;
        }
        else {
            this->setPixmap(QPixmap(":images/bworkright.png"));
            footToSwap = 0;
        }
    }
    lookin_r = 1;

    QPointF pos = this->pos();

    int newX = pos.x() + this->speed;

    this->setPos(newX, pos.y());

    // variables pour le saut
    jmpdr = 1;
    jmpdl = 0;

    // bordure droite
    QPointF pos2 = this->pos();
    if (pos2.x()+500 > this->scene()->width()) {
        this->setPos(this->scene()->width()-500, pos2.y());
    }
}

void Player::gauche(){

    if (footToSwap) {
        if(otherFoot) {
            this->setPixmap(QPixmap(":images/rbwork.png"));
            footToSwap = 0;
        }
        else {
            this->setPixmap(QPixmap(":images/bwork.png"));
            footToSwap = 0;
        }
    }
    lookin_r = 0;

    QPointF pos = this->pos();

    int newX = pos.x() - this->speed;

    this->setPos(newX, pos.y());

    // variables pour le saut
    jmpdr = 0;
    jmpdl = 1;

    // bordure gauche
    QPointF pos2 = this->pos();
    if (pos2.x() < 0) {
        this->setPos(0, pos2.y());
    }
}

void Player::toggleFoot(){ // bug tp qd change de direction apres saut ....
    if (!otherFoot){
    otherFoot = 1;
    footToSwap = 1;
    }
    else {
    otherFoot = 0;
    footToSwap = 1;
    }
}

void Player::crouch(){
    this->crouched = 1;
    if ( this->lookin_r == 1){
        this->setPixmap(QPixmap(":images/cbworkright.png"));
    }else {
        this->setPixmap(QPixmap(":images/cbwork.png"));
    }
    this->setPos(this->pos().x(),this->pos().y()+250);
}

void Player::releve(){
    this->crouched = 0;
    if ( this->lookin_r == 1){
        this->setPixmap(QPixmap(":images/bworkright.png"));
    }else {
        this->setPixmap(QPixmap(":images/bwork.png"));
    }
    this->setPos(this->pos().x(),this->pos().y()-250);
}

void Player::stillr(){ // Ces méthodes servent à savoir si droite ou gauche est maintenu, pour savoir si l'on doit sauter en statique ou vers la gauche ou la droite
    jmpdr = 0; // Elles sont utilisées dans le KeyReleaseEvent et l'attribut jmp* est utilisé dans le saut
}

void Player::stilll(){
    jmpdl = 0;
}
void Player::jump(){
    this->posx = this->pos().x();
    this->posy = this->pos().y();


    this->v_init = 2;
    this->angle_init = pi/3;
    this->t = 0;
    this->v_x = cos(angle_init)*v_init;
    this->v_y = sin(angle_init)*v_init;
    //this->posi = this->pos();
    if (lookin_r) // Ici je fais sauter le bwork dans sa forme originale seulement car sa variante est source de bug de collisions
        this->setPixmap(QPixmap(":images/bworkright.png"));
    else
        this->setPixmap(QPixmap(":images/bwork.png"));
    footToSwap = 0;
    jumpboucle();
}
int Player::collision(QGraphicsItem* coll){
    if (crouched == 0) {
        if (this->collidesWithItem(coll) &&
            (coll->pos().y() >= this->pos().y() - 1460)) { // checker juste si il touche la surface avec ses pieds
            return 1;
        }
        return 0;
    }
    if (this->pos().y() >= 1460) { // checker juste si il touche la surface avec ses pieds
        return 1;
    }
    return 0;
}

int Player::collcheck(){
    // Il boucle dans le vector des collisions et vérifie avec collision() si le perso touche un des objets du vector, cela retourne 0 si le perso ne touche pas d'objets du vector
    int nbcoll = 0;
    for (int i = 0; i < this->collList.size(); i++) {
        nbcoll += collision(this->collList[i]);
    }
    return nbcoll;
}

void Player::add_collList(QGraphicsItem* newcoll){
    this->collList.push_back(newcoll);
}

void Player::jumpboucle(){

    // si on saute à droite
    if( jmpdr == 1 ) {
        this->setPos(this->posx + (int) (v_x * t),
                     +900 - (int) ((v_y * t) - ((g * t * t) / 2000))); // 500 cest la hauteur de l'img du perso
    }

    // si on saute à gauche, le + se change en moins dans le setPos(x)
    if( jmpdl == 1 ) {
            this->setPos(this->posx - (int) (v_x * t),
                         +900 - (int) ((v_y * t) - ((g * t * t) / 2000))); // 500 cest la hauteur de l'img du perso
        }

    // on saute droit car les deux touches sont relachées
    if ( jmpdr == 0 && jmpdl == 0 )
    {
        this->setPos(this->posx ,
                     +900 - (int) ((v_y * t) - ((g * t * t) / 2000))); // 500 cest la hauteur de l'img du perso
    }

    // on bloque le depassement des bordures
    QPointF pos2 = this->pos();
    if (pos2.x() < 0) {
        this->setPos(0, pos2.y());
    }

    if (pos2.x()+500 > this->scene()->width()) {
        this->setPos(this->scene()->width()-500, pos2.y());
    }

    // le temps passe :
        t += 10;

}

int Player::ecollision(Mutes* coll){
    if (this->collidesWithItem(coll)){
        return 1;
    }
    return 0;
}

int Player::ecollcheck(){ // ici on reprend le concept du vector d'objets à collision mais pour les ennemis
    int nbcoll = 0;
    for (int i = 0; i < this->EcollList.size(); i++) {
        nbcoll += ecollision(this->EcollList[i]);
    }
    return nbcoll;
}

void Player::add_ecollList(Mutes* newcoll){
    this->EcollList.push_back(newcoll);
}

const QVector<Mutes*> Player::getEnemies(){
    return this->EcollList;
}

void Player::deleteE(int* index){
    // delete l'alloc et decrementer l'index de boucle for pour ne pas oublier d'element
    delete EcollList[*index];
    EcollList.erase(EcollList.begin()+(*index));
    (*index)--;
}

void Player::raiseOOB(int index){
    this->EcollList[index]->setOOB(index);
}

void Player::hideE(){
    for (int i = 0; i < this->EcollList.size(); i++) {
        EcollList[i]->hide();
    }
}

int Player::perdVie(){
   this->LP = LP - 1;
   return LP;
}

int Player::acollision(Arrows* coll, Mutes* ennemy){
    if (ennemy->collidesWithItem(coll)){ // check si la flèche touche l'ennemi
        return 1;
    }
    return 0;
}

int Player::acollcheck(){
    for (int i = 0; i < this->AcollList.size(); i++) { // on utilise une boucle imbriqué pour que chaque élément des deux Vector soit vérifié ensemble
        for (int j = 0; j < this->EcollList.size(); ++j) {
            if(acollision(this->AcollList[i],this->EcollList[j])){
                deleteE(&j); // si la flèche touche l'ennemi on efface l'ennemi
                return BONUS_SEC;
            }
        }
    }
    return 0;
}

void Player::add_acollList(Arrows* newcoll){
    this->AcollList.push_back(newcoll);
}

const QVector<Arrows*> Player::getArrows(){
    return this->AcollList;
}

void Player::deleteA(int* index){
    // delete l'alloc et decrementer l'index de boucle for pour ne pas oublier d'element
    delete AcollList[*index];
    AcollList.erase(AcollList.begin()+(*index));
    (*index)--;
}

void Player::AraiseOOB(int index){
    this->AcollList[index]->setOOB(index);
}

void Player::hideA(){
    for (int i = 0; i < this->AcollList.size(); i++) {
        AcollList[i]->hide();
    }
}

int Player::IsItIB(QPointF posi){
    if ( posi.x() < 0 || posi.x() > 5760 || posi.y() < 0 || posi.y() < 1920)  // pas sur des limites en Y, probable source de BUG
        return 0;
    else
        return 1;
}

void Player::aupdate() {
    QVector<Arrows*> alist = getArrows();
    for (int arrowIndex = 0; arrowIndex < alist.size(); arrowIndex++){
        // Raise les OOB pour clean ceux qui arrivent au bord de l'écran
        if (alist[arrowIndex]->getOOB() == 0 && !IsItIB(alist[arrowIndex]->pos()))
            AraiseOOB(arrowIndex);
        // Clean les OOB et fait avancer les ennemis
        if ( alist[arrowIndex]->getOOB() == 1 && arrowIndex > 2) // pour éviter de vider le vect et que ça bug
            deleteA(&arrowIndex);
        else if ( alist[arrowIndex]->getGoinL() == 1)
            alist[arrowIndex]->RightToLeft();
        else
            alist[arrowIndex]->LeftToRight();
    }
}

QGraphicsPixmapItem* Player::generateurArrow(){
    // droite ou gauche avec un chrono
        int direction = 0;
        if (lookin_r)
            direction = 0; // tirer à gauche ou droite
        else
            direction = 1; // tirer à droite

        // creation d'une nouvelle flèche
        // vitesse flèche

        if (direction) add_acollList(new Arrows(":images/larrow.png", direction, VITESSE_ARROW, this->pos()));
        else add_acollList(new Arrows(":images/rarrow.png", direction, VITESSE_ARROW, this->pos()));

        return (getArrows().back());
}
