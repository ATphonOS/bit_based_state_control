/**
 * @file bit_based_state_control.cpp
 * @brief Implementation of the BBStateControl class using a bitfield.
 * @author ATphonOS
 * @version v1.2.0
 * @date 2024
 * @license MIT
 */

#include "bit_based_state_control.h"
#include <avr/pgmspace.h>
#include <Arduino.h> 

// Strings stored in program memory for debugging
static const char NOT_INDEX[] PROGMEM = "- unassigned";
static const char INDEX[] PROGMEM = " assigned";

// Constructor: Initializes bitfield and saved state
BBStateControl::BBStateControl(uint8_t size)
    : def_size(size > 0 ? size : 1), byte_size((size + 7) >> 3),
      array(nullptr), savedState(nullptr), trueIndex(-1), savedTrueIndex(-1) {
    if (def_size > 254) def_size = 254; // Limit for uint8_t indexing
    byte_size = (def_size + 7) >> 3;    // Calculate required bytes
    array = new uint8_t[byte_size]();
    if (!array) {
        def_size = 0;
        byte_size = 0;
        return;
    }
    savedState = new uint8_t[byte_size]();
    if (!savedState) {
        delete[] array;
        array = nullptr;
        def_size = 0;
        byte_size = 0;
    }
}

// Destructor: Frees allocated memory
BBStateControl::~BBStateControl() {
    delete[] array;
    delete[] savedState;
}

// Sets a bit at the given index
void BBStateControl::setBit(uint8_t index, bool state) {
    if (!isValidIndex(index)) return;
    uint8_t byte = index >> 3;  // Byte index (index / 8)
    uint8_t bit = index & 0x07; // Bit position (index % 8)
    if (state) {
        array[byte] |= (1 << bit);
    } else {
        array[byte] &= ~(1 << bit);
    }
}

// Gets the value of a bit
bool BBStateControl::getBit(uint8_t index) const {
    if (!isValidIndex(index)) return false;
    uint8_t byte = index >> 3;
    uint8_t bit = index & 0x07;
    return (array[byte] & (1 << bit)) != 0;
}

// Sets a state to true
void BBStateControl::setState(uint8_t index, bool exclusive) {
    setState(index, true, exclusive);
}

// Sets a state to a specific value
void BBStateControl::setState(uint8_t index, bool state, bool exclusive) {
    if (!isValidIndex(index)) return;
    setBit(index, state);
    if (state) {
        trueIndex = index;
        if (exclusive) clearOthers(index);
    } else if (index == trueIndex) {
        trueIndex = getFirstTrueIndex();
    }
}

// Saves the current state
void BBStateControl::saveState() {
    if (!array || !savedState || byte_size == 0) return;
    for (uint8_t i = 0; i < byte_size; i++) {
        savedState[i] = array[i];
    }
    savedTrueIndex = trueIndex;
}

// Restores the saved state
void BBStateControl::restoreSavedState() {
    if (!array || !savedState || byte_size == 0) return;
    for (uint8_t i = 0; i < byte_size; i++) {
        array[i] = savedState[i];
    }
    trueIndex = savedTrueIndex;
}

// Toggles a state
void BBStateControl::toggleState(uint8_t index) {
    if (!isValidIndex(index)) return;
    bool new_state = !getBit(index);
    setBit(index, new_state);
    if (new_state) {
        trueIndex = index;
        clearOthers(index);
    } else if (index == trueIndex) {
        trueIndex = getFirstTrueIndex();
    }
}

// Resets all states to false
void BBStateControl::resetArray() {
    if (!array || byte_size == 0) return;
    for (uint8_t i = 0; i < byte_size; i++) {
        array[i] = 0;
    }
    trueIndex = -1;
}

// Sets all states to a value
void BBStateControl::setAllStates(bool state) {
    if (!array || byte_size == 0) return;
    for (uint8_t i = 0; i < byte_size; i++) {
        array[i] = state ? 0xFF : 0;
    }
    trueIndex = state ? 0 : -1;
}

// Sets the first state to true
void BBStateControl::setDefaultIndex() {
    if (!array || byte_size == 0) return;
    resetArray();
    setBit(0, true);
    trueIndex = 0;
}

