#include "OptionParserImplement.h"

namespace mwl {

    OptionParser::Implement::~Implement() {
        _Reset();
    }

    int32_t OptionParser::Implement::_SetCmd(const String &cmd) {
        _cmd = cmd;
        Array<String> splitted = _cmd.Split();
        Array<const char *> argVals;
        for (int32_t i = 0; i < splitted.Size(); ++i) {
            argVals.Append(splitted[i].C_Str());
        }
        return _SetArgs(argVals.Size(), argVals.Data());
    }

    int32_t OptionParser::Implement::_SetArgs(int32_t argc, const char *argv[]) {
        _argv.Clear();
        for (int32_t i = 0; i < argc; ++i) {
            _argv.Append(argv[i]);
            String &lastArg = _argv.Back();
            if (lastArg.StartsWith("-") && !lastArg.StartsWith("--") && lastArg.Len() > 2) {
                String compoundOpt = lastArg;
                _argv.Decrease();
                Array<String> expandedOpts;
                for (int32_t i = 1; i < compoundOpt.Len(); ++i) {
                    expandedOpts.Append("-" + compoundOpt.SubString(i, 1));
                }
                _argv.Append(expandedOpts);
            }
        }
        _cmd = String(" ").Join(_argv);
        return 0;
    }

    int32_t OptionParser::Implement::_AddOption(const char *shortName, const char *longName, const char *helpMsg,
            const char *metaVar, const char *defaultVal, ArgumentType argType) {
        String sName(shortName);
        sName.LStrip("-", 1);
        String lName(longName);
        lName.LStrip("-", 2);
        String hMsg(helpMsg);
        String mVar(metaVar);
        String defVal(defaultVal);

        if (sName.Empty() && lName.Empty()) {
            return -EINVAL;
        }
        if (hMsg.Empty()) {
            return -EINVAL;
        }
        if (!sName.Empty() && _opts.find(sName) != _opts.end()) {
            return OPT_ERR_DUPLICATE_OPT;
        }
        if (!lName.Empty() && _opts.find(lName) != _opts.end()) {
            return OPT_ERR_DUPLICATE_OPT;
        }

        _OptionInfo *pOptInfo = new _OptionInfo();
        pOptInfo->shortName = sName;
        pOptInfo->longName = lName;
        pOptInfo->helpMsg = hMsg;
        pOptInfo->defVal = defaultVal;
        pOptInfo->metaVar = metaVar;
        pOptInfo->argType = argType;
        _optInfos.Append(pOptInfo);

        _opts[sName] = pOptInfo;
        _opts[lName] = pOptInfo;
        return 0;

    }

    String OptionParser::Implement::_Usage() {
        if (_argv.Empty()) {
            return "Error: no arguments set";
        }

        Array<String> usages;
        String helpArg("-h, --help");
        String helpHelp("Show this help message and exit");
        int32_t maxUsageLen = helpArg.Len();
        for (int32_t i = 0; i < _optInfos.Size(); ++i) {
            usages.Increase();
            String &usage = usages.Back();
            const _OptionInfo &optInfo = *_optInfos[i];
            if (!optInfo.shortName.Empty()) {
                usage += "-" + optInfo.shortName;
                if (!optInfo.metaVar.Empty()) {
                    if (optInfo.argType == ARG_REQUIRED) {
                        usage += " <" + optInfo.metaVar + ">";
                    } else {
                        usage += " [" + optInfo.metaVar + "]";
                    }
                }
                if (!optInfo.longName.Empty()) {
                    usage += ", ";
                }
            }
            if (!optInfo.longName.Empty()) {
                usage += "--" + optInfo.longName;
                if (!optInfo.metaVar.Empty()) {
                    if (optInfo.argType == ARG_REQUIRED) {
                        usage += "=<" + optInfo.metaVar + ">";
                    } else {
                        usage += "=[" + optInfo.metaVar + "]";
                    }
                }
            }
            if (usage.Len() > maxUsageLen) {
                maxUsageLen = usage.Len();;
            }
        }
        usages.Append(helpArg);
        maxUsageLen += 2;

        for (int32_t i = 0; i < _optInfos.Size(); ++i) {
            const _OptionInfo &optInfo = *_optInfos[i];
            String &usage = usages[i];
            int32_t spaceToAppend = maxUsageLen - usage.Len();
            for (int32_t j = 0; j < spaceToAppend; ++j) {
                usage += " ";
            }
            usage += optInfo.helpMsg;
            if (!optInfo.defVal.Empty()) {
                usage += String().Format(" [default: %s]", optInfo.defVal.C_Str());
            }
        }
        String &usage = usages.Back();
        int32_t spaceToAppend = maxUsageLen - usage.Len();
        for (int32_t j = 0; j < spaceToAppend; ++j) {
            usage += " ";
        }
        usages.Back() += helpHelp;
        return String("\n").Join(usages);

    }

