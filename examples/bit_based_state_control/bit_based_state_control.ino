#include "bit_based_state_control.h"

BBStateControl states(10);

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ; 
    }
    Serial.println(F("bit-based_state_control Example"));

    Serial.println(F("\nSet state at index 3"));
    states.setState(3);
    printStates();

    Serial.println(F("Toggle state at index 3"));
    states.toggleState(3);
    printStates();

    Serial.println(F("Set a range of states (2-5 true)"));
    states.setRangeStates(2, 5, true);
    printStates();

    Serial.println(F("Save last states and modify state 7"));
    states.saveState();
    states.setState(7);
    printStates();

    Serial.println(F("Restore saved state before modification"));
    states.restoreSavedState();
    printStates();

    Serial.println(F("Invert states"));
    states.invertStates();
    printStates();

    Serial.println(F("Get all true indices"));
    uint8_t count;
    uint8_t* trueIndices = states.getAllTrueIndices(count);
    Serial.print(F("True indices (count="));
    Serial.print(count);
    Serial.print(F("): "));
    for (uint8_t i = 0; i < count; i++) {
        Serial.print(trueIndices[i]);
        Serial.print(F(" "));
    }
    Serial.println();
    delete[] trueIndices; // Free allocated memory
}

void loop() {
    // Empty loop
}

// Helper function to print states
void printStates() {
    char buffer[32];
    states.serializeStates(buffer, sizeof(buffer));
    Serial.print(F("States: "));
    Serial.println(buffer);

    char indexBuf[16];
    states.getIndex(indexBuf, sizeof(indexBuf));
    Serial.print(F("First True Index: "));
    Serial.println(indexBuf);

    Serial.print(F("True Count: "));
    Serial.println(states.countTrueStates());
    Serial.println();
}
