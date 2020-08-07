#include "Include/VVVFlip.hpp"

#include <algorithm>

using namespace vvvflip;

static auto constexpr MAX_SONGS = 127u;
static auto constexpr MAX_PATH_SIZE = 48u;
static auto constexpr SONG_HEADER_SIZE = 60u;
static auto constexpr FILE_HEADER_SIZE = SONG_HEADER_SIZE * (MAX_SONGS + 1);
static auto constexpr INVALID_SONG = 0x00;
static auto constexpr VALID_SONG = 0x01;

struct SongHeader
{
	char const path[MAX_PATH_SIZE];

	union
	{
		uint32_t start;
		uint32_t mdSize;
		uint32_t fileMdSize;
	};

	union
	{
		uint32_t size;
		uint32_t sumSize;
	};

	bool valid;
};

static std::vector<std::string> const SONG_NAMES =
{
	"Level Complete",
	"Pushing Onwards",
	"Positive Force",
	"Potential For Anything",
	"Passion For Exploring",
	"Pause",
	"Presenting VVVVVV",
	"Plenary",
	"Predestined Fate",
	"ecroF evitisoP",
	"Popular Potpourri",
	"Pipe Dream",
	"Pressure Cooker",
	"Paced Energy",
	"Piercing The Sky",
	"Predestined Fate Remix"
};

static std::vector<std::string> const SONG_FILENAMES =
{
	"0levelcomplete",
	"1pushingonwards",
	"2positiveforce",
	"3potentialforanything",
	"4passionforexploring",
	"5intermission",
	"6presentingvvvvvv",
	"7gamecomplete",
	"8predestinedfate",
	"9positiveforcereversed",
	"10popularpotpourri",
	"11pipedream",
	"12pressurecooker",
	"13pacedenergy",
	"14piercingthesky",
	"predestinedfatefinallevel",
};

//Helpers

inline void initHeader(
	SongHeader* headers,
	uint32_t const index,
	uint32_t const size)
{
	auto& header = headers[index];
	auto realPath = vvvflip::getSongRealPath(index);

	if (!realPath.empty())
	{
		memcpy(
			&header,
			realPath.c_str(),
			std::min(realPath.size(), MAX_PATH_SIZE));
	}

	header.size = size;
	header.valid = true;
}

//Blob

bool Blob::fromBuffer(std::vector<uint8_t> const& data)
{
	auto headers = reinterpret_cast<SongHeader const*>(data.data());
	auto const& md = headers[MAX_SONGS];
	auto p = data.data() + FILE_HEADER_SIZE;

	//Load songs

	for (auto i = 0; i < MAX_SONGS; ++i)
	{
		auto const& header = headers[i];
		auto const size = header.size;

		if (header.valid)
			addSong(std::vector<uint8_t>(p, p + size));

		p += size;
	}

	//Load metadata

	//...it's loaded, trust me

	return !m_Songs.empty();
}

bool Blob::fromStream(std::istream& in)
{
	std::vector<uint8_t> buffer;

	in.seekg(0, std::ios::end);
	buffer.resize(in.tellg());
	in.seekg(0, std::ios::beg);

	in.read(
		reinterpret_cast<char*>(buffer.data()),
		buffer.size());

	return fromBuffer(buffer);
}

bool Blob::fromFile(std::ifstream& file)
{
	if (file.is_open())
		return fromStream(file);

	return false;
}

bool Blob::fromPath(std::wstring const& path)
{
	std::ifstream file(path, std::ios::binary);

	return fromFile(file);
}

bool Blob::fromPath(std::string const& path)
{
	std::ifstream file(path, std::ios::binary);

	return fromFile(file);
}

std::vector<uint8_t> Blob::toBuffer()
{
	std::vector<uint8_t> buffer;
	auto headers = reinterpret_cast<SongHeader*>(buffer.data());

	buffer.resize(FILE_HEADER_SIZE, 0x00);

	//Save songs

	for (auto i = 0u; i < std::min(getSize(), MAX_SONGS); ++i)
	{
		auto& song = getSong(i);

		initHeader(headers, i, song.getSize());

		buffer.insert(
			buffer.end(),
			song.m_Data.begin(),
			song.m_Data.end());
	}

	//Save metadata

	//...or not

	return buffer;
}

bool Blob::toStream(std::ostream& out)
{
	auto buffer = toBuffer();

	if (buffer.size())
	{
		out.write(
			reinterpret_cast<char*>(buffer.data()),
			buffer.size());

		return out.good();
	}

	return false;
}

bool Blob::toFile(std::ofstream& file)
{
	if (file.is_open())
		return toStream(file);

	return false;
}

bool Blob::toPath(std::wstring const& path)
{
	std::ofstream file(path, std::ios::binary);

	return toFile(file);
}

bool Blob::toPath(std::string const& path)
{
	std::ofstream file(path, std::ios::binary);

	return toFile(file);
}

//vvvflip

std::string vvvflip::getSongRealName(uint32_t const index)
{
	std::string name;

	if (index < SONG_NAMES.size())
		name = SONG_NAMES[index];

	return name;
}

std::string vvvflip::getSongRealFileName(uint32_t const index)
{
	std::string filename;

	if (index < SONG_FILENAMES.size())
		filename = SONG_FILENAMES[index];

	return filename;
}

