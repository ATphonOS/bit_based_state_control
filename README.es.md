Esta documentación también está disponible en **[Inglés](./README.md)**.

# 🔀 bit-based_state_control
## Resumen
`BBStateControl` es una clase C++ ligera y eficiente en el uso de memoria diseñada para gestionar múltiples estados booleanos utilizando un campo de bits. Está optimizada para entornos de microcontroladores y soporta hasta **254 estados independientes**. Diseñada como librería para entornos Arduino.

---

## ⚙️ Características

- Almacenamiento de estados booleanos basado en campos de bits.
- Soporta hasta 254 estados individuales.

> `uint8_t` permite valores de 0 a 255, pero el número máximo de estados se establece en **254** para evitar problemas potenciales con la indexación de arreglos y el almacenamiento de campos de bits. El rango de índices va de `0` a `253`, garantizando una gestión segura de la memoria y evitando desbordamientos. `uint8_t` es óptimo en Arduino Uno (2 KB de RAM) para un bajo consumo de memoria y una ejecución rápida.

- Configuración, alternancia, reinicio y consulta de valores de estado.
- Activación exclusiva de estados (control one-hot).
- Guardado y restauración de estados previos.
- Serialización a formato de cadena.
- Operaciones de rango y en bloque.

### 📌 Microcontroladores compatibles

Compatible con arquitecturas AVR de 8 bits, ARM Cortex-M de 32 bits y Xtensa de 32 bits, siempre que sean compatibles con el IDE de Arduino.

---

## 📦 Instalación

**Opción 1: Copiar los archivos e incluirlos**:

1. Copia los siguientes archivos en tu proyecto de Arduino o proyecto C++ embebido:
   - `bit_based_state_control.h`
   - `bit_based_state_control.cpp`

2. Incluye el archivo de cabecera en tu archivo fuente:
   ```cpp
   #include "bit_based_state_control.h"
   ```

Opción 2: Instalar la biblioteca e incluirla:

