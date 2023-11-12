#include "map_renderer.h"

#include <deque>

using namespace std;
using namespace transport_catalogue;
using namespace picture;
using namespace svg;

Renderer picture::GetRenderSettings(const json::Dict& settings, const std::vector<types::Buses*>& data) {
	Settings sett(settings);

	if (settings.empty()) {
		return Renderer{};
	}
	return RenderSettings(move(sett), data);
}

Color picture::ReadColor(json::Node color) {
	if (color.IsString()) {
		return { color.AsString() };
	}

	auto vec = color.AsArray();

	int red = vec[0].AsInt();
	int green = vec[1].AsInt();
	int blue = vec[2].AsInt();

	if (vec.size() == 4) {
		double opacity = vec[3].AsDouble();
		return Rgba{ red, green, blue, opacity };
	}

	return Rgb{ red, green, blue };
}

Renderer picture::RenderSettings(Settings&& settings, const std::vector<types::Buses*>& data) {
	vector<details::Coordinates> coord;
	for (const auto& bus : data) {
		if (!bus->route.empty()) {
			for (const auto& stop : bus->route) {
				coord.push_back({ stop->lati, stop->longi });
			}
		}
	}
	SphereProjector projector(coord.begin(), coord.end(), settings.max_width, settings.max_height, settings.padding);

	Renderer rends(settings);
	map<string, svg::Point> stops;
	vector<BusInfo> buses;
	size_t color_number = 0;

	for (const auto& bus : data) {
		if (color_number == settings.color_palette.size()) {
			color_number = 0;
		}
		if (bus->route.empty()) {
			continue;
		}

		vector<svg::Point> coordinates;
		for (const auto& stop : bus->route) {
			const auto lat = stop->lati;
			const auto lon = stop->longi;
			Point curr = projector({ lat, lon });

			coordinates.push_back(curr);
			stops.emplace(stop->name, curr);
		}
		buses.push_back({ bus->name, coordinates.front(), color_number, 1 });

		if (bus->is_circle == false) {
			if (bus->route.front()->name != bus->route.back()->name) {
				buses.push_back({ bus->name, coordinates.back(), color_number, 2 });
			}

			vector<svg::Point> half_coordinates(coordinates.begin(), coordinates.end());
			for (auto stop = half_coordinates.rbegin() + 1; stop != half_coordinates.rend(); ++stop) {
				coordinates.push_back(*stop);
			}
		}
		rends.AddPolylinePoints(move(coordinates), color_number);
		++color_number;
	}
	rends.AddBusTextes(move(buses));
	rends.AddCirclePoints(stops);
	rends.AddStopTextes(stops);

	return rends;
}

void transport_catalogue::RenderVector(std::ostream& out, const picture::Renderer& rend) {
	svg::Document result;
	for (const auto& line : rend.GetPolyline()) {
		result.Add(line);
	}

	for (const auto& [text, underlayer] : rend.GetBusNames()) {
		result.Add(underlayer);
		result.Add(text);
	}

	for (const auto& circle : rend.GetCircles()) {
		result.Add(circle);
	}

	for (const auto& [text, underlayer] : rend.GetStopNames()) {
		result.Add(underlayer);
		result.Add(text);
	}

	result.Render(out);
}

picture::Settings::Settings(json::Dict settings) {
	max_height = settings.at("height").AsDouble();
	max_width = settings.at("width").AsDouble();
	padding = settings.at("padding").AsDouble();
	line_width = settings.at("line_width").AsDouble();
	stop_radius = settings.at("stop_radius").AsDouble();
	bus_label_offset.x = settings.at("bus_label_offset").AsArray().front().AsDouble();
	bus_label_offset.y = settings.at("bus_label_offset").AsArray().back().AsDouble();
	stop_label_offset.x = settings.at("stop_label_offset").AsArray().front().AsDouble();
	stop_label_offset.y = settings.at("stop_label_offset").AsArray().back().AsDouble();
	bus_label_font_size = settings.at("bus_label_font_size").AsInt();
	stop_label_font_size = settings.at("stop_label_font_size").AsInt();
	underlayer_width = settings.at("underlayer_width").AsDouble();
	underlayer_color = ReadColor(settings.at("underlayer_color"));
	for (auto& color : settings.at("color_palette").AsArray()) {
		color_palette.push_back(ReadColor(color));
	}
}


