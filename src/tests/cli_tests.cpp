#include <iostream>
#include <thread>
#include <chrono>

#include "sim/circuit.h"
#include "sim/simulator.h"
#include "sim/parts/clockSource.h"
#include "sim/parts/ic/notGate.h"
#include "sim/Net.h"

static const char* levelToStr(sim::LogicLevel v)
{
    if (v == sim::LogicLevel::Low)
    {
        return "Low";
    }
    else if (v == sim::LogicLevel::High)
    {
        return "High";
    }
    else if (v == sim::LogicLevel::unconnected)
    {
        return "unconnected";
    }
    else
    {
        return "error";
    }
}

int main()
{
    sim::Circuit circuit;

    sim::Net* netCLK = circuit.createNet("CLK");
    sim::Net* netN1 = circuit.createNet("N1");

    auto clk = std::make_unique<sim::ClockSource>("CLK1", 0); // 1 Hz
    auto inv = std::make_unique<sim::NotGate>("U1");

    sim::Part* pClk = circuit.addPart(std::move(clk));
    sim::Part* pInv = circuit.addPart(std::move(inv));

    // Connect clock OUT (pin 0) -> CLK
    pClk->connect(0, netCLK);

    // Connect inverter A (pin 0) -> CLK
    pInv->connect(0, netCLK);

    // Connect inverter Y (pin 1) -> N1
    pInv->connect(1, netN1);

    sim::Simulator sim(&circuit);
    sim.reset();

    for (int i = 0; i < 50; i++)
    {
        sim.step();

        sim::LogicLevel clkLevel = netCLK->level();
        sim::LogicLevel n1Level = netN1->level();

        std::cout << "Step " << i
                  << " | CLK=" << levelToStr(clkLevel)
                  << " | N1=" << levelToStr(n1Level)
                  << std::endl;
    }

    return 0;
}
