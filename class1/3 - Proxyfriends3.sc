s.boot;

p = ProxySpace.new;
p.fadeTime = 2; // fadeTime specifies crossfade
p[\out].play; // monitor an empty placeholder through hardware ouput
// set its source
p[\out] = { Pulse.ar([350, 351.3] / 4, 0.4) * 0.2 };
p[\out] = Pbind(\dur, 0.03, \freq, Pbrown(0, 1, 0.1, inf).linexp(0, 1, 200, 350));
p[\out] = { Impulse.ar([5, 7]/2, [0, 0.5]) };

// route one proxy through another:
p[\out] = { Ringz.ar(p[\in].ar, [350, 351.3] * 8, 0.2) * 4 };
p[\in] = { Impulse.ar([5, 7]/2, [0, 0.5]) };

p.clear(3); // clear after 3 seconds
p.clear(3);
