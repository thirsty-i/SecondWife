

class noncopyable
{
protected:
	noncopyable() {}
	~noncopyable() {}

	noncopyable(const noncopyable&) = delete;
	noncopyable& operator=(const noncopyable&) = delete;
};