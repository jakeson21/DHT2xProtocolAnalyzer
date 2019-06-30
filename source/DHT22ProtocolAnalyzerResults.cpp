#include "DHT22ProtocolAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "DHT22ProtocolAnalyzer.h"
#include "DHT22ProtocolAnalyzerSettings.h"
#include <iostream>
#include <fstream>
#include <sstream>

DHT22ProtocolAnalyzerResults::DHT22ProtocolAnalyzerResults(DHT22ProtocolAnalyzer* analyzer, DHT22ProtocolAnalyzerSettings* settings)
    : AnalyzerResults(),
    mSettings(settings),
    mAnalyzer(analyzer)
{}

DHT22ProtocolAnalyzerResults::~DHT22ProtocolAnalyzerResults()
{}

void DHT22ProtocolAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base)
{
    ClearResultStrings();
    Frame frame = GetFrame(frame_index);

    std::string name_short;
    switch (frame.mType)
    {
    case DHT2xFrameType::RELATIVEHUMIDITY:
        name_short = "RH";
        break;
    case DHT2xFrameType::TEMPERATURE:
        name_short = "Temp";
        break;
    case DHT2xFrameType::CHECKSUM:
        name_short = "CS";
        break;
    }
    AddResultString(name_short.c_str());
    AddResultString(GetMedDecodedString(frame_index, display_base).c_str());
    AddResultString(GetLongDecodedString(frame_index, display_base).c_str());
}

std::string DHT22ProtocolAnalyzerResults::GetMedDecodedString(U64 frame_index, DisplayBase display_base)
{
    Frame frame = GetFrame(frame_index);

    char raw_str[128];
    std::stringstream output;
    AnalyzerHelpers::GetNumberString(frame.mData1, display_base, frame.mData2, raw_str, 128);

    switch (frame.mType)
    {
    case DHT2xFrameType::RELATIVEHUMIDITY:
        output << "RelH ";
        break;
    case DHT2xFrameType::TEMPERATURE:
        output << "Temp ";
        break;
    case DHT2xFrameType::CHECKSUM:
        output << "CSum ";
        break;
    }

    output << raw_str;
    return output.str();
}

std::string DHT22ProtocolAnalyzerResults::GetLongDecodedString(U64 frame_index, DisplayBase display_base)
{
    Frame frame = GetFrame(frame_index);

    char raw_str[128];
    std::stringstream output;
    std::stringstream value;
    AnalyzerHelpers::GetNumberString(frame.mData1, display_base, frame.mData2, raw_str, 128);

    switch (frame.mType)
    {
    case DHT2xFrameType::RELATIVEHUMIDITY:
        output << "Relative Humidity ";

        if(mSettings->mDHTxType == DHTxTYPE::DHT11)
        {
            value << " " << ((frame.mData1 >> 8) & (0xFF)) << "." << ((frame.mData1 >> 0) & (0xFF)) << " %";
        }
        else if (mSettings->mDHTxType == DHTxTYPE::DHT22)
        {
            value << " " << frame.mData1 / 10.0 << " %";
        }
        else
        {

        }

        break;
    case DHT2xFrameType::TEMPERATURE:
        output << "Temperature ";

        if(mSettings->mDHTxType == DHTxTYPE::DHT11)
        {
            value << " " << ((frame.mData1 >> 8) & (0xFF))  << "." << ((frame.mData1 >> 0) & (0xFF)) << " C ";
        }
        else if (mSettings->mDHTxType == DHTxTYPE::DHT22)
        {
            value << " " << frame.mData1 / 10.0 << " C ";
        }
        else
        {

        }

        break;
    case DHT2xFrameType::CHECKSUM:
        output << "Checksum ";
        break;
    }

    output << raw_str << value.str();
    return output.str();
}

S16 DHT22ProtocolAnalyzerResults::DecodeTemperature(U64 inTemp)
{
    U16 sign_mask = 0x8000;
    U16 temp_mask = 0x7FFF;
    U16 adjustedTemp = inTemp;
    // Check for negative flag
    adjustedTemp = (sign_mask & inTemp > 0) ? -(inTemp & temp_mask) : inTemp;
    return adjustedTemp;
}

void DHT22ProtocolAnalyzerResults::GenerateExportFile(const char* file, DisplayBase display_base, U32 export_type_user_id)
{
    std::ofstream file_stream(file, std::ios::out);

    U64 trigger_sample = mAnalyzer->GetTriggerSample();
    U32 sample_rate = mAnalyzer->GetSampleRate();

    file_stream << "Time[s], Value" << std::endl;

    U64 num_frames = GetNumFrames();
    for (U32 i = 0; i < num_frames; i++)
    {
        Frame frame = GetFrame(i);

        char time_str[128];
        AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128);

        char number_str[128];
        AnalyzerHelpers::GetNumberString(frame.mData1, display_base, frame.mData2, number_str, 128);

        file_stream << time_str << ", " << number_str << std::endl;

        if (UpdateExportProgressAndCheckForCancel(i, num_frames) == true)
        {
            file_stream.close();
            return;
        }
    }

    file_stream.close();
}

void DHT22ProtocolAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
{
    ClearTabularText();
    AddTabularText(GetMedDecodedString(frame_index, display_base).c_str());
}

void DHT22ProtocolAnalyzerResults::GeneratePacketTabularText(U64 packet_id, DisplayBase display_base)
{
    ClearTabularText();
    AddTabularText("not supported");
}

void DHT22ProtocolAnalyzerResults::GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base)
{
    ClearTabularText();
    AddTabularText("not supported");
}