#include "json_builder.h"
#include "json.h"

using namespace json;

KeyItemContext Builder::Key(std::string&& key) {
	if (!(!nodes_stack_.empty() && nodes_stack_.back()->IsMap())) {
		throw std::logic_error("Key outside the dictionary");
	}
	nodes_stack_.emplace_back(&const_cast<Dict&>(nodes_stack_.back()->AsMap())[key]);
	return *this;
}

Builder& Builder::Value(json::Node&& value) {
	if (nodes_stack_.empty() || (!nodes_stack_.back()->IsNull() && !nodes_stack_.back()->IsArray())) {
		throw std::logic_error("Value error");
	}
	if (nodes_stack_.back()->IsArray()) {
		const_cast<Array&>(nodes_stack_.back()->AsArray()).emplace_back(value);
	}
	else {
		*nodes_stack_.back() = value;
		nodes_stack_.pop_back();
	}
	return *this;
}

DictItemContext Builder::StartDict() {
	if (nodes_stack_.empty() || (!nodes_stack_.back()->IsNull() && !nodes_stack_.back()->IsArray())) {
		throw std::logic_error("StartDict error");
	}
	InputResult(Dict());
	return *this;
}

Builder& Builder::EndDict() {
	if (!(!nodes_stack_.empty() && nodes_stack_.back()->IsMap())) {
		throw std::logic_error("EndDict outside the dictionary");
	}
	nodes_stack_.pop_back();
	return *this;
}

ArrayItemContext  Builder::StartArray() {
	if (nodes_stack_.empty() || (!nodes_stack_.back()->IsNull() && !nodes_stack_.back()->IsArray())) {
		throw std::logic_error("StartArray error");
	}
	InputResult(Array());
	return *this;
}

Builder& Builder::EndArray() {
	if (!(!nodes_stack_.empty() && nodes_stack_.back()->IsArray())) {
		throw std::logic_error("EndArray outside the array");
	}
	nodes_stack_.pop_back();
	return *this;
}

Node Builder::Build() {
	if (!nodes_stack_.empty()) {
		throw std::logic_error("Object haven't build");
	}
	return root_;
}

KeyItemContext ItemContext::Key(std::string key) {
	return builder_.Key(std::move(key));
}

Builder& ItemContext::Value(Node value) {
	return builder_.Value(std::move(value));
}

DictItemContext ItemContext::StartDict() {
	return builder_.StartDict();
}

ArrayItemContext ItemContext::StartArray() {
	return builder_.StartArray();
}

Builder& ItemContext::EndDict() {
	return builder_.EndDict();
}

Builder& ItemContext::EndArray() {
	return builder_.EndArray();
}

ValueContext KeyItemContext::Value(Node value) {
	return ItemContext::Value(std::move(value));
}