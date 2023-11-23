#pragma once
#include "transport_catalogue.h"
#include "domain.h"
#include "json.h"
#include "geo.h"
#include "svg.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>
#include <sstream>

namespace transport_catalogue {

	namespace picture {

		inline const double EPSILON = 1e-6;


		inline bool IsZero(double value) {
			return std::abs(value) < EPSILON;
		}

		svg::Color ReadColor(json::Node color);

		class SphereProjector {
		public:
			SphereProjector() = default;

			// points_begin ? points_end ?????? ?????? ? ????? ????????? ????????? geo::Coordinates
			template <typename PointInputIt>
			SphereProjector(PointInputIt points_begin, PointInputIt points_end,
				double max_width, double max_height, double padding)
				: padding_(padding) //
			{
				if (points_begin == points_end) {
					return;
				}

				const auto [left_it, right_it] = std::minmax_element(
					points_begin, points_end,
					[](auto lhs, auto rhs) { return lhs.lng < rhs.lng; });
				min_lon_ = left_it->lng;
				const double max_lon = right_it->lng;

				const auto [bottom_it, top_it] = std::minmax_element(
					points_begin, points_end,
					[](auto lhs, auto rhs) { return lhs.lat < rhs.lat; });
				const double min_lat = bottom_it->lat;
				max_lat_ = top_it->lat;

				std::optional<double> width_zoom;
				if (!IsZero(max_lon - min_lon_)) {
					width_zoom = (max_width - 2 * padding) / (max_lon - min_lon_);
				}

				std::optional<double> height_zoom;
				if (!IsZero(max_lat_ - min_lat)) {
					height_zoom = (max_height - 2 * padding) / (max_lat_ - min_lat);
				}

				if (width_zoom && height_zoom) {
					zoom_coeff_ = std::min(*width_zoom, *height_zoom);
				}
				else if (width_zoom) {
					zoom_coeff_ = *width_zoom;
				}
				else if (height_zoom) {
					zoom_coeff_ = *height_zoom;
				}
			}

			svg::Point operator()(details::Coordinates coords) const {
				return {
					(coords.lng - min_lon_) * zoom_coeff_ + padding_,
					(max_lat_ - coords.lat) * zoom_coeff_ + padding_
				};
			}

		private:
			double padding_ = 0;
			double min_lon_ = 0;
			double max_lat_ = 0;
			double zoom_coeff_ = 0;
		};

		struct Settings {
			Settings() = default;

			Settings(json::Dict settings);

			double max_width = 0, max_height = 0, padding = 0; 
			double line_width = 0;
			double stop_radius = 0;
			int bus_label_font_size = 0;
			svg::Point bus_label_offset = { 0,0 }; // dx, dy text
			svg::Point stop_label_offset = { 0,0 }; // dx, dy text
			int stop_label_font_size = 0;
			double underlayer_width = 0; // stroke-width
			svg::Color underlayer_color = "";
			std::vector<svg::Color> color_palette = {};
		};

		struct BusInfo {
			std::string name;
			svg::Point point;
			size_t color_number;
			size_t order = 0;
		};

		struct Text2D {
			Text2D() = default;
			svg::Text text{};
			svg::Text underlayer{};
		};

		class Renderer { // text, path_props
		private:
			std::vector<svg::Polyline> lines_{};
			std::vector<svg::Circle> circles_ = {};
			std::vector<Text2D> stop_names_ = {};
			std::vector<Text2D> bus_names_{};
			Settings settings_;
			SphereProjector projector_;

		public:
			Renderer() = default;

			Renderer(Settings settings, SphereProjector projector)
				:settings_(std::move(settings)), projector_(projector)
			{
			}

			//---------------- get_characters -----------------------------------------------------------

			std::vector<svg::Polyline> GetPolyline() const {
				return lines_;
			}

			std::vector<svg::Circle> GetCircles() const {
				return circles_;
			}

			std::vector<Text2D> GetStopNames() const {
				return stop_names_;
			}

			std::vector<Text2D> GetBusNames() const {
				return bus_names_;
			}

			//---------------- set_characters -----------------------------------------------------------

			void SetPolylineCharacters(svg::Polyline& line, size_t color_number);

			void SetBusCharacters(Text2D& bus_name, size_t color_number);

			void SetStopCharacters(Text2D& names);

			//---------------- add_chracters ------------------------------------------------------------

			void AddPolylinePoints(std::vector<svg::Point>&& points, size_t color_number);

			void AddCirclePoints(const std::map<std::string, svg::Point>& stops);

			void AddBusTextes(std::vector<BusInfo>&& buses);

			void AddStopTextes(const std::map<std::string, svg::Point>& stops);

			//---------------- make_picture -------------------------------------------------------------

			void RenderSettings(const std::vector<types::Buses*>& data);

		};


		//---------------- show_picture -----------------------------------------------------------------

		Renderer GetRenderSettings(const json::Node& settings, const std::vector<types::Buses*>& data);

		void RenderVector(std::ostream& out, const picture::Renderer& rend);
	}
}