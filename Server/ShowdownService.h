#pragma once

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "shlwapi.lib")

#include <v8.h>
#include "include/libplatform/libplatform.h"
#include "include/v8-context.h"
#include "include/v8-initialization.h"
#include "include/v8-isolate.h"
#include "include/v8-local-handle.h"
#include "include/v8-primitive.h"
#include "include/v8-script.h"

class ShowdownService {
public:
	ShowdownService();
	void sendToShowdown(int UUID, std::vector<std::string> messages);
	


private:
	std::unique_ptr<v8::Platform> platform;
	v8::Isolate* isolate;
	v8::Global<v8::Context> context_;

	v8::Local<v8::Function> sendBattleMessage;

};