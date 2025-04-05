#ifndef __MIDI_EXTRACT_H__
#define __MIDI_EXTRACT_H__
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

// 函数声明
void initializePitchToNote();                                                    // 初始化音高到音符的映射
bool isValidNoteEvent(const smf::MidiEvent &midiEvent);                          // 判断音符事件是否有效
void extractNotes(const std::string &inputFilePath, const std::string &outputFilePath); // 从midi文件中提取音符

#endif