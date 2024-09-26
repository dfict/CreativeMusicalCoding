(
// moto rev
~motorev = { | pw = 0.1, revRate = 0.2, loRev = 11, hiRev = 31,  cutoff = 100, rq = 0.1|
	var mod = SinOsc.kr(revRate).range(loRev, hiRev);
	var audio = LFPulse.ar(mod, 0,  pw) ;
	audio = RLPF.ar(audio, cutoff, rq).clip2(0.4);
	audio = Limiter.ar(audio, 1);
	Pan2.ar(audio, LFTri.kr(SinOsc.kr(revRate).range(05, 0);))
})

n = NodeProxy.audio(s, 2);
n.play;
n.gui;

n[0] = ~motorev;

Spec.add(\pw, [0,1.3])
Spec.add(\revRate, [0.01, 100, 0, 0, 'exp'])

