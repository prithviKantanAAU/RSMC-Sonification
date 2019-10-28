// // // // // // // // // // // // // // // // // // // // // // // // // // // // D E F I N I T I O N S // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
import("stdfaust.lib"); 				numStyles = 4;							numScales = 5;							freqOffset_LR = 0.5;
kickNoise = multiNoiseSelector(0); 		snareNoise = multiNoiseSelector(1); 	hhNoise = multiNoiseSelector(2);	melodyNoise = multiNoiseSelector(3);	crashNoise = multiNoiseSelector(4);
major_scale = waveform{-1,0,2,4,5,7,9,11,12,14,16,17};  							majorScaleTable(i) = major_scale,i : rdtable;
minor_scale = waveform{-2,0,2,3,5,7,8,10,12,14,15,17,19,20,22,24};					minorScaleTable(i) = minor_scale,i : rdtable;
phrygian_scale = waveform{-2,0,1,3,5,7,8,10,12,13,15,17};							phrygianScaleTable(i) = phrygian_scale,i : rdtable;
arabic_scale = waveform{-2,0,1,4,5,7,8,10,12,13,16,17,19,20,22,24};					arabicScaleTable(i) = arabic_scale,i : rdtable;
pentatonic_scale = waveform{-2,0,2,3,5,6,7,10,12,14,15,17,19,22,24,26};				pentatonicScaleTable(i) = pentatonic_scale,i : rdtable;
// Order = Major	Minor	Maj7	Min7	Dominant	Maj9	Min9	Power
chord_Seconds = 4,3,4,3,4,4,3,7; 	chord_Thirds = 7,7,7,7,7,7,7,12; 	chord_Fourths = 12,12,11,10,10,14,14,19;

hh_S1_FreqList = 14903, 20715, 72502; 		hh_S1_DepthList = 8516,8516; 		hh_S1_Carrier = 14903; 		hh_S1_Release = 0.15;
crash_S1_FreqList = 2100, 6500, 9543; 		crash_S1_DepthList = 8516,8516; 	crash_S1_Carrier = 2000;
// // // // // // // // // // // // // // // // // // // // // // // // // // // // U S E R  I N T E R F A C E // // // // // // // // // // // // // // // // // // // // // // // // // // //
clockTrigger = button("Master Clock"); clockDiff = clockTrigger - clockTrigger'; masterClock = (clockDiff  > 0);
musicInfoTab(x) = tgroup("Mapping_Tabs",hgroup("Music_Info",x));  				melodyInfo(x) = musicInfoTab(vgroup("Melody",x)); 		chordInfo(x) = musicInfoTab(vgroup("Chords",x)); 	drumInfo(x) = musicInfoTab(vgroup("Drums",x));									masterGainTab(x) = tgroup("Mapping_Tabs",hgroup("Mixer_And_Master",x)); 		
trackGainGroup(x) = masterGainTab(vgroup("Track Gain",x)); 						trackMuteGroup(x) = masterGainTab(vgroup("Track Mute",x)); 						
masterGroup(x) = masterGainTab(vgroup("Master Gain",x));						masterEQGroup(x) = masterGainTab(vgroup("Master EQ",x));
masterSoniTypeToggle(x) = masterGainTab(vgroup("Sonification Type",x));
eqTab(x) = tgroup("Mapping_Tabs",vgroup("EQ_Section",x)); 						eqTrackGroup(i,x) = eqTab(hgroup("Track %i",x));
compTab(x) = tgroup("Mapping_Tabs",vgroup("Comp_Section",x)); 					compTrackGroup(i,x) = compTab(hgroup("Track %i",x));
sonificationTab(x) = tgroup("Mapping_Tabs",hgroup("Sonification Types",x));		
basicSoni(x) = sonificationTab(vgroup("Basic_Strategies",x));
referenceSoni(x) = sonificationTab(vgroup("Reference_Strategies",x));
refZoomSoni(x) = sonificationTab(vgroup("RefZoom_Strategies",x));
traditionalSoni(x) = sonificationTab(vgroup("Traditional_Strategies",x));

tonic = musicInfoTab(vslider("Tonic",36,30,42,1)) : limit(20,70);
activeScale = musicInfoTab(vslider("Active Scale",1,0,numScales-1,1)) : limit(0,numScales-1);
tempo = musicInfoTab(vslider("Tempo",120,50,150,0.1)) : limit(60,150);
currentStyle = musicInfoTab(vslider("Current Style",0,0,numStyles - 1,1) : limit(0,numStyles - 1));

mf_present = melodyInfo(hslider("MF Current",1,0,9,1) : int : limit(0,9));
mv_present = melodyInfo(hslider("MV Current",4,1,9,0.1) : int : limit(0,9));
mf_octave = melodyInfo(hslider("MF Octave",0,-2,2,1) : int : limit(0,9));

rf_present = melodyInfo(hslider("RF Current",1,0,9,1) : int : limit(0,9));
rv_present = melodyInfo(hslider("RV Current",4,1,9,0.1) : int : limit(0,9));
rf_octave = melodyInfo(hslider("RF Octave",-1,-2,5,1) : int : limit(0,9));

