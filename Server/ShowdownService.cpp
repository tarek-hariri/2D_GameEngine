#include "ShowdownService.h"
#include <iostream>

std::string readFile(const std::string& file_name) {
	std::ifstream file(file_name);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << file_name << std::endl;
		return "";
	}

	return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

ShowdownService::ShowdownService() {
	platform = v8::platform::NewDefaultPlatform();
	v8::V8::InitializePlatform(platform.get());
	v8::V8::Initialize();

	// Create a new Isolate and make it the current one.
	v8::Isolate::CreateParams create_params;
	create_params.array_buffer_allocator =
		v8::ArrayBuffer::Allocator::NewDefaultAllocator();
	isolate = v8::Isolate::New(create_params);

	{
		v8::Isolate::Scope isolate_scope(isolate);
		// Create a stack-allocated handle scope.
		v8::HandleScope handle_scope(isolate);
		// Create a new context.
		v8::Local<v8::Context> context = v8::Context::New(isolate);
		// Enter the context for compiling and running the hello world script.
		v8::Context::Scope context_scope(context);
		{

			// Read js file
			std::string script_content = readFile("cobblemon-showdown/cobblemon-showdown/out/index.js");


			// Create a string containing the JavaScript source code.
			// TO DO:  Actually check if the MaybeLocal is valid before marking it as ToLocalChecked().
			v8::Local<v8::String> source =
				v8::String::NewFromUtf8(isolate, script_content.c_str(), v8::NewStringType::kNormal).ToLocalChecked();


			// Compile the source code.
			v8::Local<v8::Script> script =
				v8::Script::Compile(context, source).ToLocalChecked();

			// Run the script
			script->Run(context).ToLocalChecked();


			v8::Local<v8::String> name = v8::String::NewFromUtf8(
				isolate, "sendBattleMessage", v8::NewStringType::kInternalized).ToLocalChecked();

			v8::Local<v8::Value> sendBattleMessageVal =
				context->Global()->Get(context, name).ToLocalChecked();

			if (!sendBattleMessageVal->IsFunction()) {
				std::cout << "sendBattleMessage not a function. Yer boned, kid." << std::endl;
				return;
			}

			sendBattleMessage = v8::Local<v8::Function>::Cast(sendBattleMessageVal);
			context_.Reset(isolate, context);
		}
	}
}

void ShowdownService::sendToShowdown(int UUID, std::vector<std::string> messages){
	v8::Local<v8::Context> context = context_.Get(isolate);
	v8::Local<v8::Value> receiver = v8::Object::New(isolate);
	v8::Local<v8::Value> output;

	// interpret UUID as a string
	std::string UUIDStringified = std::to_string(UUID);
	v8::Local<v8::Array> messagesArray = v8::Array::New(isolate, messages.size());

	for (int i = 0; i < messages.size(); i ++) {
		v8::Local<v8::String> str = v8::String::NewFromUtf8(isolate, messages[i].c_str()).ToLocalChecked();


		// TO DO: Make new context so all of these local values don't float around forever-- 
		// I don't think they are freed automatically.
		messagesArray->Set(context, i, str);
	}

	// Preparing args for sendBattleMessage
	v8::Local<v8::Value> args[2];
	args[0] = v8::String::NewFromUtf8(isolate, UUIDStringified.c_str()).ToLocalChecked();
	args[1] = messagesArray;

	if (sendBattleMessage->CallAsFunction(context, receiver, 2, args).ToLocal(&output)) {
		v8::String::Utf8Value utf8(isolate, output);
		std::cout << "sendBattleMessage_fun returned: " << *utf8 << std::endl;
	}
	else {
		std::cout << "Damn" << std::endl;
	}

	// So, cleanup here?

	// seriously dont be lazy... go back and do this brav
}

