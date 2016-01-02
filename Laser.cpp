#include "Laser.h"
#include "LaserSource.h"

Laser::Laser() {
    _isNull = true;
}

Laser::Laser(LaserSource* source) : source(source) {
    _isNull = false;
}

bool Laser::isNull() {
    return _isNull;
}

void Laser::draw() {

}
void Laser::update(Keyboard k) {

}


Laser* NULL_LASER = new Laser();