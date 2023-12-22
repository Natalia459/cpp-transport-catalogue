#include "json_builder.h"
#include "json.h"

using namespace json;

Builder::DictValueContext Builder::Key(std::string&& key) {
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

Builder::DictKeyContext Builder::StartDict() {
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

Builder::ArrayContext Builder::StartArray() {
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
Node Builder::BaseContext::Build() {
	return builder_.Build();
}


Builder::DictValueContext Builder::BaseContext::Key(std::string key) {
	return builder_.Key(std::move(key));
}

Builder& Builder::BaseContext::Value(Node value) {
	return builder_.Value(std::move(value));
}

Builder::DictKeyContext Builder::BaseContext::StartDict() {
	return builder_.StartDict();
}

Builder::ArrayContext Builder::BaseContext::StartArray() {
	return builder_.StartArray();
}

Builder& Builder::BaseContext::EndDict() {
	return builder_.EndDict();
}

Builder& Builder::BaseContext::EndArray() {
	return builder_.EndArray();
}