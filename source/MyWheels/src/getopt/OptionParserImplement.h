#ifndef __MWL_OPTION_PARSER_IMPLEMENT_H__
#define __MWL_OPTION_PARSER_IMPLEMENT_H__

#include "inc/OptionParser.h"
#include <map>

namespace mwl {
    struct _OptionInfo {
        String shortName;
        String longName;
        String helpMsg;
        String defVal;
        String metaVar;
        int32_t argType;
    };

    struct OptionParser::Implement {
        ~Implement();
        int32_t _SetCmd(const String &cmd);
        int32_t _SetArgs(int32_t argc, const char *argv[]);
        int32_t _AddOption(const char *shortName, const char *longName, const char *helpMsg,
            const char *metaVar, const char *defaultVal, ArgumentType argType);
        String _Usage();
        int32_t _ParseOption(Array<CLIOption> &options, Array<String> &residualArgs, String *pInvalidOpt);
        void _Reset();

        String _cmd;
        Array<String> _argv;
        Array<_OptionInfo*> _optInfos;
        std::map<String, _OptionInfo*> _opts;
    };
}

#endif // __MWL_OPTION_PARSER_IMPLEMENT_H__
