
n = NodeProxy.audio(s,2).play;
n.gui;

// using curvature to shape crescendo/decrescento
Spec.add(\curve,[-10, 10]);
n[0] = { | amp, curve = 2 |
	LFTri.kr(1/10).curverange(0, 1, curve) *
	BrownNoise.ar * amp.dup };


// using midiratio to change the rate of Dust
n[0] = { |amp, widefreq = 4, freq|
	widefreq = widefreq *
	LFTri.kr(freq 200).range(-48,48).midiratio;
	Dust.ar(widefreq) * amp.dup };

// using Slope to trigger an envelope whenever the value changes
n[0] = { |widefreq = 30, lo = 200, hi = 200 |
	var gate, f = LFClipNoise.ar(widefreq).range(lo, hi);
	gate = Slope.kr(A2K.kr(f)).abs;
	Linen.kr(gate,0, 1, 0.1) *
	SinOsc.ar(130) * SinOsc.ar(f ) * 0.1.dup;
};
