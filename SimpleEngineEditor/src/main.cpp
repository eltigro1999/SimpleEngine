#include <iostream>
#include <memory>
#include "SimpleEngineCore/Application.hpp"

class MyApp : public SimpleEngine::Application{
public:
	virtual void on_update() {
		std::cout << "Update frame: " << frame++ << std::endl;
	}
private:
	int frame = 0;
};
int main() {
	std::unique_ptr myApp = std::make_unique<MyApp>();
	int returnCode = myApp->start(1024, 768, "My first app");
	return 0;
}