cd_present = chordInfo(hslider("CD Current",1,0,9,1) : int : limit(0,9));
ct_present = chordInfo(hslider("CT Current",1,0,9,1) : int : limit(0,9));
cv_present = chordInfo(hslider("CV Current",4,0,9,1) : int : limit(0,9));

cs_v_present = chordInfo(hslider("CSV Current",4,0,9,1) : int : limit(0,9));
cs_d_present = chordInfo(hslider("CSD Current",0,3,3,1) : int : limit(0,9));
cs_arpMode = chordInfo(hslider("Arp Mode",0,0,1,1) : int : limit(0,9));

kick_v_present = drumInfo(hslider("Kick Velocity",9,0,9,8) : int : limit(0,9));
snare_v_present = drumInfo(hslider("Snare Velocity",9,0,9,8) : int : limit(0,9));
hh_v_present = drumInfo(hslider("HH Velocity",9,0,9,8) : int : limit(0,9)); hh_v_true = hh_v_present : _%(5) : *(2); hh_Openness = 1 + 2 * (hh_v_present > 5);
crash_v_present = drumInfo(hslider("Crash Velocity",9,0,9,8) : int : limit(0,9));

//Sonification
Soni_X_B1_MelBaseFreq = basicSoni(hslider("x_B1_MelBaseFreq",0,0,1,0.001)) : si.smoo;
Soni_X_B2_MelLoudness = basicSoni(hslider("x_B2_MelLoudness",0,0,1,0.001)) : si.smoo;
// B3 HANDLED OUTSIDE IN JUCE
Soni_X_B4_MelBrightness = basicSoni(hslider("x_B4_MelBrightness",0,0,1,0.001)) : si.smoo;
Soni_X_B5_PercRelTime = basicSoni(hslider("x_B5_PercRelTime",0,0,1,0.001)) : si.smoo;
Soni_X_R1_InstSynch = referenceSoni(hslider("x_R1_InstSynch",0,0,1,0.001)) : si.smoo;
Soni_X_R2_Inharmonicity = referenceSoni(hslider("x_R2_Inharmonicity",0,0,1,0.001)) : si.smoo;
Soni_X_R3_Tremolo = referenceSoni(hslider("x_R3_Tremolo",0,0,1,0.001)) : si.smoo;
Soni_X_R4_Dist = referenceSoni(hslider("x_R4_Distortion",0,0,1,0.001)) : si.smoo;
Soni_X_RZ1_MultiPhaser = refZoomSoni(hslider("x_RZ1_MultiPhaser",0,0,1,0.001)) : si.smoo;
Soni_X_RZ2_MultiWah = refZoomSoni(hslider("x_RZ1_MultiWah",0,0,1,0.001)) : si.smoo;
Soni_X_Trad = traditionalSoni(hslider("x_Traditional",0,0,1,0.001)) : si.smoo;
Soni_Choice_Trad = traditionalSoni(hslider("Choice_Traditional",0,0,8,1)); 

// // // // // // // // // // // // // // // // // // // // // // // // // // // // F U N C T I O N S // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
// 1 - UTILITY
noiseMulti = no.multinoise(8); multiNoiseSelector(n) = noiseMulti : ba.selectn(8,n);
quarterNoteSampleTime = ba.tempo(tempo); beatTime = 60.0/tempo;
limit(lower,higher) = _ : max(lower) : min(higher);
hard_clip(limit) = _ : min(limit) : max(-1*limit) : _;
envAmplitude(velocity) = velocity : ba.sAndH(masterClock * velocity) : si.smooth(ba.tau2pole(0.0001)) : *(0.1);
masterLimiter(ipGaindB) = _ : compLimiter(ipGaindB,10,0,0.001,0.05,0.050);

scaleIntervals(activeNoteDegree) = majorScaleTable(activeNoteDegree), minorScaleTable(activeNoteDegree), phrygianScaleTable(activeNoteDegree), arabicScaleTable(activeNoteDegree), pentatonicScaleTable(activeNoteDegree); 

melodyFundamentalFreq(activeScale,tonic,octave,activeNoteDegree,lowerlim_MIDI,upperlim_MIDI) = ba.midikey2hz(finalMidiValue) : limit(20,5000) : Soni_B1_FreqWarpFactor	with
{ activeInterval = scaleIntervals(activeNoteDegree) : ba.selectn(numScales,activeScale);
  preliminaryMidiValue = tonic + 12*octave + activeInterval; tooLow = lowerlim_MIDI > preliminaryMidiValue; tooHigh = upperlim_MIDI < preliminaryMidiValue;
  octavesUnder = (lowerlim_MIDI - preliminaryMidiValue)*tooLow / 12 : +(1*tooLow) : int;
  octaveOver = (preliminaryMidiValue - upperlim_MIDI)*tooHigh / 12 : +(1*tooHigh) : int;
  finalMidiValue = preliminaryMidiValue + 12*octavesUnder - 12*octaveOver;		};

