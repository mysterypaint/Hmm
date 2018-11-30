#pragma once
#include <string>
//#include "../PHL.hpp"
#include "Components.hpp"

class TestComponent2 : public Component {
public:
	int tag;
	TestComponent* test1;

	TestComponent2(int _t) {
		tag = _t;
	}

	void Create() override {
		test1 = &entity->GetComponent<TestComponent>();
		printf("\n%d", test1->tag);
	}

	void Step() override {
	}
private:
};