#ifndef DHT22PROTOCOL_ANALYZER_SETTINGS
#define DHT22PROTOCOL_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

enum DHTxTYPE
{
	DHT11,
	DHT22
};

class DHT22ProtocolAnalyzerSettings : public AnalyzerSettings
{
public:
	DHT22ProtocolAnalyzerSettings();
	virtual ~DHT22ProtocolAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;
	U32 mPulseWidthZero_us;
	U32 mPulseWidthOne_us;
	DHTxTYPE mDHTxType;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mLongPulseInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mShortPulseInterface;
	std::auto_ptr< AnalyzerSettingInterfaceNumberList >	mDHTxTypeInterface;
	
};

#endif //DHT22PROTOCOL_ANALYZER_SETTINGS
