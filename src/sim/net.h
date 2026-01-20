#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace sim
{
    struct Pin; // forward declaration (Pin is defined in Part.h)

    enum class LogicLevel : uint8_t; // forward declaration (defined in Part.h)

    class Net
    {
    public:
        explicit Net(std::string name = "");

        // Identity
        const std::string& name() const noexcept { return m_name; }
        void setName(std::string n) { m_name = std::move(n); }

        // Connections
        size_t connectionCount() const noexcept { return m_pins.size(); }

        // Adds a pin pointer to this net (your circuit should call this when wiring)
        void addPin(Pin* pin);

        // Removes a pin pointer from this net
        void removePin(Pin* pin);

        // Returns the resolved logic level of this net
        LogicLevel level() const noexcept { return m_level; }

        // Resolves the net value from all connected pins
        // Rule: if any pin drives High and any pin drives Low => X
        // Rule: if only unconnected => unconnected
        // Rule: otherwise => High or Low
        void resolve();

        // writes the resolved net value back to pins that are not outputs
        // This helps simple simulation where inputs "follow" the net
        void propagateToPins();

    private:
        std::string m_name; // name of this net, example: CLK, RESET, DATA0 ...
        std::vector<Pin*> m_pins; // collection of all pins connected to this net
        LogicLevel m_level; // resolved logic level of this net
    };
} // namespace sim
