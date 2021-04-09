#include <util/TestBase.h>
#include <stdio.h>

std::string TestBase::name() {
	return "Test";
}

void TestBase::test() {
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			run();
		}
	}
	close();
}