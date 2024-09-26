~b0 = Buffer.read(s, " the name of your soundfile!");

"/Users/danielfishkin/gitprojects/CreativeMusicalCoding/class4/piano and accordian/YamahaH-C2.aif"

~b0 = Buffer.read(s, "/Users/danielfishkin/gitprojects/CreativeMusicalCoding/highlights/dax_MULTIPHONIC.wav");

~b1 = Buffer.read(s, "/Users/danielfishkin/gitprojects/CreativeMusicalCoding/highlights/Short&Sweet.wav");

~b4 = Buffer.read(s, "/Users/danielfishkin/gitprojects/CreativeMusicalCoding/class4/daxos/giraffe.wav");
~b0.play;
~b1.play;
~b4.play;


~b4.zero;

~b4.free;

//Frames
~b0.numFrames;

//Channels
~b0.numChannels;

//Duration
~b0.duration;

//buffer number--SC's organization system
~b0.bufnum;

~b0.sampleRate;

~b0.query;


// //
// So let's talk about how to load many sound files all at once, instead of having to type them one at a time. There are, in fact, lots of different ways to do this, and most of them involve some kind of iteration. One fairly simple option is to store your sound files in an Array, so that each buffer is addressable by a unique index. For this example I'll use a folder containing a chromatic scale of desk bells.
//
// //And I'll start by creating an empty Array, which will be used to hold our buffers:

~daxo = Array.new;

and I'm also going to use an instance of a class called PathName that acts as a reference to the folder of daxo sounds.

~folder = PathName.new("/Users/danielfishkin/gitprojects/CreativeMusicalCoding/class4/daxos/");

If you send the message "entries" to a PathName, it will return an Array of PathNames representing the soundfiles themselves.

~folder.entries;


(
~folder.entries.do({
	arg path;
	~daxos = ~daxos.add(Buffer.read(s, path.fullPath));
});
)

And that's really all there is to it. We can see that ~bells is now filled with buffers, and we can refer to inidividual buffers using the 'at' method, like this

~daxos;
~daxos.at(0).play;
~daxos.at(3).play;
~daxos.at(7).play;
~daxos.at(12).play;

