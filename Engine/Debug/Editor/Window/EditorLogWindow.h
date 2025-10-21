#pragma once

#ifdef DEBUG_FEATURES_ENABLE

#include "IEditorWindow.h"

#include <array>
#include <list>
#include <string>

#include <Library/Utility/Template/SingletonInterface.h>

#include "Engine/Application/Logger.h"

struct ImVec4;

class EditorLogWindow : public IEditorWindow, SingletonInterface<EditorLogWindow> {
	__CLASS_SINGLETON_INTERFACE(EditorLogWindow)

private:
	struct LogBody {
		Logger::Type type;
		std::string message;
	};

	struct LogState {
		bool isActive{ true };
		u64 numLogs{ 0 };
		string_literal icon;
		std::array<r32, 4> color;
	};

public:
	static void Allocate();
	static void Initialize(bool isActive_);

	static void Draw();
	static void DrawMenu(string_literal label);

	void draw() override;

	static void AppendLogEntry(Logger::Type type, const std::string& message);

private:
	std::array<LogState, 7> logStates;
	bool isBottomScroll{ true };
	std::list<LogBody> logs;

private:
	static constexpr u64 MAX_LOG_SIZE = 4089;
};



#endif // DEBUG_FEATURES_ENABLE
