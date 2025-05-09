#include "UiDgenerator.h"

int currentUiD = 0;

int UiDGenerator::GenerateUiD() {
    return currentUiD++;
}