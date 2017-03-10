#ifndef DHT22PROTOCOL_SIMULATION_DATA_GENERATOR
#define DHT22PROTOCOL_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class DHT22ProtocolAnalyzerSettings;

class DHT22ProtocolSimulationDataGenerator
{
public:
	DHT22ProtocolSimulationDataGenerator();
	~DHT22ProtocolSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, DHT22ProtocolAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	DHT22ProtocolAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //DHT22PROTOCOL_SIMULATION_DATA_GENERATOR