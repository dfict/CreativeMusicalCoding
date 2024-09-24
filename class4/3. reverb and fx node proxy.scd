(
s.options.memSize = 8192 * 4;
s.quit;
s.waitForBoot {
	n= NodeProxy.audio(s, 2);
	e = NodeProxy.audio(s,2);
	e.play	//  e will be the NP that we actually hear
	e[0] = { n.ar };								// and we will hear whatever is in "n"

};
);

(
~sinPulse = { | freq = 200, rate1 = 1, rate2 = 3, modfreq = 900, swellRate1 = 0.1, swellRate2 = 0.09 , amp = 0.1, pw = 0.1|
	{	SinOsc.ar(freq)
		* Lag.kr(LFPulse.kr([rate1, rate2],0, pw), 0.2)
		* SinOsc.ar (modfreq)
		* LFTri.kr([swellRate1, swellRate2].flat).range(0, 1)
		* amp
	}
};

n[0] = ~sinPulse.();
n[1] = ~sinPulse.(210, 1.1, 3.01, 800, 0.2, 0.19);
n[0] = { SinOsc.ar(200) * Lag.kr(LFPulse.kr([1, 3],0, 0.1), 0.2) * SinOsc.ar (900) * 0.1 * LFTri.kr([1/10, 1/15.1]).range(0, 1) };


e.fadeTime = 4;

~rvb =  { | maxDel = 0.5, decayTime = 3, combChans = 32, allChans = 5, wetDry = 0.9  |
	{ | in |
	var rvb, nchans = 2;
		// initial delay
	in = Line.kr(0, 1, 0.5) * in;				// fade in input to avoid making a click that will ring in the delays
	rvb = DelayN.ar(in, 0.03, 0.03);
		// 32 channels of short delay!
	combChans.do { rvb = CombN.ar(rvb, maxDel, {Rand(min(0.01,maxDel),maxDel)}, decayTime) };
	rvb = LeakDC.ar(rvb);
		//make it stereo
	rvb = SplayAz.ar(2, rvb);
		// darken the reverb
	rvb = LPF.ar(rvb, 1500);
		allChans.do{rvb = AllpassN.ar(rvb, maxDel/5, {Rand(min(0.01,maxDel),maxDel/5)}.dup, decayTime) };
   rvb * wetDry + ((1- wetDry) * in);
	rvb = LeakDC.ar(rvb);
		Limiter.ar(rvb, 0.5)
	}
};
)
e[100] = \filter -> ~rvb.(maxDel: 0.1, decayTime: 0.1,  combChans: 0, allChans: 0);
e[100] = \filter -> ~rvb.(maxDel: 0.1, decayTime: 3.1,  combChans: 1, allChans: 0);
e[100] = \filter -> ~rvb.(maxDel: 0.01, decayTime: 3.1,  combChans: 1, allChans: 0, wetDry: 0.5);
e[100] = \filter -> ~rvb.(maxDel: 0.021, decayTime: 3.1,  combChans: 1, allChans: 0, wetDry: 0.5);
e[100] = \filter -> ~rvb.(maxDel: 0.15, decayTime:  3.1,  combChans: 1, allChans: 0);
e[100] = \filter -> ~rvb.(maxDel: 0.021, decayTime: 2.1,  combChans: 4, allChans: 0, wetDry: 0.5);
e[100] = \filter -> ~rvb.(maxDel: 0.1, decayTime: 02.1,  combChans: 4, allChans: 0, wetDry: 0.5);
e[100] = \filter -> ~rvb.(maxDel: 0.15, decayTime:  3.1,  combChans: 0, allChans: 4);
e[100] = \filter -> ~rvb.(maxDel: 0.55, decayTime: 10.1,  combChans: 0, allChans: 5);
e[100] = \filter -> ~rvb.(maxDel: 0.55, decayTime: 4.1,  combChans: 5, allChans: 5);
e[100] = \filter -> ~rvb.(maxDel: 0.55, decayTime: 2.1,  combChans: 5, allChans: 5);
e[100] = \filter -> ~rvb.(maxDel: 0.55, decayTime: 0.1,  combChans: 5, allChans: 5);
e.stop;


e[99] = Task({ 100.do { |i| i.postln; 1.wait } })
e[99].pause;
e[99].resume;
e[99].reset;

e[99] = Task({
	e[100] = \filter -> ~rvb.(maxDel: 0.1, decayTime: 0.1,  combChans: 0, allChans: 0);
	rrand(2.0, 8.0).wait;
e[100] = \filter -> ~rvb.(maxDel: 0.1, decayTime: 3.1,  combChans: 1, allChans: 0);
	rrand(2.0, 8.0).wait;
e[100] = \filter -> ~rvb.(maxDel: 0.01, decayTime: 3.1,  combChans: 1, allChans: 0, wetDry: 0.5);
	rrand(2.0, 8.0).wait;
e[100] = \filter -> ~rvb.(maxDel: 0.021, decayTime: 3.1,  combChans: 1, allChans: 0, wetDry: 0.5);
	rrand(2.0, 8.0).wait;
e[100] = \filter -> ~rvb.(maxDel: 0.15, decayTime:  3.1,  combChans: 1, allChans: 0);
	rrand(2.0, 8.0).wait;
e[100] = \filter -> ~rvb.(maxDel: 0.021, decayTime: 2.1,  combChans: 4, allChans: 0, wetDry: 0.5);
	rrand(2.0, 8.0).wait;
e[100] = \filter -> ~rvb.(maxDel: 0.1, decayTime: 02.1,  combChans: 4, allChans: 0, wetDry: 0.5);
	rrand(2.0, 8.0).wait;
e[100] = \filter -> ~rvb.(maxDel: 0.15, decayTime:  3.1,  combChans: 0, allChans: 4);
	rrand(2.0, 8.0).wait;
e[100] = \filter -> ~rvb.(maxDel: 0.55, decayTime: 10.1,  combChans: 0, allChans: 5);
	rrand(2.0, 8.0).wait;
e[100] = \filter -> ~rvb.(maxDel: 0.55, decayTime: 4.1,  combChans: 5, allChans: 5);
	rrand(2.0, 8.0).wait;
e[100] = \filter -> ~rvb.(maxDel: 0.55, decayTime: 2.1,  combChans: 5, allChans: 5);
	rrand(2.0, 8.0).wait;
e[100] = \filter -> ~rvb.(maxDel: 0.55, decayTime: 0.1,  combChans: 5, allChans: 5);
	rrand(2.0, 8.0).wait;
	e[100] = 0;
})

e.play

