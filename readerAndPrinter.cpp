// TP 2 ATR (readerAndPrinter)- Luiz Henrique Silva Lelis 2014034847

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <math.h>
#include <string>
#include <mutex>
#include <condition_variable>

using namespace std;

void readNumber();
void printArithmeticMean();
double inputNumber;
int count;
int maxBuff = 5;
std::vector<double> inputNumberGroup;
std::mutex mtx;
condition_variable fullBuffer;
condition_variable emptyBuffer;

int main(){

    // Cria thead que le do teclado
    std::thread read (readNumber);

    // Cria thead que imprime
    std::thread print (printArithmeticMean); 

    // Sincroniza threads:
    read.join();                // Pausa ate read terminar
    print.join();               // Pausa ate print terminar

    return 0;
}

void readNumber(){

    int idx;

    // Quando o buffer está vazio, trava o mtx para a trhead leitura
    count = 0;
    unique_lock<mutex> lock(mtx);

    while(1){

        while(count==maxBuff){
            fullBuffer.wait(lock);
        }

        std::cout << "Reader-> Number: " << count+1 << "/"<< maxBuff<< "\n";
        std::cin >> inputNumber;

        // Verifica se e' um numero
        if (cin.fail()){

            std::cout << "Entre com um valor valido:\n";

            // Limpa o buffer de cin
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        }
        else{
                // A entrada e' um numero valido

                // Numero de entrada e'adicionado no final vetor
                inputNumberGroup.push_back(inputNumber);

                count++;
        }
        emptyBuffer.notify_one();
    }
}

void printArithmeticMean(){

    double mean;

    // Quando o buffer está cheio, trava o mtx para a trhead print
    unique_lock<mutex> lock(mtx);

    while(1){

        while(count==0){
            emptyBuffer.wait(lock);
        }

        while (!inputNumberGroup.empty()){
            mean = mean + inputNumberGroup[inputNumberGroup.size() - 1];
            inputNumberGroup.pop_back();
        }

        mean = mean/maxBuff;

        std::cout << "Printer -­> Mean value: " << mean;
        std::cout << "\n";

        mean = 0;
        count = 0;
        fullBuffer.notify_one();
    }

}