chordFrequencies(chordDegree,chordType,chordsOctave) = freqList		 with	{
  activeInterval = scaleIntervals(chordDegree) : ba.selectn(numScales,activeScale);
  midiTonic = (tonic + 12*chordsOctave + activeInterval); 
  increment_Second = chord_Seconds : ba.selectn(8,chordType);	increment_Third = chord_Thirds : ba.selectn(8,chordType);	increment_Fourth = chord_Fourths : ba.selectn(8,chordType);
  midiSecond = midiTonic + increment_Second; 				    midiThird = midiTonic + increment_Third; 				    midiFourth = midiTonic + increment_Fourth;
  freq1 = midiTonic : ba.midikey2hz : Soni_B1_FreqWarpFactor;   freq2 = midiSecond : ba.midikey2hz : Soni_B1_FreqWarpFactor; freq3 = midiThird  : ba.midikey2hz : Soni_B1_FreqWarpFactor;
  freq4 = midiFourth : ba.midikey2hz : Soni_B1_FreqWarpFactor; 	freqList = freq1,freq2,freq3,freq4;		};

getPanFunction(panIndex) = panFunction	with	{
  numPanPositions = 3;
  L_multiplier = 1,0.95,0.2 : ba.selectn(numPanPositions,panIndex);
  R_multiplier = 1,0.2,0.95 : ba.selectn(numPanPositions,panIndex);
  panFunction = _ <: _*(L_multiplier), _*(R_multiplier);	};

expEnvelope(attack,release,velocity) = en.are(attack,release,triggerCooked) * envAmp : si.smooth(ba.tau2pole(0.001))
  with {  envAmp = envAmplitude(velocity); triggerCooked = velocity * masterClock; };

chordEnvelope(attack,release,finalVal,velocity) = en.arfe(attack,release,finalVal,triggerCooked) * envAmp : si.smooth(ba.tau2pole(0.001))
  with { envAmp = envAmplitude(velocity); triggerCooked = velocity * masterClock; };

// 2 - CHANNEL STRIP

channelComp(compTrackGroup) = compLimiter(0,ratio,thresh,att,rel,0.050)	 with	{
  ratio = compTrackGroup(hslider("Ratio[style:knob]",1,1,10,0.1));
  thresh = compTrackGroup(hslider("Threshold[style:knob]",0,-96,0,0.1));
  att = compTrackGroup(hslider("Attack[style:knob]",0.1,0.001,0.5,0.001));
  rel = compTrackGroup(hslider("Release[style:knob]",0.1,0.001,0.5,0.001));		};

parametricEQ(eqTrackGroup) = fi.resonhp(fc_HP,Q_HP,1) : fi.peak_eq_cq(gain_P1,f_P1,Q_P1) : fi.peak_eq_cq(gain_P2,f_P2,Q_P2) : fi.resonlp(fc_LP,Q_LP,1)
  with	{ hpGroup(x) = hgroup("1_HPF",x);
  p1Group(x) = hgroup("2_Parametric_1",x);
  p2Group(x) = hgroup("3_Parametric_2",x);
  lpGroup(x) = hgroup("4_LPF",x);
  fc_HP = eqTrackGroup(hpGroup(hslider("Freq[style:knob]",50,50,1000,0.1))) : limit(20,20000);
  Q_HP = eqTrackGroup(hpGroup(hslider("Q[style:knob]",0.7,0.5,10,0.01))) : limit(0.4,5);
  fc_LP = eqTrackGroup(lpGroup(hslider("Freq[style:knob]",20000,50,20000,0.1))) : limit(20,20000);
  Q_LP = eqTrackGroup(lpGroup(hslider("Q[style:knob]",0.7,0.5,10,0.01))) : limit(0.4,5);
  gain_P1 = eqTrackGroup(p1Group(hslider("Gain[style:knob]",0,-24,24,0.01))) : limit(-24,24);
  f_P1 = eqTrackGroup(p1Group(hslider("Freq[style:knob]",500,100,2000,0.01))) : limit(20,20000);
  Q_P1 = eqTrackGroup(p1Group(hslider("Q[style:knob]",0.7,0.5,10,0.01))) : limit(0.4,5);
  gain_P2 = eqTrackGroup(p2Group(hslider("Gain[style:knob]",0,-24,24,0.01))) : limit(-24,24);
  f_P2 = eqTrackGroup(p2Group(hslider("Freq[style:knob]",500,1500,15000,0.01))) : limit(20,20000);
  Q_P2 = eqTrackGroup(p2Group(hslider("Q[style:knob]",0.7,0.5,10,0.01))) : limit(0.4,5);			};

// 3 - Effects
applyStereoFX(index,wet,stereoInput) = outL_final,outR_final	with	{
  leftChannel = stereoInput : select2(0);
  rightChannel = stereoInput : select2(1);
  numFX = 3;
  fx1 = _ : _;
  fx2 = ve.autowah(wet);
  fx3 = pf.flanger_mono(10,5,wet,0,0);
  
  outL_1 = leftChannel : fx1;	outR_1 = rightChannel : fx1;
  outL_2 = leftChannel : fx2;	outR_2 = rightChannel : fx2;
  outL_3 = leftChannel : fx3;	outR_3 = rightChannel : fx3;
  
  outL_final = outL_1,outL_2,outL_3
			   : ba.selectn(numFX,index);
  outR_final = outR_1,outR_2,outR_3
			   : ba.selectn(numFX,index);	};

// Dotted Delay
dotted_delay(feedback,delay,wet,condition) = +~(de.fdelay(100000,del) : filt)*feedback*wet*condition :> _	with	{ filt = fi.bandpass(2,400,1500);	del = 0.75*delay; };

