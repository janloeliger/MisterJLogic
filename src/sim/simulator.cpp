#include "simulator.h"

#include "part.h"
#include "net.h"

#include <vector>

namespace sim
{
    static void snapshotPinStates(const Circuit* circuit, std::vector<LogicLevel>& out)
    {
        out.clear();

        for (size_t i = 0; i < circuit->partCount(); i++)
        {
            const Part* p = circuit->part(i);

            for (size_t j = 0; j < p->pinCount(); j++)
            {
                out.push_back(p->pin(j).state);
            }
        }
    }

    static bool pinStatesEqual(const std::vector<LogicLevel>& a, const std::vector<LogicLevel>& b)
    {
        if (a.size() != b.size())
        {
            return false;
        }

        for (size_t i = 0; i < a.size(); i++)
        {
            if (a[i] != b[i])
            {
                return false;
            }
        }

        return true;
    }

    Simulator::Simulator(Circuit* circuit)
        : m_circuit(circuit)
    {
    }

    void Simulator::reset()
    {
        if (m_circuit == nullptr)
        {
            return;
        }

        m_circuit->reset();

        m_circuit->resolveAllNets();
        m_circuit->propagateAllNets();
    }

    void Simulator::step()
    {
        if (m_circuit == nullptr)
        {
            return;
        }

        // PASS 0: tick stateful/time-advancing parts exactly once
        for (size_t i = 0; i < m_circuit->partCount(); i++)
        {
            Part* p = m_circuit->part(i);

            if (p->kind() == PartKind::InputSource)
            {
                p->tick();
            }
        }

        // PASS 1..N: settle combinational logic
        std::vector<LogicLevel> before;
        std::vector<LogicLevel> after;

        for (size_t iter = 0; iter < m_maxSettleIterations; iter++)
        {
            snapshotPinStates(m_circuit, before);

            // Tick combinational parts (IC logic)
            for (size_t i = 0; i < m_circuit->partCount(); i++)
            {
                Part* part = m_circuit->part(i);

                if (part->kind() == PartKind::IC)
                {
                    part->tick();
                }
            }

            m_circuit->resolveAllNets();
            m_circuit->propagateAllNets();

            snapshotPinStates(m_circuit, after);

            if (pinStatesEqual(before, after))
            {
                break;
            }
        }
    }

} // namespace sim
