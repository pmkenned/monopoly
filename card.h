#ifndef _CARD_H_
#define _CARD_H_

struct card_s {
    char text[200];
    void (*func)();
};

typedef struct card_s Card_t;

#endif
