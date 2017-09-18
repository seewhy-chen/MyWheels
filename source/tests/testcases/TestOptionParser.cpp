#include "InternalCommon.h"
#include "inc/OptionParser.h"
using namespace mwl;

void TestOptionParser() {
    Array<CLIOption> options;
    Array<String> residualArgs;
    String invalOpt;

    /* errors to handle:
        1. unrecongnized option
        2. missing argV -- can't detect
        3. duplicated options -- can't detect
        4. not using default value when argV missing
    */ 
    OptionParser optParser;
    MWL_ASSERT(optParser.AddOption("-a", nullptr, "option a") >= 0);
    MWL_ASSERT(optParser.AddOption(nullptr, "--b", "option b", "metaB", nullptr, ARG_REQUIRED) >= 0);
    MWL_ASSERT(optParser.AddOption("-c", "--c1", "option c1", "metaC1", "defC1", ARG_OPTIONAL) >= 0);
    MWL_ASSERT(optParser.AddOption("-f", "--c2", "option c2", "metaC2", "defC2", ARG_OPTIONAL) >= 0);
    MWL_ASSERT(optParser.AddOption("-d", "--details", "option d") >= 0);

    MWL_ASSERT(optParser.AddOption("", "", "short name and long name can't both be empty") < 0);
    MWL_ASSERT(optParser.AddOption(nullptr, "--helpMsgCantBeEmpty", "") < 0);
    MWL_ASSERT(optParser.AddOption("-d", "--duplicated", "option d") < 0);
    MWL_ASSERT(optParser.AddOption("-x", "--details", "option d") < 0);

    String goodCmd = "progName -ac --b=argB --c2 --de=true residArg1 residArg2";
    optParser.SetCmd(goodCmd);
    MWL_ASSERT(optParser.ParseOption(options, residualArgs) >= 0);
    MWL_ASSERT(options.Size() == 5);
    for (int32_t i = 0; i < options.Size(); ++i) {
        if (options[i].opt == "a") {
            MWL_ASSERT(options[i].optValue.Empty());
        } else if (options[i].opt == "b") {
            MWL_ASSERT(options[i].optValue == "argB");
        } else if (options[i].opt == "d") {
            MWL_ASSERT(options[i].optValue == "true");
        } else if (options[i].opt == "f") {
            MWL_ASSERT(options[i].optValue == "defC2");
        } else if (options[i].opt == "c") {
            MWL_ASSERT(options[i].optValue == "defC1");
        } else {
            MWL_ASSERT(false);
        }
    }
    MWL_ASSERT(residualArgs.Size() == 2);
    MWL_ASSERT(residualArgs[0] == "residArg1");
    MWL_ASSERT(residualArgs[1] == "residArg2");


    String missingArgCmd = "progName --b";
    optParser.SetCmd(missingArgCmd);
    MWL_ASSERT(optParser.ParseOption(options, residualArgs) == OPT_ERR_MISSING_ARG);

    String invalOptCmd = "progName --invalopt";
    optParser.SetCmd(invalOptCmd);
    MWL_ASSERT(optParser.ParseOption(options, residualArgs) == OPT_ERR_UNKNOWN_OPT);

    String missTypedCmd = "progName --f"; // should be '-f', not '--f'
    optParser.SetCmd(missTypedCmd);
    MWL_ASSERT(optParser.ParseOption(options, residualArgs) == OPT_ERR_UNKNOWN_OPT);


    String duplicateOptCmd = "progName --d=true --details=false";
    optParser.SetCmd(duplicateOptCmd);
    MWL_ASSERT(optParser.ParseOption(options, residualArgs) == OPT_ERR_DUPLICATE_OPT);
    
    String ambiguousOptCmd = "progName --c";
    optParser.SetCmd(ambiguousOptCmd);
    MWL_ASSERT(optParser.ParseOption(options, residualArgs) == OPT_ERR_AMBIGUOUS_OPT);
}
