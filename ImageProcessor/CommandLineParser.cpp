/**
 * @file CommandLineParser.cpp
 */

#include "CommandLineParser.h"
#include "Exceptions.h"

#include <iostream>
#include <string>

namespace ip {

namespace {
    /// argv 의 다음 인자를 안전하게 가져온다.
    std::string nextArg(int argc, char* argv[], int& i, const std::string& flag) {
        if (i + 1 >= argc) {
            throw ArgumentError(flag + ": missing value");
        }
        return argv[++i];
    }
} // anonymous namespace

ProgramOptions CommandLineParser::parse(int argc, char* argv[]) {

    ProgramOptions programOptions;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        bool isFind = false;

        for (int index = 0; index < HELP; ++index) {
            Option option = programOptions.options[index];
            std::string* labels = option.getLabels();
            int size = option.getLabelSize();

            for (int labelIndex = 0; labelIndex < size; ++labelIndex) {
                if (arg == labels[labelIndex]) {
                    programOptions.options[index].setArg(nextArg(argc, argv, i, arg));
                    isFind = true;
                    break;
                }
            }

            if (isFind)
                break;
        }

        if(!isFind)
            throw ArgumentError("Unknown option: " + arg);


        if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            std::exit(0);
        }
    }

    Logger::init(programOptions.options[ip::OptionIndex::LOG].getArg(), true);

    // 필수 인자 검증
    if (programOptions.options[INPUT].getArg().empty()) {
        LOGGER.LogError("ArgumentError", "--input is required");
        throw ArgumentError("--input is required");
    }
    if (programOptions.options[OUTPUT].getArg().empty()) {
        LOGGER.LogError("ArgumentError", "--output is required");
        throw ArgumentError("--output is required");
    }

    for (int i = FILTER; i < HELP; ++i) {
        if (!programOptions.options[i].getArg().empty())
        {
            std::string str = programOptions.options[i].getLabels()[0] + ":" + programOptions.options[i].getArg();
            LOGGER.LogInfo("Parameter", str);
            programOptions.optionString += str + "/";
        }
    }

    //filter 인자 사용 없이 이진화만 진행했을시 에러가 발생하지 않고 진행
    if (programOptions.optionString.empty()) {
        LOGGER.LogError("ArgumentError", "--filter is required");
        throw ArgumentError("--filter is required");
    }


    return programOptions;
}

void CommandLineParser::printUsage(const std::string& exeName) {

    ProgramOptions programOptions;

    std::cout
        << "Usage:\n"
        << "  " << exeName << " --input <path> --output <path> --filter <name>\n\n"
        << "Options:\n";

    for (int i = 0; i < END; ++i) {
        std::cout << programOptions.options[i].getHelpLog();
    }

    std::cout
        << "Examples:\n"
        << "  " << exeName << " -i input.bmp -o result.bmp -f grayscale\n"
        << "  " << exeName << " -i input.bmp -o result.bmp -f threshold:128\n";
}

Option::Option(std::vector<std::string> optionLabels, std::string helpLog) : optionLabels(optionLabels), helpLog(helpLog)
{

}

std::string Option::getHelpLog()
{
    return helpLog;
}

std::string* Option::getLabels()
{
    return optionLabels.data();
}

std::string Option::getArg()
{
    return arg;
}

int Option::getLabelSize()
{
    return optionLabels.size();
}

void Option::setArg(std::string arg)
{
    this->arg = arg;
}

} // namespace ip