    int32_t OptionParser::Implement::_ParseOption(Array<CLIOption> &options, Array<String> &residualArgs, String *pInvalidOpt) {
        options.Clear();
        residualArgs.Clear();
        if (pInvalidOpt) {
            pInvalidOpt->Clear();
        }
        for (int32_t i = 1; i < _argv.Size(); ++i) {
            const String &arg = _argv[i];
            String possibleArgVal;
            bool isLong = false;
            if (!arg.StartsWith("-")) {
                residualArgs.Append(arg);
                continue;
            }

            String optName = arg.LStripped("-", 1);
            isLong = optName.StartsWith("-");
            if (isLong) {
                optName.LStrip("-", 1);
                if (optName.Contains("=")) {
                    Array<String> splitted = optName.RSplit("=", 1);
                    optName = splitted[0];
                    possibleArgVal = splitted[1];
                }
            }
            const _OptionInfo *pOptInfo = nullptr;
            if (_opts.find(optName) != _opts.end()) {
                pOptInfo = _opts[optName];
            }
            if (!pOptInfo || (isLong && pOptInfo->longName != optName) || (!isLong && pOptInfo->shortName != optName)) {
                pOptInfo = nullptr;
                if (isLong) {
                    Array<String> possibleOptName;
                    for (int32_t i = 0; i < _optInfos.Size(); ++i) {
                        if (_optInfos[i]->longName.StartsWith(optName)) {
                            possibleOptName.Append(_optInfos[i]->longName);
                        }
                    }
                    if (possibleOptName.Size() == 1) {
                        //optName = possibleOptName[0];
                        pOptInfo = _opts[possibleOptName[0]];
                    } else if (possibleOptName.Size() > 1) {
                        if (pInvalidOpt) {
                            *pInvalidOpt = optName;
                        } else {
                            fprintf(stderr, "%s: '--%s' is ambiguous; possibilities: ",
                                    _argv[0].C_Str(), optName.C_Str());
                            for (int32_t i = 0; i < possibleOptName.Size(); ++i) {
                                fprintf(stderr, "'--%s'", possibleOptName[i].C_Str());
                            }
                            fprintf(stderr, "\n");
                        }
                        return OPT_ERR_AMBIGUOUS_OPT;
                    }
                }
            }
            if (!pOptInfo) {
                if (pInvalidOpt) {
                    *pInvalidOpt = optName;
                } else {
                    fprintf(stderr, "%s: unrecongnized option -- '%s'\n", _argv[0].C_Str(), optName.C_Str());
                }
                return OPT_ERR_UNKNOWN_OPT;
            }

            bool skipNextArg = false;
            if (possibleArgVal.Empty() && i < _argv.Size() - 1) {
                possibleArgVal = _argv[i + 1];
                if (possibleArgVal.StartsWith("-")) {
                    possibleArgVal.Clear();
                } else {
                    skipNextArg = true;
                }
            }

            CLIOption option;
            switch (pOptInfo->argType) {
            case ARG_REQUIRED: {
                    if (possibleArgVal.Empty() && pOptInfo->defVal.Empty()) {
                        if (pInvalidOpt) {
                            *pInvalidOpt = arg;
                        } else {
                            fprintf(stderr, "%s: option requires an argument -- '%s'\n",
                                    _argv[0].C_Str(), optName.C_Str());
                        }
                        return OPT_ERR_MISSING_ARG;
                    }
                }
                break;
            case ARG_OPTIONAL: {
                }
                break;
            }
            option.opt = pOptInfo->shortName.Empty() ? pOptInfo->longName : pOptInfo->shortName;
            option.optValue = possibleArgVal.Empty() ? pOptInfo->defVal : possibleArgVal;
            for (int32_t i = 0; i < options.Size(); ++i) {
                if (option.opt == options[i].opt) {
                    if (pInvalidOpt) {
                        *pInvalidOpt = arg;
                    } else {
                        fprintf(stderr, "%s: duplicated options -- '%s'\n",
                                _argv[0].C_Str(), optName.C_Str());
                    }
                    return OPT_ERR_DUPLICATE_OPT;
                }
            }
            if (skipNextArg) {
                ++i;
            }
            options.Append(option);
        }
        return 0;
    }

    void OptionParser::Implement::_Reset() {
        _cmd.Clear();
        _argv.Clear();
        for (int32_t i = 0; i < _optInfos.Size(); ++i) {
            delete _optInfos[i];
        }
        _optInfos.Clear();
        _opts.clear();;
    }
}
