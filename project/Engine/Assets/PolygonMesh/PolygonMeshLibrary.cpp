#include "PolygonMeshLibrary.h"

#include <mutex>

#include <Library/Utility/Tools/SmartPointer.h>

#include "./PolygonMesh.h"
#include "./PolygonMeshBuilder.h"
#include "Engine/Assets/BackgroundLoader/BackgroundLoader.h"
#include "Engine/Debug/Output.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

std::mutex meshMutex;

PolygonMeshLibrary::PolygonMeshLibrary() noexcept = default;

PolygonMeshLibrary::~PolygonMeshLibrary() noexcept = default;

PolygonMeshLibrary& PolygonMeshLibrary::GetInstance() noexcept {
	static PolygonMeshLibrary instance{};
	return instance;
}

void PolygonMeshLibrary::RegisterLoadQue(const std::filesystem::path& filePath) {
	// ロード済みの場合は何もしない
	if (IsRegistered(filePath.filename().string())) {
		return;
	}
	// BackgroundLoaderにイベント送信
	BackgroundLoader::RegisterLoadQue(
		eps::CreateUnique<PolygonMeshBuilder>(filePath)
	);
}

std::shared_ptr<const PolygonMesh> PolygonMeshLibrary::GetPolygonMesh(const std::string& meshName) {
	std::lock_guard<std::mutex> lock{ meshMutex };
	if (IsRegisteredNonlocking(meshName)) {
		return GetInstance().meshInstanceList.at(meshName);
	}
	else {
		// 存在しないメッシュを呼び出したらエラー用メッシュを使用する
		Console("Warning : Unknown polygon mesh. Name-\'{:}\'\n", meshName);
		return GetInstance().meshInstanceList.at("ErrorObject.obj");
	}
}

bool PolygonMeshLibrary::IsRegistered(const std::string& meshName) {
	std::lock_guard<std::mutex> lock{ meshMutex };
	return IsRegisteredNonlocking(meshName);
}

void PolygonMeshLibrary::Transfer(const std::string& name, std::shared_ptr<PolygonMesh>& data) {
	std::lock_guard<std::mutex> lock{ meshMutex };
	Console("Transfer new PolygonMesh. Name-\'{:}\', Address-\'{:}\'\n", name, (void*)data.get());
	GetInstance().meshInstanceList.emplace(name, data);
}

#ifdef _DEBUG
bool PolygonMeshLibrary::MeshListGui(std::string& current) {
	bool changed = false;

	std::lock_guard<std::mutex> lock{ meshMutex };
	if (ImGui::BeginCombo("MeshList", current.c_str())) {
		auto&& list = GetInstance().meshInstanceList;
		for (auto itr = list.begin(); itr != list.end(); ++itr) {
			bool is_selected = (current == itr->first);
			if (ImGui::Selectable(itr->first.c_str(), is_selected)) {
				current = itr->first;
				changed = true;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();

	}
	return changed;
}
#endif // _DEBUG

bool PolygonMeshLibrary::IsRegisteredNonlocking(const std::string& meshName) {
	return GetInstance().meshInstanceList.contains(meshName);
}
