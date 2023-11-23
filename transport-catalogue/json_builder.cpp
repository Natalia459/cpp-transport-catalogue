#include "json_builder.h"
#include "json.h"

using namespace json;

KeyContext Builder::Key(std::string&& key) {
	if ((nodes_stack_.empty() || !nodes_stack_.back()->IsMap())) {
		throw std::logic_error("Key called before building dictionary");
	}

	nodes_stack_.emplace_back(&const_cast<Dict&>(nodes_stack_.back()->AsMap())[key]);
	return *this;
}

Builder& Builder::Value(json::Node&& value) {
	if (nodes_stack_.empty()) {
		throw std::logic_error("Node isn't exist, can't add value");
	}
	if (!(nodes_stack_.back()->IsNull() || nodes_stack_.back()->IsArray())) {
		throw std::logic_error("Error in Value method");
	}

	auto& last = nodes_stack_.back();
	if (last->IsArray()) {
		const_cast<Array&>(last->AsArray()).emplace_back(value);
	}
	else {
		*last = value;
		nodes_stack_.pop_back();
	}
	return *this;
}

DictContext Builder::StartDict() {
	if (nodes_stack_.empty()) {
		throw std::logic_error("Node isn't exist, can't start dict");
	}
	if (!(nodes_stack_.back()->IsNull() || nodes_stack_.back()->IsArray())) {
		throw std::logic_error("Dictionary can't be started");
	}

	InputArrayOrDict(Dict());
	return *this;
}

Builder& Builder::EndDict() {
	if (nodes_stack_.empty()) {
		throw std::logic_error("Node isn't exist, can't build dict");
	}
	if (!nodes_stack_.back()->IsMap()) {
		throw std::logic_error("Dictionary can't be ended");
	}

	nodes_stack_.pop_back();
	return *this;
}

ArrayContext  Builder::StartArray() {
	if (nodes_stack_.empty()) {
		throw std::logic_error("Node isn't exist, can't build array");
	}
	if (!(nodes_stack_.back()->IsNull() || nodes_stack_.back()->IsArray())) {
		throw std::logic_error("Array can't be started");
	}

	InputArrayOrDict(Array());
	return *this;
}

Builder& Builder::EndArray() {
	if ((nodes_stack_.empty() || !nodes_stack_.back()->IsArray())) {
		throw std::logic_error("Array can't be ended");
	}

	nodes_stack_.pop_back();
	return *this;
}

Node Builder::Build() {
	if (!nodes_stack_.empty()) {
		throw std::logic_error("Node isn't exist, can't build object");
	}

	return root_;
}

KeyContext MethContext::Key(std::string key) {
	return builder_.Key(std::move(key));
}

Builder& MethContext::Value(Node value) {
	return builder_.Value(std::move(value));
}

DictContext MethContext::StartDict() {
	return builder_.StartDict();
}

ArrayContext MethContext::StartArray() {
	return builder_.StartArray();
}

Builder& MethContext::EndDict() {
	return builder_.EndDict();
}

Builder& MethContext::EndArray() {
	return builder_.EndArray();
}

ValueContext KeyContext::Value(Node value) {
	return MethContext::Value(std::move(value));
}