// Compressor-Limiter

compLimiter(inputGain_dB,ratio,thresh,att,rel,kneeAtt) = _*inputGain_lin 
  <: _*(an.amp_follower_ud(att,rel) : ba.linear2db : outminusindb : kneesmooth : ba.db2linear)
with{inputGain_lin = ba.db2linear(inputGain_dB);
	 outminusindb(level) = max(level-thresh,0)*(1/ratio - 1);
	 kneesmooth = si.smooth(ba.tau2pole(kneeAtt));	};

// Sidechain Compressor
sideChainComp(sidechain,inputGain_dB,ratio,thresh,att,rel,kneeAtt) = _*inputGain_lin 
  <: _*(sidechain : an.amp_follower_ud(att,rel) : ba.linear2db : outminusindb : kneesmooth : ba.db2linear)
with{inputGain_lin = ba.db2linear(inputGain_dB);
	 outminusindb(level) = max(level-thresh,0)*(1/ratio - 1);
	 kneesmooth = si.smooth(ba.tau2pole(kneeAtt));	};

//4 - Synthesis
generalizedDrumSynth(noiseSource ,rel_noise, rel_sine, fPeak_sine, gain_noise, gain_sine, gain_dist, cutoff_LP, cutoff_HP, velocity) = output : masterFilter
  with
{
  gain_sine_lin = gain_sine : ba.db2linear; 					  gain_noise_lin = gain_noise : ba.db2linear; 		gain_dist_lin = gain_dist : ba.db2linear;
  envAmp = envAmplitude(velocity);								  trig = velocity * masterClock;
  rel_sine_Sonified = rel_sine * Soni_B5_RelFactor;				  rel_noise_Sonified = rel_noise * Soni_B5_RelFactor;
  env_sine = en.are(0.001,rel_sine_Sonified,trig);				  env_noise = en.are(0.001,rel_noise_Sonified,trig);
  freq_sine = fPeak_sine * env_sine;
  osc_sine = os.osc(freq_sine) * env_sine * gain_sine_lin;		  osc_noise = noiseSource * env_noise * gain_noise_lin;
  onCondition = (envAmp > 0);									  defaultOut = 0;
  cleanBus = (osc_sine + osc_noise) * envAmp;					  dirtyBus = cleanBus : ef.cubicnl(0.5,0) : _ *(gain_dist_lin);
  activeOut = cleanBus + dirtyBus;
  output = defaultOut,activeOut : ba.selectn(2,onCondition);
  masterFilter = fi.resonlp(cutoff_LP,0.7,1) 					  : fi.resonhp(cutoff_HP,0.7,1);
};

fmPercSynth(noiseSource,fmLeveldB,noiseLeveldB,fc_HPF,depthList,freqList,carrierFreq,release,velocity) = output : filter
  with
{
  envAmp = envAmplitude(velocity);
  trig = masterClock * velocity;
  fmLevel = fmLeveldB : ba.db2linear;
  noiseLevel = noiseLeveldB : ba.db2linear;
  filter = fi.resonhp(fc_HPF,0.7,3);
  release_Sonified = release * Soni_B5_RelFactor;
  env = en.are(0.001,release_Sonified,trig) * envAmp;
  fmSignal = sy.fm(freqList,depthList) :> _;
  onCondition = (envAmp > 0);
  defaultOut = 0;
  activeOut = fmSignal * fmLevel + noiseSource * noiseLevel : *(env);
  output = defaultOut,activeOut : ba.selectn(2,onCondition);	};

leadSynth(fundamental,synthFunc,noiseSource,velocity,synthRelease,synthCutoff,addedSineLeveldB,noiseLeveldB,noiseRelease,noiseCutoff,fx1,fx2) = output
with
{
  trigger = masterClock * velocity;
  envAmp = envAmplitude(velocity); onCondition = (envAmp > 0);
  addedSineLevel = addedSineLeveldB : ba.db2linear;
  noiseLevel = noiseLeveldB : ba.db2linear;
  
  //Cooking
  LFO = 0.5*(1 + 0.7*os.osc(tempo/15) + 0.3*no.noise);
  freqFactor = envAmp * envAmp; timeFactor = envAmp;
  synthCutoffCooked = synthCutoff * (0.01 + 0.99*(freqFactor)) : si.smoo : limit(20,20000);
  releaseCooked = synthRelease * (0.35 + 0.65*(timeFactor)) : limit(0.02,50);
  fundamentalCooked = fundamental * (1 + 0.015 * freqFactor * LFO) : limit(20,5000);
  
  //Synthesis
  melSynth = ((os.osc(fundamentalCooked) * addedSineLevel + synthFunc(fundamentalCooked)) * en.ar(0.01,synthRelease,trigger)) : fi.resonlp(synthCutoffCooked,0.7,1);
  noiseSynth = (noiseSource * en.ar(0.001,noiseRelease,trigger) * noiseLevel) : fi.resonlp(noiseCutoff,3,1);
  defaultOut = 0;
  activeOut = melSynth + noiseSynth;
  combiSynth = defaultOut,activeOut : ba.selectn(2,onCondition);
  output = combiSynth : fx1 : fx2;	};

