#pragma once
#include "EnvSingleton.h"
#include <functional>

//点と四角の当り判定
static bool pointCollision(Vec2f point_pos, Vec2f box_pos, Vec2f box_size){
	//drawPoint(point_pos.x(), point_pos.y(), 100, Color::yellow);
	return (point_pos.x() > box_pos.x() &&
			point_pos.x() < box_pos.x() + box_size.x() &&
			point_pos.y() > box_pos.y() &&
			point_pos.y() < box_pos.y() + box_size.y());
}

// 作りかけ番号入力
static int putNumber(){
	if (env.isPushKey(GLFW_KEY_0)){ return 0; }
	if (env.isPushKey(GLFW_KEY_1)){ return 1; }
	if (env.isPushKey(GLFW_KEY_2)){ return 2; }
	if (env.isPushKey(GLFW_KEY_3)){ return 3; }
	if (env.isPushKey(GLFW_KEY_4)){ return 4; }
	if (env.isPushKey(GLFW_KEY_5)){ return 5; }
	if (env.isPushKey(GLFW_KEY_6)){ return 6; }
	if (env.isPushKey(GLFW_KEY_7)){ return 7; }
	if (env.isPushKey(GLFW_KEY_8)){ return 8; }
	if (env.isPushKey(GLFW_KEY_9)){ return 9; }
}

//************************************************************************************
// Easing関連　引数(0~1まで増加する値, 始点, 終点)
//************************************************************************************

// 
float EasingBackIn(float t, float b, float e) {
	float s = 1.70158;
	return (e - b) * t * t * ((s + 1) * t - s) + b;
}

float EasingBackOut(float t, float b, float e) {
	float s = 1.70158;
	t -= 1.0;
	return (e - b) * (t * t * ((s + 1) * t + s) + 1) + b;
}

float EasingBackInOut(float t, float b, float e) {
	float s = 1.70158 * 1.525;
	if ((t /= 0.5) < 1) return (e - b) / 2 * (t * t * ((s + 1) * t - s)) + b;
	t -= 2;
	return (e - b) / 2 * (t * t * ((s + 1) * t + s) + 2) + b;
}


float EasingBounceOut(float t, float b, float e) {
	if (t < (1 / 2.75)) {
		return (e - b) * (7.5625 * t * t) + b;
	}
	else if (t < (2 / 2.75)) {
		t -= (1.5 / 2.75);
		return (e - b) * (7.5625 * t * t + 0.75) + b;
	}
	else if (t < (2.5 / 2.75)) {
		t -= (2.25 / 2.75);
		return (e - b) * (7.5625 * t * t + 0.9375) + b;
	}
	else {
		t -= (2.625 / 2.75);
		return (e - b) * (7.5625 * t * t + 0.984375) + b;
	}
}

float EasingBounceIn(float t, float b, float e) {
	return (e - b) - EasingBounceOut(1.0 - t, 0, e - b) + b;
}

float EasingBounceInOut(float t, float b, float e) {
	if (t < 0.5) return EasingBounceIn(t * 2, 0, e - b) * 0.5 + b;
	else         return EasingBounceOut(t * 2 - 1.0, 0, e - b) * 0.5 + (e - b) * 0.5 + b;
}


float EasingCircIn(float t, float b, float e) {
	return -(e - b) * (std::sqrt(1 - t * t) - 1) + b;
}

float EasingCircOut(float t, float b, float e) {
	t -= 1.0;
	return (e - b) * std::sqrt(1 - t * t) + b;
}

float EasingCircInOut(float t, float b, float e) {
	if ((t /= 0.5) < 1) return -(e - b) / 2 * (std::sqrt(1 - t * t) - 1) + b;
	t -= 2;
	return (e - b) / 2 * (std::sqrt(1 - t * t) + 1) + b;
}


float EasingCubicIn(float t, float b, float e) {
	return (e - b) * t * t * t + b;
}

float EasingCubicOut(float t, float b, float e) {
	t -= 1.0;
	return (e - b) * (t * t * t + 1) + b;
}

