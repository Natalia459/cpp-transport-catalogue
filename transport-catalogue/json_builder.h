#pragma once
#include "json.h"

#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

namespace json {



	class Builder {
	private:
		class BaseContext;
		class DictKeyContext;
		class DictValueContext;
		class ArrayContext;

		json::Node root_;
		std::vector<json::Node*> nodes_stack_;

		template <typename T>
		void InputArrayOrDict(T elem) {
			auto& last = nodes_stack_.back();
			if (last->IsArray()) {
				const_cast<json::Array&>(last->AsArray()).push_back(elem);
				nodes_stack_.emplace_back(&const_cast<Array&>(last->AsArray()).back());
			}
			else {
				*last = elem;
			}
		}

		class BaseContext {
		public:
			BaseContext(Builder& builder) :builder_(builder) {};

			DictValueContext Key(std::string key);
			Builder& Value(json::Node value);

			DictKeyContext StartDict();
			Builder& EndDict();

			ArrayContext StartArray();
			Builder& EndArray();
			Node Build();

		private:
			Builder& builder_;
		};

		class ArrayContext :public BaseContext {
		public:
			ArrayContext(Builder& builder) :BaseContext(builder) {};

			DictKeyContext Key(std::string key) = delete;
			ArrayContext Value(json::Node value) { return BaseContext::Value(std::move(value)); }

			Builder& EndDict() = delete;
			Node Build() = delete;
		};

		class DictValueContext :public BaseContext {
		public:
			DictValueContext(Builder& builder) :BaseContext(builder) {};

			DictKeyContext Value(json::Node value) { return BaseContext::Value(std::move(value)); }

			DictValueContext Key(std::string key) = delete;
			Builder& EndDict() = delete;
			Builder& EndArray() = delete;
			Node Build() = delete;
		};

		class DictKeyContext :public BaseContext {
		public:
			DictKeyContext(Builder& builder) :BaseContext(builder) {};

			DictValueContext Key(std::string key) { return BaseContext::Key(std::move(key)); }
			DictKeyContext Value(json::Node value) = delete;

			DictKeyContext StartDict() = delete;
			ArrayContext StartArray() = delete;
			Builder& EndArray() = delete;
			Node Build() = delete;
		};


	public:
		Builder() {
			nodes_stack_.emplace_back(&root_);
		}

		DictValueContext Key(std::string&& key);

		Builder& Value(json::Node&& value);

		DictKeyContext StartDict();
		Builder& EndDict();

		ArrayContext StartArray();
		Builder& EndArray();

		json::Node Build();
	};
}