fmSynth(fundamental,numMod,freqFactor,release,depth,velocity) = fmSynth * env 	with
{ trigger = masterClock * velocity;
  depthCooked = depth * velocity;
  freqList = par(i,numMod,fundamental * pow(freqFactor,i));
  depthList = par(i,numMod-1,depthCooked);
  envAmp = envAmplitude(velocity);
  env = en.ar(0.005,release, trigger) * envAmp;
  fmSynth = sy.fm(freqList,depthList);	};

fullChordSynth(freqList,synthFunc) = stereoChordOut with
{ freqSelector(n) = freqList : ba.selectn(4,n-1);
  freq1Bus = synthFunc(freqSelector(1)),synthFunc(freqSelector(1) + freqOffset_LR);
  freq2Bus = synthFunc(freqSelector(2)),synthFunc(freqSelector(2) + freqOffset_LR);
  freq3Bus = synthFunc(freqSelector(3)),synthFunc(freqSelector(3) + freqOffset_LR);
  freq4Bus = synthFunc(freqSelector(4)),synthFunc(freqSelector(4) + freqOffset_LR);
  stereoChordOut = freq1Bus,freq2Bus,freq3Bus,freq4Bus :> _,_;	};

arpChordSynth(freqList,synthFunc) = stereoNoteOut with
{ freqSelector(n) = freqList : ba.selectn(4,n-1);
  stereoNoteOut = synthFunc(freqSelector(cs_d_present)),synthFunc(freqSelector(cs_d_present) + freqOffset_LR);	};

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // S O N I // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
Soni_B1_MaxWarpFactor = 5;
Soni_B2_A_Max = 0;	Soni_B2_A_Min = -80;
Soni_B4_Fc_Max = 12000; Soni_B4_Fc_Min = 200;
Soni_B5_MaxRelFactor = 5;
Soni_R1_MaxDelSec(trackIndex) = 0.1,0.17,0.23,0.29,0.37,0.47,0.59,0.71 : ba.selectn(8,trackIndex);
Soni_RZ_OscFreqs(index) = tempo/120, tempo/60, tempo/30 : select3(index);
Soni_RZ_LFO_Riff = os.osc(Soni_RZ_OscFreqs(0));
Soni_RZ_LFO_Chords = os.osc(Soni_RZ_OscFreqs(1));
Soni_RZ_LFO_Mel = os.osc(Soni_RZ_OscFreqs(2)); 
Soni_RZ1_numNotches = 10; 							Soni_RZ1_notchWidth = 800; 								Soni_RZ1_notch1Min = 100;
Soni_RZ1_notchSpacing = 1.5;						Soni_RZ1_notch1Max = 400;
Soni_RZ2_wahMinFreq = 250;							Soni_RZ2_wahMaxFreq = 5000;

Soni_B1_FreqWarpFactor = _*(warpFactor) with { warpFactor = Soni_B1_MaxWarpFactor * Soni_X_B1_MelBaseFreq + 1; };
Soni_B2_AmpMultiplier = -1*(ba.db2linear(Soni_B2_A_Max) - ba.db2linear(Soni_B2_A_Min)) * Soni_X_B2_MelLoudness + ba.db2linear(Soni_B2_A_Max);
Soni_B4_Filter = fi.resonlp(cutoff,2.5,1) with {cutoff = Soni_B4_Fc_Max - Soni_X_B4_MelBrightness * (Soni_B4_Fc_Max - Soni_B4_Fc_Min);};
Soni_B5_RelFactor = 1 + Soni_X_B5_PercRelTime * Soni_B5_MaxRelFactor;
Soni_R1(maxDelay) = _ : de.fdelay(50000,cookedDelaySamples) with {cookedDelaySamples = maxDelay * Soni_X_R1_InstSynch * ma.SR : ba.sAndH(ba.pulse(ma.SR/2));};
Soni_R2_InharmonicityFX = _ <: *(Soni_X_R2_Inharmonicity*os.osc(tritoneFreq)),*(1-Soni_X_R2_Inharmonicity) :> _   with { tritoneFreq = (tonic + 6) : ba.midikey2hz; };
Soni_R3_TremoloFX = _ <: *(mult * tremoloLFO),*(1-mult) :> _ with { tremoloLFO = 0.5*(1 + os.osc(tempo/60)); mult = Soni_X_R3_Tremolo; };
Soni_R4_Dist = _ <: ef.cubicnl(0.8,0)*(mult),*(1-mult) :> _ with { mult = Soni_X_R4_Dist * (1 - 0.5*Soni_X_R4_Dist); };
Soni_RZ1_Phaser(oscFreq) = _ : pf.phaser2_mono(Soni_RZ1_numNotches,0,Soni_RZ1_notchWidth,Soni_RZ1_notch1Min,Soni_RZ1_notchSpacing,Soni_RZ1_notch1Max,oscFreq
											   ,(1 - Soni_X_RZ1_MultiPhaser),0,0) : _*(ba.db2linear(4*(1 - Soni_X_RZ1_MultiPhaser)));
Soni_RZ2_Wah(LFO,minFreq,maxFreq) = _ : fi.peak_eq_cq(currentGain,currentFreq,0.7) : *(ba.db2linear(-0.4*currentGain)) with
{
  LFO_unipolar = 0.5*(LFO + 1);
  currentFreq = (minFreq + LFO_unipolar*(maxFreq - minFreq));
  currentGain = 20 * ( 1- Soni_X_RZ2_MultiWah);
};
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // Generation // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 

																							//Kick

