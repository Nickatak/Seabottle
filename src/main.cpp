#include "seabottle.h"

using namespace seabottle;


std::string test_controller() {
	return "WOW ARE WE WORKING?";
};

int main()
{
	App* app = new App();
	app->on("/", &test_controller);

	app->run();
	return 0;
}
