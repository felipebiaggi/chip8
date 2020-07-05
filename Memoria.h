//
// Created by akaza on 13/10/2019.
//

#ifndef CHIP8_MEMORIA_H
#define CHIP8_MEMORIA_H

#include <array>
#include "Chip8.h"

class Memoria {

public:
    Memoria();


public:
    Chip8 cpu;
    std::array<uint8_t , 4 * 1024> memoria;

public:
    void escrever (uint16_t addr, uint8_t data);
    uint8_t ler (uint16_t addr);
    bool load(const char *file_path);
};


#endif //CHIP8_MEMORIA_H
