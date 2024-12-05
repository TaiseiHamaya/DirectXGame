#pragma once

#ifdef _DEBUG

#include <functional>
#include <string>

#include "ValueEditorObject.h"

class ImGuiValueEditor {
private:
	struct EditorFunctions {
		std::function<void(void)> saveFunc;
		std::function<void(void)> imguiFunc;
	};

public:
	ImGuiValueEditor() = default;
	~ImGuiValueEditor() = default;

public:
	template<typename T, typename ...Args>
	void register_value(std::function<void()> whiteFunction, const std::string& name, T* pValue, Args&& ...args) {
		ValueEditor::show_object<T> showObj{ name, pValue, std::forward(args)... };
		functions.emplace_back(
			whiteFunction,
			std::bind(&ValueEditor::show_object<T>::show_gui, showObj)
		);
	};

	void show_imgui();

	void save_all() const;

private:
	std::vector<EditorFunctions> functions;
};

#endif // _DEBUG
