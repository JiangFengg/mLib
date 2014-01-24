
#pragma once

template <class T> class Vector
{
public:

	//
	// Constructors
	//
    Vector()
    {
        m_data = NULL;
        m_length = 0;
        m_capacity = 0;
    }

    explicit Vector(UINT64 length)
    {
        m_data = new T[length];
        m_length = length;
        m_capacity = length;
    }

    Vector(UINT64 size, const T &clearValue)
    {
        m_data = new T[size];
        m_length = size;
        m_capacity = size;
        clear(clearValue);
    }

    Vector(const Vector<T> &V)
    {
        m_length = V.m_length;
        m_capacity = V.m_length;
        if(V.m_length == 0) m_data = NULL;
        else
        {
            m_data = new T[m_length];
            for(UINT64 i = 0; i < m_length; i++) m_data[i] = V.m_data[i];
        }
    }

    Vector(Vector<T> &&V)
    {
        m_length = V.m_length;
        m_capacity = V.m_capacity;
        m_data = V.m_data;
        V.m_length = 0;
        V.m_capacity = 0;
        V.m_data = NULL;
    }

    ~Vector()
    {
        if(m_data != NULL) delete[] m_data;
    }
    
    //
    // Memory
    //
    void deleteMemory()
    {
        if(m_data != NULL)
        {
            delete[] m_data;
            m_data = NULL;
        }
        m_length = 0;
        m_capacity = 0;
    }

    void zeroMemory()
    {
        if(m_length > 0) memset(m_data, 0, m_length * sizeof(T));
    }

    void operator = (const Vector<T> &V)
    {
        m_length = V.m_length;
        m_capacity = m_length;
        if(m_data != NULL) delete[] m_data;
        if(m_length == 0) m_data = NULL;
        else
        {
            m_data = new T[m_length];
            for(UINT64 i = 0; i < m_length; i++) m_data[i] = V.m_data[i];
        }
    }

    void operator=(Vector<T>&& V)
    {
        if(this != &V)
        {
            if(m_data != NULL) delete[] m_data;
            m_length = V.m_length;
            m_capacity = V.m_capacity;
            m_data = V.m_data;
            V.m_length = 0;
            V.m_capacity = 0;
            V.m_data = NULL;
        }
    }

    void allocate(UINT64 size)
    {
        if(size == 0)
		{
			deleteMemory();
			return;
		}
        
        if(m_data != NULL) delete[] m_data;
        m_data = new T[size];
        m_length = size;
        m_capacity = size;
    }

    void allocate(UINT64 size, const T &clearValue)
    {
        allocate(size);
        clear(clearValue);
    }

    void resize(UINT64 size)
    {
        if(size == 0) deleteMemory();
        else if(size <= m_capacity) m_length = size;
        else
        {
            UINT64 newCapacity = size * 3 / 2 + 4;
            T *newData = new T[newCapacity];
            if(m_data != NULL)
            {
                std::move(m_data, m_data + m_length, newData);
                delete[] m_data;
            }
            m_data = newData;
            m_length = size;
            m_capacity = newCapacity;
        }
    }

    void reserve(UINT64 size)
    {
        if(size > m_capacity)
        {
            T *NewData = new T[size];
            if(m_data != NULL)
            {
                std::move(m_data, m_data + m_length, NewData);
                delete[] m_data;
            }
            m_data = NewData;
            m_capacity = size;
        }
    }

    //
    // Accessors
    //
    INLINE T& operator [] (UINT64 k)
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
        if(k >= m_length) MLIB_ERROR("Out-of-bounds vector access");
#endif
        return m_data[k];
    }
    INLINE T& operator [] (int k) 
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
        if(k < 0 || k >= int(m_length)) MLIB_ERROR("Out-of-bounds vector access");
#endif
        return m_data[k];
    }
    INLINE const T& operator [] (UINT64 k) const
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
        if(k >= m_length) MLIB_ERROR("Out-of-bounds vector access");
