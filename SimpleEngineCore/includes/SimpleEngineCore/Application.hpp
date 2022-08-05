namespace SimpleEngine {
	class Application {
	public:
		Application();
		Application(const Application&)=delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&)=delete;
		Application& operator=(Application&&) = delete;
		virtual ~Application();

		virtual int start(unsigned int window_width, unsigned int window_height, const char* title);
		virtual void on_update() {}
	};
}