#include "lib/framework.hpp"
#include "EnvSingleton.h"
#include "Manager.h"



int main() {
	App::get();

	Manager manager;
	manager.setup();

	while (env.isOpen()) {
		manager.shift();
		manager.update();

		env.begin();

		manager.draw();

		env.end();
	}
}