//---------------- set_characters --------------------------------------------------------------

void picture::Renderer::SetPolylineCharacters(svg::Polyline& line, size_t color_number) {
	line.SetStrokeWidth(settings_.line_width).SetStrokeColor(settings_.color_palette[color_number]);
	line.SetFillColor("none").SetStrokeLineCap(svg::StrokeLineCap::ROUND).SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
}

void picture::Renderer::SetBusCharacters(Text2D& bus_name, size_t color_number) {
	bus_name.text.SetFontSize(settings_.bus_label_font_size).SetOffset(settings_.bus_label_offset).SetFontFamily("Verdana").SetFontWeight("bold");
	bus_name.text.SetFillColor(settings_.color_palette[color_number]);

	bus_name.underlayer.SetFontSize(settings_.bus_label_font_size).SetOffset(settings_.bus_label_offset).SetFontFamily("Verdana");
	bus_name.underlayer.SetFillColor(settings_.underlayer_color).SetFontWeight("bold").SetStrokeWidth(settings_.underlayer_width);
	bus_name.underlayer.SetStrokeColor(settings_.underlayer_color).SetStrokeLineCap(svg::StrokeLineCap::ROUND).SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
}

void picture::Renderer::SetStopCharacters(Text2D& names) {
	auto& text = names.text;
	auto& under_text = names.underlayer;

	text.SetFontSize(settings_.stop_label_font_size).SetOffset(settings_.stop_label_offset).SetFontFamily("Verdana").SetFillColor("black");
	under_text.SetFontSize(settings_.stop_label_font_size).SetFillColor(settings_.underlayer_color).SetStrokeColor(settings_.underlayer_color);
	under_text.SetStrokeWidth(settings_.underlayer_width).SetOffset(settings_.stop_label_offset).SetFontFamily("Verdana");
	under_text.SetStrokeLineCap(svg::StrokeLineCap::ROUND).SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
}

//---------------- add_chracters ---------------------------------------------------------------

void picture::Renderer::AddPolylinePoints(std::vector<svg::Point>&& points, size_t color_number) {
	lines_.push_back(svg::Polyline{});
	auto& line = lines_.back();

	SetPolylineCharacters(line, color_number);
	for (auto point : points) {
		line.AddPoint(point);
	}
}

void picture::Renderer::AddCirclePoints(const std::map<std::string, svg::Point>& stops) {
	for (const auto& [name, point] : stops) {
		circles_.push_back(svg::Circle{});
		circles_.back().SetCenter(point).SetRadius(settings_.stop_radius).SetFillColor("white");
	}
}

void picture::Renderer::AddBusTextes(std::vector<BusInfo>&& buses) {
	std::sort(buses.begin(), buses.end(), [](const auto& lhs, const auto& rhs)
		{
			if (lhs.name == rhs.name) {
				return lhs.order < rhs.order;
			}
			return lhs.name < rhs.name;
		});

	for (auto& [name, point, color_number, order] : buses) {
		bus_names_.push_back(Text2D{});
		auto& bus_name = bus_names_.back();

		SetBusCharacters(bus_name, color_number);
		bus_name.text.SetData(name).SetPosition(point);
		bus_name.underlayer.SetData(name).SetPosition(point);
	}
}

void picture::Renderer::AddStopTextes(const std::map<std::string, svg::Point>& stops) {
	for (const auto& [name, point] : stops) {
		stop_names_.push_back(Text2D{});
		auto& text = stop_names_.back().text;
		auto& under_text = stop_names_.back().underlayer;

		SetStopCharacters(stop_names_.back());
		text.SetData(name).SetPosition(point);
		under_text.SetData(name).SetPosition(point);
	}
}