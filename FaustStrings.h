#pragma once
#include <string>

class FaustStrings
{
public:

	FaustStrings()
	{
		initializeMusicInfoNames();
	};

	~FaustStrings()
	{

	};

	std::string baseName = "/RSMC_noSideChain/";

	std::string MasterClock = "Master_Clock";

	// Music Info

	void initializeMusicInfoNames()
	{
		ActiveScale = baseName + ActiveScale;
		Tempo = baseName + Tempo;
		Tonic = baseName + Tonic;
		CurrentStyle = baseName + CurrentStyle;
		KickV = baseName + KickV;
		SnareV = baseName + SnareV;
		hhV = baseName + hhV;
		crashV = baseName + crashV;
		CV = baseName + CV;
		CT = baseName + CT;
		CF = baseName + CF;
		CSV = baseName + CSV;
		CSF = baseName + CSF;
		CS_ArpMode = baseName + CS_ArpMode;
		RV = baseName + RV;
		RF = baseName + RF;
		RO = baseName + RO;
		MV = baseName + MV;
		MF = baseName + MF;
		MO = baseName + MO;
	}

	std::string ActiveScale = "Mapping_Tabs/Music_Info/Active_Scale";

	std::string Tempo = "Mapping_Tabs/Music_Info/Tempo";

	std::string Tonic = "Mapping_Tabs/Music_Info/Tonic";

	std::string CurrentStyle = "Mapping_Tabs/Music_Info/Current_Style";

	std::string KickV = "Mapping_Tabs/Music_Info/Drums/Kick_Velocity";
	std::string SnareV = "Mapping_Tabs/Music_Info/Drums/Snare_Velocity";
	std::string hhV = "Mapping_Tabs/Music_Info/Drums/HH_Velocity";
	std::string crashV = "Mapping_Tabs/Music_Info/Drums/Crash_Velocity";

	std::string CV = "Mapping_Tabs/Music_Info/Chords/CV_Current";
	std::string CF = "Mapping_Tabs/Music_Info/Chords/CD_Current";
	std::string CT = "Mapping_Tabs/Music_Info/Chords/CT_Current";

	std::string CSV = "Mapping_Tabs/Music_Info/Chords/CSV_Current";
	std::string CSF = "Mapping_Tabs/Music_Info/Chords/CSD_Current";
	std::string CS_ArpMode = "Mapping_Tabs/Music_Info/Chords/Arp_Mode";

	std::string RV = "Mapping_Tabs/Music_Info/Melody/RV_Current";
	std::string RF = "Mapping_Tabs/Music_Info/Melody/RF_Current";
	std::string RO = "Mapping_Tabs/Music_Info/Melody/RF_Octave";

	std::string MV = "Mapping_Tabs/Music_Info/Melody/MV_Current";
	std::string MF = "Mapping_Tabs/Music_Info/Melody/MF_Current";
	std::string MO = "Mapping_Tabs/Music_Info/Melody/MF_Octave";

	// Master Volume and Track Mute
	std::string MasterVol = "Mapping_Tabs/Mixer_And_Master/Master_Gain/Master_Gain";

	std::string trackGain_Base = "Mapping_Tabs/Mixer_And_Master/Track_Gain/";
	std::string trackGains[8] =
	{
		"Track_1","Track_2","Track_3","Track_4","Track_5","Track_6","Track_7","Track_8"
	};

	std::string getTrackGainString(int trackIndex)
	{
		return baseName + trackGain_Base + trackGains[trackIndex];
	}

	std::string getTrackMuteString(int trackIndex)
	{
		return baseName + trackMutes_Base + trackMutes[trackIndex];
	}

	std::string trackMutes_Base = "Mapping_Tabs/Mixer_And_Master/Track_Mute/";
	std::string trackMutes[8] = { "1","2","3","4","5","6","7","8" };

	std::string trackIDs[8] 
	= { "Track_1","Track_2","Track_3","Track_4","Track_5","Track_6","Track_7","Track_8" };

	//EQ Setters
	std::string EQ_base = "Mapping_Tabs/EQ_Section/";
	std::string EQ_filterIDs[4] = { "1_HPF/", "2_Parametric_1/", "3_Parametric_2/", "4_LPF/" };
	std::string EQ_filterParamIDs[3] = { "Freq", "Gain", "Q" };
	std::string FetchEQ_String(int trackID, int filterID, int paramID)
	{
		return baseName + EQ_base + trackIDs[trackID] + EQ_filterIDs[filterID] + EQ_filterParamIDs[paramID];
	};

	//Comp Setters
	std::string Comp_base = "Mapping_Tabs/Comp_Section/";
	std::string Comp_ParamIDs[4] = { "Threshold","Ratio","Attack","Release" };
	std::string FetchComp_String(int trackID, int paramID)
	{
		return baseName + Comp_base + trackIDs[trackID] + Comp_ParamIDs[paramID];
	};

	//Sonification
	std::string SonificationBase
		= "Mapping_Tabs/Sonification_Types/";

	std::string SonificationNames[10] = 
	{
		"Basic_Strategies/x_B1_MelBaseFreq",
		"Basic_Strategies/x_B2_MelLoudness",
				"Basic_Strategies/x_B4_MelBrightness", 
		//		"Basic_Strategies/x_B5_PercRelTime",
		"Reference_Strategies/x_R1_InstSynch",
		"Reference_Strategies/x_R2_Inharmonicity",
		"Reference_Strategies/x_R3_Tremolo",
		"Reference_Strategies/x_R4_Distortion",
		"RefZoom_Strategies/x_RZ1_MultiPhaser",
		"RefZoom_Strategies/x_RZ1_MultiWah",
		"Traditional_Strategies/x_Traditional"
	};

	std::string SonificationTypeToggle = baseName + "Mapping_Tabs/Mixer_And_Master/Sonification_Type/Traditional";
	std::string MasterGainString = baseName + "Mapping_Tabs/Mixer_And_Master/Master_Gain/Master_Gain";
	std::string TraditionalSonificationChoice = baseName + "Mapping_Tabs/Sonification_Types/Traditional_Strategies/Choice_Traditional";

	std::string getSonificationStringAddress(int sonificationIndex)
	{
		return baseName + SonificationBase + SonificationNames[sonificationIndex];
	};

};