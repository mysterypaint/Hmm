#pragma once
#include <string>
//#include "../PHL.hpp"
#include "Components.hpp"

class TestComponent : public Component {
public:
	int tag;

	TestComponent(int _t) {
		tag = _t;
	}

	void Create() override {
		printf("\n%d", tag);
		printf("\n%", &entity);
	}

	void Step() override {
	}
private:
};