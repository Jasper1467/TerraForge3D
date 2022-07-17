#include "UI/Editor.hpp"
#include "TerraForge3D.hpp"

#include "imgui/imgui.h"

namespace TerraForge3D
{

	namespace UI
	{
		static ImFont* headerFont = nullptr; // TEMP


		Editor::Editor(std::string name)
		{
			this->name = name;
			this->uid = UUID::Generate();
			this->uidStr = this->uid.ToString();
			this->menu = new Menu();
			this->menu->SetMainMenu(false);
			this->menu->SetEnabled(false);
			this->subEditorManager = new EditorManager(name + " [Editor Manager]");
		}

		Editor::~Editor()
		{
			// TF3D_SAFE_DELETE(this->menu);
			// TF3D_SAFE_DELETE(this->subEditorManager);
		}

		void Editor::Update()
		{
			OnUpdate();
			subEditorManager->Update();

		}

		bool Editor::OnContextMenu()
		{
			ImGui::Text("Editor Context Menu");
			ImGui::NewLine();
			return ImGui::Button("Close");
		}

		void Editor::Show()
		{
			if (!headerFont)
				headerFont = Application::Get()->GetFonts()["SemiHeader"].handle;

			if (menu->IsEnabled())
				windowFlags |= ImGuiWindowFlags_MenuBar;

			if (isEnabled && isVisible)
			{
				Utils::ImGuiC::PushSubFont(headerFont);
				
				// ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y * 2.0f));
				bool tmp = ImGui::Begin(name.data(), &isVisible, windowFlags);
				// ImGui::PopStyleVar();

				state.isFocused = ImGui::IsWindowFocused();
				state.isHovered = ImGui::IsWindowHovered();

				Utils::ImGuiC::PopSubFont();
				if (tmp)
				{
					ImGui::PushID(uidStr.data());
					menu->Show();
					OnShow();
					if (ImGui::BeginPopupContextItem(("##ContextMenu" + uidStr).data()))
					{
						if (OnContextMenu())
						{
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}
					ImGui::PopID();
				}
				ImGui::End();
				subEditorManager->Show();
			}
		}

		void Editor::Setup()
		{
			OnStart();
		}

		void Editor::Shutdown()
		{
			OnEnd();
		}

	}

}