import("stdfaust.lib");

chosenSoni = hslider("Chosen Sonification",0,0,8,1);
x = hslider("Distance",1,0,1,0.001) : si.smoo;

S1_Pitch = os.osc(S1_freq);
S1_freq = S1_fmin * pow(2,x*S1_nOct);
S1_fmin = 300;
S1_nOct = 3.5;

S2_Loudness = S2_Amp * os.osc(1000);
S2_Amax = -5;
S2_Amin = -40;
S2_Amp = (ba.db2linear(S2_Amax) - ba.db2linear(S2_Amin)) * x + ba.db2linear(S2_Amin);

S3_Tempo = os.osc(1000) * en.ar(0.001,0.05,S3_PulseTrain);
S3_PulseTrain = ba.pulse(S3_SampleInterval);
S3_TempoMax = 20*60; 		S3_TempoMin = 2*60;
S3_IntervalRange = ba.tempo(S3_TempoMin) - ba.tempo(S3_TempoMax);
S3_SampleInterval = ba.tempo(S3_TempoMax) + x * (S3_IntervalRange);

S4_Brightness = no.noise : fi.resonlp(S4_fc,0.7,1);
S4_fc = S4_fmin * pow(2,x*S4_nOct);
S4_fmin = 300;
S4_nOct = 3.5;

S5_FluctStrength = 0.5 * os.osc(1000) + 0.5 * os.osc(1000 + 10*x);

S6_Synchronicity = (S6_OriginalTrain + S6_Delayed) : en.ar(0.001,0.05) : *(os.osc(1000));
S6_PulseFreq = 3;  S6_PeriodSamples = ba.sec2samp(1/S6_PulseFreq);
S6_MaxSampleDelay = 0.25 * S6_PeriodSamples;
S6_CurrentDelay = S6_MaxSampleDelay * x; 
S6_OriginalTrain = ba.pulse(S6_PeriodSamples);
S6_Delayed = S6_OriginalTrain : de.fdelay(40000,S6_CurrentDelay);

S7_Inharmonicity = os.osc(S7_F) + S7_addedPartials;
S7_addedPartials = par(i,7,S7_InharmExp(i)) :> _;
S7_InharmExp(i) = os.osc(S7_F*i*sqrt(1+ x/100*i*i));
S7_F = 200;

S8_MBFM = par(i,7,os.triangle(S8_freqExp(i))) :> _;
S8_freqExp(i) = i*S8_F + S8_I * os.osc(10*x);
S8_I = 30;
S8_F = 100;

S9_MSB = par(k,S9_N,S9_innerLoop(k)) :> _;
S9_innerLoop(k) = par(m,S9_M,S9_innerExp(k,m));
S9_innerExp(k,m) = os.osc(S9_F * k *(1 + m*(S9_alpha(x) - 1)));
S9_alpha(dist) = 1 + 0.06*(dist);
S9_F = 200; S9_N = 5; S9_M = 5;


process = S1_Pitch,S2_Loudness,S3_Tempo,S4_Brightness,S5_FluctStrength,S6_Synchronicity,S7_Inharmonicity,S8_MBFM,S9_MSB : ba.selectn(9,chosenSoni) <: _,_;
			