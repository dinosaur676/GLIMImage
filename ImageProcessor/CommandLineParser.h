#pragma once

/**
 * @file CommandLineParser.h
 * @brief 커맨드라인 인자 파싱.
 *
 * 본 클래스는 기본 옵션(--input, --output, --filter)만 파싱한다.
 * 고급 옵션(--pipeline, --threads, --log 등)을 추가하려면
 * 본 클래스를 확장하거나 ProgramOptions 에 필드를 추가하라.
 */

#include <string>
#include <vector>

#include "Logger.h"

namespace ip {
    
    enum OptionIndex{
        INPUT,
        OUTPUT,
        LOG,
        FILTER,
        THRESHOLD,
        BRIGHT,
        CONTRAST,
        PIPELINE,
        SHARPEN,
        CROP,
        RESIZE,
        FLIP,
        HISTOGRAM,
        HELP,
        END
    };

    class Option {
    private:
        std::vector<std::string> optionLabels;
        std::string helpLog;
        std::string arg;
    public:
        Option() = default;
        Option(std::vector<std::string> optionLabels, std::string helpLog);

        std::string getHelpLog();
        std::string* getLabels();
        std::string getArg();

        int getLabelSize();
        void setArg(std::string arg);
    };

/// 파싱 결과를 담는 단순 구조체.
struct ProgramOptions {
    Option options[END] = {
        Option({"--input", "-i"},       "  -i, --input       <path>   Input BMP file (24-bit, uncompressed)\n"),
        Option({"--output", "-o"},      "  -o, --output      <path>   Output BMP file\n  -o, --output  <path>   Output BMP file\n"),
        Option({"--log", "-l"},         "  -l, --log         <path>   Log file\n"),
        Option({"--filter", "-f"},      "  -f, --filter      <name>   Filter to apply (e.g. grayscale, threshold:128)\n"),
        Option({"--threshold", "-t"},   "  -t, --threshold   <number> Execute thresholding with threshold\n"),
        Option({"--brightness", "-b"},  "  -b, --brightness  <number> Execute brightness\n"),
        Option({"--contrast", "-c"},    "  -c, --contrast    <number> Execute contrast\n"),
        Option({"--pipeline", "-p"},    "  -p, --pipeline    <names>  Execute the filters. (ex. grayscale;blur;threshold:128)\n"),
        Option({"--sharpen", "-s"},     "  -s, --sharpen     <number> When using Blur, set sharpen to 1 and blur to 0 \n"),
        Option({"--crop", "-cr"},       "  -cr, --crop       <number list> Enter the start (x, y) and end (x, y) coordinates for cropping, separated by commas. (ex. -cr 250,250,300,300) \n"),
        Option({"--resize", "-re"},     "  -re, --resize     <number list> Enter the width and height for resizing, separated by commas. (ex. -re 250,250) \n"),
        Option({"--flip", "-fl"},       "  -fl, --flip       <number 1 or 0> Enter the desired flip direction in the order of vertical flip and horizontal flip. (ex. -fl 1, 0) \n"),
        Option({"--histogram", "-hi"},  "  -hi, --histogram                  Print Histogram \n"),
        Option({"--help", "-h"},        "  -h, --help                 Show this message\n\n"),
    };

    std::string optionString;

    //std::string inputPath;      ///< --input  / -i
    //std::string outputPath;     ///< --output / -o
    //std::string filterName;     ///< --filter / -f
    //std::string log;            ///< --log / -l
    //std::string threshold;      ///< --threshold / -t
    //std::string bright;         ///< --brightness / -b
    //std::string contrast;       ///< --contrast / -c
    //std::string pipeline;       ///< --pipeline / -p
};

class CommandLineParser {
public:
    CommandLineParser() = delete;  // 인스턴스화 금지 (정적 메서드만 제공)

    /**
     * @brief argv 를 파싱하여 ProgramOptions 를 반환한다.
     * @throws ArgumentError 필수 인자 누락, 알 수 없는 옵션, 형식 오류 등.
     */
    static ProgramOptions parse(int argc, char* argv[]);

    /// 사용법을 표준 출력에 출력한다.
    static void printUsage(const std::string& exeName);
};

} // namespace ip
