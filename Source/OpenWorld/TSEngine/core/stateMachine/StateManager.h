#include "EntityState.h"


namespace TSEngine {
	class StateManager {
	public:
		void Update();

	private:
		int loadCount;
		int renderCount;
		int closeCount;
		std::vector<Entity*> loadingEntities;

		std::unordered_map<std::string, EntityState> entityStateMap;
		std::unordered_map<std::string, std::unordered_map<std::string, int>> entityResMap;
		void OnLoad();

		void OnRender();

		void OnClose();
	};
}