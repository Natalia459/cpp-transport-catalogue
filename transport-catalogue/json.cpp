#include "json.h"

#include <optional>
#include <iterator>

#include <iostream>

using namespace std;
using namespace json;

//------------------- NODE -----------------------------------
//------------------- container ------------------------------

Node::Node()
	:data_(nullptr) //
{
}

Node::Node(std::nullptr_t ptr)
	:data_(ptr) //
{
}

Node::Node(bool value)
	:data_(value) //
{
}

Node::Node(int value)
	:data_(value) //
{
}

Node::Node(double value)
	:data_(value) //
{
}

Node::Node(string value) {
	if (value == "null"s) {
		data_ = nullptr;
	}
	else {
		data_ = value;
	}
}

Node::Node(Array array)
	: data_(move(array)) //
{
}

Node::Node(Dict map)
	: data_(move(map)) //
{
}


//------------------- check_data -----------------------------

bool Node::IsInt() const {
	return holds_alternative<int>(data_);
}

bool Node::IsDouble() const {
	return holds_alternative<int>(data_) || holds_alternative<double>(data_);
}

bool Node::IsPureDouble() const {
	return holds_alternative<double>(data_);
}

bool Node::IsBool() const {
	return holds_alternative<bool>(data_);
}

bool Node::IsString() const {
	return holds_alternative<string>(data_);
}

bool Node::IsNull() const {
	return holds_alternative<nullptr_t>(data_);
}

bool Node::IsArray() const {
	return holds_alternative<Array>(data_);
}

bool Node::IsMap() const {
	return holds_alternative<Dict>(data_);
}


//------------------- get_data -------------------------------

const Node::Value& Node::GetValue() const {
	return data_;
}

int Node::AsInt() const {
	if (IsInt()) {
		return get<int>(data_);
	}
	else {
		throw std::logic_error("");
	}
}

double Node::AsDouble() const {
	if (IsPureDouble()) {
		return get<double>(data_);
	}
	else if (IsInt()) {
		return static_cast<double>(get<int>(data_));
	}
	else {
		throw std::logic_error("");
	}
}

bool Node::AsBool() const {
	if (IsBool()) {
		return get<bool>(data_);
	}
	else {
		throw std::logic_error("");
	}
}

const string& Node::AsString() const {
	if (IsString()) {
		return get<string>(data_);
	}
	else {
		throw std::logic_error("");
	}
}

const Array& Node::AsArray() const {
	if (IsArray()) {
		return get<Array>(data_);
	}
	else {
		throw std::logic_error("");
	}
}

const Dict& Node::AsMap() const {
	if (IsMap()) {
		return get<Dict>(data_);
	}
	else {
		throw std::logic_error("");
	}
}



//------------------- DOCUMENT -------------------------------
//------------------- container ------------------------------

Document::Document(Node root)
	: root_(move(root)) //
{
}

//------------------ get_data --------------------------------

const Node& Document::GetRoot() const {
	return root_;
}

void json::Print(Document node, std::ostream& out) {
	const PrintContext ctx = PrintContext(out);
	print::PrintValue(node.GetRoot().GetValue(), ctx);
}

//------------------- load_data ------------------------------

bool load::Bool(std::istream& input) {
	auto it = std::istreambuf_iterator<char>(input);
	auto end = std::istreambuf_iterator<char>();
	char c = *it;

	std::string tru = "";
	std::string fals = "";
	if (c == 't') {
		tru += c;
		int i = 0;
		++it;
		while (i < 3 && it != end) {
			c = *it;
			tru += c;
			++i;
			++it;
		}

		if (tru != "true"s) {
			throw ParsingError("Bool parsing error");
		}
		else {
			return true;
		}
	}
	else {
		fals += c;
		int i = 0;
		++it;
		while (i < 4 && it != end) {
			c = *it;
			fals += c;
			++i;
			++it;
		}

		if (fals != "false"s) {
			throw ParsingError("Bool parsing error");
		}
		else {
			return false;
		}
	}
}

nullptr_t load::Nullptr(std::istream& input) {
	auto it = std::istreambuf_iterator<char>(input);
	auto end = std::istreambuf_iterator<char>();
	char c;
	c = *it;
	++it;
	std::string null = "";
	null += c;
	int i = 0;
	while (it != end && i < 3) {
		c = *it;
		null += c;
		++i;
		++it;
	}

	if (null != "null"s) {
		throw ParsingError("Null parsing error");
	}
	else {
		input.putback(c);
		return nullptr;
	}
}

