#pragma once

#include <istream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <cassert>
#include <sstream>
#include <variant>

namespace json {

	class ParsingError : public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};

	//------------------- NODE -----------------------------------

	using Number = std::variant<int, double>;
	class Node {
	public:
		using Array = std::vector<Node>;
		using Dict = std::map<std::string, Node>;
		using Value = std::variant<std::nullptr_t, bool, int, double, std::string, Array, Dict>;

		//--------------- container ------------------------------

		Node();
		Node(std::nullptr_t ptr);
		Node(Array array);
		Node(Dict map);
		Node(int value);
		Node(std::string value);
		Node(double value);
		Node(bool value);

		//--------------- check_data -----------------------------

		bool IsInt() const;
		bool IsDouble() const;
		bool IsPureDouble() const;
		bool IsBool() const;
		bool IsString() const;
		bool IsNull() const;
		bool IsArray() const;
		bool IsMap() const;

		//--------------- get_data -------------------------------

		const Value& GetValue() const;
		int AsInt() const;
		double AsDouble() const;
		bool AsBool() const;
		const std::string& AsString() const;
		const Array& AsArray() const;
		const Dict& AsMap() const;

		//--------------- compare --------------------------------

		inline bool operator==(Node other) const {
			return data_ == other.data_;
		}

		inline bool operator!=(Node other) const {
			return !(*this == other);
		}

	private:
		Value data_;
	};
	using Array = Node::Array;
	using Dict = Node::Dict;

	//------------------ PRINT_CONTEXT ---------------------------

	struct PrintContext {
		PrintContext(std::ostream& o, int in_step = 4, int in = 0)
			:out(o), indent_step(in_step), indent(in)
		{
		}

		std::ostream& out;
		int indent_step = 4;
		int indent = 0;

		void PrintIndent() const {
			for (int i = 0; i < indent; ++i) {
				out.put(' ');
			}
		}

		// Возвращает новый контекст вывода с увеличенным смещением
		PrintContext Indented() const {
			return { out, indent_step, indent_step + indent };
		}
	};



	//------------------- DOCUMENT -------------------------------

	class Document {
	public:
		explicit Document(Node root);

		const Node& GetRoot() const;

		inline bool operator==(Document other) const {
			return root_ == other.root_;
		}

		inline bool operator!=(Document other) const {
			return !(*this == other);
		}

	private:
		Node root_;
	};

	//------------------ LOAD_VALUE ------------------------------
	Document Load(std::istream& input);
	void Print(Document node, std::ostream& out);



	namespace load {

		json::Node LNode(std::istream& input);

		bool Bool(std::istream& input);

		nullptr_t Nullptr(std::istream& input);

		json::Number LNumber(std::istream& input);

		std::string String(std::istream& input);

		json::Array LArray(std::istream& input);

		json::Dict LDict(std::istream& input);


		bool FindNextDivider(std::istream& input, char end_sequence);

	}




	//------------------- PRINT_VALUE ----------------------------
	namespace print {
		using namespace std::literals;

		std::string String(const std::string& val);

		void PrintValue(const json::Node::Value& val, const json::PrintContext& ctx);
	}
}