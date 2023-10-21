#pragma once

namespace transport_catalogue {

	namespace tests {

		void Tests();

		void AddStop();

		void AddBus();

		void FindStop();

		void FindBus();

		void ReadData();

		void ShowData();

		void GetBusInfo();

		void GetStopInfo();

		void GetDistanceInfo();

		void tsB_case1_input();

		void tsB_case2_input();

		void tsC_case1_input();

		void PatrCCase2Input();

		namespace speed {

			void AddStop();

			void AddBus();

			void FindStop();

			void FindBus();

			void ReadData();

			void ShowData();
		}
	}
}