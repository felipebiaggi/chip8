//
// Created by akaza on 13/10/2019.
//

#include "Chip8.h"
#include "Memoria.h"

Chip8::Chip8() {

}

uint8_t Chip8::ler(uint8_t addr) {
    return memoria->ler(addr);

}

void Chip8::escrever(uint16_t addr, uint8_t data) {
    memoria->escrever(addr, data);
}


void Chip8::clock() {

    opcode = ler(programCounter ) << 8 | ler(programCounter + 1);

    switch (opcode & 0xF000){

        case 0x0000:

            switch (opcode){

                /* 00E0 - CLS
                Clear the display. */
                case 0x00E0:
                    programCounter += 2;
                    break;

                    /* 00EE - RET
                    Return from a subroutine.
                    The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer. */
                case 0x00EE:
                    --stackPointer;
                    programCounter = stack[stackPointer];
                    programCounter += 2;
                    break;
            }

            break;

            /* 1nnn - JP addr
            Jump to location nnn.
            The interpreter sets the program counter to nnn. */
        case 0x1000:
            programCounter = opcode & 0x0FFF;
            break;

            /* 2nnn - CALL addr
            Call subroutine at nnn.
            The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn. */
        case 0x2000:
            stack[stackPointer] = programCounter;
            ++stackPointer;
            programCounter = opcode & 0x0FFF;
            break;

            /* 3xkk - SE Vx, byte
            Skip next instruction if Vx = kk.
            The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2. */
        case 0x3000:
            if(registradores[(opcode & 0x0F00) >> 8]  == (opcode & 0x00FF)){
                programCounter += 4;
            } else {
                programCounter += 2;
            }
            break;

            /* 4xkk - SNE Vx, byte
            Skip next instruction if Vx != kk.
            The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2. */
        case 0x4000:
            if(registradores[(opcode & 0x0F00) >> 8]  != (opcode & 0x00FF)){
                programCounter += 4;
            } else {
                programCounter += 2;
            }
            break;

            /* 5xy0 - SE Vx, Vy
            Skip next instruction if Vx = Vy.
            The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2. */
        case 0x5000:
            if(registradores[(opcode & 0x0F00) >> 8]  != registradores[(opcode & 0x00F0) >> 8]){
                programCounter += 4;
            } else {
                programCounter += 2;
            }
            break;

            /* 6xkk - LD Vx, byte
            Set Vx = kk.
            The interpreter puts the value kk into register Vx. */
        case 0x6000:
            registradores[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
            programCounter += 2;
            break;

            /* 7xkk - ADD Vx, byte
            Set Vx = Vx + kk.
            Adds the value kk to the value of register Vx, then stores the result in Vx. */
        case 0x7000:
            registradores[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
            programCounter += 2;
            break;


        case 0x8000:

            switch (opcode & 0x000F){

                /* 8xy0 - LD Vx, Vy
                Set Vx = Vy.
                Stores the value of register Vy in register Vx. */
                case 0x0000:
                    registradores[(opcode & 0x0F00) >> 8] = registradores[(opcode & 0x00F0) >> 4];
                    programCounter += 2;
                    break;

                    /* 8xy1 - OR Vx, Vy
                    Set Vx = Vx OR Vy.
                    Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0. */
                case 0x0001:
                    registradores[(opcode & 0x0F00) >> 8] |= registradores[(opcode & 0x00F0) >> 4];
                    programCounter += 2;
                    break;

                    /* 8xy2 - AND Vx, Vy
                    Set Vx = Vx AND Vy.
                    Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0. */
                case 0x0002:
                    registradores[(opcode & 0x0F00) >> 8] &= registradores[(opcode & 0x00F0) >> 4];
                    programCounter += 2;
                    break;

                    /* 8xy3 - XOR Vx, Vy
                    Set Vx = Vx XOR Vy.
                    Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0. */
                case 0x0003:
                    registradores[(opcode & 0x0F00) >> 8] = registradores[(opcode & 0x00F0) >> 4];
                    programCounter += 2;
                    break;

                    /*8xy4 - ADD Vx, Vy
                    Set Vx = Vx + Vy, set VF = carry.
                    The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.*/
                case 0x0004:
                    registradores[(opcode & 0x0F00) >> 8] += registradores[(opcode & 0x00F0) >> 4];
                    if(registradores[(opcode & 0x00F0) >> 4] > (0xFF - registradores[(opcode & 0x0F00) >> 8]))
                        registradores[0xF] = 1;
                    else
                        registradores[0xF] = 0;

                    programCounter += 2;
                    break;

                    /*8xy5 - SUB Vx, Vy
                    Set Vx = Vx - Vy, set VF = NOT borrow.
                    If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx*/
                case 0x0005:
                    if(registradores[(opcode & 0x00F0) >> 4] > registradores[(opcode & 0x0F00) >> 8])
                        registradores[0xF] = 0;
                    else
                        registradores[0xF] = 1;

                    registradores[(opcode & 0x0F00) >> 8] -= registradores[(opcode & 0x00F0) >> 4];
                    programCounter += 2;
                    break;

                    /*8xy6 - SHR Vx {, Vy}
                    Set Vx = Vx SHR 1.
                    If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.*/
                case 0x0006:
                    registradores[0xF] = registradores[(opcode & 0x0F00) >> 8] & 0xF;
                    registradores[(opcode & 0x0F00) >> 8] >>= 1;
                    programCounter += 2;
                    break;
            }

            break;
    }

}
