#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <optional>

#include <variant>
#include <map>
#include <sstream>

namespace svg {

	using namespace std::literals;

	enum class StrokeLineCap {
		BUTT,
		ROUND,
		SQUARE,
	};

	enum class StrokeLineJoin {
		ARCS,
		BEVEL,
		MITER,
		MITER_CLIP,
		ROUND,
	};

	inline std::ostream& operator<<(std::ostream& os, const StrokeLineCap& data) {
		switch (data) {
		case StrokeLineCap::BUTT:
			os << "butt"sv; break;
		case StrokeLineCap::ROUND:
			os << "round"sv; break;
		case StrokeLineCap::SQUARE:
			os << "square"sv; break;
		default:break;
		}
		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const StrokeLineJoin& data) {
		switch (data) {
		case StrokeLineJoin::ARCS:
			os << "arcs"; break;
		case StrokeLineJoin::BEVEL:
			os << "bevel"sv; break;
		case StrokeLineJoin::MITER:
			os << "miter"sv; break;
		case StrokeLineJoin::MITER_CLIP:
			os << "miter-clip"sv; break;
		case StrokeLineJoin::ROUND:
			os << "round"sv; break;
		default: break;
		}
		return os;
	}

	//const std::map<StrokeLineJoin, std::string_view> stroke_line_join = {
	//	{StrokeLineJoin::ARCS, "arcs"sv},
	//	{StrokeLineJoin::BEVEL, "bevel"sv},
	//	{StrokeLineJoin::MITER, "miter"sv},
	//	{StrokeLineJoin::MITER_CLIP, "miter-clip"sv},
	//	{StrokeLineJoin::ROUND, "round"sv}
	//};
	//inline std::ostream& operator<<(std::ostream& os, const StrokeLineJoin& joi) {
	//	os << stroke_line_join.at(joi);
	//	return os;
	//}
	//
	//const std::map<StrokeLineCap, std::string_view> stroke_line_cap = {
	//	{StrokeLineCap::BUTT, "butt"sv},
	//	{StrokeLineCap::ROUND, "round"sv},
	//	{StrokeLineCap::SQUARE, "square"sv}
	//};
	//inline std::ostream& operator<<(std::ostream& os, const StrokeLineCap& cap) {
	//	os << stroke_line_cap.at(cap);
	//	return os;
	//}


	struct Rgb {
		Rgb(int r = 0, int g = 0, int b = 0)
			:red(r), green(g), blue(b)
		{}

		int red = 0;
		int green = 0;
		int blue = 0;
	};

	struct Rgba {
		Rgba(int r = 0, int g = 0, int b = 0, double a = 1.0)
			:red(r), green(g), blue(b), opacity(a)
		{
		}

		int red = 0;
		int green = 0;
		int blue = 0;
		double opacity = 1.0;
	};

	using Color = std::variant<std::monostate, std::string, Rgb, Rgba>;

	// Объявив в заголовочном файле константу со спецификатором inline,
	// мы сделаем так, что она будет одной на все единицы трансляции,
	// которые подключают этот заголовок.
	// В противном случае каждая единица трансляции будет использовать свою копию этой константы
	inline const Color NoneColor{ "none" };

	inline void PrintColor(std::ostream& os, std::monostate) {
		os << "none"sv;
	}

	inline void PrintColor(std::ostream& os, std::string color) {
		os << color;
	}

	inline void PrintColor(std::ostream& os, Rgb color) {
		std::string red = std::to_string(color.red);
		std::string green = std::to_string(color.green);
		std::string blue = std::to_string(color.blue);
		std::ostringstream ostr;
		ostr << "rgb("sv << red << ","sv << green << ","sv << blue << ")"sv;
		os << ostr.str();
	}

	inline void PrintColor(std::ostream& os, Rgba color) {
		std::string red = std::to_string(color.red);
		std::string green = std::to_string(color.green);
		std::string blue = std::to_string(color.blue);
		std::ostringstream ostr;
		ostr << "rgba("sv << red << ","sv << green << ","sv << blue << ","sv << color.opacity << ")"sv;
		os << ostr.str();
	}

	inline std::ostream& operator<<(std::ostream& os, Color color) {
		std::visit([&os](auto color) {PrintColor(os, color); }, color);
		return os;
	}

	struct Point {
		Point() = default;
		Point(double x, double y)
			: x(x)
			, y(y) {
		}
		double x = 0;
		double y = 0;
	};

	/*
	 * Вспомогательная структура, хранящая контекст для вывода SVG-документа с отступами.
	 * Хранит ссылку на поток вывода, текущее значение и шаг отступа при выводе элемента
	 */
	struct RenderContext {
		RenderContext(std::ostream& out)
			: out(out) {
		}

		RenderContext(std::ostream& out, int indent_step, int indent = 0)
			: out(out)
			, indent_step(indent_step)
			, indent(indent) {
		}

		RenderContext Indented() const {
			return { out, indent_step, indent + indent_step };
		}

		void RenderIndent() const {
			for (int i = 0; i < indent; ++i) {
				out.put(' ');
			}
		}

		std::ostream& out;
		int indent_step = 0;
		int indent = 0;
	};

