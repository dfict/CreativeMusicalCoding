//w amplitude



(
// Create and play a new NodeProxy
b = NodeProxy.new.play;
// Set fadeTime for smooth transitions
b.fadeTime = 1; // 1 second crossfade, adjust as needed
// Define the source
b.source = {
    var numOscs = 44;
    var freqRange = [50, 4000];
    var pulsRange = [0.1, 10.0];
    Mix.fill(numOscs, {
        var freq = exprand(freqRange[0], freqRange[1]);
        var pulseFreq = linrand(pulsRange[0], pulsRange[1]);
        var pan = rrand(-1.0, 1.0);
        var puls = SinOsc.kr(pulseFreq).range(0.5, 1);  // Changed to .range for smoother modulation
		var sig = SinOsc.ar(freq, 0.5 * puls, 0.75);
        Pan2.ar(sig, pan, 1/numOscs);
    }) * 0.1; // Overall volume reduction
};
)

// To regenerate with new random frequencies (with crossfade):
b.source = b.source;

// To stop with fadeout:
b.stop(5); // 5 second fadeout

// To start again with fadein:
b.play(5); // 5 second fadein

// To clear the proxy and remove it from the server:
b.clear(5); // 5 second fadeout before clearing


// To regenerate with new random frequencies (with crossfade):
 b.source = b.source;

// To stop with fadeout:
 b.stop(5); // 5 second fadeout

// To start again with fadein:
 b.play(5); // 5 second fadein

// To clear the proxy and remove it from the server:
 b.clear(5); // 5 second fadeout before clearing





