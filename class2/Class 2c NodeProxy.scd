Basic elements of the Server

Unit Generators -  UGen
	Unit Generators are the building blocks used for sound generation within the server framework. They are represented with UGen classes in the language

SynthDefs - SynthDef
	SynthDefs specify different interconnections of Unit Generators that actually produce sound. They are represented by SynthDef objects in the language. Inside the SynthDef,  instances of the UGen classes represent the different Unit Generators used to produce sound.

Synths - Synth
	Synths are instances of SynthDefs that actually produce sound on the server.  In the language,
Synth objects are often used to represent those synths.

Groups - Group
	Groups are to Synths as folders are to files.  They provide a way to organize synths in a tree like structure.  This structure determines the order in which the Synths compute audio.  This is important as sound processing synths need to run after the sound generating synths they process.

Buses - Bus
	Buses are how Synths move sound to and from the outside world and other Synths.
	Buses are always monophonic and are identified by a numerical index that begins at 0.    There are output buses to send audio to computer outputs, input buses to get audio from computer inputs  and private buses for synths to share audio amongst themselves.  The main stereo output is provided by buses 0 and 1.  With stereo input, the numbering would be:
		0, 1 - output buses
		2, 3 - input buses
		4...  - private buses


-----------------------------------------------------------------------------------------------------------
NodeProxy is an organizing tool that manages most of these details.
Creating a NodeProxy involves specifying the server it runs on,  how many
channels it uses and whether its synths run at audio or control rate.
NodeProxy.audio(server, numChannels)
NodeProxy.control(server, numChannels)

NodeProxy uses an Order to organize its constituent sounds.
New sounds can be inserted using the 'put' method or its square bracket equivalent.
A sound can be removed by inserting 'nil' in its place.
NodeProxy has a 'fadeTime' method that sets a cross fade time for replacement and removal.


'put'(index, obj, channelOffset, extraArgs)
	index - can be any old Float or Integer
	obj - the sound producer (a synthesis function, synthdef, pattern, etc)
	channelOffset - select which of the NodeProxy's internal buses should be used
	extraArgs - we will discuss later

Here is an example;
(
n = NodeProxy.audio(s, 2);  // audio rate NP running on the default server with two internal channels
n.fadeTime = 4;
n.play;

n.put(0, { SinOsc.ar([3000, 3100]) * LFTri.kr(1/10, 3).range(-90, -30).dbamp });
n.put(1, { SinOsc.ar([3030, 3060]) * LFTri.kr(1/5.4, 3).range(-60, -30).dbamp });
n.put(2, { SinOsc.ar([3030, 3060]/20) * LFTri.kr(1/5.4, 3).range(-60, -30).dbamp });
n.put(3, { LFTri.ar([3030, 3060]/30) * LFTri.kr(1/2.4, 3).range(-60, -10).dbamp });

n.source = nil;
// the square bracket syntax
n[0] = { LFPulse.ar([30, 30.2]) * 0.02 };


One really useful feature of NodeProxy is that you can replace one synthesis function by another:

n[0] = { LFTri.ar([200, 200.1]) * 0.2 };

And the truly magical feature is that you can set a fadeTime to have transitions occur as a crossfade.
n.fadeTime = 10;
n[0] = { LFTri.ar([230, 240.1]) * 0.2 };

And yet another great feature is that it will "wrap" extra channels of output so that they remain audible:
n[0] = { LFTri.ar([230, 240.1, 255, 190]) * 0.2 };
// now we will make a little mess
n[1] = { SinOsc.ar([230, 240.1, 255, 190] * 8) * 0.2 * LFPulse.ar([4,5,6,7]/10, 0, 0.01).lag(0.1) };
n[2] = { SinOsc.ar([230, 240.1, 255, 190] * 8.1) * 0.2 * LFPulse.ar([4,5,6,7]/10.4, 0, 0.01).lag(0.1) };
n[3] = { SinOsc.ar([230, 240.1, 255, 190] * 8.3) * 0.2 * LFPulse.ar([4,5,6,7]/11, 0 , 0.01).lag(0.1) };
n[4] = { SinOsc.ar([230, 240.1, 255, 190] * 8.3) * 0.2 * LFPulse.ar([4,5,6,7]/12, 0, 0.01).lag(0.1) };

n[5] =  \filter -> { | audio |  CombN.ar(audio, 1.4, 1.4, 10) }
n[6] =  \filter -> { | audio |  CombN.ar(audio, 0.04, 0.4, 10)  * SinOsc.ar(50)}
n[7] =  \filter -> { | audio |  CombN.ar(audio, 0.005, 0.4, 10) }
n.source = nil;






n[0] = { LFPulse.ar((0, 0.2 ..10) + 30).sum * 0.02 };
n[0] = { LFPulse.ar((0, 0.2 ..10) + 30, {3.0.rand}.dup(50), 0.1) * 0.02 };
n[0] = { LFPulse.ar((0, 0.2 ..10) + 30, {3.0.rand}.dup(50), 0.01) * 0.02 };
n[0] = { LFPulse.ar((0, 0.2 ..10) + 30, {3.0.rand}.dup(50), 0.001) * 0.02 };
n[0] = { LFPulse.ar((0, 0.2 ..10) + 30, {3.0.rand}.dup(50), LFNoise1.kr(0.1.dup(5)).exprange(0.001, 0.5)) * 0.02 };
n.source = nil;
)

