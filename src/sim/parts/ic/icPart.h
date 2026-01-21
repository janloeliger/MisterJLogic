#pragma once

#include "part.h"

#include <cstddef>
#include <cstdint>
#include <string>

namespace sim
{
    class IcPart : public Part
    {
    public:
        explicit IcPart(std::string typeName, std::string instanceName, PartKind kind = PartKind::IC);

    protected:
        // --- Basic logic helpers (4-state-ish with unconnected + X) ---

        // Inverts a logic level (High <-> Low, unconnected stays unconnected, X stays X)
        LogicLevel logicNot(LogicLevel a) const;

        // AND for logic levels (simple model: if any input is X -> X, if any is unconnected -> unconnected)
        LogicLevel logicAnd(LogicLevel a, LogicLevel b) const;

        // OR built using De Morgan (uses only logicNot + logicAnd)
        LogicLevel logicOr(LogicLevel a, LogicLevel b) const;

        // XOR built from OR/AND/NOT:
        // XOR = (A OR B) AND NOT(A AND B)
        LogicLevel logicXor(LogicLevel a, LogicLevel b) const;

        // --- Convenience helpers for reading/writing pins as bits ---

        // Converts a logic level to a boolean bit.
        // Returns true for High, false for Low. For unconnected/X it returns false and sets ok=false.
        bool toBit(LogicLevel v, bool& ok) const;

        // Converts a boolean bit to a logic level (Low/High)
        LogicLevel fromBit(bool bit) const;

        // Adds a bus of pins: baseName0, baseName1, ... baseName(width-1)
        void addBus(const std::string& baseName, size_t width, PinDir dir);

        // Reads a bus as an unsigned integer (LSB at offset 0).
        // If any pin is X/unconnected, ok becomes false (value still returned but may be meaningless).
        uint32_t readBus(size_t firstPinIndex, size_t width, bool& ok) const;

        // Drives a bus from an unsigned integer (LSB at offset 0).
        void writeBus(size_t firstPinIndex, size_t width, uint32_t value);
    };
} // namespace sim