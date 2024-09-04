//writing the same patch different ways

{ 	var frq = LFPulse.kr(0.09, 0, 0.16, 10, 7);
	var amp = LFPulse.kr(frq, 0, 0.25, 0.1);
	var audio = WhiteNoise.ar(amp);
	audio= BPZ2.ar(audio);
	audio ! 2;
}.play;

//is the same as:


{
	BPZ2.ar(
		WhiteNoise.ar(
			LFPulse.kr(
				LFPulse.kr(0.09, 0, 0.16, 10, 7),
				0, 0.25, 0.1
			)
		)
	)!2
}.play

//or

{
BPZ2.ar(WhiteNoise.ar(LFPulse.kr(LFPulse.kr(0.09, 0, 0.16, 10, 7), 0, 0.25, 0.1)))!2
}.play