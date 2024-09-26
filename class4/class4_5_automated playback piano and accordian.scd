
// Let's get some other sound files into buffers
~dirpath = "piano and accordian/*".resolveRelative;
~filepaths = ~dirpath.pathMatch;
~buffers = ~filepaths.collect { |path|  Buffer.read(s, path) };

Now, ~buffers is an array object that contains Buffer objects that represent soundfiles downloaded by the server.
Arrays respond to the message 'choose' by returning one of their elements at random
So, we can use that method to play a random sound file

~np = NodeProxy.audio(s, 2).play;
~np.play;
~np.clear;

(~np[0] = {
	var buf = ~buffers.choose;
	var rate = 1;
			Pan2.ar(
				PlayBuf.ar(buf.numChannels,buf.bufnum, rate: rate,  doneAction: 2),
				Rand(-1.0, 1.0)
			)
		};
)

(~np[0] = {
	var buf = ~buffers.choose;
	var rate = (0..12).choose.midiratio;// It randomly selects an integer between 0 and 12.It then converts this integer to a frequency ratio as if it were a MIDI note number difference.
	var amp = 0.1;
			Pan2.ar(
				PlayBuf.ar(buf.numChannels,buf.bufnum, rate: rate,  doneAction: 2) * amp,
				Rand(-1.0, 1.0)
			)
		};
)


(~np[0] = {
	var buf = ~buffers.at(1);
	var rate = (0..2).choose.midiratio;
	var amp = rrand(-40, 0).dbamp;
			Pan2.ar(
				PlayBuf.ar(buf.numChannels,buf.bufnum, rate: rate,  doneAction: 2) * amp,
				Rand(-1.0, 1.0)
			)
		};
)



Now, what we would like to do is make a shuffle play program.
To do that, we need a means of choosing and playing a soundfile, waiting until it finishes playing and then doing that again.
Buffers respond to the 'duration' method with their duration.

But, to play something, wait and play something again, we need a souped up form of a function provided by the class Task
and the timing facilities of the class TempoClock.

A Task is a function that can stop running using the method 'wait' and then resume running immediately after
wherever that 'wait' in the function definition.  A TempoClock provides the actual timing control to run the task
wait the appropriate amount of time and rerun it.

Here is a simple example:

~np[0] = Task({
	"1 mississippi".postln; 1 .wait;
	"2 mississippi".postln; 1 .wait;
	"3 mississippi".postln; 1 .wait;
	"4 mississippi".postln; 1 .wait;
})



// Test the function
~np[0].value(5).play;

~np[1] =  Task({ var buf, rate, amp;
	 buf =  ~buffers.choose;
	 rate = (0..12).choose.midiratio;
	 amp = 0.1;
	~np[0] = {
			Pan2.ar(
				PlayBuf.ar(buf.numChannels,buf.bufnum, rate: rate,  doneAction: 2) * amp,
				Rand(-1.0, 1.0)
			)
		};
	buf.duration.wait;
	buf =  ~buffers.choose;
   rate = (0..12).choose.midiratio;
		~np[0] = {
			Pan2.ar(
				PlayBuf.ar(buf.numChannels,buf.bufnum, rate: rate,  doneAction: 2) * amp,
				Rand(-1.0, 1.0)
			)
		};
	buf.duration.wait
})

// This is pretty inefficient.
// instead we can use loop to do this autmatically:
~np[1] =  Task({ var buf, rate, amp;
	loop {
		buf =  ~buffers.choose;
		rate = (0..12).choose.midiratio;
		amp = 0.1;
		~np[0] = {
			Pan2.ar(
				PlayBuf.ar(buf.numChannels,buf.bufnum, rate: rate,  doneAction: 2) * amp,
				Rand(-1.0, 1.0)
			)
		};
		buf.duration.wait;
	}
});

Now we would like to create more density.
Happily, NodeProxy has an 'add' method that lets us add in synthesis functions
without replacing others.

In this example the duration of the file is divided by 4, so there tend to be four or more sound files playing at the same time.
Of course the decaying nature of the sound files is important to the sonic restulr

~np[0] = Task({
	var buf;
	"first, the slow section".postln;
	50.do { |i | i;
		buf = ~buffers.choose;
		~np.add({
			Pan2.ar(
				PlayBuf.ar(buf.numChannels,buf.bufnum, rate: (1..12).choose.midiratio, doneAction: 2) * rrand(-40, 0).dbamp,
				Rand(-1.0, 1.0)
			)
		});
		(buf.duration/4).wait
	};
	"now, the speedy section".postln;
	100.do { | i |
	buf = ~buffers.choose;
	~np.add({
			Pan2.ar(
				PlayBuf.ar(buf.numChannels,buf.bufnum, rate: (1..12).choose.midiratio, doneAction: 2) * rrand(-40, 0).dbamp,
				Rand(-1.0, 1.0)
			);
		});
		rrand(0.05, 0.3).wait
	};
	"done!".postln;
});

~np.play

