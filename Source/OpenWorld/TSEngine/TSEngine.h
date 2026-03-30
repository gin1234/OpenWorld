#pragma once
#include <string>
#include "JsEnv.h"
namespace TSEngine {
	
	class TSEngine {
	public:
		TSEngine();
		//TSEngine(std::string path);
		void Start();

	private:
		std::string projectPath;
		TSharedPtr<puerts::FJsEnv> JsEnv;
	};
}