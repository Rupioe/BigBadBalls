#include "Arrows.h"

void Arrows::LeftToRight(){
    this->setPos(pos().x()+vitesse,pos().y());
}

void Arrows::RightToLeft(){
    this->setPos(pos().x()-vitesse,pos().y());
}

int Arrows::getOOB(){
    return this->OutOfBound;
}

void Arrows::setOOB(int boule){
    this->OutOfBound = boule;
}

int Arrows::getGoinR(){
    return this->GoinR;
}

int Arrows::getGoinL(){
    return this->GoinL;
}
