#include "part.h"
#include "../net.h"
#include <stdexcept>

namespace sim
{
    static void requireIndex(size_t index, size_t count)
    {
        if (index >= count)
            throw std::out_of_range("Pin index out of range");
    }

    Part::Part(std::string typeName, std::string instanceName, PartKind kind)
        : m_type(std::move(typeName)),
          m_name(std::move(instanceName)),
          m_kind(kind)
    {
    }

    const Pin& Part::pin(size_t index) const
    {
        requireIndex(index, m_pins.size());
        return m_pins[index];
    }

    Pin& Part::pin(size_t index)
    {
        requireIndex(index, m_pins.size());
        return m_pins[index];
    }

    int Part::findPinIndex(const std::string& pinName) const
    {
        for (size_t i = 0; i < m_pins.size(); ++i)
        {
            if (m_pins[i].name == pinName)
            {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    void Part::connect(size_t pinIndex, Net* net)
    {
        requireIndex(pinIndex, m_pins.size());

        // if already connected, detach from old net
        if (m_pins[pinIndex].net && m_pins[pinIndex].net != net)
        {
            m_pins[pinIndex].net->removePin(&m_pins[pinIndex]);
        }

        // attach to new net
        m_pins[pinIndex].net = net;

        if (net)
        {
            net->addPin(&m_pins[pinIndex]);
        }
    }

    LogicLevel Part::read(size_t pinIndex) const
    {
        requireIndex(pinIndex, m_pins.size());
        return m_pins[pinIndex].state;
    }

    void Part::setLogicLevel(size_t pinIndex, LogicLevel level)
    {
        requireIndex(pinIndex, m_pins.size());
        m_pins[pinIndex].state = level;
    }

    void Part::setDir(size_t pinIndex, PinDir dir)
    {
        requireIndex(pinIndex, m_pins.size());
        m_pins[pinIndex].dir = dir;
    }

    void Part::addPin(std::string pinName, PinDir dir)
    {
        Pin p;
        p.name = std::move(pinName);
        p.dir = dir;
        p.net = nullptr;
        p.state = LogicLevel::unconnected;

        m_pins.push_back(std::move(p));
    }

    void Part::reset()
    {
        // Standard: alli Pins uf unconnected
        for (auto& p : m_pins){
            p.state = LogicLevel::unconnected;
        }
        onReset();
    }

    void Part::tick()
    {
        onTick();
    }

} // namespace sim
