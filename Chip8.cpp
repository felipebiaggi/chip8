//
// Created by akaza on 13/10/2019.
//

#include "Chip8.h"
#include "Memoria.h"
#include <iostream>

Chip8::Chip8() {
    programCounter = 0x0200;
    opcode = 0x0000;

    for(auto& i : registradores) i = 0x00;
    for(auto& i : stack) i = 0x0000;
    for(auto& i : key) i = 0x00;
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
            std::cout << unsigned(opcode) << std::endl;

            for(int i = 0; i < 2048; i++){
                gfx[i] = 0;
            }

            drawFlag = true;
            programCounter += 2;
            break;

            /* 00EE - RET
                    Return from a subroutine.
                    The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer. */
        case 0x00EE:
            std::cout << unsigned(opcode) << std::endl;
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
        std::cout << unsigned(opcode) << std::endl;
        programCounter = opcode & 0x0FFF;
        break;

        /* 2nnn - CALL addr
            Call subroutine at nnn.
            The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn. */
    case 0x2000:
        std::cout << unsigned(opcode) << std::endl;
        stack[stackPointer] = programCounter;
        ++stackPointer;
        programCounter = opcode & 0x0FFF;
        break;

        /* 3xkk - SE Vx, byte
            Skip next instruction if Vx = kk.
            The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2. */
    case 0x3000:
        std::cout << unsigned(opcode) << std::endl;
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
        std::cout << unsigned(opcode) << std::endl;
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
        std::cout << unsigned(opcode) << std::endl;
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
        std::cout << unsigned(opcode) << std::endl;
        registradores[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
        programCounter += 2;
        break;

        /* 7xkk - ADD Vx, byte
            Set Vx = Vx + kk.
            Adds the value kk to the value of register Vx, then stores the result in Vx. */
    case 0x7000:
        std::cout << unsigned(opcode) << std::endl;
        registradores[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
        programCounter += 2;
        break;


    case 0x8000:

        switch (opcode & 0x000F){

        /* 8xy0 - LD Vx, Vy
                Set Vx = Vy.
                Stores the value of register Vy in register Vx. */
        case 0x0000:
            std::cout << unsigned(opcode) << std::endl;
            registradores[(opcode & 0x0F00) >> 8] = registradores[(opcode & 0x00F0) >> 4];
            programCounter += 2;
            break;

            /* 8xy1 - OR Vx, Vy
                    Set Vx = Vx OR Vy.
                    Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0. */
        case 0x0001:
            std::cout << unsigned(opcode) << std::endl;
            registradores[(opcode & 0x0F00) >> 8] |= registradores[(opcode & 0x00F0) >> 4];
            programCounter += 2;
            break;

            /* 8xy2 - AND Vx, Vy
                    Set Vx = Vx AND Vy.
                    Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0. */
        case 0x0002:
            std::cout << unsigned(opcode) << std::endl;
            registradores[(opcode & 0x0F00) >> 8] &= registradores[(opcode & 0x00F0) >> 4];
            programCounter += 2;
            break;

            /* 8xy3 - XOR Vx, Vy
                    Set Vx = Vx XOR Vy.
                    Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0. */
        case 0x0003:
            std::cout << unsigned(opcode) << std::endl;
            registradores[(opcode & 0x0F00) >> 8] = registradores[(opcode & 0x00F0) >> 4];
            programCounter += 2;
            break;

            /*8xy4 - ADD Vx, Vy
                    Set Vx = Vx + Vy, set VF = carry.
                    The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.*/
        case 0x0004:
            std::cout << unsigned(opcode) << std::endl;
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
            std::cout << unsigned(opcode) << std::endl;
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
            std::cout << unsigned(opcode) << std::endl;
            registradores[0xF] = registradores[(opcode & 0x0F00) >> 8] & 0xF;
            registradores[(opcode & 0x0F00) >> 8] >>= 1;
            programCounter += 2;
            break;

            /*8xy7 - SUBN Vx, Vy
                    Set Vx = Vy - Vx, set VF = NOT borrow.
                    If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.*/
        case 0x0007:
            std::cout << unsigned(opcode) << std::endl;
            if(registradores[(opcode & 0x0F00) >> 8] > registradores[(opcode & 0x00F0) >> 4])
                registradores[0xF] = 0;
            else
                registradores[0xF] = 1;

            registradores[(opcode & 0x0F00) >> 8] = registradores[(opcode & 0x00F0) >> 4] - registradores[(opcode & 0x0F00) >> 8];
            programCounter += 2;
            break;

            /*8xyE - SHL Vx {, Vy}
                    Set Vx = Vx SHL 1.
                    If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.*/
        case 0x000E:
            std::cout << unsigned(opcode) << std::endl;
            registradores[0xF] = registradores[(opcode & 0x0F00) >> 8] >> 7;
            registradores[(opcode & 0x0F00) >> 8] <<= 1;
            programCounter += 2;
            break;
        }
        break;

        /*9xy0 - SNE Vx, Vy
            Skip next instruction if Vx != Vy.
            The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.*/
    case 0x9000:
        std::cout << unsigned(opcode) << std::endl;
        if(registradores[(opcode & 0x0F00) >> 8] != registradores[(opcode & 0x00F0) >> 4])
            programCounter += 4;
        else
            programCounter += 2;
        break;

        /*Annn - LD I, addr
            Set I = nnn.
            The value of register I is set to nnn.*/
    case 0xA000:
        std::cout << unsigned(opcode) << std::endl;
        indexRegistradores = opcode & 0x0FFF;
        programCounter += 2;
        break;

        /*Bnnn - JP V0, addr
            Jump to location nnn + V0.
            The program counter is set to nnn plus the value of V0.*/
    case 0xB000:
        std::cout << unsigned(opcode) << std::endl;
        programCounter = (opcode & 0x0FFF) + registradores[0];
        break;

        /*Cxkk - RND Vx, byte
            Set Vx = random byte AND kk.
            The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.*/
    case 0xC000:
        std::cout << unsigned(opcode) << std::endl;
        registradores[(opcode & 0x0F00) >> 8] = (rand() % (0xFF + 1)) & (opcode & 0x00FF);
        programCounter += 2;
        break;

        /*Dxyn - DRW Vx, Vy, nibble
            Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
            The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.*/
    case 0xD000:
        unsigned short x = registradores[(opcode & 0x0F00) >> 8];
        unsigned short y = registradores[(opcode & 0x0F00) >> 4];
        unsigned short height = opcode & 0x000F;
        unsigned short pixel;

        registradores[0xF] = 0;

        for(int yline = 0; yline < height; yline++){
            pixel = memoria->ler(indexRegistradores + yline);
            for(int xline = 0; xline < 8; xline++){
                if((pixel & (0x80 >> xline)) != 0) {
                    if(gfx[(x + xline + ((y + yline) * 64))] == 1) {
                        registradores[0xF] = 1;
                    }
                    gfx[x + xline + ((y + yline) * 64)] ^= 1;
                }
            }
        }

        drawFlag = true;
        std::cout << unsigned(opcode) << std::endl;
        programCounter += 2;
        break;

    case 0xE000:

        switch (opcode & 0x00FF){

        /*Ex9E - SKP Vx
                Skip next instruction if key with the value of Vx is pressed.
                Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.*/
        case 0x009E:
            std::cout << unsigned(opcode) << std::endl;
            if(key[registradores[(opcode & 0x0F00) >> 8]] != 0)
                programCounter += 4;
            else
                programCounter += 2;
            break;

            /*ExA1 - SKNP Vx
                    Skip next instruction if key with the value of Vx is not pressed.
                    Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.*/
        case 0x00A1:
            std::cout << unsigned(opcode) << std::endl;
            if(key[registradores[(opcode & 0x0F00) >> 8]] == 0)
                programCounter += 4;
            else
                programCounter += 2;
            break;
        }
        break;


    case 0xF000:

        switch (opcode & 0x00FF){

        /*Fx07 - LD Vx, DT
                Set Vx = delay timer value.
                The value of DT is placed into Vx.*/
        case 0x0007:
            std::cout << unsigned(opcode) << std::endl;
            registradores[(opcode & 0x0F00) >> 8] = delay;
            programCounter += 2;
            break;

            /*Fx0A - LD Vx, K
                    Wait for a key press, store the value of the key in Vx.
                    All execution stops until a key is pressed, then the value of that key is stored in Vx*/
        case 0x000A:
            std::cout << unsigned(opcode) << std::endl;
            key_pressed = false;

            for(int i = 0; i < 16; ++i)
            {
                if(key[i] != 0)
                {
                    registradores[(opcode & 0x0F00) >> 8] = i;
                    key_pressed = true;
                }
                if(!key_pressed)
                    return;

                programCounter += 2;
            }

            break;

            /*Fx15 - LD DT, Vx
                    Set delay timer = Vx.
                    DT is set equal to the value of Vx.*/
        case 0x0015:
            std::cout << unsigned(opcode) << std::endl;
            delay = registradores[(opcode & 0x0F00) >> 8];
            programCounter += 2;
            break;

            /*Fx18 - LD ST, Vx
                    Set sound timer = Vx.
                    ST is set equal to the value of Vx.*/
        case 0x0018:
            std::cout << unsigned(opcode) << std::endl;
            sound = registradores[(opcode & 0x0F00) >> 8];
            programCounter += 2;
            break;

            /*Fx1E - ADD I, Vx
                    Set I = I + Vx.
                    The values of I and Vx are added, and the results are stored in I.*/
        case 0x001E:
            std::cout << unsigned(opcode) << std::endl;
            if(indexRegistradores + registradores[(opcode & 0x0F00) >> 8] > 0x0FFF)
                registradores[0xF] = 1;
            else
                registradores[0xF] = 0;

            indexRegistradores += registradores[(opcode & 0x0F00) >> 8];
            programCounter += 2;
            break;

            /*Fx29 - LD F, Vx
                    Set I = location of sprite for digit Vx.
                    The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. See section 2.4, Display, for more information on the Chip-8 hexadecimal font.*/
        case 0x0029:
            std::cout << unsigned(opcode) << std::endl;
            indexRegistradores = registradores[(opcode & 0x0F00) >> 8] * 0x5;
            programCounter += 2;
            break;

            /*Fx33 - LD B, Vx
                    Store BCD representation of Vx in memory locations I, I+1, and I+2.
                    The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.*/
        case 0x0033:
            std::cout << unsigned(opcode) << std::endl;
            escrever(indexRegistradores, registradores[(opcode & 0x0F00) >> 8] / 100);
            escrever(indexRegistradores + 1, (registradores[(opcode & 0x0F00) >> 8] / 10) % 10);
            escrever(indexRegistradores + 2, registradores[(opcode & 0x0F00) >> 8] % 10);
            programCounter += 2;
            break;

        case 0x0055:
            std::cout << unsigned(opcode) << std::endl;
            for(int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i)
                escrever(indexRegistradores + i, registradores[i]);

            indexRegistradores += ((opcode & 0x0F00) >> 8) + 1;
            programCounter += 2;
            break;
        }
        break;
    }

    if (delay > 0){
        --delay;
    }

    if(sound > 0)
        if(sound == 1);
    //implementar o som
    --sound;

}
