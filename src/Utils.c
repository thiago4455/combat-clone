#include "Utils.h"

ALLEGRO_COLOR al_map_hex_a(unsigned int n) {
    unsigned char r = (n >> 24) & 0xFF;
    unsigned char g = (n >> 16) & 0xFF;
    unsigned char b = (n >> 8) & 0xFF;
    unsigned char a = n & 0xFF;
    return al_map_rgb(r,g,b);
}

ALLEGRO_COLOR al_map_hex(unsigned int n) {
    return al_map_hex_a((n << 8));
}