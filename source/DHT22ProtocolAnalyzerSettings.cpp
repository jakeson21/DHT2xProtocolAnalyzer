#include "DHT22ProtocolAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


DHT22ProtocolAnalyzerSettings::DHT22ProtocolAnalyzerSettings()
:	mInputChannel( UNDEFINED_CHANNEL ),
	mPulseWidthOne_us( 70 ),
	mPulseWidthZero_us( 25 ),
	mDHTxType(DHTxTYPE::DHT11)
{
	mInputChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelInterface->SetTitleAndTooltip( "Data Line", "DHT2x 1-Wire Data" );
	mInputChannelInterface->SetChannel( mInputChannel );

	mDHTxTypeInterface.reset(new AnalyzerSettingInterfaceNumberList());
	mDHTxTypeInterface->SetTitleAndTooltip("Sensor Type", "");
	mDHTxTypeInterface->AddNumber(DHTxTYPE::DHT11,"DHT11","Use DHT11 decoding for Temperature and humidity");
	mDHTxTypeInterface->AddNumber(DHTxTYPE::DHT22,"DHT22","Use DHT22 decoding for Temperature and humidity");
	mDHTxTypeInterface->SetNumber(mDHTxType);

	mLongPulseInterface.reset(new AnalyzerSettingInterfaceInteger());
	mLongPulseInterface->SetTitleAndTooltip("One Valued Pulse Duration (microseconds)", "Specify the duration of a One pulse (µs).");
	mLongPulseInterface->SetMax(1000000);
	mLongPulseInterface->SetMin(1);
	mLongPulseInterface->SetInteger(mPulseWidthOne_us);

	mShortPulseInterface.reset(new AnalyzerSettingInterfaceInteger());
	mShortPulseInterface->SetTitleAndTooltip("Zero Valued Pulse Duration (microseconds)", "Specify the duration of a Zero pulse (µs).");
	mShortPulseInterface->SetMax(1000000);
	mShortPulseInterface->SetMin(1);
	mShortPulseInterface->SetInteger(mPulseWidthZero_us);

	AddInterface(mInputChannelInterface.get());
	AddInterface(mLongPulseInterface.get());
	AddInterface(mShortPulseInterface.get());
	AddInterface(mDHTxTypeInterface.get());

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mInputChannel, "Data", false );
}

DHT22ProtocolAnalyzerSettings::~DHT22ProtocolAnalyzerSettings()
{
}

bool DHT22ProtocolAnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannel = mInputChannelInterface->GetChannel();
	mPulseWidthOne_us = mLongPulseInterface->GetInteger();
	mPulseWidthZero_us = mShortPulseInterface->GetInteger();
	mDHTxType = (DHTxTYPE) mDHTxTypeInterface->GetNumber();

	ClearChannels();
	AddChannel( mInputChannel, "Data", true );

	return true;
}

void DHT22ProtocolAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelInterface->SetChannel(mInputChannel);
	mLongPulseInterface->SetInteger(mPulseWidthOne_us);
	mShortPulseInterface->SetInteger(mPulseWidthZero_us);
	mDHTxTypeInterface->SetNumber(mDHTxType);
}

void DHT22ProtocolAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mInputChannel;
	text_archive >> mPulseWidthOne_us;
	text_archive >> mPulseWidthZero_us;
	text_archive >> *((U32*)(&mDHTxType));

	ClearChannels();
	AddChannel( mInputChannel, "Data", true );

	UpdateInterfacesFromSettings();
}

const char* DHT22ProtocolAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannel;
	text_archive << mPulseWidthOne_us;
	text_archive << mPulseWidthZero_us;
	text_archive << mDHTxType;

	return SetReturnString( text_archive.GetString() );
}