kickSynth_S1 = generalizedDrumSynth(kickNoise ,0.113, 0.261, 225, -2, 10.7, -22, 15000, 20, kick_v_present) : *(ba.db2linear(-6));
kickSynth_S2 = os.osc(100);
kickSynth_S3 = os.osc(200);
kickSynth_S4 = os.osc(300);

kickSynth_ChosenStyle = kickSynth_S1,kickSynth_S2,kickSynth_S3,kickSynth_S4 : ba.selectn(numStyles,currentStyle) : Soni_R1(Soni_R1_MaxDelSec(0));

kickTrack = kickSynth_ChosenStyle : channelComp(compTrackGroup(1)) : parametricEQ(eqTrackGroup(1)) : getPanFunction(0);

																						//Snare

snareSynth_S1 = generalizedDrumSynth(snareNoise ,0.23, 0.238, 200, 2, 2, -17, 15000, 200, snare_v_present) : *(ba.db2linear(-6));
snareSynth_S2 = os.osc(400);
snareSynth_S3 = os.osc(600);
snareSynth_S4 = os.osc(800);

snareSynth_ChosenStyle = snareSynth_S1,snareSynth_S2,snareSynth_S3,snareSynth_S4 : ba.selectn(numStyles,currentStyle) : Soni_R1(Soni_R1_MaxDelSec(1));

snareTrack =snareSynth_ChosenStyle : channelComp(compTrackGroup(2)) : parametricEQ(eqTrackGroup(2)) : getPanFunction(0);

																						//HH

hhSynth_S1 = fmPercSynth(hhNoise,-5,-10,6000,hh_S1_DepthList,hh_S1_FreqList,hh_S1_Carrier,0.18 * hh_Openness,hh_v_true);
hhSynth_S2 = os.osc(800);
hhSynth_S3 = os.osc(1200);
hhSynth_S4 = os.osc(1600);

hhSynth_ChosenStyle = hhSynth_S1,hhSynth_S2,hhSynth_S3,hhSynth_S4 : ba.selectn(numStyles,currentStyle) : Soni_R1(Soni_R1_MaxDelSec(2));

hhTrack = hhSynth_ChosenStyle  : channelComp(compTrackGroup(3)) : parametricEQ(eqTrackGroup(3)) : getPanFunction(1);

																							//Chord

//Frequency
currentChordFreqs = chordFrequencies(cd_present,ct_present,1);
currentChordSynthFunc(freq) = chordSynthFunc_S1(freq), chordSynthFunc_S2(freq), chordSynthFunc_S3(freq), chordSynthFunc_S4(freq) : ba.selectn(numStyles,currentStyle);

//Envelope
chordAttack = 0.001, 0.001, 0.001, 0.001 : ba.selectn(numStyles,currentStyle);
chordRelease = 0.8, 0.8, 0.8, 0.8 : ba.selectn(numStyles,currentStyle);
chordFinalVal = 0.7, 0.5, 0.5, 0.5 : ba.selectn(numStyles,currentStyle);
chordEnv = chordEnvelope(chordAttack,chordRelease,chordFinalVal,cv_present);

//Synthesis sideChainComp(sidechain,inputGain_dB,ratio,thresh,att,rel,kneeAtt)
chordSynthFunc_S1(freq) = os.sawtooth(freq);// : sideChainComp(kickSynth_ChosenStyle,0,7,-10,0.005,0.03,0);
chordSynthFunc_S2(freq) = os.square(freq);
chordSynthFunc_S3(freq) = os.sawtooth(freq);
chordSynthFunc_S4(freq) = os.osc(freq);
chordFinal = fullChordSynth(currentChordFreqs,currentChordSynthFunc) : _*chordEnv, _*chordEnv : Soni_R1(Soni_R1_MaxDelSec(3)), Soni_R1(Soni_R1_MaxDelSec(3)) 
  : Soni_RZ1_Phaser(Soni_RZ_OscFreqs(0)), Soni_RZ1_Phaser(Soni_RZ_OscFreqs(0))
	: Soni_RZ2_Wah(Soni_RZ_LFO_Chords,Soni_RZ2_wahMinFreq,Soni_RZ2_wahMaxFreq), Soni_RZ2_Wah(Soni_RZ_LFO_Chords,Soni_RZ2_wahMinFreq,Soni_RZ2_wahMaxFreq);

chordTrack = chordFinal : channelComp(compTrackGroup(4)),channelComp(compTrackGroup(4)) : parametricEQ(eqTrackGroup(5)),parametricEQ(eqTrackGroup(5));

																							//Riff

riff_fundamental_freq = melodyFundamentalFreq(activeScale,tonic,rf_octave,rf_present,24,38);
riffAttackBase = 0.001,	0.001,	0.001,	0.001 : ba.selectn(numStyles,currentStyle);
riffReleaseBase = 0.2,	0.15,	0.15,	0.15 : ba.selectn(numStyles,currentStyle);

riffSynth_S1 = fmSynth(riff_fundamental_freq,5,3,riffReleaseBase,18,rv_present) * ba.db2linear(12);
riffSynth_S2 = os.osc(2000);
riffSynth_S3 = os.osc(3000);
riffSynth_S4 = os.osc(4000);

