#include "net.h"
#include "part.h"

#include <algorithm>

namespace sim
{
    Net::Net(std::string name)
        : m_name(std::move(name)),
          m_level(LogicLevel::unconnected)
    {
    }

    void Net::addPin(Pin* pin)
    {
        if (pin == nullptr)
        {
            return;
        }

        // Check if the pin is already in the list
        auto it = std::find(m_pins.begin(), m_pins.end(), pin);
        if (it != m_pins.end()) 
        {
            return;
        }

        // Add it
        m_pins.push_back(pin);

        // Make sure the pin points to this net
        pin->net = this;
    }

    void Net::removePin(Pin* pin)
    {
        if (pin == nullptr)
        {
            return;
        }

        // Remove the pin pointer from the list (remove-erase idiom)
        auto it = std::remove(m_pins.begin(), m_pins.end(), pin);

        if (it != m_pins.end())
        {
            m_pins.erase(it, m_pins.end());
        }

        // If the pin still points to this net, disconnect it
        if (pin->net == this)
        {
            pin->net = nullptr;
        }
    }

    void Net::resolve()
    {
        bool seenHigh = false;
        bool seenLow = false;
        bool seenDriver = false;

        // Scan all connected pins and check what OUTPUT pins are driving
        for (Pin* p : m_pins)
        {
            if (p == nullptr)
            {
                continue;
            }

            if (p->dir != PinDir::Output)
            {
                continue;
            }

            if (p->state == LogicLevel::High)
            {
                seenHigh = true;
                seenDriver = true;
            }
            else if (p->state == LogicLevel::Low)
            {
                seenLow = true;
                seenDriver = true;
            }
            else if (p->state == LogicLevel::X)
            {
                // If any driver is unknown, the net becomes unknown
                m_level = LogicLevel::X;
                return;
            }
            else if (p->state == LogicLevel::unconnected)
            {
                // Output is not driving anything
            }
        }

        // Conflict
        if (seenHigh && seenLow)
        {
            m_level = LogicLevel::X;
            return;
        }

        // Nothing driven
        if (!seenDriver)
        {
            m_level = LogicLevel::unconnected;
            return;
        }

        // Driven to a stable value
        if (seenHigh)
        {
            m_level = LogicLevel::High;
        }
        else
        {
            m_level = LogicLevel::Low;
        }
    }

    void Net::propagateToPins()
    {
        // Push the net level into pins that are not outputs
        for (Pin* p : m_pins)
        {
            if (p == nullptr)
            {
                continue;
            }

            if (p->dir == PinDir::Output)
            {
                continue;
            }

            p->state = m_level;
        }
    }

} // namespace sim
