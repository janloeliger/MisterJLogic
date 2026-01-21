#include "Circuit.h"

#include "net.h"
#include "parts/part.h"

#include <stdexcept>

namespace sim
{
    Net* Circuit::createNet(const std::string& name)
    {
        m_nets.push_back(std::make_unique<Net>(name));
        return m_nets.back().get();
    }

    Part* Circuit::addPart(std::unique_ptr<Part> part)
    {
        if (!part)
        {
            return nullptr;
        }

        m_parts.push_back(std::move(part));
        return m_parts.back().get();
    }

    Net* Circuit::findNet(const std::string& name)
    {
        for (auto& n : m_nets)
        {
            if (n->name() == name)
            {
                return n.get();
            }
        }

        return nullptr;
    }

    Part* Circuit::part(size_t index)
    {
        if (index >= m_parts.size())
        {
            throw std::out_of_range("Circuit::part index out of range");
        }

        return m_parts[index].get();
    }

    Net* Circuit::net(size_t index)
    {
        if (index >= m_nets.size())
        {
            throw std::out_of_range("Circuit::net index out of range");
        }

        return m_nets[index].get();
    }

    const Part* Circuit::part(size_t index) const
    {
        if (index >= m_parts.size())
        {
            throw std::out_of_range("Circuit::part index out of range");
        }

        return m_parts[index].get();
    }

    const Net* Circuit::net(size_t index) const
    {
        if (index >= m_nets.size())
        {
            throw std::out_of_range("Circuit::net index out of range");
        }

        return m_nets[index].get();
    }

    void Circuit::reset()
    {
        for (auto& p : m_parts)
        {
            p->reset();
        }
    }

    void Circuit::resolveAllNets()
    {
        for (auto& n : m_nets)
        {
            n->resolve();
        }
    }

    void Circuit::propagateAllNets()
    {
        for (auto& n : m_nets)
        {
            n->propagateToPins();
        }
    }

} // namespace sim