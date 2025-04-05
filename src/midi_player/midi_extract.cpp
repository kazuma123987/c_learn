#include "midi_extract.h"

// 定义音阶映射
std::map<int, std::string> pitchToNote;
void initializePitchToNote()
{
    std::vector<std::string> notes = {"1", "2", "3", "4", "5", "6", "7"};
    for (int i = 0; i < 128; ++i)
    {
        int octave = (i / 12) - 1;
        std::string prefix;
        if (octave < 4)
        {
            prefix = "A"; // 低音
        }
        else if (octave == 4)
        {
            prefix = "B"; // 中音
        }
        else
        {
            prefix = "C"; // 高音
        }
        pitchToNote[i] = prefix + notes[i % 7];
    }
}

bool isValidNoteEvent(const smf::MidiEvent &midiEvent)
{
    // 过滤掉无效的音符，例如音高范围之外的音符或持续时间过短的音符
    int pitch = midiEvent.getKeyNumber();
    double duration = midiEvent.getDurationInSeconds();
    return (pitch >= 21 && pitch <= 108) && (duration > 0.1); // 例如，音高在21到108之间，持续时间大于0.1秒
}

void extractNotes(const std::string &inputFilePath, const std::string &outputFilePath)
{
    smf::MidiFile midiFile;
    midiFile.read(inputFilePath);
    midiFile.doTimeAnalysis();
    midiFile.linkNotePairs();
    midiFile.joinTracks();

    std::ofstream outFile(outputFilePath);
    if (!outFile.is_open())
    {
        std::cerr << "Failed to open output file: " << outputFilePath << std::endl;
        return;
    }

    for (int i = 0; i < midiFile[0].size(); ++i)
    {
        smf::MidiEvent &midiEvent = midiFile[0][i];
        if (midiEvent.isNoteOn() && isValidNoteEvent(midiEvent))
        {
            int pitch = midiEvent.getKeyNumber();
            double time = midiEvent.seconds;
            std::string note = pitchToNote[pitch]; // 将音高转换为自定义音阶表示
            outFile << "Time: " << time << " Note: " << note << std::endl;
        }
    }

    std::cout << "Notes extracted to: " << outputFilePath << std::endl;

    outFile.close();
}

int test_extract(int argc, char **argv)
{
    initializePitchToNote();
    std::cout << "Extracting notes from MIDI file..." << std::endl;
    extractNotes(argv[1], "./notes.txt");
    std::cout << "Extraction finished." << std::endl;
    return 0;
}