#ifndef __MWL_STATIC_ASSERT_H__
#define __MWL_STATIC_ASSERT_H__

namespace mwl {
#if _MSC_VER >= 1900
#define MWL_STATIC_ASSERT(exp, msg) static_assert(exp, #msg)
#else

    template<bool> struct STATIC_ASSERT_FAIL;
    template<> struct STATIC_ASSERT_FAIL<true> {};

#if defined(__GNUC__)
    // calm gcc down about the unused-variable warning...
#define MWL_STATIC_ASSERT(exp, msg) \
    static const STATIC_ASSERT_FAIL<static_cast<bool>((exp))> msg __attribute__((unused))
#else
#define MWL_STATIC_ASSERT(exp, msg) \
    static const STATIC_ASSERT_FAIL<static_cast<bool>((exp))> msg
#endif

#endif // _MSC_VER

} /* namespace mwl */

#endif // __MWL_STATIC_ASSERT_H__
