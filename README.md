This documentation is also available in **[Spanish](./README.es.md)**.


# 🔀 bit_based_state_control
## Overview
`BBStateControl` is a lightweight and memory-efficient C++ class designed to manage multiple boolean states using a bitfield. It's optimized for microcontroller environments and supports up to **254 independent states** with efficient memory usage.
Designed as a library for use in Arduino environments.

---

## ⚙️ Features

- Bitfield-based boolean state storage.
- Supports up to 254 individual states. 

> `uint8_t` allows values from 0 to 255, the maximum number of states is set to **254** to avoid potential issues with array indexing and bitfield storage. The index range goes from `0` to `253`, ensuring safe memory management and preventing overflow. `uint8_t` is optimal on Arduino Uno (2 KB RAM) for low memory use and fast execution.

- Set, toggle, reset, and query state values.
- Exclusive state activation (one-hot control).
- Save and restore previous state.
- Serialization to string format.
- Range and bulk operations.

### 📌 Compatible Microcontrollers

Compatible with 8-bit AVR, 32-bit ARM Cortex-M and Xtensa architectures, as long as they are compatible with the Arduino IDE.

---

## 📦 Installation

Option 1: Copy the files and include:

1. Copy the following files into your Arduino project or C++ embedded project:
   - `bit_based_state_control.h`
   - `bit_based_state_control.cpp`

2. Include the header in your source file:
   ```cpp
   #include "bit_based_state_control.h"
   ```
   

Option 2: Install the library and include: 

