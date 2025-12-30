# MP3 and WAV to Ulaw converter for Linux
MP3 and WAV to Ulaw converter for Linux. Specifically designed for Asterisk use.


---

# MP3/WAV to G.711 μ-law Converter (Linux, C++)

## Overview

This project provides a lightweight C++ command-line utility for Linux that converts audio files (MP3, WAV, and any format supported by FFmpeg) into **G.711 μ-law (ULAW)** audio.

The tool is designed for telephony and VoIP use cases, including **Asterisk**, **FreePBX**, IVR systems, and call playback environments that require **8 kHz, mono, μ-law–encoded audio**.

The program acts as a safe and minimal wrapper around FFmpeg, ensuring accurate decoding, resampling, and μ-law encoding without reimplementing complex audio codecs in C++.

---

## Features

* Converts MP3, WAV, and other FFmpeg-supported formats to G.711 μ-law
* Outputs either:

  * Raw `.ulaw` (headerless, telephony-ready), or
  * `.wav` container using μ-law codec
* Enforces telephony-safe parameters:

  * 8 kHz sample rate
  * Mono audio
* Designed for Linux environments
* Simple CLI usage
* Production-stable and script-friendly

---

## Requirements

* Linux (tested on Debian/Ubuntu, compatible with most distributions)
* FFmpeg
* C++17-compatible compiler (GCC or Clang)

---

## Installation

### Install FFmpeg

**Debian / Ubuntu**

```bash
sudo apt update
sudo apt install -y ffmpeg
```

**Fedora**

```bash
sudo dnf install -y ffmpeg
```

> Note: On some RHEL/CentOS systems, FFmpeg may require RPM Fusion or EPEL repositories.

---

## Build Instructions

Clone the repository and compile:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic to_ulaw.cpp -o to_ulaw
```

This produces a single executable named `to_ulaw`.

---

## Usage

```bash
./to_ulaw <input_audio_file> [output_file] [--wav]
```

### Arguments

| Argument           | Description                                                    |
| ------------------ | -------------------------------------------------------------- |
| `input_audio_file` | Input MP3, WAV, or any FFmpeg-supported format                 |
| `output_file`      | Optional output filename                                       |
| `--wav`            | Output a WAV container with μ-law codec instead of raw `.ulaw` |

---

## Examples

### Convert MP3 to raw μ-law (recommended for Asterisk)

```bash
./to_ulaw greeting.mp3 greeting.ulaw
```

### Auto-generate output filename

```bash
./to_ulaw greeting.wav
# Output: greeting.ulaw
```

### Output μ-law WAV container

```bash
./to_ulaw greeting.mp3 greeting.wav --wav
```

---

## Output Formats

### Raw `.ulaw`

* Headerless G.711 μ-law data
* 8 kHz, mono
* Commonly used by Asterisk and FreePBX for direct playback

### WAV (μ-law)

* Standard WAV container
* Codec: `pcm_mulaw`
* Useful for compatibility with audio tools that require headers

---

## Telephony Compatibility

This tool enforces audio settings compatible with most telephony platforms:

* Sample rate: **8000 Hz**
* Channels: **1 (mono)**
* Encoding: **G.711 μ-law**

It is suitable for:

* IVR prompts
* Voicemail greetings
* Call playback files
* PBX and VoIP systems

---

## Why FFmpeg?

Implementing MP3 decoding, resampling, and μ-law encoding directly in C++ is complex and error-prone. FFmpeg provides a battle-tested audio pipeline that ensures:

* Accurate decoding
* Proper resampling
* Correct μ-law encoding
* Broad format support

This tool intentionally delegates audio processing to FFmpeg while maintaining a minimal, transparent C++ interface.

---

## License

MIT License

---

## Disclaimer

This project assumes FFmpeg is available on the system and accessible via the system PATH. Audio format support depends on the FFmpeg build and enabled codecs.

---

