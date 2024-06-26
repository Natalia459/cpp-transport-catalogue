#pragma once

#include <cmath>
namespace transport_catalogue {

	namespace details {

		struct Coordinates {
			double lat;
			double lng;

			bool operator==(const Coordinates& other) const {
				return lat == other.lat && lng == other.lng;
			}
			bool operator!=(const Coordinates& other) const {
				return !(*this == other);
			}
		};

		double ComputeDistance(Coordinates from, Coordinates to);
	}
}