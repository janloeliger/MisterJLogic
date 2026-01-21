#include <cstdio>
#include <string>
#include <chrono>

#include "sim/circuit.h"
#include "sim/simulator.h"
#include "sim/net.h"

#include "sim/parts/clockSource.h"
#include "sim/parts/ic/notGate.h"

// If you already moved this into your library, include it instead:
// #include "sim/util/logic.h"

// Dear ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// GLFW + OpenGL
#include <GLFW/glfw3.h>

static const char* levelToStr(sim::LogicLevel v)
{
    // If you added it into the library, call it here:
    // return sim::logicLevelToStr(v);

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
    else if (v == sim::LogicLevel::X)
    {
        return "X";
    }
    else
    {
        return "error";
    }
}

static void glfwErrorCallback(int error, const char* description)
{
    std::fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main()
{
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
    {
        std::fprintf(stderr, "Failed to init GLFW\n");
        return 1;
    }

    // OpenGL 3.0+ works fine with ImGui OpenGL3 backend
    const char* glslVersion = "#version 130";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "sim GUI", nullptr, nullptr);
    if (window == nullptr)
    {
        std::fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    // -----------------------
    // Build circuit
    // -----------------------
    sim::Circuit circuit;

    sim::Net* netCLK = circuit.createNet("CLK");
    sim::Net* netN1 = circuit.createNet("N1");

    size_t cycleSpeed = 0; // 0 = toggle every step (your meaning)

    auto clk = std::make_unique<sim::ClockSource>("CLK1", cycleSpeed);
    sim::ClockSource* clkPtr = clk.get();

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

    // -----------------------
    // UI state
    // -----------------------
    bool run = false;
    int stepsPerSecond = 10;
    int stepCounter = 0;

    int uiCycleSpeed = static_cast<int>(cycleSpeed);

    auto lastTime = std::chrono::steady_clock::now();
    double accumulatorSeconds = 0.0;

    // -----------------------
    // Main loop
    // -----------------------
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Timing for "Run"
        auto now = std::chrono::steady_clock::now();
        double dt = std::chrono::duration<double>(now - lastTime).count();
        lastTime = now;

        if (dt < 0.0)
        {
            dt = 0.0;
        }

        accumulatorSeconds += dt;

        // -----------------------
        // Controls window
        // -----------------------
        ImGui::Begin("Controls");

        if (ImGui::Button("Reset"))
        {
            sim.reset();
            stepCounter = 0;
            accumulatorSeconds = 0.0;
        }

        ImGui::SameLine();
        if (ImGui::Button("Step"))
        {
            sim.step();
            stepCounter++;
        }

        ImGui::Checkbox("Run", &run);
        ImGui::SliderInt("Steps / second", &stepsPerSecond, 1, 500);

        ImGui::Separator();

        ImGui::SliderInt("CycleSpeed (0 = every tick)", &uiCycleSpeed, 0, 100);

        if (clkPtr != nullptr)
        {
            clkPtr->changeCycleSpeed(static_cast<size_t>(uiCycleSpeed));
        }

        // Auto stepping when Run = true
        if (run)
        {
            double stepInterval = 1.0 / static_cast<double>(stepsPerSecond);

            while (accumulatorSeconds >= stepInterval)
            {
                accumulatorSeconds -= stepInterval;

                sim.step();
                stepCounter++;
            }
        }

        ImGui::Separator();
        ImGui::Text("Step: %d", stepCounter);
        ImGui::Text("CLK: %s", levelToStr(netCLK->level()));
        ImGui::Text("N1 : %s", levelToStr(netN1->level()));

        ImGui::End();

        // -----------------------
        // Nets window
        // -----------------------
        ImGui::Begin("Nets");

        for (size_t i = 0; i < circuit.netCount(); i++)
        {
            sim::Net* n = circuit.net(i);
            ImGui::Text("%s = %s", n->name().c_str(), levelToStr(n->level()));
        }

        ImGui::End();

        // -----------------------
        // Parts window
        // -----------------------
        ImGui::Begin("Parts");

        for (size_t i = 0; i < circuit.partCount(); i++)
        {
            sim::Part* p = circuit.part(i);

            std::string header = p->type() + std::string("  ") + p->name();
            if (ImGui::CollapsingHeader(header.c_str()))
            {
                for (size_t j = 0; j < p->pinCount(); j++)
                {
                    const sim::Pin& pin = p->pin(j);

                    const char* dirStr = "Passive";
                    if (pin.dir == sim::PinDir::Input)
                    {
                        dirStr = "Input";
                    }
                    else if (pin.dir == sim::PinDir::Output)
                    {
                        dirStr = "Output";
                    }

                    const char* netName = "(null)";
                    if (pin.net != nullptr)
                    {
                        netName = pin.net->name().c_str();
                    }

                    ImGui::Text("Pin %llu  %-6s  %-8s  net=%s  state=%s",
                                static_cast<unsigned long long>(j),
                                pin.name.c_str(),
                                dirStr,
                                netName,
                                levelToStr(pin.state));
                }
            }
        }

        ImGui::End();

        // -----------------------
        // Render
        // -----------------------
        ImGui::Render();

        int displayW = 0;
        int displayH = 0;
        glfwGetFramebufferSize(window, &displayW, &displayH);

        glViewport(0, 0, displayW, displayH);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // -----------------------
    // Shutdown
    // -----------------------
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
