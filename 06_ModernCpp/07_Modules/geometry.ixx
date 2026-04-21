export module Geometry;

import Math;

// No #include — avoid stdlib in module interface on GCC 14

export namespace Geometry {

    struct Point     { double x, y; };
    struct Circle    { Point center; double radius; };
    struct Rectangle { Point topLeft; double width, height; };

    double circleArea(const Circle& c) {
        return Math::PI * c.radius * c.radius;
    }

    double circleCircumference(const Circle& c) {
        return 2.0 * Math::PI * c.radius;
    }

    double rectangleArea(const Rectangle& r) {
        return r.width * r.height;
    }

    // sqrt without #include <cmath> — use Newton's method
    double sqrtNewton(double x) {
        if (x <= 0.0) return 0.0;
        double guess = x / 2.0;
        for (int i = 0; i < 20; ++i)
            guess = (guess + x / guess) / 2.0;
        return guess;
    }

    double distance(const Point& a, const Point& b) {
        double dx = b.x - a.x;
        double dy = b.y - a.y;
        return sqrtNewton(dx*dx + dy*dy);
    }
}
