#include <iostream>
#include "grammar.h"

using namespace std;

int main() {
    Grammar CFG("input.txt");
    CFG.transformToChomskyNormalForm();
    CFG.printGrammar();
    return 0;
}