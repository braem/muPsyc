#include <iostream>
#include <string>
#include <math.h>
#include "MusicDS.h"

std::ostream & music::operator<<(std::ostream & strm, const Duration & d) {
	std::string str;
	switch (d.type()) {
	case 1: str = "o"; break;
	case 2: str = "ol"; break;
	case 4: str = "xl"; break;
	case 8: str = "xl\'"; break;
	case 16: str = "xl\'\'"; break;
	case 32: str = "xl\'\'\'"; break;
	case 64: str = "xl\'\'\'\'"; break;
	case 128: str = "xl\'\'\'\'\'"; break;
	}
	for (int i = 0; i < d.dots(); i++) {
		str = str + ".";
	}
	return strm << str;
}

void music::Duration::fraction(unsigned int * numerator, unsigned int * denominator) const {
	int num = 1;
	int den = t;
	for (unsigned int i = 0; i < d; i++) { // 1/4 -> 3/8 -> 7/16 -> 15/32
		den *= 2;
		num = (den / 2) - 1;
	}
	*numerator = num;
	*denominator = den;
}

double music::Duration::realDuration() {
	unsigned int num, den;
	fraction(&num, &den);
	return static_cast<double>(num) / static_cast<double>(den);
}

unsigned int music::Duration::tickLength() const {
	int tick = (t * MAX_DURATION);
	for (int i = 0; i < d; i++) //add another note of half duration (defn of dot)
		tick += (t * 2 * (i + 1)) * MAX_DURATION;
	return tick;
}

bool music::Duration::add(Duration first, Duration second, Duration * result) {
	if (first.t == second.t && first.d == 0 && second.d == 0) {
		*result = Duration(first.t / 2, 0);
	}
	else {
		if (first.d > 0 || second.d > 0) { //dotted
			//half + dotted quarter = double dotted half
			if (first.d == 0 && second.t / 2 == first.t) *result = Duration(first.t, second.d + 1);
			else if (second.d == 0 && first.t / 2 == second.t) *result = Duration(second.t, first.d + 1);
			//dotted half + quarter = whole
			else if (second.d == 0) {
				Duration tmp = Duration(first);
				for (int i = 0; i < first.d; i++) tmp.halfDuration();
				if (tmp.t == second.t) *result = Duration(first.t / 2, 0);
				else return false;
			}
			else if (first.d == 0) {
				Duration tmp = Duration(second);
				for (int i = 0; i < second.d; i++) tmp.halfDuration();
				if (tmp.t == first.t) *result = Duration(second.t / 2, 0);
				else return false;
			}
			else if (first.d == second.d && first.t == second.t) *result = Duration(first.t / 2, first.d);
			else return false;
		}
		else { //not dotted
			if (second.t / 2 == first.t) *result = Duration(first.t, 1);
			else if (first.t / 2 == second.t) *result = Duration(second.t, 1);
			else return false;
		}
	}
	return true;
}

bool music::Duration::operator==(const Duration & other) const {
	if (this->t == other.t)
		return true;
	else return false;
}

bool music::Duration::operator!=(const Duration & other) const {
	if (*this == other) 
		return false;
	else return true;
}

//for <, <=, >, >= : a half note will never be as long or longer than a whole note
// & vice versa; lower t will always be longer
bool music::Duration::operator>=(const Duration & other) const {
	if (t < other.t)
		return true;
	else if (*this == other)
		return true;
	else return false;
}

bool music::Duration::operator>(const Duration & other) const {
	if (t < other.t)
		return true;
	else return false;
}

bool music::Duration::operator<=(const Duration & other) const {
	if (t > other.t)
		return true;
	else if (*this == other)
		return true;
	else return false;
}

bool music::Duration::operator<(const Duration & other) const {
	if (t > other.t)
		return true;
	else return false;
}

unsigned int music::Duration::operator+(const Duration & other) const {
	return this->tickLength() + other.tickLength();
}

unsigned int music::Duration::operator-(const Duration & other) const {
	return this->tickLength() >= other.tickLength() ? 
		this->tickLength() - other.tickLength() : other.tickLength() - this->tickLength();
}
