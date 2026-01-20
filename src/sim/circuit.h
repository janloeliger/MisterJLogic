#pragma once

#include <memory>
#include <string>
#include <vector>

namespace sim
{
    class Part;
    class Net;

    class Circuit
    {
    public:
        Circuit() = default;

        // Creates a new net and returns a raw pointer to it
        Net* createNet(const std::string& name);

        // Adds a part to the circuit and returns a raw pointer to it
        Part* addPart(std::unique_ptr<Part> part);

        // Find a net by name (returns nullptr if not found)
        Net* findNet(const std::string& name);

        // Accessors
        size_t partCount() const noexcept
        {
            return m_parts.size();
        }

        size_t netCount() const noexcept
        {
            return m_nets.size();
        }

        Part* part(size_t index);
        Net* net(size_t index);

        const Part* part(size_t index) const;
        const Net* net(size_t index) const;

        // Simulation helpers
        void reset();              // calls reset on all parts
        void resolveAllNets();      // resolves all nets
        void propagateAllNets();    // propagates net levels into input pins

    private:
        std::vector<std::unique_ptr<Part>> m_parts; // all parts in this circuit
        std::vector<std::unique_ptr<Net>> m_nets;   // all nets in this circuit
    };
} // namespace sim