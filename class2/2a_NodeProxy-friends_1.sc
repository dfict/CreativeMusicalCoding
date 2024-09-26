s.boot;

(a = NodeProxy.new.play; // play to hardware output.
a.fadeTime = 10; // fadeTime specifies crossfade
// set the source
a.source = { SinOsc.ar([350, 351.3], 0, 0.2) };)

a.play;

a.source = { Pulse.ar([350, 351.3, 660, 1000, 1500] / 4, 0.4) * 0.2 };
a.source = Pbind(\dur, 0.03, \freq, Pbrown(0, 1, 0.1, inf).linexp(0, 1, 200, 350));

b = NodeProxy.new;

b.fadeTime = 20; // fadeTime specifies crossfade

a.source = { Ringz.ar(b.ar, [350, 351.3] * 8, 0.2) * 4 };
b.source = { Impulse.ar([5, 7]/2, [0, 0.5]) };

b.source = { Impulse.ar([5, 7]/2, [0, 0.5]) };

b.source = { RLPF.ar(Saw.ar(200, 0.1), FSinOsc.kr(XLine.kr(0.7, 300, 20), 0, 3600, 4000), 0.2) };
b.source = { RLPF.ar(Impulse.ar([5, 7]/2, [0, 0.5]), FSinOsc.kr(XLine.kr(0.7, 300, 20), 0, 3600, 4000), 0.2) };

b.play;

a.clear(3); // clear after 3 seconds
b.clear(3);