#endif
        return m_data[k];
    }
    INLINE const T& operator [] (int k) const
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
        if(k < 0 || k >= int(m_length)) MLIB_ERROR("Out-of-bounds vector access");
#endif
        return m_data[k];
    }
    INLINE UINT64 size() const
    {
        return m_length;
    }
    __forceinline T* ptr()
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
        if(m_length == 0) MLIB_ERROR("ptr called on zero-length vector");
#endif
        return m_data;
    }
    __forceinline const T* ptr() const
    {
#if defined(MLIB_BOUNDS_CHECK) || defined(DEBUG)
		if(m_length == 0) MLIB_ERROR("ptr called on zero-length vector");
#endif
        return m_data;
    }

	//
	// Random element uses rand() but we should also have a version that uses a RNG object
	//
    __forceinline T& randomElement()
    {
        MLIB_ASSERT(m_length > 0, "RandomElement called with no elements");
        return m_data[rand() % m_length];
    }
    __forceinline const T& randomElement() const
    {
        MLIB_ASSERT(m_length > 0, "RandomElement called with no elements");
        return m_data[rand() % m_length];
    }

    __forceinline T& back()
    {
        MLIB_ASSERT(m_length > 0, "Last called with no elements");
        return m_data[m_length - 1];
    }
    __forceinline const T& back() const
    {
        MLIB_ASSERT(m_length > 0, "Last called with no elements");
        return m_data[m_length - 1];
    }
    __forceinline T& front()
    {
        MLIB_ASSERT(m_length > 0, "First called with no elements");
        return m_data[0];
    }
    __forceinline const T& front() const
    {
        MLIB_ASSERT(m_length > 0, "First called with no elements");
        return m_data[0];
    }
    
    __forceinline T* begin()
    {
        return m_data;
    }
    __forceinline const T* begin() const
    {
        return m_data;
    }
    __forceinline T* end()
    {
        return m_data + m_length;
    }
    __forceinline const T* end() const
    {
        return m_data + m_length;
    }

    //
    // Modifiers
    //
    void pushBack(const T &t);
    void pushBack(T &&t);
	void popBack();
    void removeCopy(UINT64 index);
    void removeSwap(UINT64 index);
    void sort();
    template<class orderingType> void sort(orderingType Function)
    {
        if(m_length >= 2)
        {
            std::sort(m_data, m_data + m_length, Function);
        }
    }
	void scale(const T &t);

	//
	// map usage:
	// v.map([](T n) {return String(n);});
	//
    template<class mapFunction>
    auto map(mapFunction function) -> Vector<decltype(function(std::declval<T>()))>
    {
        Vector<decltype(function(std::declval<T>()))> result(m_length);
        for(UINT64 i = 0; i < m_length; i++) result[i] = function(m_data[i]);
        return result;
    }
    
    void clear(const T &T);

	//randomize should also take a RNG
	void randomize();

    //
    // Query
    //
    T sum() const;
    T product() const;

    const T& maxValue() const;
	const T& minValue() const;

    UINT64 maxIndex() const;
	UINT64 minIndex() const;

    UINT64 maxIndex(const std::function<double(const T&)> &valueFunction) const;
    UINT64 minIndex(const std::function<double(const T&)> &valueFunction) const;
    
    bool contains(const T &t) const;
    UINT64 count(const T &t) const;
    int findFirstIndex(const T &t) const;
    
private:
    T*     m_data;
    UINT64 m_length;
    UINT64 m_capacity;
};

template <class T> __forceinline bool operator == (const Vector<T> &a, const Vector<T> &b)
{
	if(a.length() != b.length()) return false;
	const UINT length = a.length();
	for(UINT i = 0; i < length; i++)
		if(a[index] != b[index])
			return false;
	return true;
}

template <class T> __forceinline bool operator != (const Vector<T> &a, const Vector<T> &b)
{
	if(a.length() != b.length()) return true;
	const UINT length = a.length();
	for(UINT i = 0; i < length; i++)
		if(a[i] != b[i])
			return true;
	return false;
}

#include "../../src/core-base/vector.cpp"