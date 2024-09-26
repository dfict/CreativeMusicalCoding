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

~daxos = Array.new;
~folder = Pathname.new("/Users/danielfishkin/gitprojects/
CreativeMusicalCoding/class4/daxos/")

(
~folder.entries.do({
	arg path;
	~daxos = ~daxos.add(Buffer.read(s, path.fullPath));
});
)


~daxos;
~daxos.at(21).play;

~daxos[20].play;


