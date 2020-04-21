/*
 * Polygon.cpp
 *
 *  Created on: Jul 20, 2013
 *      Author: dodd
 */

#include "Polygon.h"

vector<Polygon> polygons;
vector<int> srcInPolygons;

bool pointInPolygon(double x, double y, Polygon polygon) {

	int n = polygon.corners.size() - 1, i, sum, t1, t2, f; // commented out: prob = 0;

//	point t;

	vector<point> tempPolygon;

	for (i = 0; i <= n; i++) {
		point temppoint;
		temppoint.x = polygon.corners[i]->x - x;
		temppoint.y = polygon.corners[i]->y - y;
		tempPolygon.push_back(temppoint);

	}

	t1 = tempPolygon[0].x >= 0 ?
			(tempPolygon[0].y >= 0 ? 0 : 3) : (tempPolygon[0].y >= 0 ? 1 : 2);
	for (sum = 0, i = 1; i <= n; i++) {
		if (!tempPolygon[i].x && !tempPolygon[i].y)
			break;
		f = tempPolygon[i].y * tempPolygon[i - 1].x
				- tempPolygon[i].x * tempPolygon[i - 1].y;
		if (!f && tempPolygon[i - 1].x * tempPolygon[i].x <= 0
				&& tempPolygon[i - 1].y * tempPolygon[i].y <= 0)
			break;
		t2 = tempPolygon[i].x >= 0 ?
				(tempPolygon[i].y >= 0 ? 0 : 3) :
				(tempPolygon[i].y >= 0 ? 1 : 2);
		if (t2 == (t1 + 1) % 4)
			sum += 1;
		else if (t2 == (t1 + 3) % 4)
			sum -= 1;
		else if (t2 == (t1 + 2) % 4) {
			if (f > 0)
				sum += 2;
			else
				sum -= 2;
		}
		t1 = t2;
	}
	if (i <= n || sum)
		return true;
	else
		return false;
//	for (i = 0; i <= n; i++)
//		polygon.corners[i]->x += x, polygon.corners[i]->y += y;

	return false;

}

