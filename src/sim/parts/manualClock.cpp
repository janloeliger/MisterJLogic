#include "manualClock.h"

namespace sim
{
    ManualClock::ManualClock(std::string instanceName)
        : Part("ManualClock", std::move(instanceName), PartKind::InputSource)
    {
        addPin("OUT", PinDir::Output);
    }

    void ManualClock::setHigh()
    {
        m_high = true;
        m_pulsePendingLow = false;
        updateOutput();
    }

    void ManualClock::setLow()
    {
        m_high = false;
        m_pulsePendingLow = false;
        updateOutput();
    }

    void ManualClock::pulse()
    {
        // Rising edge now, falling edge on next tick
        m_high = true;
        m_pulsePendingLow = true;
        updateOutput();
    }

    void ManualClock::onReset()
    {
        m_high = false;
        m_pulsePendingLow = false;
        updateOutput();
    }

    void ManualClock::onTick()
    {
        // If a pulse was started, drop low on the next tick
        if (m_pulsePendingLow)
        {
            m_high = false;
            m_pulsePendingLow = false;
            updateOutput();
        }
        else
        {
            // keep output stable
            updateOutput();
        }
    }

    void ManualClock::updateOutput()
    {
        if (m_high)
        {
            setLogicLevel(0, LogicLevel::High);
        }
        else
        {
            setLogicLevel(0, LogicLevel::Low);
        }
    }

} // namespace sim