1. [Download](https://github.com/ATphonOS/bit-based-state-control/archive/refs/heads/main.zip) the repo.
2. Open Arduino IDE.
3. Go to Sketch > `Include Library` > `Add ZIP libraries`... in the menu.
4. Select the RAR file from the library.
5. Import the Library:

To use the library in your sketch, go to Sketch > `Include Library`.
Select the installed library from the list to add its #include directive to your code.
```cpp
   #include <bit_based_state_control.h>
```

---

## 📚 Class: `BBStateControl`

### Constructor & Destructor

```cpp
explicit BBStateControl(uint8_t size);
~BBStateControl();
```

| Method | Description | Parameters | Returns |
|--------|-------------|------------|---------|
| `explicit BBStateControl(uint8_t size)` | Constructs a new BBStateControl object with a specified number of states. | `size (uint8_t)`: number of states to manage (maximum 254) | — |
| `~BBStateControl()` | Destructor. Frees allocated memory used by the internal state array. | None | — |

## 🔓 Public Methods

### State Control
```cpp
void setState(uint8_t index, bool exclusive = true);
void setState(uint8_t index, bool state, bool exclusive = true);
void toggleState(uint8_t index);
void resetArray();
void setAllStates(bool state);
void setDefaultIndex();
void setRangeStates(uint8_t start, uint8_t end, bool state);
void invertStates();
```

| Method | Description | Parameters | Returns |
|--------|-------------|------------|---------|
| `void setState(uint8_t index, bool exclusive = true)` | Sets the state at the specified index to `true`, optionally clearing others. | `index (uint8_t)`: index of the state<br>`exclusive (bool)`: if `true`, clears all other states (default `true`) | `void` |
| `void setState(uint8_t index, bool state, bool exclusive = true)` | Sets the state at the specified index to the given value, optionally clearing others. | `index (uint8_t)`: index of the state<br>`state (bool)`: value to set<br>`exclusive (bool)`: if `true`, clears all other states (default `true`) | `void` |
| `void toggleState(uint8_t index)` | Inverts the value of the state at the given index. | `index (uint8_t)`: index of the state | `void` |
| `void resetArray()` | Resets all states to `false`. | None | `void` |
| `void setAllStates(bool state)` | Sets all states to the specified value. | `state (bool)`: value to assign to all states | `void` |
| `void setDefaultIndex()` | Sets the first index (0) to `true`, and all others to `false`. | None | `void` |
| `void setRangeStates(uint8_t start, uint8_t end, bool state)` | Sets a range of states to a given value. | `start (uint8_t)`: starting index<br>`end (uint8_t)`: ending index<br>`state (bool)`: value to assign | `void` |
| `void invertStates()` | Inverts all states (`true` to `false`, and vice versa). | None | `void` |


### Query Methods

```cpp
bool getState(uint8_t index) const;
int8_t getFirstTrueIndex() const;
uint8_t* getAllTrueIndices(uint8_t& count) const;
void getIndex(char* buffer, uint8_t bufSize) const;
int8_t findState(bool state) const;
uint8_t countTrueStates() const;
bool isAssignedIndex() const;
bool validateSingleState() const;
```

| Method | Description | Parameters | Returns |
|--------|-------------|------------|---------|
| `bool getState(uint8_t index) const` | Returns the boolean value of the state at the specified index. | `index (uint8_t)`: index of the state (0 to def_size - 1) | `bool`: state value (`true` or `false`) |
| `int8_t getFirstTrueIndex() const` | Returns the index of the first `true` state. | None | `int8_t`: index of first true state, or `-1` if none |
| `uint8_t* getAllTrueIndices(uint8_t& count) const` | Returns an array of indices where states are `true`. Caller must `delete[]` the array. | `count (uint8_t&)`: reference to store the number of true states | `uint8_t*`: pointer to array of indices, or `nullptr` if none |
| `void getIndex(char* buffer, uint8_t bufSize) const` | Writes a string representation of the current true index into the provided buffer. | `buffer (char*)`: buffer to store the string<br>`bufSize (uint8_t)`: size of the buffer | `void` |
| `int8_t findState(bool state) const` | Finds the index of the first state matching the given value. | `state (bool)`: value to search for (`true` or `false`) | `int8_t`: index of matching state, or `-1` if not found |
| `uint8_t countTrueStates() const` | Counts the number of active (`true`) states. | None | `uint8_t`: number of true states |
| `bool isAssignedIndex() const` | Checks whether at least one state is `true`. | None | `bool`: `true` if any state is active, otherwise `false` |
| `bool validateSingleState() const` | Validates that exactly one state is `true`. | None | `bool`: `true` if exactly one active state, otherwise `false` |


### Serialization

```cpp
void serializeStates(char* buffer, uint8_t bufSize) const;
uint8_t serializeStatesSize() const;
```

| Method | Description | Parameters | Returns |
|--------|-------------|------------|---------|
| `void serializeStates(char* buffer, uint8_t bufSize) const` | Serializes the current states into a string of `'0'` and `'1'`, stored in the provided buffer. | `buffer (char*)`: buffer to store the serialized string<br>`bufSize (uint8_t)`: size of the buffer | `void` |
| `uint8_t serializeStatesSize() const` | Calculates the buffer size required to serialize all states (including null terminator). | None | `uint8_t`: number of characters required |


### State Save & Restore

```cpp
void saveState();
void restoreSavedState();
```

| Method | Description | Parameters | Returns |
|--------|-------------|------------|---------|
| `void saveState()` | Saves the current state and active index for later restoration. | None | `void` |
| `void restoreSavedState()` | Restores the previously saved state and active index. | None | `void` |



## 🔒 Private Methods

### Internal helpers to ensure safe bit manipulation and logic control

```cpp
bool isValidIndex(uint8_t index) const;
void clearOthers(uint8_t index);
void setBit(uint8_t index, bool state);
bool getBit(uint8_t index) const;
```

| Method | Description | Parameters | Returns |
|--------|-------------|------------|---------|
| `bool isValidIndex(uint8_t index) const` | Checks whether the given index is within valid bounds. | `index (uint8_t)`: index to check | `bool`: `true` if valid, `false` otherwise |
| `void clearOthers(uint8_t index)` | Clears all states except the one at the specified index. | `index (uint8_t)`: index of the state to keep `true` | `void` |
| `void setBit(uint8_t index, bool state)` | Sets the bit at the given index to the specified value. | `index (uint8_t)`: bit index<br>`state (bool)`: value to set | `void` |
| `bool getBit(uint8_t index) const` | Gets the value of the bit at the specified index. | `index (uint8_t)`: bit index to read | `bool`: bit value at the given index |


### 🔐 Internal Members

```cpp
uint8_t def_size;         // Total number of states.
uint8_t byte_size;        // Number of bytes used for storage.
uint8_t* array;           // Bitfield array for current states.
uint8_t* savedState;      // Saved state array.
int8_t trueIndex;         // Index of the first true state.
int8_t savedTrueIndex;    // Saved trueIndex.
```

---

## 🧪 Example of use

```cpp
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
```

## 📤 Program Output Example

```
bit_based_state_control Example

Set state at index 3
States: 0001000000
First True Index: 3
True Count: 1

Toggle state at index 3
States: 0000000000
First True Index: - unassigned
True Count: 0

Set a range of states (2-5 true)
States: 0011110000
First True Index: 2
True Count: 4

Save last states and modify state 7
States: 0000000100
First True Index: 7
True Count: 1

Restore saved state before modification
States: 0011110000
First True Index: 2
True Count: 4

Invert states
States: 1100001111
First True Index: 0
True Count: 6

Get all true indices
True indices (count=6): 0 1 6 7 8 9
```

---

## 🧠 Design Notes
+ Internally stores states in a dynamically allocated uint8_t array using bitwise operations.

+ Ensures safety with index bounds checking.

+ Provides serialization features for external storage or communication.

---

## 🧾 License
This project is licensed under the MIT License — see the [LICENSE](https://github.com/ATphonOS/bit_based_state_control/blob/main/LICENSE.md) file for details.
