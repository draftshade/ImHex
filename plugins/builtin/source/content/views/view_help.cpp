#include "content/views/view_help.hpp"
#include <hex/helpers/paths.hpp>

#include <hex/helpers/fmt.hpp>
#include <hex/helpers/utils.hpp>

namespace hex::plugin::builtin {

    ViewHelp::ViewHelp() : View("hex.builtin.view.help.about.name") {
    }

    ViewHelp::~ViewHelp() {

    }

    static void link(const std::string &label, const std::string &url) {
        if (ImGui::BulletHyperlink(label.data()))
            hex::openWebpage(url.data());
    }

    void ViewHelp::drawAboutMainPage() {
        ImGui::TextFormatted("ImHex Hex Editor v{} by WerWolv - " ICON_FA_CODE_BRANCH, IMHEX_VERSION);

        #if defined(GIT_BRANCH) && defined(GIT_COMMIT_HASH)
            ImGui::SameLine();
            if (ImGui::Hyperlink(hex::format("{0}@{1}", GIT_BRANCH, GIT_COMMIT_HASH).c_str()))
                hex::openWebpage("https://github.com/WerWolv/ImHex/commit/" GIT_COMMIT_HASH);
        #endif

        ImGui::TextUnformatted("hex.builtin.view.help.about.translator"_lang);

        ImGui::TextUnformatted("hex.builtin.view.help.about.source"_lang); ImGui::SameLine();
        if (ImGui::Hyperlink("WerWolv/ImHex"))
            hex::openWebpage("https://github.com/WerWolv/ImHex");
        ImGui::NewLine();

        ImGui::TextUnformatted("hex.builtin.view.help.about.donations"_lang);
        ImGui::Separator();

        constexpr const char* Links[] = { "https://werwolv.net/donate", "https://www.patreon.com/werwolv", "https://github.com/sponsors/WerWolv" };

        ImGui::TextWrapped("%s", static_cast<const char *>("hex.builtin.view.help.about.thanks"_lang));

        ImGui::NewLine();

        for (auto &link : Links) {
            if (ImGui::Hyperlink(link))
                hex::openWebpage(link);
        }
    }

    void ViewHelp::drawContributorPage() {
        link("Mary for porting ImHex to MacOS", "https://github.com/Thog");
        link("Roblabla for adding the MSI Windows installer", "https://github.com/roblabla");
        link("jam1garner for adding support for Rust plugins", "https://github.com/jam1garner");

    }

    void ViewHelp::drawLibraryCreditsPage() {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.2F, 0.2F, 0.2F, 0.3F));

        link("ImGui by ocornut", "https://github.com/ocornut/imgui");
        link("imgui_club by ocornut", "https://github.com/ocornut/imgui_club");
        link("imnodes by Nelarius", "https://github.com/Nelarius/imnodes");
        link("ImGuiColorTextEdit by BalazsJako", "https://github.com/BalazsJako/ImGuiColorTextEdit");
        link("ImPlot by epezent", "https://github.com/epezent/implot");
        link("capstone by aquynh", "https://github.com/aquynh/capstone");
        link("JSON for Modern C++ by nlohmann", "https://github.com/nlohmann/json");
        link("YARA by VirusTotal", "https://github.com/VirusTotal/yara");
        link("Native File Dialog Extended by btzy and mlabbe", "https://github.com/btzy/nativefiledialog-extended");
        link("Native File Dialog Extended by btzy and mlabbe", "https://github.com/btzy/nativefiledialog-extended");
        ImGui::NewLine();
        link("GNU libmagic", "http://www.darwinsys.com/file/");
        link("GLFW3", "https://github.com/glfw/glfw");
        link("LLVM", "https://github.com/llvm/llvm-project");
        link("Python 3", "https://github.com/python/cpython");
        link("FreeType", "https://gitlab.freedesktop.org/freetype/freetype");
        link("Mbed TLS", "https://github.com/ARMmbed/mbedtls");

        ImGui::PopStyleColor();
    }

    void ViewHelp::drawPathsPage() {
        if (ImGui::BeginTable("##imhex_paths", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit)) {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("Type");
            ImGui::TableSetupColumn("Paths");

            constexpr std::array<std::pair<const char*, ImHexPath>, 8> PathTypes = {{
                { "Resources",          ImHexPath::Resources },
                { "Config",             ImHexPath::Config },
                { "Magic",              ImHexPath::Magic },
                { "Patterns",           ImHexPath::Patterns },
                { "Patterns Includes",  ImHexPath::PatternsInclude },
                { "Plugins",            ImHexPath::Plugins },
                { "Python Scripts",     ImHexPath::Python },
                { "Yara Patterns",      ImHexPath::Yara }
            }};

            ImGui::TableHeadersRow();
            for (const auto &[name, type] : PathTypes) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(name);

                ImGui::TableNextColumn();
                for (auto &path : hex::getPath(type))
                    ImGui::TextUnformatted(path.c_str());
            }

            ImGui::EndTable();
        }
    }

    void ViewHelp::drawAboutPopup() {
        ImGui::SetNextWindowSize(ImVec2(600, 350) * SharedData::globalScale, ImGuiCond_Always);
        if (ImGui::BeginPopupModal(View::toWindowName("hex.builtin.view.help.about.name").c_str(), &this->m_aboutWindowOpen, ImGuiWindowFlags_NoResize)) {

            if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Escape)))
                ImGui::CloseCurrentPopup();

            if (ImGui::BeginTabBar("about_tab_bar")) {

                if (ImGui::BeginTabItem("ImHex")) {
                    ImGui::NewLine();
                    this->drawAboutMainPage();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("hex.builtin.view.help.about.contributor"_lang)) {
                    ImGui::NewLine();
                    this->drawContributorPage();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("hex.builtin.view.help.about.libs"_lang)) {
                    ImGui::NewLine();
                    this->drawLibraryCreditsPage();
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("hex.builtin.view.help.about.paths"_lang)) {
                    ImGui::NewLine();
                    this->drawPathsPage();
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::EndPopup();
        }
    }

    void ViewHelp::drawContent() {
        if (!this->m_aboutWindowOpen)
            this->getWindowOpenState() = false;

        this->drawAboutPopup();
    }

    void ViewHelp::drawMenu() {
        if (ImGui::BeginMenu("hex.menu.help"_lang)) {
            if (ImGui::MenuItem("hex.builtin.view.help.about.name"_lang, "")) {
                View::doLater([] { ImGui::OpenPopup(View::toWindowName("hex.builtin.view.help.about.name").c_str()); });
                this->m_aboutWindowOpen = true;
                this->getWindowOpenState() = true;
            }
            if (ImGui::MenuItem("hex.builtin.view.help.documentation"_lang, "")) {
                hex::openWebpage("https://imhex.werwolv.net/docs");
            }
            ImGui::EndMenu();
        }
    }

}
