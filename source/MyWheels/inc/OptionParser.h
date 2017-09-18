#ifndef __MWL_OPTION_PARSER_H__
#define __MWL_OPTION_PARSER_H__

#include "inc/BasicDefines.h"
#include "String.h"

namespace mwl {
    struct MWL_API CLIOption {
        String opt;
        String optValue;
    };

    enum ArgumentType {
        ARG_NONE = 0L,
        ARG_REQUIRED = 1L,
        ARG_OPTIONAL = 2L,
    };

    enum OptionParserError {
        OPT_ERR_NONE = 0L,
        OPT_ERR_UNKNOWN_OPT = -1L,
        OPT_ERR_DUPLICATE_OPT = -2L,
        OPT_ERR_AMBIGUOUS_OPT = -3L,
        OPT_ERR_MISSING_ARG = -4L,
        OPT_ERR_REDUNDANT_ARG = -5L,
    };

    class MWL_API OptionParser : private NonCopyable {
    public:
        OptionParser();
        ~OptionParser();
        int32_t SetCmd(const String &cmd);
        int32_t SetArgs(int32_t argc, const char *argv[]);
        int32_t AddOption(const char *shortName, const char *longName, const char *helpMsg,
            const char *metaVar = nullptr, const char *defaultVal = nullptr,
            ArgumentType argType = ARG_NONE);
        String Usage() const;
        int32_t ParseOption(Array<CLIOption> &options, Array<String> &residualArgs, String *pInvalidOpt = nullptr);
        void Reset();

    private:
        struct Implement;
        Implement *m_pImpl;
    };


};

#endif // __MWL_OPTION_PARSER_H__
