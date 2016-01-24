#ifndef I_CLICKABLE_H_
#define I_CLICKABLE_H_

class IClickable {
    virtual bool isWithinClickHitbox(int x, int y) = 0;
};




#endif