#pragma once
#include "json.h"

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

namespace json {

	class MethContext;
	class KeyContext;
	class ValueContext;
	class DictContext;
	class ArrayContext;

	class Builder {
	private:
		json::Node root_;
		std::vector<json::Node*> nodes_stack_;

		template <typename T>
		void InputArrayOrDict(T elem) {
            auto& last = nodes_stack_.back();
			if (last->IsArray()) {
				const_cast<json::Array&>(last->AsArray()).push_back(elem);
				nodes_stack_.emplace_back(&(last->AsArray()).back());
			}
			else {
				*last = elem;
			}
		}

	public:
		Builder() {
			nodes_stack_.emplace_back(&root_);
		}

        KeyContext Key(std::string&& key);

		Builder& Value(json::Node&& value);

        DictContext StartDict();
		Builder& EndDict();

        ArrayContext  StartArray();
		Builder& EndArray();

		json::Node Build();
	};

    class MethContext {
    public:
        MethContext(Builder& builder) :builder_(builder) {};
        KeyContext Key(std::string key);
        Builder& Value(json::Node value);
        DictContext StartDict();
        ArrayContext StartArray();
        Builder& EndDict();
        Builder& EndArray();
    private:
        Builder& builder_;
    };

    class KeyContext :public MethContext {
    public:
        KeyContext(Builder& builder) :MethContext(builder) {};
        KeyContext Key(std::string key) = delete;
        ValueContext Value(json::Node value);
        Builder& EndDict() = delete;
        Builder& EndArray() = delete;
    };

    class ValueContext :public MethContext {
    public:
        ValueContext(Builder& builder) :MethContext(builder) {};
        Builder& Value(json::Node value) = delete;
        DictContext StartDict() = delete;
        ArrayContext StartArray() = delete;
        Builder& EndArray() = delete;
    };

    class DictContext :public MethContext {
    public:
        DictContext(Builder& builder) :MethContext(builder) {};
        Builder& Value(json::Node value) = delete;
        DictContext StartDict() = delete;
        ArrayContext StartArray() = delete;
        Builder& EndArray() = delete;
    };

    class ArrayContext :public MethContext {
    public:
        ArrayContext(Builder& builder) :MethContext(builder) {};
        KeyContext Key(std::string key) = delete;
        ArrayContext Value(json::Node value) { return MethContext::Value(std::move(value)); }
        Builder& EndDict() = delete;
    };
}
