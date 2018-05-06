#pragma once
typedef enum {
	kdown = 0,
	kleft,
	kright,
	kup,
	knormal
}HeroDirection;

typedef enum {
	knull = 1,
	kwall,
	kdoor,
	kteleport,
	kenemy,
	kitem,
	kstore,
	knpc
}CollisionType;

typedef enum {
	kZfloor,
	kZwall,
	kZdoor,
	kZteleport,
	kZnpc,
	kZenemy,
	kZitem,
	kZhero,
}kZorder;