Number load::LNumber(std::istream& input) {
	using namespace std::literals;

	std::string parsed_num;

	// Считывает в parsed_num очередной символ из input
	auto read_char = [&parsed_num, &input] {
		parsed_num += static_cast<char>(input.get());
		if (!input) {
			throw ParsingError("Failed to read number from stream"s);
		}
		};

	// Считывает одну или более цифр в parsed_num из input
	auto read_digits = [&input, read_char] {
		if (!std::isdigit(input.peek())) {
			throw ParsingError("A digit is expected"s);
		}
		while (std::isdigit(input.peek())) {
			read_char();
		}
		};

	if (input.peek() == '-') {
		read_char();
	}
	// Парсим целую часть числа
	if (input.peek() == '0') {
		read_char();
		// После 0 в JSON не могут идти другие цифры
	}
	else {
		read_digits();
	}

	bool is_int = true;
	// Парсим дробную часть числа
	if (input.peek() == '.') {
		read_char();
		read_digits();
		is_int = false;
	}

	// Парсим экспоненциальную часть числа
	if (int ch = input.peek(); ch == 'e' || ch == 'E') {
		read_char();
		if (ch = input.peek(); ch == '+' || ch == '-') {
			read_char();
		}
		read_digits();
		is_int = false;
	}

	try {
		if (is_int) {
			// Сначала пробуем преобразовать строку в int
			try {
				return std::stoi(parsed_num);
			}
			catch (...) {
				// В случае неудачи, например, при переполнении,
				// код ниже попробует преобразовать строку в double
			}
		}
		return std::stod(parsed_num);
	}
	catch (...) {
		throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
	}
}

// Считывает содержимое строкового литерала JSON-документа
// Функцию следует использовать после считывания открывающего символа ":
std::string load::String(std::istream& input) {
	using namespace std::literals;

	auto it = std::istreambuf_iterator<char>(input);
	auto end = std::istreambuf_iterator<char>();
	std::string s;
	while (true) {
		if (it == end) {
			// Поток закончился до того, как встретили закрывающую кавычку?
			throw ParsingError("String parsing error");
		}
		const char ch = *it;

		if (ch == '"') {
			// Встретили закрывающую кавычку
			++it;
			break;
		}
		else if (ch == '\\') {
			// Встретили начало escape-последовательности
			++it;
			if (it == end) {
				// Поток завершился сразу после символа обратной косой черты
				throw ParsingError("String parsing error");
			}
			const char escaped_char = *(it);
			// Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
			switch (escaped_char) {
			case 'n':
				s.push_back('\n');
				break;
			case 't':
				s.push_back('\t');
				break;
			case 'r':
				s.push_back('\r');
				break;
			case '"':
				s.push_back('"');
				break;
			case '\\':
				s.push_back('\\');
				break;
			default:
				// Встретили неизвестную escape-последовательность
				throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
			}
		}
		else if (ch == '\n' || ch == '\r') {
			// Строковый литерал внутри- JSON не может прерываться символами \r или \n
			throw ParsingError("Unexpected end of line"s);
		}
		else {
			// Просто считываем очередной символ и помещаем его в результирующую строку
			s.push_back(ch);
		}
		++it;
	}

	return s;
}

//если достигнут конец последовательности, возвращается нулевой итератор
// если найден следующий разделитель вернет его
bool load::FindNextDivider(std::istream& input, char end_sequence) {
	auto it = std::istreambuf_iterator<char>(input);
	auto end = std::istreambuf_iterator<char>();

	bool found_end = false;
	std::string occupy = "";
	while (*it != ',') {
		bool miss = false;

		if (*it == '[') {
			while (*it != ']') {
				if (it == end) {
					throw ParsingError("Array parsing error"s);
				}
				occupy += *it;
				++it;
			}
			occupy += *it;
			++it;
			miss = true;
		}

		if (*it == '{') {
			while (*it != '}') {
				if (it == end) {
					throw ParsingError("Dict parsing error"s);
				}
				occupy += *it;
				++it;
			}
			occupy += *it;
			++it;
			miss = true;
		}

		if (*it == end_sequence) {
			found_end = true;
			break;
		}

		if (!miss) {
			occupy += *it;
			++it;
		}
	}

	for (auto occ = occupy.rbegin(); occ != occupy.rend(); ++occ) {
		input.putback(*occ);
	}
	return found_end;
}


Array load::LArray(std::istream& input) {
	std::vector<Node> result;

	for (char c; input >> c && c != ']';) {
		if (c != ',') {
			input.putback(c);
		}
		result.push_back(load::LNode(input));
	}
	if (!input) {
		throw ParsingError("Array parsing error"s);
	}
	return result;
}

