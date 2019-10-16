//
// Created by akaza on 13/10/2019.
//

#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H
#include <cstdint>
#include <vector>
#include <array>
#include <string>

class Memoria;

class Chip8 {

public:
    Chip8();

public:
    std::array<uint8_t , 16> registradores;
    std::array<uint16_t , 16> stack;
    uint16_t stackPointer = 0x0000;
    uint16_t programCounter = 0x00;
    uint16_t indexRegistradores = 0x0000;
    uint16_t  opcode = 0x0000;

public:
    void clock();

    void ConectarMemoria(Memoria* n) {
        memoria = n;
    }

private:
    Memoria* memoria = nullptr;
    uint8_t ler(uint8_t addr);
    void escrever(uint16_t addr, uint8_t data);
};


#endif //CHIP8_CHIP8_H
