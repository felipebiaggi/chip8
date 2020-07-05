//
// Created by akaza on 13/10/2019.
//

#include "Memoria.h"
#include<cstdint>
#include <iostream>

unsigned char chip8_fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};

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


bool Memoria::load(const char *file_path) {
    printf("Loading ROM: %s\n", file_path);

    FILE* rom = fopen(file_path, "rb");
       if (rom == NULL) {
           std::cerr << "Failed to open ROM" << std::endl;
           return false;
       }

       fseek(rom, 0, SEEK_END);
          long rom_size = ftell(rom);
          rewind(rom);

          // Allocate memory to store rom
          char* rom_buffer = (char*) malloc(sizeof(char) * rom_size);
          if (rom_buffer == NULL) {
              std::cerr << "Failed to allocate memory for ROM" << std::endl;
              return false;
          }

          // Copy ROM into buffer
          size_t result = fread(rom_buffer, sizeof(char), (size_t)rom_size, rom);
          if (result != rom_size) {
              std::cerr << "Failed to read ROM" << std::endl;
              return false;
          }

          // Copy buffer to memory
          if ((4096-512) > rom_size){
              for (int i = 0; i < rom_size; ++i) {
                 escrever([(i + 512)] = (uint8_t)rom_buffer[i];   // Load into memory starting
                                                              // at 0x200 (=512)
              }
          }
          else {
              std::cerr << "ROM too large to fit in memory" << std::endl;
              return false;
          }

          // Clean up
          fclose(rom);
          free(rom_buffer);

          return true;

}
