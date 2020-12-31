#pragma once

#include "stroke.h"
#include <gdkmm.h>
#include <vector>

#include <X11/X.h>

#define STROKE_SIZE 64

class Stroke;
class PreStroke;

typedef std::shared_ptr<Stroke> RStroke;
typedef std::shared_ptr<PreStroke> RPreStroke;

struct Triple {
	float x;
	float y;
	Time t;
};
typedef std::shared_ptr<Triple> RTriple;
void update_triple(RTriple e, float x, float y, Time t);
RTriple create_triple(float x, float y, Time t);

class PreStroke;
class Stroke {
	friend class PreStroke;
public:
	struct Point {
		double x;
		double y;
		Point operator+(const Point &p) {
			Point sum = { x + p.x, y + p.y };
			return sum;
		}
		Point operator-(const Point &p) {
			Point sum = { x - p.x, y - p.y };
			return sum;
		}
		Point operator*(const double a) {
			Point product = { x * a, y * a };
			return product;
		}
	};

private:

	Glib::RefPtr<Gdk::Pixbuf> draw_(int size, double width = 2.0, bool inv = false) const;
	mutable Glib::RefPtr<Gdk::Pixbuf> pb[2];

	static Glib::RefPtr<Gdk::Pixbuf> drawEmpty_(int);
	static Glib::RefPtr<Gdk::Pixbuf> pbEmpty;
public:
	int trigger;
	int button;
	unsigned int modifiers;
	bool timeout;
	std::shared_ptr<stroke_t> stroke;

	Stroke() : trigger(0), button(0), modifiers(AnyModifier), timeout(false) {}
    Stroke(PreStroke &s, int trigger_, int button_, unsigned int modifiers_, bool timeout_);

	Glib::RefPtr<Gdk::Pixbuf> draw(int size, double width = 2.0, bool inv = false) const;
	void draw(Cairo::RefPtr<Cairo::Surface> surface, int x, int y, int w, int h, double width = 2.0, bool inv = false) const;
	void draw_svg(std::string filename) const;
	bool show_icon();

	static RStroke trefoil();
	static int compare(RStroke, RStroke, double &);
	static Glib::RefPtr<Gdk::Pixbuf> drawEmpty(int);
	static Glib::RefPtr<Gdk::Pixbuf> drawDebug(RStroke, RStroke, int);

	unsigned int size() const { return stroke ? stroke_get_size(stroke.get()) : 0; }
	bool trivial() const { return size() == 0 && button == 0; }
	Point points(int n) const { Point p; stroke_get_point(stroke.get(), n, &p.x, &p.y); return p; }
	double time(int n) const { return stroke_get_time(stroke.get(), n); }
	bool is_timeout() const { return timeout; }
};

class PreStroke : public std::vector<RTriple> {
public:
	static RPreStroke create() { return RPreStroke(new PreStroke()); }
	void add(RTriple p) { push_back(p); }
	bool valid() const { return size() > 2; }
};