#pragma once
class MixerSettings
{
public:
	MixerSettings() {};
	~MixerSettings() {};

	int styleSwing_Timing[4] = {5, 10, 10, 10};
	float styleSwing_Velocity_Percent[4] = { 10.0, 10.0, 10.0, 10.0 };
	
	float trackGains[8][4] =
	{
		{1,		1,	-24,	-6},
		{6,		6,	-26,	-6},
		{-7,	-7,	-6},
		{-10,	2,		-9,		-29},
		{-4,	-4,		-9,		-29},
		{0,	-3,		-9,		-29},
		{-10,	-4,		-9,		-29},
		{-18,	-18,		-9,		-29}
	};

	float eqSettings_S1[8][12] =
	{
		{60, 0, 3.0,	300, -2, 0.7,		4000, 2.6, 0.7,		12000, 0, 0.7},
		{100, 0, 0.7,	550, -4.5, 0.7,		5356, 4.2, 0.7,		16000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{150, 0, 0.7,	200, 5, 0.7,		2000, -9, 0.3,		1000, 0, 0.7},
		{80, 0, 0.7,	110, 3, 0.7,		800, 5, 0.7,		1500, 3, 3},
		{150, 0, 0.7,	200, -3, 0.7,		4000, -2, 0.7,		12000, 0, 0.7},
		{150, 0, 0.7,	250, -4, 0.7,		3500, 4, 0.4,		10000, 0, 0.7},
		{9000, 0, 1.7,	800, -5, 0.7,		9000, 5, 0.7,		20000, 0, 0.7}
	};

	float eqSettings_S2[8][12] =
	{
		{60, 0, 3.0,	300, -2, 0.7,		4000, 2.6, 0.7,		12000, 0, 0.7},
		{100, 0, 0.7,	1000, 4.5, 0.7,		5356, 4.2, 0.7,		16000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{150, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.3,		5000, 0, 0.7},
		{80, 0, 0.7,	110, 3, 0.7,		800, 5, 0.7,		1500, 3, 3},
		{150, 0, 0.7,	200, -3, 0.7,		4000, -2, 0.7,		12000, 0, 0.7},
		{150, 0, 0.7,	250, -4, 0.7,		3500, 4, 0.4,		10000, 0, 0.7},
		{9000, 0, 1.7,	800, -5, 0.7,		9000, 5, 0.7,		20000, 0, 0.7}
	};

	float eqSettings_S3[8][12] =
	{
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7}
	};

	float eqSettings_S4[8][12] =
	{
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7},
		{20, 0, 0.7,	200, 0, 0.7,		2000, 0, 0.7,		20000, 0, 0.7}
	};

	float compSettings_S1[8][4] =
	{
		{-11, 4.5, 0.05, 0.2},
		{-18, 5, 0.05, 0.2},
		{-6, 4, 40, 0.1},
		{0, 1, 10, 100},
		{0, 1, 10, 100},
		{0, 1, 10, 100},
		{0, 1, 10, 100},
		{0, 1, 10, 100}
	};

	float compSettings_S2[8][4] =
	{
		{-11, 4.5, 0.05, 0.2},
		{-18, 5, 0.05, 0.2},
		{-6, 4, 40, 0.1},
		{0, 1, 10, 100},
		{0, 1, 10, 100},
		{0, 1, 10, 100},
		{0, 1, 10, 100},
		{0, 1, 10, 100}
	};

	float compSettings_S3[8][4] =
	{
		{0, 4, 10, 100},
		{0, 4, 10, 100},
		{0, 4, 10, 100},
		{0, 4, 10, 100},
		{0, 4, 10, 100},
		{0, 4, 10, 100},
		{0, 4, 10, 100},
		{0, 4, 10, 100}
	};

	float compSettings_S4[8][4] =
	{
		{0, 4, 10, 100},
		{0, 4, 10, 100},
		{0, 4, 10, 100},
		{0, 4, 10, 100},
		{0, 4, 10, 100},
		{0, 4, 10, 100},
		{0, 4, 10, 100},
		{0, 4, 10, 100}
	};

	float fetchEQValue(int styleID, int trackID, int filterID, int paramID)
	{
		float out = 0;
		switch (styleID)
		{
		case 0:
			out = eqSettings_S1[trackID][3 * filterID + paramID];
			break;
		case 1:
			out = eqSettings_S2[trackID][3 * filterID + paramID];
			break;
		case 2:
			out = eqSettings_S3[trackID][3 * filterID + paramID];
			break;
		case 3:
			out = eqSettings_S4[trackID][3 * filterID + paramID];
			break;
		}
		return out;
	};

	float fetchCompValue(int styleID, int trackID, int paramID)
	{
		float out = 0;
		switch (styleID)
		{
		case 0:
			out = compSettings_S1[trackID][paramID];
			break;
		case 1:
			out = compSettings_S2[trackID][paramID];
			break;
		case 2:
			out = compSettings_S3[trackID][paramID];
			break;
		case 3:
			out = compSettings_S4[trackID][paramID];
			break;
		}
		return out;
	};
};
