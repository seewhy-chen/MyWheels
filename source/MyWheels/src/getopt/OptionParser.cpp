#include "inc/OptionParser.h"
#include "OptionParserImplement.h"
#include <map>

namespace mwl {

    OptionParser::OptionParser() : m_pImpl(new Implement()) {}

    OptionParser::~OptionParser() {
        delete m_pImpl;
    }

    int32_t OptionParser::SetCmd(const String &cmd) {
        return m_pImpl->_SetCmd(cmd);
    }

    int32_t OptionParser::SetArgs(int32_t argc, const char *argv[]) {
        return m_pImpl->_SetArgs(argc, argv);
    }

    int32_t OptionParser::AddOption(const char *shortName, const char *longName, const char *helpMsg,
        const char *metaVar, const char *defaultVal, ArgumentType argType) {
        return m_pImpl->_AddOption(shortName, longName, helpMsg, metaVar, defaultVal, argType);
    }

    String OptionParser::Usage() const {
        return m_pImpl->_Usage();
    }

    int32_t OptionParser::ParseOption(Array<CLIOption> &options, Array<String> &residualArgs, String *pInvalidOpt) {
        return m_pImpl->_ParseOption(options, residualArgs, pInvalidOpt);
    }

    void OptionParser::Reset() {
        return m_pImpl->_Reset();
    }
}
