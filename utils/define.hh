#ifndef __RLX_DEFINES__
#define __RLX_DEFINED__

#define GETVAL(val, node)                      \
    if (node[#val])                            \
        _##val = node[#val].as<std::string>(); \
    else                                       \
        throw std::runtime_error(#val " is missing in recipe file");

#define GETVAL_TYPE(val, node, type)    \
    if (node[#val])                     \
        _##val = node[#val].as<type>(); \
    else                                \
        throw std::runtime_error(#val " is missing in recipe file");

#define OPTVAL(val, node, opt)                 \
    if (node[#val])                            \
        _##val = node[#val].as<std::string>(); \
    else                                       \
        _##val = opt;

#define DEFINE_GET_METHOD(type, value) \
    type const value() const           \
    {                                  \
        return _##value;               \
    }

#define DEFINE_SET_METHOD(type, value) \
    void value(type _v)                \
    {                                  \
        _##value = _v;                 \
    }

#define DEFINE_RETURNING_SET_METHOD(obj, id, type) \
    obj &id(type __##id)                           \
    {                                              \
        _##id = __##id;                            \
        return *this;                              \
    }

#define DEFINE_SELF_RETURNING_GET_SET_METHOD(obj, value, type) \
    DEFINE_GET_METHOD(type, value)                             \
    DEFINE_RETURNING_SET_METHOD(obj, value, type)

#define DEFINE_GET_METHOD_PUSH(type, value) \
    std::vector<type> const value() const   \
    {                                       \
        return _##value##s;                 \
    }

#define DEFINE_RETURNING_SET_METHOD_PUSH(obj, id, type) \
    obj &id(type _##id)                                 \
    {                                                   \
        _##id##s.push_back(_##id);                      \
        return *this;                                   \
    }

#define DEFINE_SELF_RETURNING_GET_SET_METHOD_PUSH(obj, value, type) \
    DEFINE_GET_METHOD_PUSH(type, value)                             \
    DEFINE_RETURNING_SET_METHOD_PUSH(obj, value, type)

#endif