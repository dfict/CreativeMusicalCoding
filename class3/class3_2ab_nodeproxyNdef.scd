// NodeProxy's GUI and ControlSpecs
/*
NodeProxy::gui generates a GUI control interface that
updates its content automatically as you interact with the NodeProxy
*/
(
// create a NodeProxy and a gui
~np = NodeProxy.audio(s, 2);
(
// now let's add a synthesis function
~np[0] = {|freq= 400, amp= 0.1, pan = 0, beats = 2 |
	var audio = SinOsc.ar(freq) * LFTri.kr(beats);
	audio = Pan2.ar(audio, pan, amp)
};
~gui = ~np.gui;
)
)
~np.stop;
// Notice that the individual controls have a reasonable range of values

// now let's add the same synthesis function,
// but with different control names
(
~np.clear;
~np[0] = {|freq1, amp1 = 0.1, pan1, beats1= 0.1 |
	var audio = SinOsc.ar(freq1) * LFTri.kr(beats1);
	audio = Pan2.ar(audio, pan1, amp1)
};
)
~np.gui;

Notice that the individual controls do not have a reasonable ranges!

Why?  The difference is that the symbols freq, amp, pan and beats
have been associated with ControlSpec objects while freq1, amp1, pan1,
and beats1 have not.

A ControlSpec defines a range of control values using the following parameters.

minval		range minimum
maxval	range maximium
warp		shape of the range can be  \lin, \exp or numerical curvature value
step			The smallest permitted increment
default	The default value

ControlSpec constructor
	ControlSpec(minval, maxval, warp, step, default)

But the ControlSpec needs to be registered so that NodeProxy can find it and
use it to scale the named control.

	Spec.add(key, [minval, maxval, warp, step, default])

If a ControlSpec of the desired range already exists, this can be shortened to:

	Spec.add(newKey, existingKey)

// Here are useful predefined ControlSpecs

Spec.add(\bipolar, [-1,1,0,0, 0]);
Spec.add(\pan, \bipolar);
Spec.add(\unipolar, [0,1]);
Spec.add(\detune, [-20,20,0,0, 0]);
Spec.add(\beats, [0,20]);

Spec.add(\rate, [1/8, 8, \exp, 0, 1]);
Spec.add(\delay, [0.0001, 1, \exp, 0, 0.3]);

Spec.add(\freq, [20, 20000, \exp, 0, 440]);  // min, max, warp, step, default
Spec.add(\lofreq, [0.1, 100, \exp, 0, 6]);  // min, max, warp, step, default
Spec.add(\widefreq, [0.1, 20000, \exp, 0, 440]);  // min, max, warp, step, default
Spec.add(\phase, [0,2pi]);
Spec.add(\rq, [0.001, 2, \exp, 0, 0.707]);  // min, max, warp, step, default

Spec.add(\midi, [0, 127, 0, 0, 64]);
Spec.add(\midinote, [0, 127, 0, 0, 60]);
Spec.add(\midivelocity, [1, 127, 0, 0, 64]);

Spec.add(\audiobus, [0,Server.default.options.numAudioBusChannels-1, 0, 1]);
Spec.add(\controlbus, [0,Server.default.options.numControlBusChannels-1, 0, 1]);


// Define additional control specs for NodeProxy
(
Spec.add(\freq1, \freq);
Spec.add(\amp1, \amp);
Spec.add(\pan1,  \pan);
//Let's define the beats1 ControlSpec by hand
Spec.add(\beats1, [0.01, 200, \exp]);
~gui.paramGui.updateViewSpecs(~gui.paramGui.getState[\specs]);
)
