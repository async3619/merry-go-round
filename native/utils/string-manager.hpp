#ifndef MERRY_GO_ROUND_STIRNG_MANAGER_HPP
#define MERRY_GO_ROUND_STIRNG_MANAGER_HPP

class Object;

typedef const char* node_string_t;
typedef std::vector< node_string_t> string_array_t;
typedef std::function<void(const Object*)> object_notfier_t;
typedef std::map<const Object*, std::vector<node_string_t>> dictionary_t;

class StringManager 
{
private:
	DISALLOW_COPY_AND_ASSIGN(StringManager);
	DEFINE_SINGLETON(StringManager);

private:
	static bool isFinalized;

private:
	StringManager(void);
	~StringManager(void);

public:
	object_notfier_t registerObject(const Object* object);
	char* allocateString(const Object* object, const char* original);

private:
	void onObjectDestruct(const Object* object);

private:
	dictionary_t dictionary;
};

#define NODE_STRING(x) StringManager::getInstance().allocateString(this, code_cvt_helper::toUtf8(x))

#endif // MERRY_GO_ROUND_STIRNG_MANAGER_HPP
