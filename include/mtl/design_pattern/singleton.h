#ifndef _H_SINGLETON_H_
#define _H_SINGLETON_H_

namespace mtl
{
	template <class T>
	class CSingleton
	{
	public:
		static T& GetInstance()
		{
			if (m_Instance == nullptr)
				m_Instance = new T;

			return *m_Instance;
		}

		static void DestroyInstance()
		{
			delete m_Instance;
			m_Instance = nullptr;
		}


		CSingleton(T&&) = delete;
		CSingleton(const T&) = delete;
		void operator= (const T&) = delete;

	protected:
		CSingleton(void) = default;
		virtual ~CSingleton() = default;

		static T* m_Instance;
	};

	template <class T>
	T* CSingleton<T>::m_Instance = nullptr;
};
#endif // _H_SINGLETON_H_