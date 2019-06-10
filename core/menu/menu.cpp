#include "menu.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "../features/misc/logs.hpp"
#include "../features/skinchanger/parser.hpp"

#define UNLEN 256
IDirect3DStateBlock9 *state_block;
bool reverse = false;
int offset = 0;
bool show_popup = false;
static bool save_config = false;
static bool load_config = false;

namespace ImGui {
	static auto vector_getter = [](void* vec, int idx, const char** out_text) {
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool combo_array(const char* label, int* current_index, std::vector<std::string>& values) 
	{
		if (values.empty()) { return false; }
		return Combo(label, current_index, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	long get_mils() {
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	}

	void begin_popup(const char* text, int onScreenMils, bool* done) {
		if (!done)
			show_popup = true;

		ImGuiIO &io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		int width = io.DisplaySize.x;
		static long old_time = -1;
		ImGui::SetNextWindowPos(ImVec2(width - offset, 100));
		style.WindowMinSize = ImVec2(100.f, 20.f);
		ImGui::Begin("##PopUpWindow", &show_popup, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImVec2 p = ImGui::GetCursorScreenPos();

		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x - 15, p.y - 13), ImVec2(p.x + ImGui::GetWindowWidth(), p.y - 16), ImColor(167, 24, 71, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));

		long current_time_ms = get_mils();

		ImVec2 text_size = ImGui::CalcTextSize(text);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2 - text_size.y / 2);
		ImGui::Text(text);

		if (!reverse) {
			if (offset < ImGui::GetWindowWidth())
				offset += (ImGui::GetWindowWidth() + 5) * ((1000.0f / ImGui::GetIO().Framerate) / 100);

			if (offset >= ImGui::GetWindowWidth() && old_time == -1) {
				old_time = current_time_ms;
			}
		}

		if (current_time_ms - old_time >= onScreenMils && old_time != -1)
			reverse = true;

		if (reverse) {
			if (offset > 0)
				offset -= (ImGui::GetWindowWidth() + 5) * ((1000.0f / ImGui::GetIO().Framerate) / 100);
			if (offset <= 0) {
				offset = 0;
				reverse = false;
				*done = true;
				old_time = -1;
				show_popup = false;
			}
		}

		ImGui::End();
	}
}

void c_menu::run() {
	//ImGui::GetIO().MouseDrawCursor = opened;
	static int page = 0;

	if (c_menu::get().opened)
	{
		interfaces::inputsystem->enable_input(false);

	}
	else if (!c_menu::get().opened)
	{
		interfaces::inputsystem->enable_input(true);

	}

	if (opened) {
		ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = ImVec4(0 / 255.f, 100 / 255.f, 200 / 255.f, 1.f);
		ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] = ImVec4(0 / 255.f, 100 / 255.f, 200 / 255.f, 1.f);
		ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] = ImVec4(0 / 255.f, 100 / 71, 200 / 255.f, 1.f);

