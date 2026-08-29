#include "player.hpp"
#include <string>

player::player(int number, QString firstName, QString lastName, int handedness, QString country, bool isDressed)
{
	this->number = number;
	this->firstName = firstName;
	this->lastName = lastName;
	this->handedness = handedness;
    this->country = country;
    this->isDressed = isDressed;
    playerIcon = new QGraphicsEllipseItem(-2, -2, 4, 4);
}

void player::setNumber(int num)
{
	number = num;
}

void player::setFirstName(QString name)
{
    firstName = name.simplified();
}

void player::setLastName(QString name)
{
    lastName = name.simplified();
}

void player::setHandedness(int num)
{
	handedness = num;
}

QString player::getHandedness() const
{
    if (handedness == 0) return "L";
    if (handedness == 1) return "R";
	return "DEFAULT";
}

void player::configurePlayerIcon(qreal x, qreal y, QColor c){
    playerIcon->setBrush(QBrush(c));
    playerIcon->setPen(QPen(Qt::black, 0)); //one pixel
    playerIcon->setPos(x, y);

    QGraphicsTextItem *numberIcon = new QGraphicsTextItem(QString::number(number), playerIcon);

    //set font size
    QFont font = numberIcon->font();
    font.setPointSize(2);

    numberIcon->setFont(font);

    //center the number
    x = ((playerIcon->rect().width() - numberIcon->boundingRect().width()) / 2) - 2;
    y = ((playerIcon->rect().height() - numberIcon->boundingRect().height()) / 2) - 2;
    numberIcon->setPos(x, y);

}

