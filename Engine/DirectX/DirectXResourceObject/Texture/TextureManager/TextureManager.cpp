#include "TextureManager.h"

#include <functional>
#include <mutex>
#include <condition_variable>
#include <cassert>

#include "Engine/DirectX/DirectXCommand/DirectXCommand.h"
#include "Engine/DirectX/DirectXResourceObject/Texture/Texture.h"

TextureManager::TextureManager() = default;
std::mutex executeMutex;
std::mutex referenceMutex;
std::condition_variable waitConditionVariable;
std::condition_variable loadConditionVariable;

TextureManager::~TextureManager() {
	isEndProgram = true;
	if (loadFunc.joinable()) {
		loadConditionVariable.notify_all();
		loadFunc.join();
	}
};

TextureManager& TextureManager::GetInstance() {
	static std::unique_ptr<TextureManager> instance{ new TextureManager };
	return *instance;
}

void TextureManager::Initialize() {
	GetInstance().initialize();
}

void TextureManager::RegisterLoadQue(const std::string& filePath, const std::string& textureName) {
	if (GetInstance().textureRegisteredList.find(textureName) != GetInstance().textureRegisteredList.end()) {
		return;
	}
	std::lock_guard<std::mutex> lock(referenceMutex);
	GetInstance().loadEvents.push_back({ LoadEvent::EmplaceData, std::make_unique<LoadingQue>(filePath, textureName, LoadStatus::InQueue, std::make_shared<Texture>()) });
	loadConditionVariable.notify_all();
}

void TextureManager::LoadImperative() {
	GetInstance().isExecuting = true;
	std::lock_guard<std::mutex> lock(referenceMutex);
	GetInstance().loadEvents.push_back({ LoadEvent::Execute, nullptr });
	loadConditionVariable.notify_all();
}

void TextureManager::WaitEndExecute() {
	std::unique_lock<std::mutex> uniqueLock(executeMutex);
	waitConditionVariable.wait(uniqueLock, [] {return !GetInstance().isExecuting; });
}

void TextureManager::LoadImperativeAndWait() {
	LoadImperative();
	WaitEndExecute();
}

std::weak_ptr<Texture> TextureManager::GetTexture(const std::string& textureName) {
	std::lock_guard<std::mutex> lock(referenceMutex);
	assert(GetInstance().textureRegisteredList.find(textureName) != GetInstance().textureRegisteredList.end());
	return GetInstance().textureInstanceList.at(textureName);
}

void TextureManager::initialize() {
	isExecuting = false;
	isEndProgram = false;
	loadFunc = std::thread{ std::bind(&TextureManager::load_manager, this) };
}

void TextureManager::load_manager() {
	while (!isEndProgram) {
		std::unique_lock<std::mutex> lock{ referenceMutex };
		loadConditionVariable.wait(lock, [] {return !GetInstance().loadEvents.empty() || GetInstance().isEndProgram; });
		if (GetInstance().isEndProgram) {
			break;
		}
		auto&& nowEvent = loadEvents.begin();
		lock.unlock();
		switch (nowEvent->eventId) {
		case LoadEvent::Execute:
			DirectXCommand::ExecuteTextureCommand();
			DirectXCommand::WaitTextureCommand();
			DirectXCommand::ResetTextureCommand();
			create_view();
			isExecuting = false;
			waitConditionVariable.notify_all();
			break;
		case LoadEvent::EmplaceData:
			nowEvent->que->intermediateResource = nowEvent->que->texture->load_texture(nowEvent->que->filePath + "/" + nowEvent->que->fileName);
			nowEvent->que->status = LoadStatus::SetCommand;
			waitLoadingQue.push_back(std::move(loadEvents.front()));
			break;
		default:
			assert("EventID is wrong");
			break;
		}
		lock.lock();
		loadEvents.pop_front();
	}
}

void TextureManager::create_view() {
	for (auto itr = waitLoadingQue.begin(); itr != waitLoadingQue.end(); ++itr) {
		itr->que->texture->create_resource_view();
		std::lock_guard<std::mutex> lock(referenceMutex);
		textureInstanceList.emplace(itr->que->fileName, itr->que->texture);
		textureRegisteredList.emplace(itr->que->fileName);
	}
	waitLoadingQue.clear();
}