Dict load::LDict(std::istream& input) {
	Dict dict;

	for (char c; input >> c && c != '}';) {
		if (c == '"') {
			std::string key = load::String(input);
			if (input >> c && c == ':') {
				if (dict.find(key) != dict.end()) {
					throw ParsingError("Duplicate key '"s + key + "' have been found");
				}
				dict.emplace(std::move(key), load::LNode(input));
			}
			else {
				throw ParsingError(": is expected but '"s + c + "' has been found"s);
			}
		}
		else if (c != ',') {
			throw ParsingError(R"(',' is expected but ')"s + c + "' has been found"s);
		}
	}
	if (!input) {
		throw ParsingError("Dictionary parsing error"s);
	}
	return dict;
}

Node load::LNode(istream& input) {
	char c;
	input >> c;

	switch (c) {

	case '[':
		return Node{ (LArray(input)) };

	case '{':
		return Node{ (LDict(input)) };

	case '"':
		return Node{ (String(input)) };

	case 'n':
		input.putback(c);
		return Node{ (Nullptr(input)) };

	case 't':
		input.putback(c);
		return Node{ (Bool(input)) };

	case 'f':
		input.putback(c);
		return Node{ (Bool(input)) };

	default:
		input.putback(c);
		auto numb = LNumber(input);
		if (holds_alternative<int>(numb)) {
			return Node{ get<int>(numb) };
		}
		else if (holds_alternative<double>(numb)) {
			return Node{ get<double>(numb) };
		}
		else {
			return Node{ nullptr };
		}
	}
}

Document json::Load(std::istream& input) {
	return Document{ move(load::LNode(input)) };
}


//------------------ print_data ------------------------------

std::string print::String(const std::string& val) {
	using namespace std::literals;

	std::string s = "";
	for (auto c : val) {

		// Встретили начало escape-последовательности
		if (static_cast<int>(c) <= 34 || static_cast<int>(c) == 92) {//
			// Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
			// При чтении строкового литерала последовательности \r,\n,\t,\\,\"
			// должны преобразовываться в соответствующие символы.
			// При выводе эти символы должны экранироваться, кроме \t.
			switch (c) {
			case '\n':
				s += "\\n"s;
				break;
			case '\t':
				s += "\t"s;
				break;
			case '\r':
				s += "\\r"s;
				break;
			case '\"':
				//s += "\\";
				s += "\\\""s;
				break;
			case '\\':
				s += "\\\\"s;
				break;
			default:
				s += c;
			}
		}
		else {
			s += c;
		}
	}

	return s;

}

void print::PrintValue(const Node::Value& val, const PrintContext& ctx) {
	using namespace std;

	if (holds_alternative<nullptr_t>(val)) {
		ctx.out << "null"sv;
	}
	else if (holds_alternative<int>(val)) {
		ctx.out << get<int>(val);
	}
	else if (holds_alternative<double>(val)) {
		//string num = DeleteZeros(get<double>(val));
		ctx.out << get<double>(val);
	}
	else if (holds_alternative<bool>(val)) {
		ctx.PrintIndent();
		ctx.out << boolalpha << get<bool>(val);
	}
	else if (holds_alternative<string>(val)) {
		std::string res = print::String(get<string>(val));
		ctx.out << "\""sv << res << "\""sv;
	}
	else if (holds_alternative<Array>(val)) {
		ctx.out << " ["sv;
		ctx.out << std::endl;
		const PrintContext new_pt = ctx.Indented();
		auto vec = get<Array>(val);
		bool is_first = true;
		for (const auto& v : vec) {
			if (!is_first) {
				ctx.out << ","sv << std::endl;
			}
			new_pt.PrintIndent();
			std::visit(
				[&ctx, &new_pt](const Node::Value& v) { PrintValue(v, new_pt); },
				v.GetValue());
			is_first = false;
		}
		ctx.out << std::endl;
		new_pt.PrintIndent();
		ctx.out << "]"sv;
	}
	else if (holds_alternative<Dict>(val)) {
		bool is_not_first = false;

		ctx.out << std::endl;
		ctx.PrintIndent();
		ctx.out << "{"sv << std::endl;
		const PrintContext new_pt = ctx.Indented();

		for (const auto& [key, v] : get<Dict>(val)) {

			if (is_not_first) {
				ctx.out << ","sv << std::endl;
			}
			is_not_first = true;
			new_pt.PrintIndent();
			ctx.out << "\""sv << key << "\": "sv;
			std::visit(
				[&ctx, &new_pt](const Node::Value& v) { PrintValue(v, new_pt); },
				v.GetValue());
		}
		ctx.out << std::endl;
		new_pt.PrintIndent();
		ctx.out << "}"sv;
	}
}