riffSynth_ChosenStyle = riffSynth_S1, riffSynth_S2, riffSynth_S3, riffSynth_S4 : ba.selectn(numStyles,currentStyle) : Soni_R1(Soni_R1_MaxDelSec(4)) : Soni_RZ1_Phaser(Soni_RZ_OscFreqs(1)) 
  : Soni_RZ2_Wah(Soni_RZ_LFO_Riff,Soni_RZ2_wahMinFreq,Soni_RZ2_wahMaxFreq);

riffTrack = riffSynth_ChosenStyle : channelComp(compTrackGroup(5)) : parametricEQ(eqTrackGroup(5)) : getPanFunction(0);

																							//Melody Main

melody_fundamental_freq = melodyFundamentalFreq(activeScale,tonic,mf_octave,mf_present,10,80);
melodyAttackBase = 		0.001,	0.001,	0.001,	0.001 : ba.selectn(numStyles,currentStyle);
melodyReleaseBase = 	0.4 * 120/tempo,	0.4,	0.4,	0.4	  : ba.selectn(numStyles,currentStyle);

melody_S1_fx1 = dotted_delay(0.8,quarterNoteSampleTime,0.4,1);
melody_S1_fx2 = _ : _; 

melodySynth_S1 = leadSynth(melody_fundamental_freq,os.sawtooth,melodyNoise,mv_present,melodyReleaseBase,4000,4,-8,0.05,6000,melody_S1_fx1,melody_S1_fx2);
melodySynth_S2 = os.sawtooth(melody_fundamental_freq);
melodySynth_S3 = os.triangle(melody_fundamental_freq);
melodySynth_S4 = os.osc(melody_fundamental_freq);

melodySynth_ChosenStyle = melodySynth_S1, melodySynth_S2, melodySynth_S3, melodySynth_S4 : ba.selectn(numStyles,currentStyle) : Soni_R1(Soni_R1_MaxDelSec(5)) : Soni_RZ1_Phaser(Soni_RZ_OscFreqs(1)) : Soni_RZ2_Wah(Soni_RZ_LFO_Mel,Soni_RZ2_wahMinFreq,Soni_RZ2_wahMaxFreq);

melodyTrack = melodySynth_ChosenStyle : channelComp(compTrackGroup(6)) : parametricEQ(eqTrackGroup(6)) : getPanFunction(0);

																						//Chord Stabs

//Frequency
currentChordStabFreqs = chordFrequencies(cd_present,ct_present,2);
currentChordStabSynthFunc(freq) = chordStabSynthFunc_S1(freq), chordStabSynthFunc_S2(freq), chordStabSynthFunc_S3(freq), chordStabSynthFunc_S4(freq) : ba.selectn(numStyles,currentStyle);

//Envelope
chordStabAttack = 0.001, 0.001, 0.001, 0.001 : ba.selectn(numStyles,currentStyle);
chordStabRelease = 0.4, 0.4, 0.4, 0.4 : ba.selectn(numStyles,currentStyle);
chordStabEnv = expEnvelope(chordAttack,chordRelease,cs_v_present);

//Synthesis
chordStabSynthFunc_S1(freq) = os.square(freq);
chordStabSynthFunc_S2(freq) = os.square(freq);
chordStabSynthFunc_S3(freq) = os.sawtooth(freq);
chordStabSynthFunc_S4(freq) = os.osc(freq);

fullChords = fullChordSynth(currentChordStabFreqs,currentChordStabSynthFunc) : *(0.3), *(0.3);
arpChords = arpChordSynth(currentChordStabFreqs,currentChordStabSynthFunc);
chordStabSynthCurrent = fullChords, arpChords : ba.select2stereo(cs_arpMode);

chordStabFinal = chordStabSynthCurrent : _*chordStabEnv, _*chordStabEnv : Soni_R1(Soni_R1_MaxDelSec(6)), Soni_R1(Soni_R1_MaxDelSec(6)) : Soni_RZ1_Phaser(Soni_RZ_OscFreqs(2)), Soni_RZ1_Phaser(Soni_RZ_OscFreqs(2));

chordStabTrack = chordStabFinal : channelComp(compTrackGroup(7)), channelComp(compTrackGroup(7)) : parametricEQ(eqTrackGroup(7)), parametricEQ(eqTrackGroup(7));

																						//Crash

crashSynth_S1 = fmPercSynth(crashNoise,-6,-5,6000,crash_S1_DepthList,crash_S1_FreqList,crash_S1_Carrier,1.8,crash_v_present);
crashSynth_S2 = os.osc(800);
crashSynth_S3 = os.osc(1200);
crashSynth_S4 = os.osc(1600);
crashSynth_ChosenStyle = crashSynth_S1,crashSynth_S2,crashSynth_S3,crashSynth_S4 : ba.selectn(numStyles,currentStyle) : Soni_R1(Soni_R1_MaxDelSec(7));

crashTrack = crashSynth_ChosenStyle : channelComp(compTrackGroup(8)) : parametricEQ(eqTrackGroup(8)) : getPanFunction(2);

