#pragma once
#include "json.h"

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

namespace json {

	class ItemContext;
	class KeyItemContext;
	class ValueContext;
	class DictItemContext;
	class ArrayItemContext;

	class Builder {
	private:
		json::Node root_;
		std::vector<json::Node*> nodes_stack_;

		template <typename T>
		void InputResult(T elem) {
			if (nodes_stack_.back()->IsArray()) {
				const_cast<json::Array&>(nodes_stack_.back()->AsArray()).push_back(elem);
				nodes_stack_.emplace_back(&const_cast<json::Array&>(nodes_stack_.back()->AsArray()).back());
			}
			else {
				*nodes_stack_.back() = elem;
			}
		}

	public:
		Builder() {
			nodes_stack_.emplace_back(&root_);
		}

        KeyItemContext Key(std::string&& key);

		Builder& Value(json::Node&& value);

        DictItemContext StartDict();
		Builder& EndDict();

        ArrayItemContext  StartArray();
		Builder& EndArray();

		json::Node Build();
	};

    class ItemContext {
    public:
        ItemContext(Builder& builder) :builder_(builder) {};
        KeyItemContext Key(std::string key);
        Builder& Value(json::Node value);
        DictItemContext StartDict();
        ArrayItemContext StartArray();
        Builder& EndDict();
        Builder& EndArray();
    private:
        Builder& builder_;
    };

    class KeyItemContext :public ItemContext {
    public:
        KeyItemContext(Builder& builder) :ItemContext(builder) {};
        KeyItemContext Key(std::string key) = delete;
        ValueContext Value(json::Node value);
        Builder& EndDict() = delete;
        Builder& EndArray() = delete;
    };

    class ValueContext :public ItemContext {
    public:
        ValueContext(Builder& builder) :ItemContext(builder) {};
        Builder& Value(json::Node value) = delete;
        DictItemContext StartDict() = delete;
        ArrayItemContext StartArray() = delete;
        Builder& EndArray() = delete;
    };

    class DictItemContext :public ItemContext {
    public:
        DictItemContext(Builder& builder) :ItemContext(builder) {};
        Builder& Value(json::Node value) = delete;
        DictItemContext StartDict() = delete;
        ArrayItemContext StartArray() = delete;
        Builder& EndArray() = delete;
    };

    class ArrayItemContext :public ItemContext {
    public:
        ArrayItemContext(Builder& builder) :ItemContext(builder) {};
        KeyItemContext Key(std::string key) = delete;
        ArrayItemContext Value(json::Node value) { return ItemContext::Value(std::move(value)); }
        Builder& EndDict() = delete;
    };
}