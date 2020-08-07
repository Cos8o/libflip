#ifndef _VVVFLIP_HPP
#define _VVVFLIP_HPP

#include "VVVFlip.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

namespace vvvflip
{
	class Song
	{
		friend class Blob;
	protected:
		std::string m_Name;
		std::string m_Filename;
		std::string m_Notes;
		std::vector<uint8_t> m_Data;

		Song() {}
	public:
		explicit Song(Song const&) = default;

		std::string getName() const
		{
			return m_Name;
		}

		std::string getFilename() const
		{
			return m_Filename;
		}

		std::string getNotes() const
		{
			return m_Notes;
		}

		std::vector<uint8_t> getData() const
		{
			return m_Data;
		}

		size_t getSize() const
		{
			return m_Data.size();
		}

		void setName(std::string const& name)
		{
			m_Name = name;
		}

		void setFilename(std::string const& filename)
		{
			m_Filename = filename;
		}

		void setNotes(std::string const& notes)
		{
			m_Notes = notes;
		}

		void setData(std::vector<uint8_t> const& data)
		{
			m_Data = data;
		}
	};

	class VVVFLIP_DECL Blob
	{
		std::vector<Song> m_Songs;
	public:
		Song& addSong(std::vector<uint8_t> const& data = {})
		{
			Song song;

			song.setData(data);
			m_Songs.push_back(song);

			return song;
		}

		bool removeSong(uint32_t const index)
		{
			if (index < m_Songs.size())
			{
				m_Songs.erase(m_Songs.begin() + index);
				return true;
			}

			return false;
		}

		Song& getSong(uint32_t const index)
		{
			if (index < m_Songs.size())
				return m_Songs[index];

			throw std::out_of_range("Out of range exception");
		}

		Song const& getSong(uint32_t const index) const
		{
			if (index < m_Songs.size())
				return m_Songs[index];

			throw std::out_of_range("Out of range exception");
		}

		size_t getSize()
		{
			return m_Songs.size();
		}

		std::vector<Song>::iterator begin()
		{
			return m_Songs.begin();
		}

		std::vector<Song>::const_iterator begin() const
		{
			return m_Songs.begin();
		}

		std::vector<Song>::iterator end()
		{
			return m_Songs.end();
		}

		std::vector<Song>::const_iterator end() const
		{
			return m_Songs.end();
		}

		//Input

		bool fromBuffer(std::vector<uint8_t> const& data);
		bool fromStream(std::istream& in);
		bool fromFile(std::ifstream& file);
		bool fromPath(std::wstring const& path);
		bool fromPath(std::string const& path);

		//Output

		std::vector<uint8_t> toBuffer();
		bool toStream(std::ostream& out);
		bool toFile(std::ofstream& file);
		bool toPath(std::wstring const& path);
		bool toPath(std::string const& path);

		//Operators

		operator bool() const
		{
			return !m_Songs.empty();
		}

		void operator+=(std::vector<uint8_t> const& data)
		{
			addSong(data);
		}

		Song& operator[](uint32_t const index)
		{
			return getSong(index);
		}

		Song const& operator[](uint32_t const index) const
		{
			return getSong(index);
		}
	};

	inline Blob fromBuffer(std::vector<uint8_t> const& data)
	{
		Blob blob;

		blob.fromBuffer(data);

		return blob;
	}

	inline Blob fromStream(std::istream& in)
	{
		Blob blob;

		blob.fromStream(in);

		return blob;
	}

	inline Blob fromFile(std::ifstream& file)
	{
		Blob blob;

		blob.fromFile(file);

		return blob;
	}

	inline Blob fromPath(std::wstring const& path)
	{
		Blob blob;

		blob.fromPath(path);

		return blob;
	}

	inline Blob fromPath(std::string const& path)
	{
		Blob blob;

		blob.fromPath(path);

		return blob;
	}

	VVVFLIP_DECL inline std::string getSongRealName(uint32_t const index);
	VVVFLIP_DECL inline std::string getSongRealFileName(uint32_t const index);
	VVVFLIP_DECL inline std::string getSongRealPath(uint32_t const index);
}

#endif /* _VVVFLIP_HPP */