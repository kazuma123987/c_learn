#include "midi_play.h"
std::map<std::string, int> noteToPitch; // 音符到音高的映射
void initializeNoteToPitch()
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
        noteToPitch[prefix + notes[i % 7]] = i;
    }
}

std::vector<NoteEvent> readNotesFromFile(const std::string &filePath)
{
    std::vector<NoteEvent> notes;
    std::ifstream inFile(filePath);
    if (!inFile.is_open())
    {
        std::cerr << "Failed to open input file: " << filePath << std::endl;
        return notes;
    }

    std::string line;
    while (std::getline(inFile, line))
    {
        std::istringstream iss(line);
        std::string timeStr, noteStr;
        double time;
        std::string note;
        if (iss >> timeStr >> time >> noteStr >> note)
        {
            notes.push_back({time, note});
        }
    }

    inFile.close();
    return notes;
}

void playNotes(const std::vector<NoteEvent> &notes)
{
    RtMidiOut midiOut;
    if (midiOut.getPortCount() == 0)
    {
        std::cerr << "No MIDI output ports available!" << std::endl;
        return;
    }
    midiOut.openPort(0);

    auto startTime = std::chrono::steady_clock::now();
    for (const auto &noteEvent : notes)
    {
        auto waitTime = startTime + std::chrono::duration<double>(noteEvent.time);
        while (std::chrono::steady_clock::now() < waitTime)
        {
            // 忙等待
        }

        std::vector<unsigned char> message;
        int pitch = noteToPitch[noteEvent.note];
        message.push_back(0x90); // Note On
        message.push_back(pitch);
        message.push_back(127); // Velocity
        midiOut.sendMessage(&message);

        // Note Off after a short duration
        message[0] = 0x80; // Note Off
        midiOut.sendMessage(&message);
    }
}

void playMidiFile(const std::string &filePath)
{
    smf::MidiFile midiFile;
    midiFile.read(filePath);
    if (!midiFile.status())
    {
        std::cerr << "Error reading MIDI file: " << filePath << std::endl;
        return;
    }
    midiFile.doTimeAnalysis();
    midiFile.linkNotePairs();
    midiFile.joinTracks();

    RtMidiOut midiOut;
    if (midiOut.getPortCount() == 0)
    {
        std::cerr << "No MIDI output ports available!" << std::endl;
        return;
    }
    midiOut.openPort(0);

    std::vector<unsigned char> message;
    auto startTime = std::chrono::steady_clock::now();
    for (int i = 0; i < midiFile[0].size(); ++i)
    {
        smf::MidiEvent &midiEvent = midiFile[0][i];

        // 忙等待处理
        auto waitTime = startTime + std::chrono::duration<double>(midiEvent.seconds);
        while (std::chrono::steady_clock::now() < waitTime)
        {
            __asm("nop");
        }
        message.clear();
        for (int j = 0; j < midiEvent.size(); ++j)
        {
            message.push_back(midiEvent[j]);
        }
        midiOut.sendMessage(&message);
    }
}

void test_play_notes(int argc, char **argv)
{
    initializeNoteToPitch();
    std::cout << "Reading notes from file..." << std::endl;
    std::vector<NoteEvent> notes = readNotesFromFile("./notes.txt");
    std::cout << "Playing notes..." << std::endl;
    playNotes(notes);
    std::cout << "Playback finished." << std::endl;
}

void test_play_midi_file(int argc, char **argv)
{
    std::cout << "Playing MIDI file..." << std::endl;
    playMidiFile(argv[1]);
    std::cout << "Playback finished." << std::endl;
}