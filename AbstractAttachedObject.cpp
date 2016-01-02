#include "AbstractAttachedObject.h"
#include "Platform.h"

AbstractAttachedObject::AbstractAttachedObject(Platform* platform, int orientation) : AbstractAttachedObject(platform, orientation, 0) {}

AbstractAttachedObject::AbstractAttachedObject(Platform* platform, int orientation, int position) :
platform(platform), orientation(orientation), position(position) {
    initialise();
}

void AbstractAttachedObject::initialise() {}