#include "clockSource.h"

namespace sim
{
    ClockSource::ClockSource(std::string instanceName, size_t cycleSpeed)
        : Part("ClockSource", std::move(instanceName), PartKind::InputSource),
          m_cycleSpeed(cycleSpeed)
    {
        // One output pin: OUT
        addPin("OUT", PinDir::Output);
    }

    void ClockSource::changeCycleSpeed(size_t cycleSpeed)
    {
        m_cycleSpeed = cycleSpeed;
    }

    void ClockSource::onReset()
    {
        m_accumulatorCycles = 0.0;
        m_clockHigh = false;

        // drive initial state
        setLogicLevel(0, LogicLevel::Low);
    }

    void ClockSource::onTick()
    {
        if (m_accumulatorCycles >= m_cycleSpeed)
        {
            printf("inside clock:");
            if (m_clockHigh)
            {
                printf("    clock is low\n");
                setLogicLevel(0, LogicLevel::Low);
            }
            else
            {
                printf("    clock is high\n");
                setLogicLevel(0, LogicLevel::High);
            }
            m_accumulatorCycles = 0;
            m_clockHigh = !m_clockHigh;
        } 
        else 
        {
            m_accumulatorCycles++;
        }
    }

} // namespace sim
