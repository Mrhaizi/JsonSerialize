#pragma once

#include <json/json.h>

#define REFLECT__PP_FOREACH_1(f, _1) f(_1)
#define REFLECT__PP_FOREACH_2(f, _1, _2) f(_1) f(_2)
#define REFLECT__PP_FOREACH_3(f, _1, _2, _3) f(_1) f(_2) f(_3)
#define REFLECT__PP_FOREACH_4(f, _1, _2, _3, _4) f(_1) f(_2) f(_3) f(_4)
#define REFLECT__PP_FOREACH_5(f, _1, _2, _3, _4, _5) f(_1) f(_2) f(_3) f(_4) f(_5)
#define REFLECT__PP_FOREACH_6(f, _1, _2, _3, _4, _5, _6) f(_1) f(_2) f(_3) f(_4) f(_5) f(_6)
#define REFLECT__PP_FOREACH_7(f, _1, _2, _3, _4, _5, _6, _7) f(_1) f(_2) f(_3) f(_4) f(_5) f(_6) f(_7)
#define REFLECT__PP_FOREACH_8(f, _1, _2, _3, _4, _5, _6, _7, _8) f(_1) f(_2) f(_3) f(_4) f(_5) f(_6) f(_7) f(_8)
#define REFLECT__PP_FOREACH_9(f, _1, _2, _3, _4, _5, _6, _7, _8, _9) f(_1) f(_2) f(_3) f(_4) f(_5) f(_6) f(_7) f(_8) f(_9)
#define REFLECT__PP_FOREACH_10(f, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) f(_1) f(_2) f(_3) f(_4) f(_5) f(_6) f(_7) f(_8) f(_9) f(_10)

#define REFLECT__PP_IMPAL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define REFLECT__PP_NARGS(...) REFLECT__PP_IMPAL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)


#define REFLECT__PP_CALL_2(...) __VA_ARGS__
#define REFLECT__PP_CALL(...) REFLECT__PP_CALL_2(__VA_ARGS__)
#define REFLECT__PP_CONCAT_2(x, y) x##y
#define REFLECT__PP_CONCAT(x, y) REFLECT__PP_CONCAT_2(x, y) 
#define REFLECT__PP_FOREACH(f, ...) REFLECT__PP_CALL(REFLECT__PP_CONCAT(REFLECT__PP_FOREACH_, REFLECT__PP_NARGS(__VA_ARGS__))(f, __VA_ARGS__)); 

template <class T>
struct reflect_trait {
    static  constexpr bool has_members() { 
        return requires(T t) {
            t.for_each_members([](const char* key, auto &) {});
        };
    }; 
    template <class Func>
    static constexpr void for_each_members(T const& cself, Func&& func) {
        T &self = const_cast<T &>(cself);
        self.for_each_members(func);
    }
};


#define REFLECT__TYPE_BEGIN(Type, ...) \
template <> \
struct reflect_trait<Type> { \
    static  constexpr bool has_members() {return true;}; \
    template <class Func> \
    static constexpr void for_each_members(Type const& cself, Func&& func) { \
        Type &self = const_cast<Type &>(cself);


#define REFLECT__TYPE_TEMPLATE_BEGIN(Type, ...) \
template <__VA_ARGS__> \
struct reflect_trait<REFLECT__PP_CALL(REFLECT__PP_CALL Type)> { \
    static  constexpr bool has_members() {return true;}; \
    template <class Func> \
    static constexpr void for_each_members(REFLECT__PP_CALL(REFLECT__PP_CALL Type) const& cself, Func&& func) { \
        REFLECT__PP_CALL(REFLECT__PP_CALL Type)  &self = const_cast<REFLECT__PP_CALL(REFLECT__PP_CALL Type)  &>(cself);


#define REFLECT__TYPE_PER_MEMBER(x) \
    func(#x, self.x); 

#define REFLECT__PRE_MEMBER(x) \
    func(#x, x);

#define REFLECT__TYPE_END()  \
    }  \
};


#define REFLECT_TYPE(Type, ...) \
REFLECT__TYPE_BEGIN(Type) \
REFLECT__PP_FOREACH(REFLECT__TYPE_PER_MEMBER, __VA_ARGS__) \
REFLECT__TYPE_END()

#define REFLECT_TYPE_TEMPLATED(Type, ...) \
REFLECT__PP_CALL(REFLECT__TYPE_TEMPLATE_BEGIN Type) \
REFLECT__PP_FOREACH(REFLECT__TYPE_PER_MEMBER, __VA_ARGS__) \
REFLECT__TYPE_END()

    
#define REFLECT(...) \
template <class Func> \
void for_each_members(Func &&func) { \
    REFLECT__PP_FOREACH(REFLECT__PRE_MEMBER, __VA_ARGS__) \
} 