std::string vvvflip::getSongRealPath(uint32_t const index)
{
	std::string path;

	if (index < SONG_FILENAMES.size())
		path = "data/music/" + SONG_FILENAMES[index] + ".ogg";

	return path;
}

//C bindings

inline Blob* createBlob()
{
	return new Blob();
}

inline void destroyBlob(Blob* p)
{
	if (p)
		delete p;
}

inline Song* castSongFrom(song_t p)
{
	return reinterpret_cast<Song*>(p);
}

inline blob_t castBlobTo(Blob* p)
{
	return reinterpret_cast<blob_t>(p);
}

inline Blob* castBlobFrom(blob_t p)
{
	return reinterpret_cast<Blob*>(p);
}

void vvvflip::vvvflip_free(void* p)
{
	delete[] p;
}

uint8_t* vvvflip::song_getData(song_t const song)
{
	auto s = castSongFrom(song);

	if (s)
	{
		auto buffer = s->getData();

		if (buffer.size())
		{
			auto p = new uint8_t[buffer.size()];

			memcpy(
				p,
				buffer.data(),
				buffer.size());

			return p;
		}
	}

	return nullptr;
}

size_t vvvflip::song_getSize(song_t const song)
{
	auto s = castSongFrom(song);

	return s
		? s->getSize()
		: 0;
}

int vvvflip::song_setData(
	song_t song,
	uint8_t* p,
	size_t const size)
{
	auto s = castSongFrom(song);

	if (s)
	{
		s->setData(std::vector<uint8_t>(
			p,
			p + size));

		return 1;
	}

	return 0;
}

blob_t vvvflip::blob_create()
{
	return castBlobTo(createBlob());
}

void vvvflip::blob_destroy(vvvflip::blob_t blob)
{
	destroyBlob(castBlobFrom(blob));
}

blob_t vvvflip::blob_fromBuffer(
	uint8_t const* p,
	size_t const size)
{
	auto blob = createBlob();

	if (blob)
	{
		if (!blob->fromBuffer(std::vector<uint8_t>(
			p,
			p + size)))
		{
			destroyBlob(blob);
			blob = nullptr;
		}
	}

	return castBlobTo(blob);
}

blob_t vvvflip::blob_fromFile(FILE* file)
{
	auto blob = createBlob();

	if (blob)
	{
		std::ifstream h(file);

		if (!blob->fromFile(h))
		{
			destroyBlob(blob);
			blob = nullptr;
		}
	}

	return castBlobTo(blob);
}

blob_t vvvflip::blob_fromWidePath(wchar_t const* path)
{
	auto blob = createBlob();

	if (blob)
	{
		if (!blob->fromPath(std::wstring(path)))
		{
			destroyBlob(blob);
				blob = nullptr;
		}
	}

	return castBlobTo(blob);
}

blob_t vvvflip::blob_fromAnsiPath(char const* path)
{
	auto blob = createBlob();

	if (blob)
	{
		if (!blob->fromPath(std::string(path)))
		{
			destroyBlob(blob);
			blob = nullptr;
		}
	}

	return castBlobTo(blob);
}

uint8_t* vvvflip::blob_toBuffer(blob_t blob)
{
	auto b = castBlobFrom(blob);

	if (b)
	{
		auto buffer = b->toBuffer();

		if (buffer.size())
		{
			auto p = new uint8_t[buffer.size()];

			memcpy(
				p,
				buffer.data(),
				buffer.size());

			return p;
		}
	}

	return nullptr;
}

int vvvflip::blob_toFile(
	blob_t const blob,
	FILE* file)
{
	auto b = castBlobFrom(blob);

	if (b)
	{
		std::ofstream h(file);
		return b->toFile(h);
	}

	return 0;
}

int vvvflip::blob_toWidePath(
	blob_t const blob,
	wchar_t const* path)
{
	auto b = castBlobFrom(blob);

	return b
		? b->toPath(path)
		: 0;
}

int vvvflip::blob_toAnsiPath(
	blob_t const blob,
	char const* path)
{
	auto b = castBlobFrom(blob);

	return b
		? b->toPath(path)
		: 0;
}

song_t vvvflip::blob_addSong(
	blob_t blob,
	uint8_t const* p,
	size_t const size)
{
	auto b = castBlobFrom(blob);

	if (b)
	{
		auto& song = b->addSong(std::vector<uint8_t>(
			p,
			p + size));

		return reinterpret_cast<song_t>(&song);
	}

	return nullptr;
}

int vvvflip::blob_removeSong(
	blob_t blob,
	uint32_t const index)
{
	auto b = castBlobFrom(blob);

	return b
		? b->removeSong(index)
		: 0;
}

song_t vvvflip::blob_getSong(
	blob_t blob,
	uint32_t const index)
{
	auto b = castBlobFrom(blob);

	if (b)
	{
		try
		{
			auto& song = b->getSong(index);
			return reinterpret_cast<song_t>(&song);
		}
		catch (std::out_of_range const&) {}
	}

	return nullptr;
}

size_t vvvflip::blob_getSize(blob_t blob)
{
	auto b = castBlobFrom(blob);

	return b
		? b->getSize()
		: 0;
}