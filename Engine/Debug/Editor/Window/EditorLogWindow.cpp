#ifdef DEBUG_FEATURES_ENABLE

#include "EditorLogWindow.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Engine/Application/Output.h"

using namespace std::string_literals;

void EditorLogWindow::Allocate() {
	GetInstance();
}

void EditorLogWindow::Initialize(bool isActive_) {
	auto& instance = GetInstance();
	instance.isActive = isActive_;
	//instance.logStates[static_cast<u8>(LogType::Editor)] = { false, 0, "\ue3c9", { 0.2f, 0.2f, 0.2f, 1.0f } };
	instance.logStates[static_cast<u8>(LogType::Information)] = { true, 0, "\ue88e", { 0.5f, 0.5f, 0.5f, 1.0f } };
	instance.logStates[static_cast<u8>(LogType::Warning)] = { true, 0, "\ue002", { 0.8f, 0.8f, 0.1f, 1.0f } };
	instance.logStates[static_cast<u8>(LogType::Error)] = { true, 0, "\ue99a",{ 0.8f, 0.1f, 0.1f, 1.0f } };
	instance.logStates[static_cast<u8>(LogType::Critical)] = { true, 0, "\uf5cf",{ 1.0f, 0.5f, 0.5f, 1.0f } };
}

void EditorLogWindow::Draw() {
	GetInstance().draw();
}

void EditorLogWindow::DrawMenu(string_literal label) {
	GetInstance().draw_menu(label);
}

void EditorLogWindow::draw() {
	if (!isActive) {
		return;
	}

	int flags = 0;
	ImGui::Begin("Log", &isActive, flags);

	// ---------- ログ消去ボタン ----------
	ImGui::Columns(2, "##LogColumns", false);
	ImGui::SetColumnWidth(0, 75.0f);
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0], 15.0f);
	if (ImGui::Button("\ueb80")) {
		logs.clear();
		for (auto& logState : logStates) {
			logState.numLogs = 0;
		}
	}
	if (ImGui::IsItemHovered()) {
		ImGui::PopFont();
		ImGui::SetTooltip("Clear logs");
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0], 15.0f);
	}

	ImGui::NewLine();

	// ---------- ログの種類チェックボックス ----------
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2{ 0.0f, 0.5f });
	for (u8 i = 0; i < logStates.size(); ++i) {
		auto& logState = logStates[i];
		if (logState.isActive) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 0.5f, 0.5f, 0.05f, 1.0f });
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4{ 0.05f, 0.05f, 0.05f, 1.0f });
		}
		if (ImGui::Button(std::format("{} {}", logState.icon, logState.numLogs <= 999 ? std::to_string(logState.numLogs) : "999+"s).c_str(), ImVec2{ 65.0f, 25.0f })) {
			logState.isActive = !logState.isActive;
		}
		ImGui::PopStyleColor(2);
	}
	ImGui::PopFont();
	ImGui::PopStyleVar(2);

	// ---------- ログの表示領域 ----------
	ImGui::NextColumn();
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0.03f,0.03f,0.03f,1.0f });
	ImGui::BeginChild("##LogWindow", ImVec2{}, ImGuiChildFlags_FrameStyle);
	ImGui::PopStyleColor();

	// ---------- ログの表示 ----------
	for (const LogBody& log : logs) {
		auto& logState = logStates[static_cast<const u8>(log.type)];
		if (!logState.isActive) {
			continue;
		}
		ImGui::TextColored(
			std::bit_cast<ImVec4, std::array<r32, 4>>(logState.color),
			std::format("{} {}", logState.icon, log.message).c_str()
		);
	}

	// ---------- スクロール処理 ----------
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImGuiID active_id = ImGui::GetActiveID();
	bool any_scrollbar_active = active_id && (active_id == ImGui::GetWindowScrollbarID(window, ImGuiAxis_X) || active_id == ImGui::GetWindowScrollbarID(window, ImGuiAxis_Y));
	if (ImGui::GetIO().MouseWheel > 0) {
		isBottomScroll = false;
	}
	else if (ImGui::GetScrollMaxY() == ImGui::GetScrollY()) {
		isBottomScroll = true;
	}
	else if (any_scrollbar_active) {
		isBottomScroll = false;
	}
	if (isBottomScroll) {
		ImGui::SetScrollY(ImGui::GetScrollMaxY()); // 最後にスクロール
	}

	ImGui::EndChild();

	ImGui::End();
}

void EditorLogWindow::AppendLogEntry(LogType type, const std::string& message) {
	auto& instance = GetInstance();
	++instance.logStates[static_cast<u8>(type)].numLogs;
	instance.logs.emplace_back(type, message);
	if (instance.logs.size() >= MAX_LOG_SIZE) {
		auto& tmp = instance.logs.front();
		--instance.logStates[static_cast<u8>(tmp.type)].numLogs;
		instance.logs.pop_front();
	}
}

#endif // DEBUG_FEATURES_ENABLE
