//
// Created by akaza on 13/10/2019.
//

#include "Memoria.h"
#include<cstdint>

Memoria::Memoria() {
    for(auto& i : memoria) i = 0x00;
    cpu.ConectarMemoria(this);
}

void Memoria::escrever(uint16_t addr, uint8_t data) {
    if(addr >= 0x0000 && addr <= 0x0FFF)
        memoria[addr] = data;
}

uint8_t Memoria::ler(uint16_t addr) {
    if(addr >= 0x0000 && addr <= 0x0FFF)
        return memoria[addr];

    return 0x000;
}