#pragma once

#define OG_BIT(x) (1 << x)

#define OG_DEFINE_ENUM_FLAG_OPERATORS(enumtype) \
extern "C++" { \
    inline constexpr enumtype operator | (enumtype a, enumtype b) noexcept { return enumtype(((int)a) | ((int)b)); } \
    inline enumtype &operator |= (enumtype &a, enumtype b) noexcept { return (enumtype &)(((int &)a) |= ((int)b)); } \
    inline constexpr enumtype operator & (enumtype a, enumtype b) noexcept { return enumtype(((int)a) & ((int)b)); } \
    inline enumtype &operator &= (enumtype &a, enumtype b) noexcept { return (enumtype &)(((int &)a) &= ((int)b)); } \
    inline constexpr enumtype operator ^ (enumtype a, enumtype b) noexcept { return enumtype(((int)a) ^ ((int)b)); } \
    inline enumtype &operator ^= (enumtype &a, enumtype b) noexcept { return (enumtype &)(((int &)a) ^= ((int)b)); } \
    inline constexpr enumtype operator ~ (enumtype a) noexcept { return enumtype(~((int)a)); } \
}
