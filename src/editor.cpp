#include "editor.h"

bool Editor::Setup() {
    // setup GLFW
    glfwInit();

    // OpenGL 3.3 CORE
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(1280, 720, "Quilt", nullptr, nullptr);
    if (!window) {
        Quilt::Debug::Error("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    // configure OpenGL
    gladLoadGL();
    glViewport(0, 0, 1280, 720);
    
    
    // init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // add fonts
    io.Fonts->AddFontDefault();
    font = io.Fonts->AddFontFromFileTTF("res/font/Consolas.ttf", FONT_SIZE, NULL, io.Fonts->GetGlyphRangesDefault());

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // load translations, parameters, etc
    LoadTranslations();
    LoadEnemyTranslations();
    LoadParams();

    running = true;
    return true;
}

void Editor::Run() {
    while (running && !glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (font) ImGui::PushFont(font);

        // editor logic begin

        ImGui::DockSpaceOverViewport(ImGuiDockNodeFlags_PassthruCentralNode);

        Menu();
        Tabs();

        // editor logic end

        if (font) ImGui::PopFont();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Editor::Cleanup() {
    selected_node = nullptr;
    ClearNodes();
    ClearMapdata();
    ClearParams();
    ClearFilenames();

    delete mapHeader;
    delete enHeader;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}