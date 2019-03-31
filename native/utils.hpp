#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(TypeName&) = delete; \
	void operator=(TypeName) = delete
