#ifndef __MIDI_PLAY_H__
#define __MIDI_PLAY_H__
#include "rtmidi/RtMidi.h"
#include "midifile/include/MidiFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <map>
#include <string.h>
#include <windows.h>

// 结构体定义
struct NoteEvent
{
    double time;
    std::string note;
};

// 函数声明
void initializeNoteToPitch();                                                    // 初始化音符到音高的映射
std::vector<NoteEvent> readNotesFromFile(const std::string &filePath);           // 从文件中读取音符
void playNotes(const std::vector<NoteEvent> &notes);                             // 播放音符
void playMidiFile(const std::string &filePath);                                  // 播放midi文件
void test_play_notes(int argc, char **argv);                                     // 测试播放音符
void test_play_midi_file(int argc, char **argv);                                 // 测试播放midi文件

#endif