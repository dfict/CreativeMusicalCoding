// Class 3: Synth Controls and NodeProxy GUIs
//
// FUNCTIONS
//
// SYNTHESIS FUNCTIONS
//
// CONTROLSPECS
//
// THE NODEPROXY gui method
//
//
// A few words on functions and arguments
//
// Synthesis functions - functionarguments are treated as Control declarations
//
// ControlSpec - how you define the range of a particular control
//
// NodeProxy GUI control



(
// Here is analog bubbles with some basic parameters declared as function arguments
// When treated as a synthesis function, these parameters become controls
// |containers for arguments|

f = { |  r0 = 0.4, range = 24, r1 = 8, r2 = 7.23, lok= 78, hik = 84  , delay = 0.2, decay = 4 |
	var k, f, a, mod1, mod2;
	mod1 = LFSaw.kr(r1);
	mod1 = mod1.range( lok, hik);

	mod2 = LFSaw.kr(r2);					// define the second modulator
	mod2 = mod2.range( lok, hik);

	k = LFSaw.kr(r0) * range + [mod1, mod2];	// use the second modulator
	f = k.midicps;
	a = SinOsc.ar(f) * 0.04;
	CombN.ar(a, 1, Lag.kr(delay,1), decay)
};

// f.play;			// note: the 'play' method treats the function as a synthesis function...so don't use it...

//
instead define controls

(
// here is the tedious process of defining ControlSpecs for each control...
Spec.add(\r0, [0.1,100, 3]);   // lo, hi, warp, step, default
Spec.add(\r1, [0.1,100, 3]);
Spec.add(\r2, [0.1,100, 3]);
Spec.add(\range, \midi);			// 'midi' is an existing ControlSpec
Spec.add(\lok, \midi);
Spec.add(\hik, \midi);
Spec.add(\delay,[0.0001, 1, 3]);
Spec.add(\decay,[0.01, 10, 3]);
)

)
(
// All of that work allows the very simple creation of the NodeProxy and gui
~n = NodeProxy.audio(s,2).play;
~n[0] = f;									// NodeProxy treats the function as a synthesis function
~n.gui;											//numerical view resolution is only to 0.01
)

)

///another example!

Spec.add(\period, ControlSpec( 0.01, 100, 'exp',0, 30) );
Spec.add(\low, ControlSpec( -48, 100, 0, 0, -24) );
Spec.add(\hi, ControlSpec( -48, 100, 0, 0, 96) );
Spec.add(\interval, ControlSpec( -24, 24, 0, 1, 7) );


Ndef(\cowell, {|period = 30, low = -24, hi = 96, interval = 7 |
	var midiKey = LFSaw.kr(1/period, 1).range(low, hi);
	LFPulse.ar ((midiKey + [0,interval]).midicps) * 0.1
}).gui;
