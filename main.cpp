#include <iostream>
#include "Memoria.h"


int main() {

    Memoria memo;

    memo.escrever(0x010A, 0xA0);

    std::cout << unsigned(memo.ler(0x010A)) << std::endl;

    memo.escrever(0x010A, 0xFF);

    std::cout << unsigned(memo.ler(0x010A)) << std::endl;




    return 0;
}