#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(TypeName&) = delete; \
	void operator=(TypeName) = delete

#define DEFINE_SINGLETON(class_name) \
	public: \
		static class_name& getInstance(void);

#define IMPLEMENT_SINGLETON(class_name) \
	class_name& class_name::getInstance() { \
		static class_name instance; \
		return instance; \
	}

class Watcher 
{
public:
	template <typename TCallback>
	Watcher(TCallback callback) :
		callback(callback) { }
	~Watcher(void) {
		this->callback();
	}

private:
	std::function<void(void)> callback;
};