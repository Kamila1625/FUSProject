#pragma once

struct EllipseData
{
	int center[3];
	float angles[2];
};

void PrintEllipse(EllipseData* data);