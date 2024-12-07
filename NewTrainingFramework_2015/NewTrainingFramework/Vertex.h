#pragma once
#include "Math.h"

struct Vertex 
{
	Vector3 pos;
	Vector3 color;
	Vector3 norm, binorm, tgt;
	Vector2 uv;

	Vertex() : pos(0.0f, 0.0f, 0.0f), color(0.0f, 0.0f, 0.0f), norm(0.0f, 0.0f, 0.0f), binorm(0.0f, 0.0f, 0.0f), tgt(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f) {}

	Vertex(const Vertex &v)
	{
		pos.x = v.pos.x; pos.y = v.pos.y; pos.z = v.pos.z;
		color.x = v.color.x; color.y = v.color.y; color.z = v.color.z;
		norm.x = v.norm.x; norm.y = v.norm.y; norm.z = v.norm.z;
		binorm.x = v.binorm.x; binorm.y = v.binorm.y; binorm.z = v.binorm.z;
		tgt.x = v.tgt.x; tgt.y = v.tgt.y; tgt.z = v.tgt.z;
		uv.x = v.uv.x; uv.y = v.uv.y;
	}
};