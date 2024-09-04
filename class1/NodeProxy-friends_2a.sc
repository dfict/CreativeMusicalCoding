s.boot;

(
var a, c, s, z, y;
~d = 6; // Set the global variable
c = 6; // number of comb delays
a = 4; // number of allpass delays

b = NodeProxy.new; // create NodeProxy without direct output
b.fadeTime = 10; // set fade time

// Set initial source for NodeProxy b using the global variable
b.source = {
    Mix.ar(Array.fill(~d, { Resonz.ar(Dust.ar(2/~d, 50), 200 + 3000.0.rand, 0.003)}) )
};

// Now create the main processing chain that uses NodeProxy b
b = b.play;

// Main processing chain
(
    play({
        // Get the source from NodeProxy b
        var source = b.ar;

        // Reverb predelay time
        z = DelayN.ar(source, 0.048);

        // 6 modulated comb delays in parallel
        y = Mix.ar(CombL.ar(z, 0.1, LFNoise1.kr(Array.fill(c,{0.1.rand}), 0.04, 0.05), 15));

        // Chain of 4 allpass delays on each of two channels (8 total)
        a.do({ y = AllpassN.ar(y, 0.050, [0.050.rand,0.050.rand], 1) });

        // Add original sound to reverb and play it
        source + y
    })
);
)

b.fadeTime = 5; // set fade time

// Example: After some time, switch to a different source
(	 ~d=6;
	b.source = {    Decay.ar(Dust.ar(12 / ~d, 0.2), 0.15, PinkNoise.ar)};)

// Clear NodeProxy b after 3 seconds if needed
b.clear(3);
