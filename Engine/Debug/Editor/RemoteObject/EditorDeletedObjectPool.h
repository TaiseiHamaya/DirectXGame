#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <Library/Utility/Template/Reference.h>

#include "IRemoteObject.h"

class EditorDeletedObjectPool {
private:
	struct SequenceObject {
		Reference<IRemoteObject> parent;
		Reference<IRemoteObject> object;
	};

public:
	void delete_sequence(Reference<IRemoteObject> parent, Reference<IRemoteObject> object);
	void solution_sequence();
	std::unique_ptr<IRemoteObject> get_deleted_force(Reference<IRemoteObject> search);

private:
	std::vector<SequenceObject> sequences;
	std::unordered_map<Reference<IRemoteObject>, std::unique_ptr<IRemoteObject>> deletedInstances;
};