		ImGui::SetNextWindowSize(ImVec2(600, 400));
		ImGui::Begin("spuriii", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar); {
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImColor c = ImColor(32, 114, 247);

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 30), ImVec2(p.x + ImGui::GetWindowWidth(), p.y - 3), ImColor(30, 30, 39));
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x, p.y + 32), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + +30), ImColor(0, 100, 200, 255), ImColor(58, 31, 120, 255), ImColor(58, 31, 120, 255), ImColor(0, 100, 200, 255));
			ImGui::PushFont(font_tabs);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7); 
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7); 
			ImGui::Text("");
			ImGui::SameLine();

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9);
			if (ImGui::ButtonT("rage", ImVec2(50, 30), page, 0, false, false)) page = 0; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("legit", ImVec2(50, 30), page, 1, false, false)) page = 1; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("visuals", ImVec2(50, 30), page, 2, false, false)) page = 2; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("misc", ImVec2(50, 30), page, 3, false, false)) page = 3; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("skins", ImVec2(50, 30), page, 4, false, false)) page = 4; ImGui::SameLine(0, 0);
			if (ImGui::ButtonT("settings", ImVec2(50, 30), page, 5, false, false)) page = 5; ImGui::SameLine(0, 0);
			
			ImGui::PopFont();

			ImGui::PushFont(font_menu);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 45);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 222); 

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
			ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

			if (c_config::get().visuals_preview) {
				run_visuals_preview();
			}

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();

			static int test = 0;
			static int test2 = 0;
			switch (page) 
			{
			case 0:

				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				//push window color for child
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				//push border color for child
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
				ImGui::BeginChild("aimbot", ImVec2(279, 171), true); {
					ImGui::Checkbox("active", &c_config::get().rage_enable);
					if (c_config::get().rage_enable)
					{
						ImGui::Checkbox("auto stop", &c_config::get().aim_auto_stop);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("no recoil", &c_config::get().no_recoil);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
					}

				} ImGui::EndChild(true);

				//ImGui::NextColumn();

				ImGui::PopStyleVar();
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				ImGui::BeginChild("misc", ImVec2(279, 164), true);
				{
						ImGui::Checkbox("anti-aim", &c_config::get().anti_aim);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						if (c_config::get().anti_aim) 
						{
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::Combo("anti-aim pitch", &c_config::get().anti_aim_pitch, "none\0down"); //todo add custom bone selection - designer
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::Combo("anti-aim yaw", &c_config::get().anti_aim_yaw, "none\0manual"); //todo add custom bone selection - designer
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::Checkbox("desync", &c_config::get().desync);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::SliderFloat("range", &c_config::get().desync_range, 0.0f, 59.0f, "%.2f");
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::Checkbox("fake duck", &c_config::get().fake_duck);
							ImGui::Checkbox("fake walk", &c_config::get().fake_walk);
						}
				}
				ImGui::EndChild(true);

				ImGui::NextColumn();
				ImGui::BeginChild("settings", ImVec2(279, 343), true); {
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);

					if (ImGui::ButtonT("pistol", ImVec2(50, 30), test, 0, false, ImColor(0, 0, 0))) test = 0; ImGui::SameLine(0, 0);
					if (ImGui::ButtonT("rifle", ImVec2(50, 30), test, 1, false, false)) test = 1; ImGui::SameLine(0, 0);
					if (ImGui::ButtonT("sniper", ImVec2(50, 30), test, 2, false, false)) test = 2; ImGui::SameLine(0, 0);
					if (ImGui::ButtonT("smg", ImVec2(50, 30), test, 3, false, false)) test = 3; ImGui::SameLine(0, 0);
					if (ImGui::ButtonT("heavy", ImVec2(50, 30), test, 4, false, false)) test = 4;

					ImGui::PushFont(font_menu);

						switch (test) {
						case 0:
							break;
						case 1:

							break;
						case 2:

							break;
						case 3:

							break;
						case 4:

							break;
						}

					ImGui::PopFont();
				} ImGui::EndChild(true);
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();

				break;
			case 1:
			
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				//push window color for child
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				//push border color for child
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
				ImGui::BeginChild("aimbot", ImVec2(279, 171), true); {
				ImGui::Checkbox("active", &c_config::get().aim_enabled);
					
					if (c_config::get().aim_enabled)
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Combo("mode", &c_config::get().aim_mode, "hitbox\0nearest hitbox"); //todo add custom bone selection - designer
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("dynamic fov", &c_config::get().aim_distance_based_fov);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("draw fov", &c_config::get().fov_crosshair);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("silent aim", &c_config::get().aim_silent);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("scope aim", &c_config::get().scope_aim);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("smoke aim", &c_config::get().smoke_check);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

						ImGui::Checkbox("friendly fire", &c_config::get().aim_team_check);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());

					}
					ImGui::Checkbox("backtrack", &c_config::get().backtrack);
					if (c_config::get().backtrack )
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::SliderFloat("backtrack milisec", &c_config::get().backtrack_ms, 1.0f, 200.0f, "%.2f");
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
					}
					if (c_config::get().backtrack && c_config::get().aim_enabled)
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("aim at records", &c_config::get().aim_at_backtrack);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
					}
					ImGui::Checkbox("standalone rcs", &c_config::get().rcs_standalone);
					if (c_config::get().rcs_standalone)
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::SliderFloat("rcs amount", &c_config::get().rcs_standalone_x, 0.0f, 2.0f, "%.2f");
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

					}

				} ImGui::EndChild(true);

				//ImGui::NextColumn();

				ImGui::PopStyleVar();
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				ImGui::BeginChild("triggerbot", ImVec2(279, 164), true); 
				{
					ImGui::Checkbox("enable", &c_config::get().trigger_enable);
					if (c_config::get().trigger_enable)
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						if (ImGui::BeginCombo("hitbox", "...", ImVec2(0, 105))) {
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("head"), &c_config::get().trigger_hitbox_head, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("body"), &c_config::get().trigger_hitbox_body, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("arms"), &c_config::get().trigger_hitbox_arms, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("legs"), &c_config::get().trigger_hitbox_legs, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::EndCombo();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						}
						ImGui::SliderInt("delay", &c_config::get().trigger_delay, 0, 20);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("recoil", &c_config::get().trigger_recoil);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Text("[trigger keybind is mouse4]");
						//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						//ImGui::Hotkey("##trigger key", &c_config::get().trigger_key, ImVec2(100, 20));
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
					}

				}
				ImGui::EndChild(true);

				ImGui::NextColumn();
				ImGui::BeginChild("settings", ImVec2(279, 343), true); {
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);

						if (ImGui::ButtonT("pistol", ImVec2(50, 30), test, 0, false, ImColor(0, 0, 0))) test = 0; ImGui::SameLine(0, 0);
						if (ImGui::ButtonT("rifle", ImVec2(50, 30), test, 1, false, false)) test = 1; ImGui::SameLine(0, 0);
						if (ImGui::ButtonT("sniper", ImVec2(50, 30), test, 2, false, false)) test = 2; ImGui::SameLine(0, 0);
						if (ImGui::ButtonT("smg", ImVec2(50, 30), test, 3, false, false)) test = 3; ImGui::SameLine(0, 0);
						if (ImGui::ButtonT("heavy", ImVec2(50, 30), test, 4, false, false)) test = 4;

						ImGui::PushFont(font_menu);
						if (c_config::get().aimbot_mode == 0)
						{
							switch (test) {
							case 0:
								if (c_config::get().aim_mode == 0) {
									ImGui::Combo("pistol hitbox", &c_config::get().aim_bone_pistol, "head\0neck\0chest\0stomach\0pelvis");
								}
								ImGui::SliderFloat("pistol fov", &c_config::get().aim_fov_pistol, 0.0f, 5.0f, "%.2f");
								ImGui::SliderFloat("pistol smooth", &c_config::get().aim_smooth_pistol, 1.f, 10.f, "%.2f");
								//ImGui::SliderFloat("pistol rcs x", &c_config::get().rcs_x_pistol, 0.0f, 1.0f, "%.2f");
								//ImGui::SliderFloat("pistol rcs y", &c_config::get().rcs_y_pistol, 0.0f, 1.0f, "%.2f");
								break;
							case 1:
								if (c_config::get().aim_mode == 0) {
									ImGui::Combo("rifle hitbox", &c_config::get().aim_bone_rifle, "head\0neck\0chest\0stomach\0pelvis");
								}
								ImGui::SliderFloat("rifle fov", &c_config::get().aim_fov_rifle, 0.0f, 5.0f, "%.2f");
								ImGui::SliderFloat("rifle smooth", &c_config::get().aim_smooth_rifle, 1.f, 10.f, "%.2f");
								ImGui::SliderFloat("rifle rcs x", &c_config::get().rcs_x_rifle, 0.0f, 1.0f, "%.2f");
								ImGui::SliderFloat("rifle rcs y", &c_config::get().rcs_y_rifle, 0.0f, 1.0f, "%.2f");
								break;
							case 2:
								if (c_config::get().aim_mode == 0) {
									ImGui::Combo("sniper hitbox", &c_config::get().aim_bone_sniper, "head\0neck\0chest\0stomach\0pelvis");
								}
								ImGui::SliderFloat("sniper fov", &c_config::get().aim_fov_sniper, 0.0f, 5.0f, "%.2f");
								ImGui::SliderFloat("sniper smooth", &c_config::get().aim_smooth_sniper, 1.f, 10.f, "%.2f");
								//ImGui::SliderFloat("sniper rcs x", &c_config::get().rcs_x_sniper, 0.0f, 1.0f, "%.2f");
								//ImGui::SliderFloat("sniper rcs y", &c_config::get().rcs_y_sniper, 0.0f, 1.0f, "%.2f");
								break;
							case 3:
								if (c_config::get().aim_mode == 0) {
									ImGui::Combo("smg hitbox", &c_config::get().aim_bone_smg, "head\0neck\0chest\0stomach\0pelvis");
								}

								ImGui::SliderFloat("smg fov", &c_config::get().aim_fov_smg, 0.0f, 5.0f, "%.2f");
								ImGui::SliderFloat("smg smooth", &c_config::get().aim_smooth_smg, 1.f, 10.f, "%.2f");
								ImGui::SliderFloat("smg rcs x", &c_config::get().rcs_x_smg, 0.0f, 10.0f, "%.2f");
								ImGui::SliderFloat("smg rcs y", &c_config::get().rcs_y_smg, 0.0f, 10.0f, "%.2f");
								break;
							case 4:
								if (c_config::get().aim_mode == 0) {
									ImGui::Combo("heavy hitbox", &c_config::get().aim_bone_heavy, "head\0neck\0chest\0stomach\0pelvis");
								}
								ImGui::SliderFloat("heavy fov", &c_config::get().aim_fov_heavy, 0.0f, 5.0f, "%.2f");
								ImGui::SliderFloat("heavy smooth", &c_config::get().aim_smooth_heavy, 1.f, 10.f, "%.2f");
								ImGui::SliderFloat("heavy rcs x", &c_config::get().rcs_x_heavy, 0.0f, 1.0f, "%.2f");
								ImGui::SliderFloat("heavy rcs y", &c_config::get().rcs_y_heavy, 0.0f, 1.0f, "%.2f");
								break;
							}
						}
						

					
					ImGui::PopFont();
				} ImGui::EndChild(true);
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleVar();

				break;
			case 2:
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

				ImGui::BeginChild("esp", ImVec2(279, 343), true); {
					ImGui::Checkbox("active", &c_config::get().visuals_enabled);
					if (c_config::get().visuals_enabled) {
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("show team", &c_config::get().visuals_team_check);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("esp on key", &c_config::get().esp_hold);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("dead esp", &c_config::get().dead_esp);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("dormancy check", &c_config::get().dormant_check);			
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						//ImGui::Checkbox("bullet beams", &c_config::get().bullet_beams);
						//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						//ImGui::SliderInt("bullet beams width", &c_config::get().bullet_beams_width, 0, 200);
						//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						//ImGui::SliderInt("bullet beams duration", &c_config::get().bullet_beams_duration, 0, 200);
						//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						if (ImGui::BeginCombo("entity", "...", ImVec2(0, 105))) {
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("name"), &c_config::get().player_name, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("box"), &c_config::get().player_box, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("skeleton"), &c_config::get().skeleton, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("health"), &c_config::get().player_health, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("weapon"), &c_config::get().player_weapon, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("footstep"), &c_config::get().sound_footstep, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("offscreen arrows"), &c_config::get().vis_arrows, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::EndCombo();
						}
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						if (ImGui::BeginCombo("flags", "...", ImVec2(0, 105))) {
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("armor"), &c_config::get().player_flags_armor, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("bomb"), &c_config::get().player_flags_c4, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("flashed"), &c_config::get().player_flags_flashed, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("behind smoke"), &c_config::get().player_flags_behind_smoke, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("money"), &c_config::get().player_flags_money, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("scoped"), &c_config::get().player_flags_scoped, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("vulnerability check"), &c_config::get().vulnerability_check, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("damage dealt"), &c_config::get().damage_indicator, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::EndCombo();
						}
						if (ImGui::BeginCombo("removals", "...", ImVec2(0, 105))) {
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("smoke"), &c_config::get().remove_smoke, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("flash"), &c_config::get().reduce_flash, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("sleeves"), &c_config::get().remove_sleeves, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("hands"), &c_config::get().remove_hands, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("weapon"), &c_config::get().remove_weapon, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("scope overlay"), &c_config::get().remove_scope, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("post processing"), &c_config::get().disable_post_processing, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::EndCombo();
						}

						if (ImGui::BeginCombo("world", "...", ImVec2(0, 105))) {
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("bomb info"), &c_config::get().bomb_planted, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("dropped weapons"), &c_config::get().dropped_weapons, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("grenades"), &c_config::get().projectiles, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("grenade prediction"), &c_config::get().nade_pred, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("misc entities"), &c_config::get().entity_esp, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4); 
							ImGui::Selectable(("danger zone"), &c_config::get().danger_zone_dropped, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::EndCombo();
						}
					}
				}
				ImGui::EndChild(true);
				ImGui::NextColumn();
				ImGui::BeginChild("glow", ImVec2(279, 168), true); {
					if (c_config::get().visuals_enabled) {
						ImGui::Checkbox("active", &c_config::get().visuals_glow);
						if (c_config::get().visuals_glow)
						{
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::Checkbox("enemy", &c_config::get().visuals_glow_enemy);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::Checkbox("planted bomb", &c_config::get().visuals_glow_planted);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::Checkbox("dropped weapons", &c_config::get().visuals_glow_weapons);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						}						
					}
				}
				ImGui::EndChild(true);
				ImGui::Spacing();
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
				ImGui::BeginChild("chams", ImVec2(279, 167), true); {
					if (c_config::get().visuals_enabled) {
						ImGui::Combo("chams enemy type", &c_config::get().vis_chams_type, "textured\0blue crystal\0pulsating\0gold\0bubble\0metallic");
						ImGui::Combo("chams misc type", &c_config::get().vis_chams_misc_type, "textured\0bubble\0pulsating\0gold\0blue crystal\0glass\0metallic");
						if (ImGui::BeginCombo("chams model", "...", ImVec2(0, 105))) 
						{
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("enemy"), &c_config::get().vis_chams_vis, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("enemy XQZ"), &c_config::get().vis_chams_invis, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("sleeves"), &c_config::get().sleeve_chams, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("hands"), &c_config::get().hand_chams, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("weapon"), &c_config::get().weapon_chams, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::EndCombo();
						}
						ImGui::Checkbox("chams misc epilepsy", &c_config::get().chams_epilepsy);
						ImGui::Checkbox("chams misc wireframe", &c_config::get().vis_chams_misc_wireframe);
						ImGui::Checkbox("chams enemy gloss", &c_config::get().chams_shiny);
						ImGui::Checkbox("backtrack", &c_config::get().backtrack_visualize);
						ImGui::Checkbox("smoke check", &c_config::get().vis_chams_smoke_check);

					}
				}
				ImGui::EndChild(true);

				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Columns();
				break;
			case 3:
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

				ImGui::BeginChild("misc", ImVec2(279, 343), true); {
					ImGui::Checkbox("active", &c_config::get().misc_enabled);
					if (c_config::get().misc_enabled)
					{

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("radar", &c_config::get().radar);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);

						if (ImGui::BeginCombo("logs", "...", ImVec2(0, 65))) {
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("player hurt"), &c_config::get().logs_player_hurt, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("player bought"), &c_config::get().logs_player_bought, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 4);
							ImGui::Selectable(("config system"), &c_config::get().logs_config_system, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::EndCombo();
						}
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);

						ImGui::Checkbox("hitmarker", &c_config::get().hitmarker);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Combo("hit sound", &c_config::get().hitmarker_sound, "none\0click\0money\0turret\0skeet\0bameware\0cod");
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("anti screenshot", &c_config::get().anti_screenshot);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("spectators", &c_config::get().spectators_list);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("smart weapon switch", &c_config::get().smart_switch);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("rank reveal", &c_config::get().rank_reveal);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("viewmodel options", &c_config::get().viewmodel_enable);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						if (c_config::get().viewmodel_enable)
						{
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::SliderInt("viewmodel field of view", &c_config::get().viewmodel_fov, 0, 135);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
							ImGui::SliderInt("field of view", &c_config::get().fov, 0, 60);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
							ImGui::Checkbox("custom viewmodel offset", &c_config::get().viewmodel_offset);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
							if (c_config::get().viewmodel_offset)
							{
								ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
								ImGui::SliderInt("viewmodel x", &c_config::get().viewmodel_x, -20, 20);
								ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
								ImGui::SliderInt("viewmodel y", &c_config::get().viewmodel_y, -20, 20);
								ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 30);
								ImGui::SliderInt("viewmodel z", &c_config::get().viewmodel_z, -20, 20);
								ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							}
						}
						ImGui::Checkbox("aspect ratio", &c_config::get().aspect_ratio);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						if (c_config::get().aspect_ratio)
						{
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::SliderInt("aspect ratio value", &c_config::get().aspect_ratio_value, 0, 200);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						}

						ImGui::Checkbox("left hand knife", &c_config::get().left_knife);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("flying ragdolls", &c_config::get().ragdoll_gravity);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("force scope", &c_config::get().force_scope);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("force crosshair", &c_config::get().force_crosshair);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("recoil crosshair", &c_config::get().recoil_crosshair);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("night mode", &c_config::get().nightmode);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::ColorEdit4("sky color", c_config::get().clr_sky, ImGuiColorEditFlags_NoInputs);

						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						if (c_config::get().nightmode) {
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::SliderInt("brightness", &c_config::get().nightmode_brightness, 0, 100);					
						}

						ImGui::Checkbox("world color", &c_config::get().world_color);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::ColorEdit4("world colorr", c_config::get().clr_world_color, ImGuiColorEditFlags_NoInputs);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("chat spam", &c_config::get().chat_spam);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Checkbox("clantag changer", &c_config::get().clan_tag);
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						ImGui::Combo("name changer", &c_config::get().name_spam, "none\0blank\0promo\0fake vote\0name stealer\0name stealer silent");
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);


					}
				}			
				ImGui::EndChild(true);
				ImGui::NextColumn();				
				ImGui::BeginChild("movement", ImVec2(279, 343), true); 
				{
					if (c_config::get().misc_enabled)
					{
						ImGui::Combo("bhop", &c_config::get().bunny_hop, "none\0normal\0legit");
						if (c_config::get().bunny_hop == 2)
						{
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::SliderInt("hitchance", &c_config::get().bunny_hop_hitchance, 0, 100);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::SliderInt("minimum hops", &c_config::get().bunny_hop_minimum_value, 0, 20);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::SliderInt("maximum hops", &c_config::get().bunny_hop_maximum_value, 0, 20);
						}


						ImGui::Combo("autostrafe", &c_config::get().auto_strafers, "none\0normal\0wasd");
						if (c_config::get().auto_strafers)
						{
							ImGui::Checkbox("silent autostrafe", &c_config::get().auto_strafers_silent);
						}


						ImGui::Checkbox("edge jump", &c_config::get().edge_jump);
						if (c_config::get().edge_jump) {
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::Checkbox("duck in air", &c_config::get().edge_jump_duck_in_air);
						}

						ImGui::Checkbox("infinite duck", &c_config::get().infinite_duck);

						ImGui::Checkbox("thirdperson", &c_config::get().third_person);
						//ImGui::Checkbox("force thirdperson spectators", &c_config::get().thirdperson_spectators);
						ImGui::Checkbox("freecam", &c_config::get().free_cam);
						if (c_config::get().free_cam)
						{
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::SliderFloat("freecam speed", &c_config::get().free_cam_speed, 5, 25);
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
						}
						ImGui::Checkbox("walkbot", &c_config::get().walk_bot);
						if (c_config::get().walk_bot)
						{
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::SliderFloat("walkbot distance", &c_config::get().walk_bot_distance, 0, 500);
							//ImGui::SliderInt("walkbot turn", &c_config::get().walk_bot_turn, -180, 180);
						}
						ImGui::Checkbox("fakelag", &c_config::get().fake_lag_enable);
						if (c_config::get().fake_lag_enable)
						{
							//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							//ImGui::Combo("type", &c_config::get().fake_lag_type, "maximum\0adaptive");
							ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
							ImGui::SliderInt("choked ticks", &c_config::get().fake_lag_amount, 0, 15);
						}

					}
				}
				ImGui::EndChild(true);
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Columns();
				break;

			case 4:
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
				ImGui::BeginChild("knife", ImVec2(279, 168), true); 
				{
					ImGui::Checkbox("enable", &c_config::get().skinchanger_enable);
					if (c_config::get().skinchanger_enable)
					{
						ImGui::Combo("knife", &c_config::get().knife_model, "default\0bayonet\0m9\0karambit\0bowie\0butterfly\0falchion\0flip\0gut\0huntsman\0shaddow daggers\0navaja\0stiletto\0talon\0ursus");
						ImGui::Combo("condition", &c_config::get().knife_wear, "factory new\0minimal wear\0field-tested\0well-worn\0battle-scarred");

						ImGui::Combo(("skin"), &c_config::get().paint_kit_vector_index_knife, [](void* data, int idx, const char** out_text) {
							*out_text = parser_skins[idx].name.c_str();
							return true;
						}, nullptr, parser_skins.size(), 10);
						c_config::get().paint_kit_index_knife = parser_skins[c_config::get().paint_kit_vector_index_knife].id;
					}
				}
				ImGui::EndChild(true);

				ImGui::PopStyleVar();
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				ImGui::BeginChild("settings", ImVec2(279, 167), true); {
					if (c_config::get().skinchanger_enable)
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
						if (ImGui::Button("force update", ImVec2(84, 18))) {
							utilities::force_update();
						}
					}

				}
				ImGui::EndChild(true);

				ImGui::NextColumn();

				ImGui::BeginChild("weapons", ImVec2(279, 343), true); {
					if (c_config::get().skinchanger_enable)
					{
						static int weapons_page = 0;
						if (ImGui::ButtonT("pistol", ImVec2(50, 30), weapons_page, 0, false, ImColor(0, 0, 0))) weapons_page = 0; ImGui::SameLine(0, 0);
						if (ImGui::ButtonT("rifle", ImVec2(50, 30), weapons_page, 1, false, false)) weapons_page = 1; ImGui::SameLine(0, 0);
						if (ImGui::ButtonT("sniper", ImVec2(50, 30), weapons_page, 2, false, false)) weapons_page = 2; ImGui::SameLine(0, 0);

						switch (weapons_page) {
						case 0:
							ImGui::Combo(("p2000"), &c_config::get().paint_kit_vector_index_p2000, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_p2000 = parser_skins[c_config::get().paint_kit_vector_index_p2000].id;


							ImGui::Combo(("usp-s"), &c_config::get().paint_kit_vector_index_usp, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_usp = parser_skins[c_config::get().paint_kit_vector_index_usp].id;

							ImGui::Combo(("glock"), &c_config::get().paint_kit_vector_index_glock, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_glock = parser_skins[c_config::get().paint_kit_vector_index_glock].id;

							ImGui::Combo(("p250"), &c_config::get().paint_kit_vector_index_p250, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_p250 = parser_skins[c_config::get().paint_kit_vector_index_p250].id;

							ImGui::Combo(("five-seven"), &c_config::get().paint_kit_vector_index_fiveseven, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_fiveseven = parser_skins[c_config::get().paint_kit_vector_index_fiveseven].id;

							ImGui::Combo(("tec9"), &c_config::get().paint_kit_vector_index_tec, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_tec = parser_skins[c_config::get().paint_kit_vector_index_tec].id;

							ImGui::Combo(("cz75a"), &c_config::get().paint_kit_vector_index_cz, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_cz = parser_skins[c_config::get().paint_kit_vector_index_cz].id;

							ImGui::Combo(("duals"), &c_config::get().paint_kit_vector_index_duals, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_duals = parser_skins[c_config::get().paint_kit_vector_index_duals].id;

							ImGui::Combo(("deagle"), &c_config::get().paint_kit_vector_index_deagle, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_deagle = parser_skins[c_config::get().paint_kit_vector_index_deagle].id;

							ImGui::Combo(("revolver"), &c_config::get().paint_kit_vector_index_revolver, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_revolver = parser_skins[c_config::get().paint_kit_vector_index_revolver].id;

							break;
						case 1:
							ImGui::Combo(("famas"), &c_config::get().paint_kit_vector_index_famas, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_famas = parser_skins[c_config::get().paint_kit_vector_index_famas].id;

							ImGui::Combo(("galil"), &c_config::get().paint_kit_vector_index_galil, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_galil = parser_skins[c_config::get().paint_kit_vector_index_galil].id;

							ImGui::Combo(("m4a4"), &c_config::get().paint_kit_vector_index_m4a4, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_m4a4 = parser_skins[c_config::get().paint_kit_vector_index_m4a4].id;

							ImGui::Combo(("m4a1"), &c_config::get().paint_kit_vector_index_m4a1, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_m4a1 = parser_skins[c_config::get().paint_kit_vector_index_m4a1].id;

							ImGui::Combo(("ak47"), &c_config::get().paint_kit_vector_index_ak47, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_ak47 = parser_skins[c_config::get().paint_kit_vector_index_ak47].id;

							ImGui::Combo(("sg 553"), &c_config::get().paint_kit_vector_index_sg553, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_sg553 = parser_skins[c_config::get().paint_kit_vector_index_sg553].id;

							ImGui::Combo(("aug"), &c_config::get().paint_kit_vector_index_aug, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_aug = parser_skins[c_config::get().paint_kit_vector_index_aug].id;

							break;
						case 2:
							ImGui::Combo(("ssg08"), &c_config::get().paint_kit_vector_index_ssg08, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_ssg08 = parser_skins[c_config::get().paint_kit_vector_index_ssg08].id;

							ImGui::Combo(("awp"), &c_config::get().paint_kit_vector_index_awp, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_awp = parser_skins[c_config::get().paint_kit_vector_index_awp].id;

							ImGui::Combo(("scar20"), &c_config::get().paint_kit_vector_index_scar, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_scar = parser_skins[c_config::get().paint_kit_vector_index_scar].id;

							ImGui::Combo(("g3sg1"), &c_config::get().paint_kit_vector_index_g3sg1, [](void* data, int idx, const char** out_text) {
								*out_text = parser_skins[idx].name.c_str();
								return true;
							}, nullptr, parser_skins.size(), 10);
							c_config::get().paint_kit_index_g3sg1 = parser_skins[c_config::get().paint_kit_vector_index_g3sg1].id;

							break;


						}
					}
				}
				ImGui::EndChild(true);
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Columns();
				break;			

			case 5:
				ImGui::Columns(2, NULL, false);
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
				ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));
				ImGui::BeginChild("config", ImVec2(279, 168), true); {
					static char entered_config_name[64] = { 0 };
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
					ImGui::PushItemWidth(175);
					ImGui::InputText(("##CFG"), entered_config_name, 64);
					static int selected;
					std::string config;
					std::vector<std::string> configs = c_config::get().get_configs();
					if (configs.size() > 0) {
						ImGui::combo_array(("configs"), &selected, configs);
						config = configs[c_config::get().counts];
					}
					c_config::get().counts = selected;

					if (configs.size() > 0) {
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
						if (ImGui::Button(("load"), ImVec2(175, 20))) {
							c_config::get().load_config(config);
							load_config = true;

							if (c_config::get().logs_config_system) {
								utilities::console_warning("[config system] ");
								interfaces::console->console_printf(config.c_str());
								interfaces::console->console_printf(" loaded. \n");
								std::stringstream ss;
								ss << config.c_str() << " loaded.";
								c_event_logs::get().add(ss.str(), color(167, 255, 255, 255));
							}
						}
					}
					if (configs.size() >= 1) {
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
						if (ImGui::Button(("save"), ImVec2(175, 20))) {
							c_config::get().save_config(config);
							save_config = true;

							if (c_config::get().logs_config_system) {
								utilities::console_warning("[config system] ");
								interfaces::console->console_printf(config.c_str());
								interfaces::console->console_printf(" saved. \n");
								std::stringstream ss;
								ss << config.c_str() << " saved.";
								c_event_logs::get().add(ss.str(), color(167, 255, 255, 255));
							}

						}
					}
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
					if (ImGui::Button(("create"), ImVec2(175, 20))) {
						std::string config_file_name = entered_config_name;
						if (config_file_name.size() < 1) {
							config_file_name = ("settings");
						}
						c_config::get().create_config(config_file_name);

					}
					if (config.size() >= 1) {
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
						if (ImGui::Button(("delete"), ImVec2(175, 20))) {
							c_config::get().remove_config(config);

						}
					}

				}
				ImGui::EndChild(true);

				ImGui::PopStyleVar();
				ImGui::Dummy(ImVec2(0, -2)); ImGui::SameLine();
				ImGui::Dummy(ImVec2(0, 0)); ImGui::SameLine();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));

				ImGui::BeginChild("misc", ImVec2(279, 167), true); {
					ImGui::Combo("keybinds", &c_config::get().keybinds_selection, "edge jump key\0force scope key\0esp hold key\0thirdperson key\0freecam key\0fake walk key");

					if (c_config::get().keybinds_selection == 0) 
					{
						ImGui::Hotkey("##edge jump key", &c_config::get().edge_jump_key, ImVec2(100, 20));
					}
					if (c_config::get().keybinds_selection == 1)
					{
						ImGui::Hotkey("##force scope key", &c_config::get().force_scope_key, ImVec2(100, 20));
					}
					if (c_config::get().keybinds_selection == 2)
					{
						ImGui::Hotkey("##esp hold key", &c_config::get().esp_hold_key, ImVec2(100, 20));
					}
					if (c_config::get().keybinds_selection == 3)
					{
						ImGui::Hotkey("##thirdperson key", &c_config::get().third_person_key, ImVec2(100, 20));
					}
					if (c_config::get().keybinds_selection == 4)
					{
						ImGui::Hotkey("##freecam key", &c_config::get().free_cam_key, ImVec2(100, 20));
					}
					if (c_config::get().keybinds_selection == 5)
					{
						ImGui::Hotkey("##fakewalk key", &c_config::get().fake_walk_key, ImVec2(100, 20));
					}



				}
				ImGui::Checkbox("key strokes", &c_config::get().key_strokes);
				ImGui::Checkbox("render velocity", &c_config::get().draw_velocity);
				ImGui::EndChild(true);

				ImGui::NextColumn();

				ImGui::BeginChild("colors / settings", ImVec2(279, 343), true); {
					char buffer[UNLEN + 1];
					DWORD size;
					size = sizeof(buffer);
					GetUserName(buffer, &size);
					char title[UNLEN];
					char ch1[25] = "welcome, ";
					char *ch = strcat(ch1, buffer);

					
				ImGui::Text("footstep color");
				ImGui::ColorEdit4("footstep color", c_config::get().clr_footstep, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("weapon color");
				ImGui::ColorEdit4("weapon color", c_config::get().clr_weapon, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("box color");
				ImGui::ColorEdit4("box color", c_config::get().clr_box, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("name color");
				ImGui::ColorEdit4("name color", c_config::get().clr_name, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("skeleton color");
				ImGui::ColorEdit4("skeleton color", c_config::get().clr_skeleton, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("chams enemy color");
				ImGui::ColorEdit4("cham enemy color", c_config::get().clr_chams_vis, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("chams enemy xqz color");
				ImGui::ColorEdit4("chams enemy (behind wall) color", c_config::get().clr_chams_invis, ImGuiColorEditFlags_NoInputs);


				ImGui::Text("chams shine color");
				ImGui::ColorEdit4("chams sine color", c_config::get().clr_chams_shiny, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("chams arms color");
				ImGui::ColorEdit4("chams arms color", c_config::get().clr_hand_chams, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("chams sleeve color");
				ImGui::ColorEdit4("chams sleeve color", c_config::get().clr_sleeve_chams, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("chams weapon color");
				ImGui::ColorEdit4("chams weapon color", c_config::get().clr_weapon_chams, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("glow enemy color");
				ImGui::ColorEdit4("glow color", c_config::get().clr_glow, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("glow weapon color");
				ImGui::ColorEdit4("glow color weapons", c_config::get().clr_glow_dropped, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("glow planted bomb color");
				ImGui::ColorEdit4("glow color planted", c_config::get().clr_glow_planted, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("offscreen arrows color");
				ImGui::ColorEdit4("offscreen arrows color", c_config::get().clr_arrows, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("damage indicator color");
				ImGui::ColorEdit4("damage indicator color", c_config::get().clr_damage_indicator, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("grenade prediction color");
				ImGui::ColorEdit4("grenade prediction color", c_config::get().clr_grenadepred, ImGuiColorEditFlags_NoInputs);

				ImGui::Text("dropped weapon color");
				ImGui::ColorEdit4("dropped weapon color", c_config::get().clr_dropped_weapon, ImGuiColorEditFlags_NoInputs);

				ImGui::SliderInt("arrows size", &c_config::get().vis_arrows_px, 0, 20);
				ImGui::SliderInt("arrows distance", &c_config::get().vis_arrows_dist, 0, 80);
					
				}
				ImGui::EndChild(true);
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::Columns();
				break;
			}
		}

		ImGui::PopFont();

		ImGui::End();
	}
}

void c_menu::run_popup() {
	ImGui::PushFont(font_menu);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(30 / 255.f, 30 / 255.f, 39 / 255.f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 0.1f));

	if (save_config) {
		bool done = false;
		ImGui::begin_popup("config saved.", 2000, &done);
		if (done)
			save_config = false;
	}

	if (load_config) {
		bool done = false;
		ImGui::begin_popup("config loaded.", 2000, &done);
		if (done)
			load_config = false;
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopFont();
}

void c_menu::run_visuals_preview() {
	static std::vector<esp_info_s> info;
	static bool enabled = true;

	ImGui::SetNextWindowSize(ImVec2(235, 400));

	ImGui::Begin("ESP Preview", &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar); {
		const auto cur_window = ImGui::GetCurrentWindow();
		const ImVec2 w_pos = cur_window->Pos;

		ImVec2 p = ImGui::GetCursorScreenPos();
		ImColor c = ImColor(32, 114, 247);

		//title bar
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x - 20, p.y + 15), ImVec2(p.x + ImGui::GetWindowWidth(), p.y - 40), ImColor(30, 30, 39));

		//draw gradient bellow title bar
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x - 20, p.y + 14), ImVec2(p.x + ImGui::GetWindowWidth(), p.y + 16), ImColor(167, 24, 71, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9); //góra, dół
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8); //lewo prawo
		//render cheat name
		ImGui::Text("visuals preview");


		if (c_config::get().player_box) 
		{
			cur_window->DrawList->AddRect(ImVec2(w_pos.x + 40, w_pos.y + 60), ImVec2(w_pos.x + 200, w_pos.y + 360), ImGui::GetColorU32(ImGuiCol_Text));
		}

		if (c_config::get().player_health)
			cur_window->DrawList->AddRectFilled(ImVec2(w_pos.x + 34, w_pos.y + 60), ImVec2(w_pos.x + 36, w_pos.y + 360), ImGui::GetColorU32(ImVec4(83 / 255.f, 200 / 255.f, 84 / 255.f, 255 / 255.f)));

		if (c_config::get().player_name)
			info.emplace_back(esp_info_s("example name", color(255, 255, 255, 255), CENTER_UP));

		if (c_config::get().player_weapon)
			info.emplace_back(esp_info_s("ak-47", color(255, 255, 255, 255), CENTER_DOWN));

		if (c_config::get().player_flags_armor)
			info.emplace_back(esp_info_s("hk", color(255, 255, 255, 255), RIGHT));

		if (c_config::get().player_flags_flashed)
			info.emplace_back(esp_info_s("flashed", color(255, 255, 255, 255), FLASHED));

		if (c_config::get().damage_indicator)
			info.emplace_back(esp_info_s("-100dmg", color(255, 255, 255, 255), CENTER));

		for (auto render : info) {
			const auto text_size = ImGui::CalcTextSize(render.f_name.c_str());

			auto pos = ImVec2(w_pos.x + 205, w_pos.y + 60);

			if (render.f_position == CENTER_DOWN) 
			{
				pos = ImVec2(w_pos.x + (240 / 2) - text_size.x / 2, pos.y + 315 - text_size.y);
			}
			else if (render.f_position == CENTER_UP) {
				pos = ImVec2(w_pos.x + (240 / 2) - text_size.x / 2, pos.y - 5 - text_size.y);
			}
			else if (render.f_position == CENTER) {
				pos = ImVec2(w_pos.x + (240 / 2) - text_size.x / 2, pos.y + 150 - text_size.y);
			}
			else if (render.f_position == FLASHED) {
				pos = ImVec2(w_pos.x + (240 / 2) - text_size.x / 2, pos.y + 170 - text_size.y);
			}

			cur_window->DrawList->AddText(pos, ImGui::GetColorU32(ImVec4(255 / 255.f, 255 / 255.f, 255 / 255.f, 255 / 255.f)), render.f_name.c_str());
		}
	}
	ImGui::End();
	info.clear();
}