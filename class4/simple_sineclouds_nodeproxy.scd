// Boot the server
s.boot;

(
// Create and play a new NodeProxy
b = NodeProxy.new.play;

// Set fadeTime for smooth transitions
b.fadeTime = 1; // 5 seconds crossfade, adjust as needed

// Define the source
b.source = {
    var numOscs = 24;
    var freqRange = [200, 2000];

    Mix.fill(numOscs, {
        var freq = exprand(freqRange[0], freqRange[1]);
        var pan = rrand(-1.0, 1.0);
        Pan2.ar(SinOsc.ar(freq), pan, 1/numOscs);
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


//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////


///you could also do it this way for slightly different parameters using an environmental variable instead of global variable:

(
// Create a ProxySpace
p = ProxySpace.push(s);

// Create and play a new NodeProxy
~sineclouds = {
    var numOscs = 24;
    var freqRange = [200, 6000];

    Mix.fill(numOscs, {
        var freq = exprand(freqRange[0], freqRange[1]);
        var pan = rrand(-1.0, 1.0);
        Pan2.ar(SinOsc.ar(freq), pan, 1/numOscs);
    }) * 0.1; // Overall volume reduction
};

// Set fadeTime for smooth transitions
~sineclouds.fadeTime = 10; // 1 second crossfade, adjust as needed

// Play the NodeProxy
~sineclouds.play;
)

// To regenerate with new random frequencies (with crossfade):
 ~sineclouds.rebuild;

// To stop with fadeout:
 ~sineclouds.stop(5); // 5 second fadeout

// To start again with fadein:
 ~sineclouds.play(5); // 5 second fadein

// To clear the proxy and remove it from the server:
 ~sineclouds.clear(5); // 5 second fadeout before clearing

// To exit the ProxySpace:
 p.pop;