// // // // // // // // // // // // // // // // // // // // // // // // // // // // TRADITIONAL STRATEGIES // // // // // // // // // // // // // // // // // // // // // // // // // // // //

x = Soni_X_Trad;

S1_Pitch = os.osc(S1_freq) *(0.5);
S1_freq = S1_fmin * pow(2,x*S1_nOct);
S1_fmin = 300;
S1_nOct = 3.5;

S2_Loudness = S2_Amp * os.osc(1000);
S2_Amax = -5;
S2_Amin = -40;
S2_Amp = (ba.db2linear(S2_Amax) - ba.db2linear(S2_Amin)) * x + ba.db2linear(S2_Amin);

S3_Tempo = os.osc(1000) * en.ar(0.001,0.05,S3_PulseTrain) *(0.5);
S3_PulseTrain = ba.pulse(S3_SampleInterval);
S3_TempoMax = 20*60; 		S3_TempoMin = 2*60;
S3_IntervalRange = ba.tempo(S3_TempoMin) - ba.tempo(S3_TempoMax);
S3_SampleInterval = ba.tempo(S3_TempoMax) + x * (S3_IntervalRange);

S4_Brightness = no.noise : fi.resonlp(S4_fc,0.7,1);
S4_fc = S4_fmin * pow(2,(1-x)*S4_nOct);
S4_fmin = 300;
S4_nOct = 3.5;

S5_FluctStrength = (0.5 * os.osc(1000) + 0.5 * os.osc(1000 + 10*x)) *(0.5);

S6_Synchronicity = (S6_OriginalTrain + S6_Delayed) : en.ar(0.001,0.05) : *(os.osc(1000) *(0.5));
S6_PulseFreq = 3;  S6_PeriodSamples = ba.sec2samp(1/S6_PulseFreq);
S6_MaxSampleDelay = 0.25 * S6_PeriodSamples;
S6_CurrentDelay = S6_MaxSampleDelay * x; 
S6_OriginalTrain = ba.pulse(S6_PeriodSamples);
S6_Delayed = S6_OriginalTrain : de.fdelay(40000,S6_CurrentDelay);

S7_Inharmonicity = (os.osc(S7_F) + S7_addedPartials) * 0.15;
S7_addedPartials = par(i,15,S7_InharmExp(i)) :> _;
S7_InharmExp(i) = os.osc(S7_F*i*sqrt(1+ x/10*i*i));
S7_F = 200;

S8_MBFM = par(i,7,os.triangle(S8_freqExp(i))) :> _ *(0.3);
S8_freqExp(i) = i*S8_F + S8_I * os.osc(10*x);
S8_I = 30;
S8_F = 100;

S9_MSB = par(k,S9_N,S9_innerLoop(k)) :> _*(0.5);
S9_innerLoop(k) = par(m,S9_M,S9_innerExp(k,m));
S9_innerExp(k,m) = os.osc(S9_F * k *(1 + m*(S9_alpha(x) - 1)));
S9_alpha(dist) = 1 + 0.06*(dist);
S9_F = 200; S9_N = 5; S9_M = 5;

tradSoniOut = S1_Pitch,S2_Loudness,S3_Tempo,S4_Brightness,S5_FluctStrength,S6_Synchronicity,S7_Inharmonicity,S8_MBFM,S9_MSB : ba.selectn(9,Soni_Choice_Trad) <: _,_;

// // // // // // // // // // // // // // // // // // // // // // // // // // // // M A S T E R // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 

track1 = kickTrack 											: stereoMasterSection(1);
track2 = snareTrack 										: stereoMasterSection(2);
track3 = hhTrack 											: stereoMasterSection(3);
track4 = chordTrack											: stereoMasterSection(4);
track5 = riffTrack 											: stereoMasterSection(5);
track6 = melodyTrack									 	: stereoMasterSection(6);
track7 = chordStabTrack									 	: stereoMasterSection(7);
track8 = crashTrack										 	: stereoMasterSection(8);

stereoMasterSection(trackIndex) = stereoOut
  with
{
  stereoOut = _,_ : *(trackGain),*(trackGain) : *(1-trackMute),*(1-trackMute);
  trackGain = trackGainGroup(hslider("Track %trackIndex",0,-96,12,0.01) : ba.db2linear : si.smoo);
  trackMute = trackMuteGroup(checkbox("%trackIndex") : si.smoo);
};

masterGain = masterGroup(vslider("Master Gain",-12,-96,12,0.01) : ba.db2linear : si.smoo);
sonificationType = masterSoniTypeToggle(checkbox("Traditional"));
melBuss = track4,track5,track6,track7 :> *(Soni_B2_AmpMultiplier), *(Soni_B2_AmpMultiplier) : Soni_R2_InharmonicityFX,Soni_R2_InharmonicityFX : Soni_R3_TremoloFX,Soni_R3_TremoloFX : Soni_B4_Filter,Soni_B4_Filter : Soni_R4_Dist,Soni_R4_Dist;
masterOut = track1,track2,track3,track8,melBuss :> parametricEQ(masterEQGroup) , parametricEQ(masterEQGroup) : 
 		  _*(masterGain),_*(masterGain) : masterLimiter(0),masterLimiter(0) : hard_clip(1), hard_clip(1);

process = masterOut,tradSoniOut : ba.select2stereo(sonificationType);