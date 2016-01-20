#pragma once
#include "lib/framework.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <memory>

#define env App::get()

enum WindowSize{
	WIDTH = 1500,
	HEIGHT = 1000,
};

class App{
public:
	static AppEnv& get(){
		static AppEnv singleton_env(WIDTH, HEIGHT,false,true);
		return singleton_env;
	}
};

enum class Scene{
	MAP_EDITOR,

};