// Gets a state value
bool BBStateControl::getState(uint8_t index) const {
    return getBit(index);
}

// Finds the first true state
int8_t BBStateControl::getFirstTrueIndex() const {
    if (!array) return -1;
    for (uint8_t i = 0; i < def_size; i++) {
        if (getBit(i)) return i;
    }
    return -1;
}

// Gets all true state indices
uint8_t* BBStateControl::getAllTrueIndices(uint8_t& count) const {
    if (!array || byte_size == 0) {
        count = 0;
        return nullptr;
    }
    count = countTrueStates();
    if (count == 0) return nullptr;

    uint8_t* indices = new uint8_t[count];
    if (!indices) {
        count = 0;
        return nullptr;
    }

    uint8_t pos = 0;
    for (uint8_t i = 0; i < def_size; i++) {
        if (getBit(i)) indices[pos++] = i;
    }
    return indices;
}

// Generates a string with the true index
void BBStateControl::getIndex(char* buffer, uint8_t bufSize) const {
    if (!buffer || bufSize == 0) return;
    if (trueIndex >= 0) {
        snprintf_P(buffer, bufSize, PSTR("%d%s"), trueIndex, INDEX);
    } else {
        strncpy_P(buffer, NOT_INDEX, bufSize);
    }
    buffer[bufSize - 1] = '\0';
}

// Finds a state with the given value
int8_t BBStateControl::findState(bool state) const {
    if (!array) return -1;
    for (uint8_t i = 0; i < def_size; i++) {
        if (getBit(i) == state) return i;
    }
    return -1;
}

// Returns serialization size
uint8_t BBStateControl::serializeStatesSize() const {
    return def_size + 1; // Chars for '0'/'1' + null
}

// Sets a range of states
void BBStateControl::setRangeStates(uint8_t start, uint8_t end, bool state) {
    if (!array || start >= def_size) return;
    end = min(end, def_size - 1);
    resetArray();
    for (uint8_t i = start; i <= end; i++) {
        setBit(i, state);
        if (state && trueIndex == -1) trueIndex = i;
    }
}

// Checks if any state is true
bool BBStateControl::isAssignedIndex() const {
    return getFirstTrueIndex() != -1;
}

// Counts true states
uint8_t BBStateControl::countTrueStates() const {
    if (!array || byte_size == 0) return 0;
    uint8_t count = 0;
    for (uint8_t i = 0; i < def_size; i++) {
        if (getBit(i)) count++;
    }
    return count;
}

// Inverts all states
void BBStateControl::invertStates() {
    if (!array || byte_size == 0) return;
    for (uint8_t i = 0; i < byte_size; i++) {
        array[i] = ~array[i];
    }
    // Mask unused bits in the last byte
    uint8_t bits_in_last = def_size & 0x07;
    if (bits_in_last != 0) {
        array[byte_size - 1] &= (1 << bits_in_last) - 1;
    }
    trueIndex = getFirstTrueIndex();
}

// Validates single true state
bool BBStateControl::validateSingleState() const {
    return countTrueStates() == 1;
}

// Checks if an index is valid
bool BBStateControl::isValidIndex(uint8_t index) const {
    return (array && index < def_size);
}

// Copies states from another object
bool BBStateControl::copyStatesFrom(const BBStateControl& source) {
    if (!array || !source.array || def_size != source.def_size) return false;
    for (uint8_t i = 0; i < byte_size; i++) {
        array[i] = source.array[i];
    }
    trueIndex = source.trueIndex;
    return true;
}

// Serializes states to a string
void BBStateControl::serializeStates(char* buffer, uint8_t bufSize) const {
    if (!array || byte_size == 0 || !buffer || bufSize == 0) {
        if (bufSize > 0) buffer[0] = '\0';
        return;
    }
    uint8_t len = min(def_size, bufSize - 1);
    for (uint8_t i = 0; i < len; i++) {
        buffer[i] = getBit(i) ? '1' : '0';
    }
    buffer[len] = '\0';
}

// Clears all states except one
void BBStateControl::clearOthers(uint8_t index) {
    if (!array || byte_size == 0) return;
    for (uint8_t i = 0; i < def_size; i++) {
        if (i != index) setBit(i, false);
    }
}