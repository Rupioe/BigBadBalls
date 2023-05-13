#include "Mutes.h"

void Mutes::LeftToRight(){
    this->setPos(pos().x()+vitesse*VITESSE_ENNEMI,pos().y());
}

void Mutes::RightToLeft(){
    this->setPos(pos().x()-vitesse*VITESSE_ENNEMI,pos().y());
}

int Mutes::getOOB(){
    return this->OutOfBound;
}

void Mutes::setOOB(int boule){
    this->OutOfBound = boule;
}

int Mutes::getGoinR(){
   return this->GoinR;
}

int Mutes::getGoinL(){
    return this->GoinL;
}
