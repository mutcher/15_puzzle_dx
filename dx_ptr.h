#pragma once
#include <Unknwnbase.h>
#include <type_traits>

template<typename _Ty>
class dx_ptr final
{
public:
    typedef typename std::add_pointer<_Ty>::type PtrType;
    typedef typename std::add_pointer<PtrType>::type PtrPtrType;

    dx_ptr(const PtrType val = nullptr)
        :_data(val)
    {
        static_assert(std::is_base_of<IUnknown, _Ty>::value, "_Ty should be derived from IUnknown inteface");
    }

    ~dx_ptr()
    {
        _delete();
    }

    void reset(const PtrType new_val = nullptr)
    {
        _delete();
        _data = new_val;
    }

    PtrType getp() const
    {
        return _data;
    }

    PtrPtrType getpp()
    {
        return &_data;
    }

    PtrType operator->() const
    {
        return getp();
    }

    operator PtrType() const
    {
        return _data;
    }

    PtrType release()
    {
        PtrType data = _data;
        _data = nullptr;
        return data;
    }

private:
    void _delete()
    {
        if (_data != nullptr)
        {
            _data->Release();
            _data = nullptr;
        }
    }

    PtrType _data;
};