#include <node.h>
#include "PEFile.h"
#include "BinaryUtil.h"
#include <vector>

using namespace v8;

std::vector<std::string> get_section_names(const std::string& path) {
	PEFile* file = BinaryUtil::LoadBinaryFromDisk(path);
	std::vector<std::string> names;

	IMAGE_SECTION_HEADER* header = file->GetFirstSectionHeader();
	for (int i = 0; i < file->GetNtHeaders()->FileHeader.NumberOfSections; i++, header++) {
		names.push_back(reinterpret_cast<char*>(header->Name));
	}

	return names;
}

void Get(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  /**
   * Get the PE path.
   */
  Local<String> path = Local<String>::Cast(args[0]);
  String::Utf8Value pathValue(path->ToString());
  
  Local<Function> cb = Local<Function>::Cast(args[1]);
  const unsigned argc = 1;
  
  /**
   * Get the sections names of the PE.
   */
  std::vector<std::string> sectionNames;
  try {
	sectionNames = get_section_names(std::string(*pathValue));
  }
  catch (...) {
	Local<Value> argv[argc] = { Boolean::New(isolate, false) };
	cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    return;
  }
  
  /**
   * Convert the vector to a v8 array.
   */
  Handle<Array> v8SectionNames = Array::New(isolate, sectionNames.size());
  for (auto i = 0; i < sectionNames.size(); i++) {
	  Local<String> section = String::NewFromUtf8(isolate, sectionNames[i].c_str());
	  v8SectionNames->Set(i, section);
  }

  /**
   * Call the callback.
   */
  Local<Value> argv[argc] = { v8SectionNames };
  cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
}

void GetSync(const FunctionCallbackInfo<Value>& args) {	
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);
    
  /**
   * Validate the arguments.
   */
  if (args.Length() < 1) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }
  if (!args[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  /**
   * Get the PE path.
   */
  Local<String> path = Local<String>::Cast(args[0]);
  String::Utf8Value pathValue(path->ToString());

  /**
   * Get the sections names of the PE.
   */
  std::vector<std::string> sectionNames;
  try {
	sectionNames = get_section_names(std::string(*pathValue));
  }
  catch (...) {
	isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "File does not exist or isn't a PE")));
    return;
  }
  
  /**
   * Convert the vector to a v8 array.
   */
  Handle<Array> v8SectionNames = Array::New(isolate, sectionNames.size());
  for (auto i = 0; i < sectionNames.size(); i++) {
	  Local<String> section = String::NewFromUtf8(isolate, sectionNames[i].c_str());
	  v8SectionNames->Set(i, section);
  }

  /**
   * Return the sections.
   */
  args.GetReturnValue().Set(v8SectionNames);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  Isolate* isolate = Isolate::GetCurrent();
  
  exports->Set(String::NewFromUtf8(isolate, "get"),
      FunctionTemplate::New(isolate, Get)->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "getSync"),
      FunctionTemplate::New(isolate, GetSync)->GetFunction());
}

NODE_MODULE(pesections, Init)