/**
 * @file bit_based_state_control.h
 * @brief Header for the BBStateControl class managing states with a bitfield.
 * @author ATphonOS
 * @version v1.2.0
 * @date 2024
 * @license MIT
 */

#ifndef BIT_BASED_STATE_CONTROL_H
#define BIT_BASED_STATE_CONTROL_H

#include <stdint.h>

/**
 * @class BBStateControl
 * @brief Manages a set of boolean states using a bitfield for memory efficiency.
 *
 * This class provides methods to manipulate boolean states stored as bits in a
 * dynamically allocated array, optimized for microcontrollers. It supports operations
 * like setting, toggling, saving, and restoring states, with a maximum of 254 states.
 */
class BBStateControl {
public:
    /**
     * @brief Initializes the object with a specified number of states.
     * @param size Number of states to manage (max 254).
     */
    explicit BBStateControl(uint8_t size);

    /**
     * @brief Frees allocated memory.
     */
    ~BBStateControl();

    /**
     * @brief Sets a state at the given index to true.
     * @param index Index of the state (0 to def_size-1).
     * @param exclusive If true, clears all other states.
     */
    void setState(uint8_t index, bool exclusive = true);

    /**
     * @brief Sets a state at the given index to a specified value.
     * @param index Index of the state (0 to def_size-1).
     * @param state Value to set (true/false).
     * @param exclusive If true, clears all other states.
     */
    void setState(uint8_t index, bool state, bool exclusive = true);

    /**
     * @brief Toggles the state at the given index.
     * @param index Index of the state (0 to def_size-1).
     */
    void toggleState(uint8_t index);

    /**
     * @brief Resets all states to false.
     */
    void resetArray();

    /**
     * @brief Sets all states to a given value.
     * @param state Value to set (true/false).
     */
    void setAllStates(bool state);

    /**
     * @brief Sets the first state to true and others to false.
     */
    void setDefaultIndex();

    /**
     * @brief Gets the value of a state at the given index.
     * @param index Index of the state (0 to def_size-1).
     * @return True if the state is active, false otherwise.
     */
    bool getState(uint8_t index) const;

    /**
     * @brief Gets the index of the first true state.
     * @return Index of the first true state, or -1 if none.
     */
    int8_t getFirstTrueIndex() const;

    /**
     * @brief Gets all indices of true states.
     * @param count Reference to store the number of true states.
     * @return Pointer to a dynamically allocated array of indices, or nullptr if none.
     * @note Caller must free the returned memory with delete[].
     */
    uint8_t* getAllTrueIndices(uint8_t& count) const;

    /**
     * @brief Writes the index of the current true state as a string.
     * @param buffer Buffer to store the string.
     * @param bufSize Size of the buffer.
     */
    void getIndex(char* buffer, uint8_t bufSize) const;

    /**
     * @brief Finds the first state matching the given value.
     * @param state Value to search for (true/false).
     * @return Index of the first matching state, or -1 if not found.
     */
    int8_t findState(bool state) const;

    /**
     * @brief Gets the size needed to serialize states.
     * @return Number of characters needed (includes null terminator).
     */
    uint8_t serializeStatesSize() const;

    /**
     * @brief Sets a range of states to a given value.
     * @param start Start index of the range (0 to def_size-1).
     * @param end End index of the range (0 to def_size-1).
     * @param state Value to set (true/false).
     */
    void setRangeStates(uint8_t start, uint8_t end, bool state);

    /**
     * @brief Checks if at least one state is true.
     * @return True if any state is active, false otherwise.
     */
    bool isAssignedIndex() const;

    /**
     * @brief Counts the number of true states.
     * @return Number of active states.
     */
    uint8_t countTrueStates() const;

    /**
     * @brief Inverts all states (true to false and vice versa).
     */
    void invertStates();

    /**
     * @brief Validates that exactly one state is true.
     * @return True if exactly one state is active, false otherwise.
     */
    bool validateSingleState() const;

    /**
     * @brief Copies states from another StateControl object.
     * @param source Source object to copy from.
     * @return True if the copy was successful, false if sizes mismatch.
     */
    bool copyStatesFrom(const BBStateControl& source);

    /**
     * @brief Serializes states into a string of '0' and '1'.
     * @param buffer Buffer to store the string.
     * @param bufSize Size of the buffer.
     */
    void serializeStates(char* buffer, uint8_t bufSize) const;

    /**
     * @brief Saves the current state for later restoration.
     */
    void saveState();

    /**
     * @brief Restores the previously saved state.
     */
    void restoreSavedState();

private:
    uint8_t def_size;         ///< Total number of states.
    uint8_t byte_size;        ///< Number of bytes needed for the bitfield.
    uint8_t* array;           ///< Bitfield array storing states.
    uint8_t* savedState;      ///< Array for saving previous state.
    int8_t trueIndex;         ///< Index of the first true state (-1 if none).
    int8_t savedTrueIndex;    ///< Saved trueIndex.

    /**
     * @brief Checks if an index is valid.
     * @param index Index to check.
     * @return True if the index is valid, false otherwise.
     */
    bool isValidIndex(uint8_t index) const;

    /**
     * @brief Clears all states except the specified one.
     * @param index Index of the state to keep.
     */
    void clearOthers(uint8_t index);

    /**
     * @brief Sets a bit at the specified index.
     * @param index Index of the bit.
     * @param state Value of the bit (true/false).
     */
    void setBit(uint8_t index, bool state);

    /**
     * @brief Gets the value of a bit at the specified index.
     * @param index Index of the bit.
     * @return Value of the bit (true/false).
     */
    bool getBit(uint8_t index) const;
};

#endif  // BIT_BASED_STATE_CONTROL_H
