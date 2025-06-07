#include "zlgcan_demo.h"
#include "CanApp.h"
// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

CanApp app;
// Main code
int main(int, char**)
{
    if(app.InitCANLibs())
    {
        return -1;
    }
    // Initialize The Video Compenent
    if(app.InitGUIS())
    {
        return -1;
    }

    // Main loop
    while (!app.IsExit())
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        if(app.WindowResized()<0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        app.RenderGUI();
        app.RenderMainWindow();
        app.RenderPlatformWindow();
    }

    // Cleanup
    app.Close();

    return 0;
}
