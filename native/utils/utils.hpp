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

#define NODE_ENV info.Env()
#define NAPI_STRING(x) node_string_t::New(NODE_ENV, x)

typedef const Napi::CallbackInfo& node_info_t;

typedef Napi::Value node_any_t;
typedef Napi::Value node_value_t;
typedef Napi::String node_string_t;
typedef Napi::Number node_number_t;
typedef Napi::Object node_object_t;
typedef Napi::Array node_array_t;

#define NAPI_DEFINE_METHOD(name) Napi::Value name(node_info_t info)

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