1. [Descargar](https://github.com/ATphonOS/bit-based-state-control/archive/refs/heads/main.zip) el repositorio.
2. Abrir el IDE de Arduino.
3. Ir a Sketch > "Incluir biblioteca" > "Añadir bibliotecas ZIP" en el menú.
4. Seleccionar el archivo RAR de la biblioteca.
5. Importar la biblioteca:

Para usar la biblioteca en el sketch, ir a Sketch > "Incluir biblioteca".
Seleccionar la biblioteca instalada de la lista para añadir su directiva #include al código.
```cpp
#include <bit_based_state_control.h>
```

---

## 📚 Clase: `BBStateControl`

### Constructor y destructor

```cpp
explicit BBStateControl(uint8_t size); ~BBStateControl();
```

| Método | Descripción | Parámetros | Devuelve |
|--------|--------------|------------|---------|
| `explicit BBStateControl(uint8_t size)` | Construye un nuevo objeto BBStateControl con un número específico de estados. | `size(uint8_t)`: número de estados a gestionar (máximo 254) | — |
| `~BBStateControl()` | Destructor. Libera la memoria asignada utilizada por la matriz de estados interna. | Ninguno | — |

## 🔓 Métodos públicos

### Control de estado
```cpp
void setState(uint8_t index, bool Exclusive = true);
void setState(uint8_t index, bool state, bool Exclusive = true);
void toggleState(uint8_t index);
void resetArray(); void setAllStates(bool estado);
void setDefaultIndex();
void setRangeStates(uint8_t inicio, uint8_t fin, bool estado);
void invertStates();
```

| Método | Descripción | Parámetros | Devuelve |
|--------|-------------|------------|----------|
| `void setState(uint8_t index, bool Exclusive = true)` | Establece el estado en el índice especificado en `true`, borrando los demás opcionalmente. | `index (uint8_t)`: índice del estado<br>`exclusive (bool)`: si es `true`, borra todos los demás estados (predeterminado `true`) | `void` |
| `void setState(uint8_t index, bool state, bool Exclusive = true)` | Establece el estado en el índice especificado en el valor dado, borrando los demás opcionalmente. | `index (uint8_t)`: índice del estado<br>`state (bool)`: valor a establecer<br>`exclusive (bool)`: si es `true`, borra todos los demás estados (predeterminado `true`) | `void` |
| `void toggleState(uint8_t index)` | Invierte el valor del estado en el índice dado. | `index(uint8_t)`: índice del estado | `void` |
| `void resetArray()` | Restablece todos los estados a `false`. | Ninguno | `void` |
| `void setAllStates(bool state)` | Establece todos los estados al valor especificado. | `state(bool)`: valor que se asignará a todos los estados | `void` |
| `void setDefaultIndex()` | Establece el primer índice (0) a `true` y todos los demás a `false`. | Ninguno | `void` |
| `void setRangeStates(uint8_t start, uint8_t end, bool state)` | Establece un rango de estados a un valor dado. | `start (uint8_t)`: índice inicial<br>`end (uint8_t)`: índice final<br>`state (bool)`: valor a asignar | `void` |
| `void invertStates()` | Invierte todos los estados (de `true` a `false`, y viceversa). | Ninguno | `void` |

### Métodos de consulta

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

| Método | Descripción | Parámetros | Devuelve |
|--------|-------------|------------|---------|
| `bool getState(uint8_t index) const` | Devuelve el valor booleano del estado en el índice especificado. | `index (uint8_t)`: índice del estado (0 a def_size - 1) | `bool`: valor del estado (`true` o `false`) |
| `int8_t getFirstTrueIndex() const` | Devuelve el índice del primer estado `true`. | Ninguno | `int8_t`: índice del primer estado verdadero, o `-1` si no hay ninguno |
| `uint8_t* getAllTrueIndices(uint8_t& count) const` | Devuelve un arreglo de índices donde los estados son `true`. El llamador debe liberar el arreglo con `delete[]`. | `count (uint8_t&)`: referencia para almacenar el número de estados verdaderos | `uint8_t*`: puntero al arreglo de índices, o `nullptr` si no hay ninguno |
| `void getIndex(char* buffer, uint8_t bufSize) const` | Escribe una representación en cadena del índice verdadero actual en el búfer proporcionado. | `buffer (char*)`: búfer para almacenar la cadena<br>`bufSize (uint8_t)`: tamaño del búfer | `void` |
| `int8_t findState(bool state) const` | Encuentra el índice del primer estado que coincide con el valor dado. | `state (bool)`: valor a buscar (`true` o `false`) | `int8_t`: índice del estado coincidente, o `-1` si no se encuentra |
| `uint8_t countTrueStates() const` | Cuenta el número de estados activos (`true`). | Ninguno | `uint8_t`: número de estados verdaderos |
| `bool isAssignedIndex() const` | Comprueba si al menos un estado es `true`. | Ninguno | `bool`: `true` si algún estado está activo, `false` en caso contrario |
| `bool validateSingleState() const` | Valida que exactamente un estado sea `true`. | Ninguno | `bool`: `true` si hay exactamente un estado activo, `false` en caso contrario |

### Serialización

```cpp
void serializeStates(char* buffer, uint8_t bufSize) const;
uint8_t serializeStatesSize() const;
```

| Método | Descripción | Parámetros | Devuelve|
|--------|-------------|------------|----------|
| `void serializeStates(char* buffer, uint8_t bufSize) const` | Serializa los estados actuales en una cadena de `'0'` y `'1'`, almacenada en el búfer proporcionado. | buffer (char*): búfer para almacenar la cadena serializada<br>bufSize (uint8_t): tamaño del búfer  |
| `uint8_t serializeStatesSize() const` | Calcula el tamaño del búfer necesario para serializar todos los estados (incluyendo el terminador nulo). | Ninguno | `uint8_t`: número de carácteres requeridos |

### Guardado y restauración de estados

```cpp
void saveState();
void restoreSavedState();
```

| Método | Description | Parameters | Devuelve |
|--------|-------------|------------|----------|
| `void saveState()` | Guarda el estado actual y el índice activo para su posterior restauración. | Ninguno | `void` |
| `void restoreSavedState()` | Restaura el estado e índice activo previamente guardados. | Ninguno | `void` |

## 🔒 Métodos privados 

### Ayudantes internos para garantizar una manipulación segura de bits y control lógico
```cpp
bool isValidIndex(uint8_t index) const;
void clearOthers(uint8_t index);
void setBit(uint8_t index, bool state);
bool getBit(uint8_t index) const;
```

| Método | Descripción | Parámetros | Devuelve |
|--------|-------------|-----------|-----------|
| `bool isValidIndex(uint8_t index) const` | Comprueba si el índice dado está dentro de los límites válidos. | `index(uint8_t)`: índice a comprobar | `bool`: `true` si es válido, `false` en caso contrario |
| `void clearOthers(uint8_t index)` | Borra todos los estados excepto el del índice especificado. | `index(uint8_t)`: índice del estado a mantener `true` | `void` |
| `void setBit(uint8_t index, bool state)` | Establece el bit del índice dado en el valor especificado. | `index(uint8_t)`: índice del bit<br>`state(bool)`: valor a establecer | `void` |
| `bool getBit(uint8_t index) const` | Obtiene el valor del bit en el índice especificado. | `index (uint8_t)`: índice del bit a leer | `bool`: valor del bit en el índice dado |


### 🔐 Miembros internos

```cpp
uint8_t def_size;         // Total number of states.
uint8_t byte_size;        // Number of bytes used for storage.
uint8_t* array;           // Bitfield array for current states.
uint8_t* savedState;      // Saved state array.
int8_t trueIndex;         // Index of the first true state.
int8_t savedTrueIndex;    // Saved trueIndex.
```

---

## 🧪 Ejemplo de uso

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

## 📤 Ejemplo de salida del programa

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

## 🧠 Notas de diseño
+ Almacena internamente los estados en una matriz uint8_t asignada dinámicamente mediante operaciones bit a bit.

+ Garantiza la seguridad mediante la comprobación de límites de índice.

+ Proporciona funciones de serialización para almacenamiento externo o comunicación.

---

## 🧾 Licencia
Este proyecto está licenciado bajo la licencia MIT. Consulte el archivo [LICENCIA](https://github.com/ATphonOS/bit_based_state_control/blob/main/LICENSE.md) para obtener más información.
