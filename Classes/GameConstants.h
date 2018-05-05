#pragma once
typedef enum {
	kdown = 0,
	kleft,
	kright,
	kup,
	knormal
}HeroDirection;

typedef enum {
	kwall,
	kdoor,
	kupstair,
	kdownstair,
	kenemy,
	kstore,
	knpc
}CollisionType;