float EasingCubicInOut(float t, float b, float e) {
	if ((t /= 0.5) < 1) return (e - b) / 2 * t * t * t + b;
	t -= 2;
	return (e - b) / 2 * (t * t * t + 2) + b;
}


float EasingElasticIn(float t, float b, float e) {
	if (t == 0) return b;
	if (t == 1) return e;

	float p = 0.3;
	float a = e - b;
	float s = p / 4;
	t -= 1;
	return -(a * std::pow(2, 10 * t) * std::sin((t - s) * (2 * M_PI) / p)) + b;
}

float EasingElasticOut(float t, float b, float e) {
	if (t == 0) return b;
	if (t == 1) return e;

	float p = 0.3;
	float a = e - b;
	float s = p / 4;
	return (a * std::pow(2, -10 * t) * std::sin((t - s) * (2 * M_PI) / p) + a + b);
}

float EasingElasticInOut(float t, float b, float e) {
	if (t == 0) return b;
	if ((t /= 0.5) == 2) return e;

	float p = 0.3 * 1.5;
	float a = e - b;
	float s = p / 4;
	if (t < 1) {
		t -= 1;
		return -0.5 * (a * std::pow(2, 10 * t) * std::sin((t - s) * (2 * M_PI) / p)) + b;
	}
	t -= 1;
	return a * std::pow(2, -10 * t) * std::sin((t - s) * (2 * M_PI) / p) * 0.5 + a + b;
}


float EasingExpoIn(float t, float b, float e) {
	return (t == 0) ? b : (e - b) * std::pow(2, 10 * (t - 1)) + b;
}

float EasingExpoOut(float t, float b, float e) {
	return (t == 1) ? e : (e - b) * (-std::pow(2, -10 * t) + 1) + b;
}

float EasingExpoInOut(float t, float b, float e) {
	if (t == 0) return b;
	if (t == 1) return e;
	if ((t /= 0.5) < 1) return (e - b) / 2 * std::pow(2, 10 * (t - 1)) + b;
	return (e - b) / 2 * (-std::pow(2, -10 * --t) + 2) + b;
}


float EasingQuadIn(float t, float b, float e) {
	return (e - b) * t * t + b;
}

float EasingQuadOut(float t, float b, float e) {
	return -(e - b) * t * (t - 2) + b;
}

float EasingQuadInOut(float t, float b, float e) {
	if ((t /= 0.5) < 1) return (e - b) / 2 * t * t + b;
	--t;
	return -(e - b) / 2 * (t * (t - 2) - 1) + b;
}


float EasingQuartIn(float t, float b, float e) {
	return (e - b) * t * t * t * t + b;
}

float EasingQuartOut(float t, float b, float e) {
	t -= 1.0;
	return -(e - b) * (t * t * t * t - 1) + b;
}

float EasingQuartInOut(float t, float b, float e) {
	if ((t /= 0.5) < 1) return (e - b) / 2 * t * t * t * t + b;
	t -= 2;
	return -(e - b) / 2 * (t * t * t * t - 2) + b;
}


float EasingQuintIn(float t, float b, float e) {
	return (e - b) * t * t * t * t * t + b;
}

float EasingQuintOut(float t, float b, float e) {
	t -= 1.0;
	return (e - b) * (t * t * t * t * t + 1) + b;
}

float EasingQuintInOut(float t, float b, float e) {
	if ((t /= 0.5) < 1) return (e - b) / 2 * t * t * t * t * t + b;
	t -= 2;
	return (e - b) / 2 * (t * t * t * t * t + 2) + b;
}


float EasingSineIn(float t, float b, float e) {
	return -(e - b) * std::cos(t * (M_PI / 2)) + (e - b) + b;
}

float EasingSineOut(float t, float b, float e) {
	return (e - b) * std::sin(t * (M_PI / 2)) + b;
}

float EasingSineInOut(float t, float b, float e) {
	return -(e - b) / 2 * (std::cos(M_PI * t) - 1) + b;
}


float EasingLinear(float t, float b, float e) {
	return (e - b) * t + b;
}
