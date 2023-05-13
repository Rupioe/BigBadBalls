#include "MainScene.h"

MainScene::MainScene() {
    SoundTrack = new QMediaPlayer; // musique de départ : berlinc.wav
    SoundTrack->setMedia(QMediaContent(QUrl::fromLocalFile(applicationPath + "/../dolby_atmos/berlinc.wav")));
    SoundTrack->setVolume(40);
    SoundTrack->play();

    // je verifie s'il y a un fichier highscore et si oui, son contenu :
    FILE* highfile = fopen(".highscore","rb");
    if(highfile != nullptr){
        // on lit d'abord les infos du fichier
        char currchar = 0;
        int afterhigh = 3;
        char readHigh[64]={0};
        int Hindex = 0;
        char readSig[64]={0};
        int Sindex = 0;

        // Dans le fichier les infos sont réparties comme suis :
        // (highscore en hexa) 0 0 0 (signature en hexa)
        // la signature c'est le highscore en base 10 * le nombre composé du __TIME__ à la compilation, par exemple pour 18:45:30, on a 184530)
        currchar = fgetc(highfile);
        while (currchar != EOF){
            if ( currchar == 0) afterhigh--;
            else {
                if (afterhigh){
                    readHigh[Hindex] = currchar;
                    Hindex++;
                }
                else {
                    readSig[Sindex] = currchar;
                    Sindex++;
                }
            }
            currchar = fgetc(highfile);
        }

        fclose(highfile);

        // ensuite on recalcule la signature pour voir si le highscore correspond à sa signature
        int readHighNb = (int)strtol(readHigh, nullptr,16);

        char cleansalt[7]={0};
        int index = 0;
        // Ici on strip __TIME__
        for ( int i = 0; i < strlen(salt); i++){
            if (salt[i] != ':'){
                cleansalt[index] = salt[i];
                index++;
            }

        }
        int highsig = readHighNb * (int)strtol(cleansalt, nullptr,10);
        char sigFromReadHigh[64]={0};
        sprintf(sigFromReadHigh,"%x",highsig);

        // ensuite on compare les signatures et si elles correspondent on continue le programme en prennant le readhighscore sinon on laisse highscore à 0
        if (!strcmp(sigFromReadHigh,readSig)) this->highscore = readHighNb;
    }

    // On met en place le background
    this->background.load(":images/background.jpg");
    this->backgroundP = new QGraphicsPixmapItem(QPixmap(":images/background.jpg"));
    this->addItem(backgroundP);
    backgroundP->setScale(1.5);
    backgroundP->setPos(backgroundP->pos().x(),backgroundP->pos().y()-500);
    this->setSceneRect(0, 0, background.width(), background.height());

    // le slot update sera exécuté toutes les 10 ms
    this->timer = new QTimer(this);
    this->timer->start(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    // J'instancie et je positionne mon perso principal
    this->perso = new Player(":images/bwork.png");
    this->addItem(perso);
    perso->setPos(this->width()-600,this->height()-500);

    // ici il y a mes objets à collisions
    this->solg = new QGraphicsRectItem(0,perso->pos().y()+500,1600,100);
    this->addItem(solg); // plateforme gauche
    // je les ajoute à la liste des collisions
    perso->add_collList(solg);

    this->mid = new QGraphicsRectItem(2100,perso->pos().y()+500,900,100);
    this->addItem(mid); // plateforme du milieu
    perso->add_collList(mid);

    this->sold = new QGraphicsRectItem(3500,perso->pos().y()+500,2600,100);
    this->addItem(sold);
    perso->add_collList(sold); // j'ajoute chaque élément à collision dans un vecteur de mon personnage

    this->bottomsol = new QGraphicsRectItem(0,+4000,4000,400);
    this->addItem(bottomsol); // sol qui fait mourir quand on tombe
    perso->add_collList(bottomsol);

    solg->setBrush(QColor(11,237,237,255));
    sold->setBrush(QColor(11,237,237,255));
    mid->setBrush(QColor(11,237,237,255));

    // écran de fins
    this->over = new QGraphicsPixmapItem(QPixmap(":images/gameover.jpg"));
    this->addItem(over);
    over->hide();
    over->setScale(2.5);
    this->win = new QGraphicsPixmapItem(QPixmap(":images/gamewin.jpg"));
    this->addItem(win);
    win->hide();
    win->setScale(2.1);

    // points de vie, rempli = 3, vide = 2, et plus de coeur = 1, on meurt à 0
    this->heart = new QGraphicsPixmapItem(QPixmap(":images/heart2.png"));
    this->addItem(heart);
    heart->hide();
    this->full = new QGraphicsPixmapItem(QPixmap(":images/full2.png"));
    this->addItem(full);

    // Style des écritures
    this->chronof = new QFont();
    chronof->setStyleHint(QFont::OldEnglish);
    chronof->setLetterSpacing(QFont::AbsoluteSpacing,4);

    // current time :
    this->decompte = new QGraphicsSimpleTextItem();
    decompte->setText("60:00");
    decompte->setFont(*chronof);
    decompte->setScale(20);
    decompte->setBrush(QBrush(QColor(250,246,0,255)));
    this->addItem(decompte);
    decompte->setPos(2100,500);

    // highscore :
    this->printhigh = new QGraphicsSimpleTextItem();
    printhigh->setText(createTime(highscore / 1000, highscore % 1000));
    printhigh->setFont(*chronof);
    printhigh->setScale(20);
    printhigh->setBrush(QBrush(QColor(Qt::green)));
    this->addItem(printhigh);
    printhigh->setPos(2100,200);

    // restart prompt :
    this->restart = new QGraphicsSimpleTextItem();
    restart->setText("Click [R] to RESTART !!!");
    restart->setFont(*chronof);
    restart->setScale(10);
    restart->setBrush(QBrush(QColor(Qt::red)));
    this->addItem(restart);
    restart->hide();

    // quit prompt :
    this->quit = new QGraphicsSimpleTextItem();
    quit->setText("Click [K] to QUIT !!!");
    quit->setFont(*chronof);
    quit->setScale(10);
    quit->setBrush(QBrush(QColor(Qt::darkYellow)));
    this->addItem(quit);
    quit->hide();
}

void MainScene::drawBackground(QPainter *painter, const QRectF &rect) {
    painter->drawPixmap(QRectF(0,0,background.width(), background.height()), background, sceneRect());
}

int MainScene::IsItIB(QPointF posi){ // savoir si le point donné est dans les limites du background
   if ( posi.x() < 0 || posi.x() > background.width() || posi.y() < 0 || posi.y() < background.height())
       return 0;
   else
       return 1;
}

void MainScene::Ristarte(){
   emit triggered();
   MainScene::~MainScene();
}
void MainScene::Kwouite(){
    emit quitsig();
    MainScene::~MainScene();
}
void MainScene::eupdate(){

    QVector<Mutes*> elist = perso->getEnemies();
    for (int ennemyIndex = 0; ennemyIndex < elist.size(); ennemyIndex++){

        // Raise les OOB pour clean ceux qui arrivent au bord de l'écran
        if (elist[ennemyIndex]->getOOB() == 0 && !IsItIB(elist[ennemyIndex]->pos()))
            perso->raiseOOB(ennemyIndex);

        // Clean les OOB et fait avancer les ennemis
        if ( elist[ennemyIndex]->getOOB() == 1 && ennemyIndex > 2) // pour éviter de vider le vect et que ça bug
            perso->deleteE(&ennemyIndex);

        else if ( elist[ennemyIndex]->getGoinL() == 1)
            elist[ennemyIndex]->RightToLeft();

        else
            elist[ennemyIndex]->LeftToRight();

    }
}

void MainScene::generateur(){ // Cette méthode va s'occuper de faire apparaitre des ennemis aléatoirement et plus (niveau) sera haut plus ils seront nombreux

    // niveau en fonction du temps passé
    if ( t > 50000 ) niveau = 10;
    else if ( t > 40000 ) niveau = 8;
    else if ( t > 30000 ) niveau = 6;
    else if ( t > 20000 ) niveau = 4;
    else if ( t > 10000 ) niveau = 2;

    // droite ou gauche avec un temps d'intervalle
    if (this->t - 5000/niveau > this->deltaT) {
        int direction = abs(int(QRandomGenerator::global()->generate())) % 2; // 0 ou 1

        // creation d'un nouvel ennemi
        // vitesse et hauteur pour ennemi
        int vitesse = abs(int(QRandomGenerator::global()->generate())) % 3;
        int hauteur = abs(int(QRandomGenerator::global()->generate())) % 3; // 0 ou 1 ou 2

        if (direction) perso->add_ecollList(new Mutes(":images/muter.png", direction, vitesse+3, hauteur)); // vitesse + 3 sinon on peut tomber sur 0
        else perso->add_ecollList(new Mutes(":images/mute.png", direction, vitesse+3, hauteur));

        this->addItem(perso->getEnemies().back());
        //ajouter dans liste et créer ennemi dans bonne direction
        this->deltaT=this->t;
    }

    // boucler sur la liste et actualiser les positions puis lever les drapeaux OOB et dégager les OOB déjà présents
    eupdate();
}

void MainScene::WriteHigh(){ // écrit le highscore dans le même format de celui lu dans le constructeur
    FILE* highfile = fopen(".highscore","wb");
    char cleansalt[7]={0};
    int index = 0;
    for ( int i = 0; i < strlen(salt); i++){
        if (salt[i] != ':'){
            cleansalt[index] = salt[i];
            index++;
        }

    }
    int highsig = highscore * (int)strtol(cleansalt, nullptr,10);
    if(highfile != nullptr){
        fprintf(highfile,"%x",this->highscore);
        fputc(0,highfile);
        fputc(0,highfile);
        fputc(0,highfile);
        fprintf(highfile,"%x",highsig);
        fclose(highfile);
    }
    else {
        qDebug()<< "writing error";
    }
}

void MainScene::GameWin(){
    // on fait disparaitre les coeurs et le perso et on met l'écran de victoire en laissant la musique
    SoundTrack->pause();
    decompte->hide();
    perso->hide();
    perso->hideE();
    perso->hideA();
    full->hide();
    heart->hide();

    QList<QGraphicsView*> mesVues;
    mesVues = this->views();
    mesVues[1]->scale(1.4,1.4);
    win->setPos(0,-100);
    mesVues[1]->centerOn(win);
    win->show();

    // On met le son de victoire, hourra !
    SoundTrack->stop();
    SoundTrack->setMedia(QMediaContent(QUrl::fromLocalFile(applicationPath + "/../dolby_atmos/phonkwoods.wav")));
    SoundTrack->setVolume(60);
    SoundTrack->play();
    highscore = t;
    WriteHigh();

    printhigh->setText(createTime(highscore / 1000, highscore % 1000));
    printhigh->setPos(1350,600);
    printhigh->setScale(30);
    printhigh->show();
    restart->setPos(1000,1300);
    restart->show();
    quit->setPos(1000,1500);
    quit->show();
    fWon = 69; // valeur utile pour la boucle update, elle est à 0 quand la partie n'est pas encore gagnée
}

void MainScene::GameStatusHandler(int playerLP){
    // le jeu commence avec le son berlin, puis à 2 vie, angry birds, puis 1 vie resistance, quand le personnage a 0 vies, il est mort, on lance la musique phonk et le jeu s'arrête
    // à chaque vie en moins on joue un son oof
    // il faut aussi changer l'apparence du coeur de : 3, full à 2, heart, à 1, rien puis 0 écran mort

    QSound::play("../dolby_atmos/oof2.wav"); // **OOF** !

    if ( playerLP == 0){
        // win condition
        if ( highscore < this->t) GameWin();
        else {
            SoundTrack->pause();
            decompte->hide();
            perso->hide();
            perso->hideE();

            QList<QGraphicsView *> mesVues;
            mesVues = this->views();
            mesVues[1]->scale(1.4, 1.4);
            over->setPos(600, -1000);
            mesVues[1]->centerOn(over);
            over->show();
            printhigh->hide();
            restart->setPos(2600,1200);
            restart->setBrush(QBrush(QColor(Qt::green)));
            restart->show();
            quit->setPos(2600,1500);
            quit->setBrush(QBrush(QColor(Qt::darkGreen)));
            quit->show();
            timer->stop();

            // on joue un rire au hasard pour encourrager le joueur à faire mieux
            int soundNb = abs(int(QRandomGenerator::global()->generate())) % 8;
            if (soundNb == 0) QSound::play("../dolby_atmos/laugh0.wav");
            if (soundNb == 1) QSound::play("../dolby_atmos/laugh1.wav");
            if (soundNb == 2) QSound::play("../dolby_atmos/laugh2.wav");
            if (soundNb == 3) QSound::play("../dolby_atmos/laugh3.wav");
            if (soundNb == 4) QSound::play("../dolby_atmos/laugh4.wav");
            if (soundNb == 5) QSound::play("../dolby_atmos/laugh5.wav");
            if (soundNb == 6) QSound::play("../dolby_atmos/laugh6.wav");
            if (soundNb == 7) QSound::play("../dolby_atmos/laugh7.wav");
            SoundTrack->stop();
            SoundTrack->setMedia(QMediaContent(QUrl::fromLocalFile(
                    applicationPath + "/../dolby_atmos/slowwoods.wav")));
            SoundTrack->setVolume(60);
            SoundTrack->play();
        }
        // Et ici on a les autres cas, quand le joueur est encore vivant :
    }else if (playerLP == 1){
        heart->hide();
        SoundTrack->stop();
        SoundTrack->setMedia(QMediaContent(QUrl::fromLocalFile(applicationPath + "/../dolby_atmos/resistancer.wav")));
        SoundTrack->setVolume(60);
        SoundTrack->play();
    }else {
        SoundTrack->stop();
        SoundTrack->setMedia(QMediaContent(QUrl::fromLocalFile(applicationPath + "/../dolby_atmos/birdsc.wav")));
        SoundTrack->setVolume(15);
        SoundTrack->play();
        full->hide();
        heart->show();
    }
}

QString MainScene::createTime(int sec, int cent){  // on formatte le temps à afficher secondes:centièmes
    char csec[40];
    char ccent[30];
    sprintf(csec, "%d:", sec);
    sprintf(ccent, "%d", cent/10);
    strcat(csec,ccent);
    strcpy(createTimehold,csec);
    return createTimehold;
}

void MainScene::update() {
    this->t += 10;
    if (!fWon) {
        // centrage de vue
        QList<QGraphicsView *> mesVues;
        mesVues = this->views();
        mesVues[1]->centerOn(perso);

        // centrage coeurs HUD et coeur suiveur (on fait une moyenne de la position en y sinon c'est moche
        histPos.insert(0, perso->pos());
        ymoy += histPos.begin()->y();
        if (abs(histPos.begin()->x() - histPos.back().x()) > 100)
            xdiff = histPos.back().x(); // pour que si le personnage droite gauche droite gauche, le coeur ne le fasse pas aussi
        full->setPos(xdiff, ymoy / histPos.size() - 200);
        heart->setPos(xdiff, ymoy / histPos.size() - 200);
        if (t > 500) {
            ymoy -= histPos.back().y();
            histPos.pop_back();
        }

        // si mon personnage est dans le vide il continue sa trajectoire
        if (!perso->collcheck() ){
            perso->jumpboucle();
            NoCrouchWhileFlying = 1;
        }
        else NoCrouchWhileFlying = 0;

        // changer de pied toutes les 0.2 sec :
        if ( (t % 200) == 0) perso->toggleFoot();

        // Genere des ennemis
        generateur();

        // Verifie les collisions avec les ennemis
        if (perso->ecollcheck() && t - timeout > INVINCIBILITE) { // le timeout sert à ce que le perso ne se fasse pas retirer tous ses pdv car le refresh est rapide
            timeout = t;
            GameStatusHandler(perso->perdVie());
        }

        // Update la position des fleches du perso
        perso->aupdate();

        // Verifie les collisions des fleches avec les ennemis + qlq sec au chrono pour un ennemi éliminé
        t += perso->acollcheck();

        // chrono
        decompte->setText(createTime(t / 1000, t % 1000));
        if (t > highscore) {
            decompte->setBrush(QBrush(QColor(Qt::green)));
            // et faire disparaitre le highscore
            printhigh->hide();
        }

        // mourir quand on tombe
        if (perso->collidesWithItem(bottomsol)) GameStatusHandler(perso->perdVie());
    }
    else {
    //toggle highscore toutes les 1 secondes
    if (t % 1000 == 0) {
        if(printhigh->isVisible() == 1) printhigh->hide();
        else printhigh->show();
    }
    }
}

// jump() amorce un saut et collcheck verifie si j'entre en collision avec un element de la liste de collisions
// et perso->getCorD renvoie 1 s'il est crouched
void MainScene::keyPressEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_D && perso->collcheck() && !(perso->getCorD()) && NoCrouchWhileFlying == 0) { // appui sur la touche D du clavier
        perso->droite();
    }else {
        if (event->key() == Qt::Key_Q && perso->collcheck() && !(perso->getCorD()) && NoCrouchWhileFlying == 0) { // appui sur la touche Q du clavier
            perso->gauche();
        }
    }
    if(event->key() == Qt::Key_Space && perso->collcheck() && !(perso->getCorD()))  { // déclenche saut si appui sur espace et check si personnage pas en l'air
        perso->jump();
    }
    if(event->key() == Qt::Key_S && NoCrouchWhileFlying == 1) {
        CantCrouch = 1;
    }
    if(event->key() == Qt::Key_S && perso->collcheck() && CantCrouch == 0) { // déclenche position allongée
        perso->crouch();
    }
    if(event->key() == Qt::Key_R) { // Restart
        Ristarte();
    }
    if(event->key() == Qt::Key_K) { // Quit
        Kwouite();
    }
    if(event->key() == Qt::Key_J && t - ARROW_RATE > this->lastSent && !(perso->getCorD()) && !fWon) { // lance une flèche
        addItem(perso->generateurArrow());
        this->lastSent = t;
    }
}

// Ici je détecte quand une touche arrete d'être maintenue, c'est pour pouvoir sauter à la verticale
void MainScene::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_D && perso->collcheck()) { // retire son doigt de la touche D du clavier
        perso->stillr();
    } else {
        if (event->key() == Qt::Key_Q && perso->collcheck()) { // retire son doigt de la touche Q du clavier
            perso->stilll();
        }

    }
    if(event->key() == Qt::Key_S && perso->collcheck() && CantCrouch == 0) { // déclenche position debout
        perso->releve();
    }
    if(event->key() == Qt::Key_S  && CantCrouch == 1) CantCrouch = 0;
}
MainScene::~MainScene() {
    SoundTrack->stop();
    delete quit;
    delete printhigh;
    delete restart;
    delete this->timer;
    delete perso;
    delete sold;
    delete mid;
    delete solg;
    delete over;
    delete win;
    delete SoundTrack;
    delete decompte;
    delete chronof;
    delete backgroundP;
    delete bottomsol;
}

