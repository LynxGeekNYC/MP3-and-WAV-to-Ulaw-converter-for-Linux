#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

// Basic shell escaping for POSIX sh: wrap in single quotes and escape embedded single quotes.
static std::string sh_escape(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 2);
    out.push_back('\'');
    for (char c : s) {
        if (c == '\'') {
            out += "'\\''"; // close quote, escape single quote, reopen
        } else {
            out.push_back(c);
        }
    }
    out.push_back('\'');
    return out;
}

static void usage(const char* prog) {
    std::cerr
        << "Usage:\n"
        << "  " << prog << " <input.(mp3|wav|...)> [output.ulaw] [--wav]\n\n"
        << "Behavior:\n"
        << "  - Default output is raw G.711 mu-law (8kHz, mono) bytes: .ulaw\n"
        << "  - If --wav is provided, output is a WAV container with mu-law codec.\n\n"
        << "Examples:\n"
        << "  " << prog << " greeting.mp3 greeting.ulaw\n"
        << "  " << prog << " greeting.wav greeting.ulaw\n"
        << "  " << prog << " greeting.mp3 greeting.wav --wav\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        usage(argv[0]);
        return 2;
    }

    std::string input = argv[1];
    std::string output;
    bool output_wav_container = false;

    // Parse args
    for (int i = 2; i < argc; i++) {
        std::string a = argv[i];
        if (a == "--wav") {
            output_wav_container = true;
        } else if (output.empty()) {
            output = a;
        } else {
            std::cerr << "Unexpected argument: " << a << "\n";
            usage(argv[0]);
            return 2;
        }
    }

    if (!fs::exists(input)) {
        std::cerr << "Input file not found: " << input << "\n";
        return 1;
    }

    // Default output naming if not provided
    if (output.empty()) {
        fs::path inpath(input);
        if (output_wav_container) {
            output = (inpath.parent_path() / (inpath.stem().string() + ".wav")).string();
        } else {
            output = (inpath.parent_path() / (inpath.stem().string() + ".ulaw")).string();
        }
    }

    // Build ffmpeg command
    // -ar 8000: resample to 8kHz
    // -ac 1: mono
    // For raw ulaw: -f mulaw
    // For wav container ulaw: -c:a pcm_mulaw (WAV muxer chosen by extension)
    std::ostringstream cmd;

    if (output_wav_container) {
        cmd
            << "ffmpeg -hide_banner -loglevel error -y "
            << "-i " << sh_escape(input) << " "
            << "-ar 8000 -ac 1 -c:a pcm_mulaw "
            << sh_escape(output);
    } else {
        cmd
            << "ffmpeg -hide_banner -loglevel error -y "
            << "-i " << sh_escape(input) << " "
            << "-ar 8000 -ac 1 -f mulaw "
            << sh_escape(output);
    }

    int rc = std::system(cmd.str().c_str());
    if (rc != 0) {
        std::cerr << "Conversion failed. Ensure ffmpeg is installed and the input is readable.\n";
        return 1;
    }

    std::cout << "Converted:\n"
              << "  Input : " << input << "\n"
              << "  Output: " << output << "\n"
              << (output_wav_container ? "  Format: WAV container (pcm_mulaw)\n"
                                      : "  Format: Raw mu-law (G.711) bytes\n");
    return 0;
}
