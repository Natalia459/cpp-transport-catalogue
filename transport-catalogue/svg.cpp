#include "svg.h"

namespace svg {

	using namespace std::literals;


	void Object::Render(const RenderContext& context) const {
		context.RenderIndent();

		// Делегируем вывод тега своим подклассам
		RenderObject(context);

		context.out << std::endl;
	}

	// ---------- Circle ------------------

	Circle& Circle::SetCenter(Point center) {
		center_ = center;
		return *this;
	}

	Circle& Circle::SetRadius(double radius) {
		radius_ = radius;
		return *this;
	}

	void Circle::RenderObject(const RenderContext& context) const {
		auto& out = context.out;
		out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
		out << "r=\""sv << radius_ << "\""sv;
		RenderAttrs(out);
		out << "/>"sv;
	}

	//----------- Polyline ----------------

	Polyline& Polyline::AddPoint(Point point) {
		if (!points_.empty()) {
			points_ += " ";
		}
		std::ostringstream ostr;
		ostr << point.x << "," << point.y;

		points_ += ostr.str();
		return *this;
	}

	void Polyline::RenderObject(const RenderContext& context) const {
		auto& out = context.out;
		out << "<polyline"sv;
		out << " points=\""sv;
		out << points_ << "\""sv;
		RenderAttrs(out);
		out << "/>"sv;
	}

	//----------- Text --------------------

	Text& Text::SetPosition(Point pos) {
		pos_ = pos;
		return *this;
	}

	Text& Text::SetOffset(Point offset) {
		offset_ = offset;
		return *this;
	}

	Text& Text::SetFontSize(uint32_t size) {
		size_ = size;
		return *this;
	}

	Text& Text::SetFontFamily(std::string font_family) {
		font_family_ = font_family;
		return *this;
	}

	Text& Text::SetFontWeight(std::string font_weight) {
		font_weight_ = font_weight;
		return *this;
	}

	Text& Text::SetData(std::string data) {
		data_ = data;
		return *this;
	}

	void Text::RenderObject(const RenderContext& context) const {
		auto& out = context.out;
		out << "<text"sv;
		RenderAttrs(out);
		out << " x=\""sv << pos_.x << "\" y=\""sv << pos_.y << "\""sv;
		out << " dx=\""sv << offset_.x << "\" dy=\""sv << offset_.y << "\""sv;
		out << " font-size=\""sv << size_ << "\""sv;
		if (font_family_ != std::nullopt) {
			out << " font-family=\""sv << font_family_.value() << "\""sv;
		}
		if (font_weight_ != std::nullopt) {
			out << " font-weight=\""sv << font_weight_.value() << "\""sv;
		}
		out << ">"sv;

		for (const char& d : data_) {
			switch (d) {
			case '\"':
				out << "&quot;"sv; break;
			case '\'':
				out << "&apos;"sv; break;
			case '<':
				out << "&lt;"sv; break;
			case '>':
				out << "&gt;"sv; break;
			case '&':
				out << "&amp;"sv; break;
			default:
				out << d;
			}
		}
		out << "</text>"sv;
	}

	//----------- Document ----------------

	void Document::AddPtr(std::unique_ptr<Object>&& obj) {
		objects_.push_back(move(obj));
	}

	void Document::Render(std::ostream& out) const {
		out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv;
		out << std::endl;
		out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv;
		out << std::endl;
		for (const auto& obj : objects_) {
			out << "  "sv;
			obj.get()->Render(out);
		}
		out << "</svg>"sv;
	}


}  // namespace svg