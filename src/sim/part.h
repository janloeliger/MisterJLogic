#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace sim
{
    class Net; // Forward declaration

    enum class PinDir : uint8_t
    {
        Input,
        Output,
        Passive
    };

    enum class LogicLevel : uint8_t
    {
        Low,   // 0
        High,  // 1
        unconnected,     // High impedance
        X      // Unknown / conflict
    };

    enum class PartKind : uint8_t
    {
        IC, 
        Transistor,
        InputSource,
        OutputProbe
    };

    struct Pin
    {
        std::string name;
        PinDir dir = PinDir::Passive;

        // Electrical connection, managed by the circuit
        Net* net = nullptr;
        LogicLevel state = LogicLevel::unconnected;
    };

    class Part
    {
    public:
        explicit Part(std::string typeName, std::string instanceName, PartKind kind);

        // Identity
        const std::string& type() const noexcept { return m_type; }
        const std::string& name() const noexcept { return m_name; }
        PartKind kind() const noexcept { return m_kind; }
        void setName(std::string n) { m_name = std::move(n); }

        // Pins
        size_t pinCount() const noexcept { return m_pins.size(); }
        const Pin& pin(size_t index) const;
        Pin& pin(size_t index);

        int findPinIndex(const std::string& pinName) const; // finds pin by the name and returns the index

        // Net wiring helpers
        void connect(size_t pinIndex, Net* net); // connects a pin to a net

        // Digital helpers
        LogicLevel read(size_t pinIndex) const;          // returns the logic level of the given pin
        void setLogicLevel(size_t pinIndex, LogicLevel level);   // sets the LogicLevel from the pin index
        void setDir(size_t pinIndex, PinDir dir); // sets the pindirection

        // Simulation lifecycle
        void reset(); // resets the simulation
        void tick(); // advances everything by a tick

    protected:
        // Derived parts add pins in their constructor
        void addPin(std::string pinName, PinDir dir);

        // Override these in derived parts
        virtual void onReset() {}
        virtual void onTick() {}

    private:
        std::string m_type; // type of this part, example: 74hc181, 74hc123 ...
        std::string m_name; // Name given to this part by the user, example: 74hc181_2, CoProcessor_1 ...
        std::vector<Pin> m_pins; // collection of all pins
        PartKind m_kind = PartKind::IC;
    };
} // namespace sim
