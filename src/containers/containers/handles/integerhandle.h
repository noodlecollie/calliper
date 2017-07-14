#ifndef INTEGERHANDLE_H
#define INTEGERHANDLE_H

#include <utility>

namespace Containers
{
    template<typename T>
    class IntegerHandle
    {
    public:
        static constexpr T INVALID_HANDLE = 0;

        IntegerHandle()
            : m_nHandle(INVALID_HANDLE)
        {
        }

        IntegerHandle(T val)
            : m_nHandle(val)
        {
        }

        IntegerHandle(T&& val)
            : m_nHandle(std::move(val))
        {
        }

        inline bool isNull() const
        {
            return m_nHandle != INVALID_HANDLE;
        }

        inline bool isValid() const
        {
            return !isNull();
        }

        inline T value() const
        {
            return m_nHandle;
        }

        inline void setValue(T val)
        {
            m_nHandle = val;
        }

        inline operator bool() const
        {
            return isValid();
        }

        inline operator T() const
        {
            return value();
        }

        inline IntegerHandle<T>& operator =(T val)
        {
            setValue(val);
            return *this;
        }

        inline IntegerHandle<T>& operator =(T&& val)
        {
            m_nHandle = std::move(val);
            return *this;
        }

        inline bool operator ==(T val) const
        {
            return m_nHandle == val;
        }

        inline bool operator !=(T val) const
        {
            return !(*this == val);
        }

    private:
        T m_nHandle;
    };
}

#endif // INTEGERHANDLE_H
