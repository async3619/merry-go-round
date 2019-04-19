#ifndef MERRY_GO_ROUND_MEMORY_MANAGER_HPP
#define MERRY_GO_ROUND_MEMORY_MANAGER_HPP

class MemoryManager : public Releasable
{
private:
	struct MemoryMap 
	{
	private:
		static char buffer[4096];

	private:
		MemoryMap(MemoryMap&) = delete;
		MemoryMap& operator=(MemoryMap const&) = delete;

	public:
		MemoryMap(void* pointer, std::size_t size, char const* path, int line);
		~MemoryMap(void) = default;

	public:
		void tryDump(void) const;

	private:
		int line;
		std::size_t size;
		char const* path;
		void* pointer;
	};

private:
	DISALLOW_COPY_AND_ASSIGN(MemoryManager);

private:
	static bool isFianlized;
	static MemoryManager* instance;

public: 
	static MemoryManager* getInstance(void);

private:
	MemoryManager(void);
	~MemoryManager(void);

public:
	void* alloc(std::size_t size, char const* path, int line) noexcept(false);
	void free(void* pointer) throw();

public:
	void checkMemoryLeak(void);

public:
	virtual void release(void) override;

private:
	std::unordered_map<void*, MemoryMap> memoryDictionary;
};

#endif // MERRY_GO_ROUND_MEMORY_MANAGER_HPP
