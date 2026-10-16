/*
  Copyright (c) 2014 John Miner

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
// CS130 Fall 2013: Computer Graphics
// point2d.h
//
// This file does not need to be modified
/////////////////////////////////////////
#ifndef __POINT2D_H__
#define __POINT2D_H__

struct Point2D
{
        double x;
        double y;

        Point2D() : x(0.0), y(0.0) {}
        Point2D(const double & nx, const double & ny) : x(nx), y(ny) {}
        
        Point2D operator+(const Point2D & rhs) const { return Point2D(x + rhs.x, y + rhs.y); }
        Point2D operator-(const Point2D & rhs) const { return Point2D(x - rhs.x, y - rhs.y); }
        Point2D operator*(double val) const { return Point2D(x * val, y * val); }
        Point2D operator/(double val) const { return Point2D(x / val, y / val); }
        Point2D operator+=(const Point2D & rhs) { x += rhs.x; y += rhs.y; return *this; }
        Point2D operator-=(const Point2D & rhs) { x -= rhs.x; y -= rhs.y; return *this; }
        Point2D operator*=(double val) { x *= val; y *= val; return *this; }
        Point2D operator/=(double val) { x /= val; y /= val; return *this; }
};

#endif