	//======================================================================

	/*
	 * Абстрактный базовый класс Object служит для унифицированного хранения
	 * конкретных тегов SVG-документа
	 * Реализует паттерн "Шаблонный метод" для вывода содержимого тега
	 */
	class Object {
	public:
		void Render(const RenderContext& context) const;

		virtual ~Object() = default;

	private:
		virtual void RenderObject(const RenderContext& context) const = 0;
	};

	//<<interface>>
	class ObjectContainer {
	public:
		template <typename T>
		void Add(T obj);

		virtual void AddPtr(std::unique_ptr<Object>&& obj) = 0;

	protected:
		~ObjectContainer() = default;
	};

	template <typename T>
	void ObjectContainer::Add(T obj) {
		AddPtr(std::make_unique<T>(std::move(obj)));
	}

	//<<interface>>
	class Drawable {
	public:
		virtual void Draw(ObjectContainer& container) const = 0;

		virtual ~Drawable() = default;
	};

	template <typename Owner>
	class PathProps {
	public:

		Owner& SetFillColor(Color color) {
			fill_color_ = color;
			return AsOwner();
		}
		Owner& SetStrokeColor(Color color) {
			stroke_color_ = color;
			return AsOwner();
		}

		Owner& SetStrokeWidth(double width) {
			stroke_width_ = width;
			return AsOwner();
		}

		Owner& SetStrokeLineCap(StrokeLineCap line_cap) {
			line_cap_ = line_cap;
			return AsOwner();
		}

		Owner& SetStrokeLineJoin(StrokeLineJoin line_join) {
			line_join_ = line_join;
			return AsOwner();
		}

	protected:
		~PathProps() = default;

		void RenderAttrs(std::ostream& out) const {
			using namespace std::literals;

			if (fill_color_) {
				out << " fill=\""sv << *fill_color_ << "\""sv;
			}
			if (stroke_color_) {
				out << " stroke=\""sv << *stroke_color_ << "\""sv;
			}
			if (stroke_width_) {
				out << " stroke-width=\""sv << *stroke_width_ << "\""sv;
			}
			if (line_cap_) {
				out << " stroke-linecap=\""sv << *line_cap_ << "\""sv;
			}
			if (line_join_) {
				out << " stroke-linejoin=\""sv << *line_join_ << "\""sv;
			}
		}

	private:
		Owner& AsOwner() {
			// static_cast безопасно преобразует *this к Owner&,
			// если класс Owner — наследник PathProps
			return static_cast<Owner&>(*this);
		}

		std::optional<Color> fill_color_; // none
		std::optional<Color> stroke_color_;
		std::optional<double> stroke_width_; // line_width
		std::optional<StrokeLineCap> line_cap_; // round
		std::optional<StrokeLineJoin> line_join_; // round
	};

	//================================================================================================

	/*
	 * Класс Circle моделирует элемент <circle> для отображения круга
	 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
	 */
	class Circle final : public Object, public PathProps<Circle> {
	public:
		Circle() = default;

		Circle& SetCenter(Point center);
		Circle& SetRadius(double radius);

	private:
		void RenderObject(const RenderContext& context) const override;

		Point center_;
		double radius_ = 1.0;
	};

	/*
	 * Класс Polyline моделирует элемент <polyline> для отображения ломаных линий
	 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
	 */
	class Polyline : public Object, public PathProps<Polyline> {
	public:
		Polyline() = default;

		// Добавляет очередную вершину к ломаной линии
		Polyline& AddPoint(Point point);

	private:
		void RenderObject(const RenderContext& context) const override;
		std::string points_ = "";
	};

	/*
	 * Класс Text моделирует элемент <text> для отображения текста
	 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
	 */
	class Text : public Object, public PathProps<Text> {
	public:
		Text() = default;

		// Задаёт координаты опорной точки (атрибуты x и y)
		Text& SetPosition(Point pos);

		// Задаёт смещение относительно опорной точки (атрибуты dx, dy)
		Text& SetOffset(Point offset);

		// Задаёт размеры шрифта (атрибут font-size)
		Text& SetFontSize(uint32_t size);

		// Задаёт название шрифта (атрибут font-family)
		Text& SetFontFamily(std::string font_family);

		// Задаёт толщину шрифта (атрибут font-weight)
		Text& SetFontWeight(std::string font_weight);

		// Задаёт текстовое содержимое объекта (отображается внутри тега text)
		Text& SetData(std::string data);

	private:
		void RenderObject(const RenderContext& context) const override;

		Point pos_ = { 0, 0 }; // x, y
		Point offset_ = { 0, 0 }; // dx, dy
		int size_ = 1;
		std::optional<std::string> font_family_;
		std::optional<std::string> font_weight_;
		std::string data_ = "";
	};

	class Document : public ObjectContainer {
	public:
		Document() = default;

		// Добавляет в svg-документ объект-наследник svg::Object
		void AddPtr(std::unique_ptr<Object>&& obj) override;

		// Выводит в ostream svg-представление документа
		void Render(std::ostream& out) const;

	private:
		std::vector<std::unique_ptr<Object>> objects_;
	};
}  // namespace svg