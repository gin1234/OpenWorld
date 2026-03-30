#include "TSEngine.h"
#include <core/entity/TypeScriptEntityManager.h>

namespace TSEngine {
	TSEngine::TSEngine()
	{
		JsEnv = MakeShared<puerts::FJsEnv>();
	}
	//TSEngine::TSEngine(std::string path):projectPath(path)
	//{	
	//	JsEnv = MakeShared<puerts::FJsEnv>(path);
	//}
	void TSEngine::Start()
	{
		JsEnv->Start("main");
		EntityManager::GetInstance();
	}
}

