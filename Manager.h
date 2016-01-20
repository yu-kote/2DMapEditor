#pragma once
#include "EnvSingleton.h"
#include "MapEditor.h"
#include "Model_MapEditor.h"


class Manager{
private:


	Scene scene_change = Scene::MAP_EDITOR;
	MapEditor mapeditor;


public:
	void setup(){
		mapeditor.setup(WIDE,LENGTH);
	};
	void update(){
		switch (scene_change)
		{
		case Scene::MAP_EDITOR:
			mapeditor.update();

			if (env.isPressKey('R') && env.isPressKey(GLFW_KEY_LEFT_CONTROL))
			{
				mapeditor.setup(WIDE, LENGTH);
			}
			break;
		}
	};
	void draw(){

		switch (scene_change)
		{
		case Scene::MAP_EDITOR:

			mapeditor.draw();

			break;
		}
	};

	void shift(){
		switch (scene_change)
		{
		case Scene::MAP_EDITOR:
			scene_change = mapeditor.shift();
			break;
		}
	}

};