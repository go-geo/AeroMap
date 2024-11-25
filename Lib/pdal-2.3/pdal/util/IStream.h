/******************************************************************************
* Copyright (c) 2014, Andrew Bell
****************************************************************************/

#pragma once

#include <sys/types.h>
#include <stdint.h>

#include <cassert>
#include <fstream>
#include <memory>
#include <stack>
#include <vector>
#include <cstring>

#include "endian.h"

namespace pdal
{
	class IStreamMarker;

	/**
	  Stream wrapper for input of binary data.
	*/
	class IStream
	{
	public:
		//ABELL - Should prevent copy construction.
		//ABELL - Should enable construction from rvalue ref.
		//ABELL - Should provide an operator << (..., string) that delegates
		//  to std::istream
		/**
		  Default constructor.
		*/
		IStream() : m_stream(NULL), m_fstream(NULL)
		{}

		/**
		  Construct an IStream from a filename.

		  \param filename  File from which to read.
		*/
		IStream(const std::string& filename) :
			m_stream(NULL), m_fstream(NULL)
		{
			open(filename);
		}

		/**
		  Construct an IStream from an input stream pointer.

		  \param stream  Stream from which to read.
		*/
		IStream(std::istream* stream) : m_stream(stream), m_fstream(NULL)
		{}

		~IStream()
		{
			delete m_fstream;
		}

		/**
		  Open a file to extract.

		  \param filename  Filename.
		  \return  -1 if a stream is already assigned, 0 otherwise.
		*/
		int open(const std::string& filename)
		{
			if (m_stream)
				return -1;
			m_stream = m_fstream = new std::ifstream(filename,
				std::ios_base::in | std::ios_base::binary);
			return 0;
		}

		/**
		  Close the underlying stream.
		*/
		void close()
		{
			delete m_fstream;
			m_fstream = NULL;
			m_stream = NULL;
		}

		/**
		  Return the state of the stream.

		  \return  The state of the underlying stream.
		*/
		operator bool()
		{
			return (bool)(*m_stream);
		}

		/**
		  Seek to a position in the underlying stream.

		  \param pos  Position to seek to,
		*/
		void seek(std::streampos pos)
		{
			m_stream->seekg(pos, std::istream::beg);
		}


		/**
		  Seek to an offset from a specified position.

		  \param off  Offset.
		  \param way  Absolute position for offset (beg, end or cur)
		*/
		void seek(std::streampos off, std::ios_base::seekdir way)
		{
			m_stream->seekg(off, way);
		}

		/**
		  Skip relative to the current position.

		  \param offset  Offset from the current position.
		*/
		void skip(std::streamoff offset)
		{
			m_stream->seekg(offset, std::istream::cur);
		}

		/**
		  Determine the position of the get pointer.

		  \return  Current get position.
		*/
		std::streampos position() const
		{
			return m_stream->tellg();
		}

		/**
		  Determine if the underlying stream is good.

		  \return  Whether the underlying stream is good.
		*/
		bool good() const
		{
			return m_stream->good();
		}

		/**
		  Fetch a pointer to the underlying stream.

		  \return  Pointer to the underlying stream.
		*/
		std::istream* stream()
		{
			return m_stream;
		}

		/**
		  Temporarily push a stream to read from.

		  \param strm  New stream to read from.
		*/
		void pushStream(std::istream* strm)
		{
			m_streams.push(m_stream);
			m_stream = strm;
		}

		/**
		  Pop the current stream and return it.  The last stream on the stack
		  cannot be popped.

		  \return  Pointer to the popped stream.
		*/
		std::istream* popStream()
		{
			// Can't pop the last stream for now.
			if (m_streams.empty())
				return nullptr;
			std::istream* strm = m_stream;
			m_stream = m_streams.top();
			m_streams.pop();
			return strm;
		}

		/**
		  Fetch data from the stream into a string.  NOTE - Stops when
		  a null byte is encountered.  Use a buffer/vector reader to
		  read data with embedded nulls.

		  \param s  String to fill.
		  \param size  Maximum number of bytes to extract.
		*/
		void get(std::string& s, size_t size)
		{
			// Could do this by appending to a string with a stream, but this
			// is probably fast enough for now (there's only a simple increment
			// to advance an istream iterator, which you'd have to call in a loop).

			// Zero-fill for null termination and to avoid reading uninitiallized
			// memory when, for example, trying to read an empty file.
			std::vector<char> buf(size + 1, 0);
			m_stream->read(buf.data(), size);
			s = buf.data();
		}

		/**
		  Fetch data from the stream into a vector of char.

		  \param buf  Buffer to fill.
		*/
		void get(std::vector<char>& buf)
		{
			assert(buf.size() != 0);
			m_stream->read((char*)&buf[0], buf.size());
		}

		/**
		  Fetch data from the stream into a vector of unsigned char.

		  \param buf  Buffer to fill.
		*/
		void get(std::vector<unsigned char>& buf) {
			assert(buf.size() != 0);
			m_stream->read((char*)&buf[0], buf.size());
		}

		/**
		  Fetch data from the stream into the specified buffer of char.

		  \param buf  Buffer to fill.
		  \param size  Number of bytes to extract.
		*/
		void get(char* buf, size_t size)
		{
			m_stream->read(buf, size);
		}

		/**
		  Fetch data from the stream into the specified buffer of unsigned char.

		  \param buf  Buffer to fill.
		  \param size  Number of bytes to extract.
		*/
		void get(unsigned char* buf, size_t size)
		{
			m_stream->read((char*)buf, size);
		}

	protected:
		std::istream* m_stream;
		std::ifstream* m_fstream; // Dup of above to facilitate cleanup.

	private:
		std::stack<std::istream*> m_streams;
	};

	/**
	  Stream wrapper for input of binary data that converts from little-endian
	  to host ordering.
	*/
	class ILeStream : public IStream
	{
	public:
		/**
		  Default constructor.
		*/
		ILeStream()
		{}

		/**
		  Constructor that opens the file and maps it to a stream.

		  \param filename  Filename.
		*/
		ILeStream(const std::string& filename) : IStream(filename)
		{}

		/**
		  Constructor that maps to a provided stream.

		  \param stream  Stream to extract from.
		*/
		ILeStream(std::istream* stream) : IStream(stream)
		{}

		/**
		  Extract an unsigned byte from the stream.

		  \param v  unsigned byte to populate
		  \return  This stream.
		*/
		ILeStream& operator >> (uint8_t& v)
		{
			v = (uint8_t)m_stream->get();
			return *this;
		}

		/**
		  Extract an unsigned byte from the stream.

		  \param v  unsigned byte to populate
		  \return  This stream.
		*/
		ILeStream& operator >> (int8_t& v)
		{
			v = (int8_t)m_stream->get();
			return *this;
		}

		/**
		  Extract an unsigned short from the stream.

		  \param v  unsigned short to populate
		  \return  This stream.
		*/
		ILeStream& operator >> (uint16_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			return *this;
		}

		/**
		  Extract an short from the stream.

		  \param v  short to populate
		  \return  This stream.
		*/
		ILeStream& operator >> (int16_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			return *this;
		}

		/**
		  Extract an unsigned int from the stream.

		  \param v  unsigned int to populate
		  \return  This stream.
		*/
		ILeStream& operator >> (uint32_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			return *this;
		}

		/**
		  Extract an int from the stream.

		  \param v  int to populate
		  \return  This stream.
		*/
		ILeStream& operator >> (int32_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			return *this;
		}

		/**
		  Extract an unsigned long int from the stream.

		  \param v  unsigned long int to populate
		  \return  This stream.
		*/
		ILeStream& operator >> (uint64_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			return *this;
		}

		/**
		  Extract a long int from the stream.

		  \param v  long int to populate
		  \return  This stream.
		*/
		ILeStream& operator >> (int64_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			return *this;
		}

		/**
		  Extract a float from the stream.

		  \param v  float to populate
		  \return  This stream.
		*/
		ILeStream& operator >> (float& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			uint32_t tmp = (*(uint32_t*)(&v));
			std::memcpy(&v, &tmp, sizeof(tmp));
			return *this;
		}

		/**
		  Extract a double from the stream.

		  \param v  double to populate
		  \return  This stream.
		*/
		ILeStream& operator >> (double& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			uint64_t tmp = (*(uint64_t*)(&v));
			std::memcpy(&v, &tmp, sizeof(tmp));
			return *this;
		}
	};


	/**
	  Stream wrapper for input of binary data that converts from big-endian
	  to host ordering.
	*/
	class IBeStream : public IStream
	{
	public:
		/**
		  Default constructor.
		*/
		IBeStream()
		{}

		/**
		  Constructor that opens the file and maps it to a stream.

		  \param filename  Filename.
		*/
		IBeStream(const std::string& filename) : IStream(filename)
		{}

		/**
		  Constructor that maps to a provided stream.

		  \param stream  Stream to extract from.
		*/
		IBeStream(std::istream* stream) : IStream(stream)
		{}

		/**
		  Extract an unsigned byte from the stream.

		  \param v  unsigned byte to populate
		  \return  This stream.
		*/
		IBeStream& operator >> (uint8_t& v)
		{
			v = (uint8_t)m_stream->get();
			return *this;
		}

		/**
		  Extract an unsigned byte from the stream.

		  \param v  unsigned byte to populate
		  \return  This stream.
		*/
		IBeStream& operator >> (int8_t& v)
		{
			v = (int8_t)m_stream->get();
			return *this;
		}

		/**
		  Extract an unsigned short from the stream.

		  \param v  unsigned short to populate
		  \return  This stream.
		*/
		IBeStream& operator >> (uint16_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			v = ByteSwap(v);
			return *this;
		}

		/**
		  Extract an short from the stream.

		  \param v  short to populate
		  \return  This stream.
		*/
		IBeStream& operator >> (int16_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			v = (int16_t)ByteSwap((uint16_t)v);
			return *this;
		}

		/**
		  Extract an unsigned int from the stream.

		  \param v  unsigned int to populate
		  \return  This stream.
		*/
		IBeStream& operator >> (uint32_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			v = ByteSwap(v);
			return *this;
		}

		/**
		  Extract an int from the stream.

		  \param v  int to populate
		  \return  This stream.
		*/
		IBeStream& operator >> (int32_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			v = (int32_t)ByteSwap((uint32_t)v);
			return *this;
		}

		/**
		  Extract an unsigned long int from the stream.

		  \param v  unsigned long int to populate
		  \return  This stream.
		*/
		IBeStream& operator >> (uint64_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			v = ByteSwap(v);
			return *this;
		}

		/**
		  Extract a long int from the stream.

		  \param v  long int to populate
		  \return  This stream.
		*/
		IBeStream& operator >> (int64_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			v = (int64_t)ByteSwap((uint64_t)v);
			return *this;
		}

		/**
		  Extract a float from the stream.

		  \param v  float to populate
		  \return  This stream.
		*/
		IBeStream& operator >> (float& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			uint32_t tmp = ByteSwap(*(uint32_t*)(&v));
			std::memcpy(&v, &tmp, sizeof(tmp));
			return *this;
		}

		/**
		  Extract a double from the stream.

		  \param v  double to populate
		  \return  This stream.
		*/
		IBeStream& operator >> (double& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			uint64_t tmp = ByteSwap(*(uint64_t*)(&v));
			std::memcpy(&v, &tmp, sizeof(tmp));
			return *this;
		}
	};


	/**
	  Stream wrapper for input of binary data that converts from either
	  little-endian or big-endian to host ordering, depending on object
	  settings.
	*/
	class ISwitchableStream : public IStream
	{
	public:
		static const bool DefaultIsLittleEndian = true;

		ISwitchableStream() : m_isLittleEndian(DefaultIsLittleEndian)
		{}

		ISwitchableStream(const std::string& filename)
			: IStream(filename)
			, m_isLittleEndian(DefaultIsLittleEndian)
		{}

		ISwitchableStream(std::istream* stream)
			: IStream(stream)
			, m_isLittleEndian(DefaultIsLittleEndian)
		{}

		bool isLittleEndian() const
		{
			return m_isLittleEndian;
		}
		void switchToLittleEndian()
		{
			m_isLittleEndian = true;
		}
		void switchToBigEndian()
		{
			m_isLittleEndian = false;
		}

		ISwitchableStream& operator>>(uint8_t& v)
		{
			v = (uint8_t)m_stream->get();
			return *this;
		}

		ISwitchableStream& operator>>(int8_t& v)
		{
			v = (int8_t)m_stream->get();
			return *this;
		}

		ISwitchableStream& operator>>(uint16_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			v = isLittleEndian() ? v : ByteSwap(v);
			return *this;
		}

		ISwitchableStream& operator>>(int16_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			v = isLittleEndian() ? v : (int16_t)ByteSwap((uint16_t)v);
			return *this;
		}

		ISwitchableStream& operator>>(uint32_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			v = isLittleEndian() ? v : ByteSwap(v);
			return *this;
		}

		ISwitchableStream& operator>>(int32_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			v = isLittleEndian() ? v : (int32_t)ByteSwap((uint32_t)v);
			return *this;
		}

		ISwitchableStream& operator>>(uint64_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			v = isLittleEndian() ? v : ByteSwap(v);
			return *this;
		}

		ISwitchableStream& operator>>(int64_t& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			v = isLittleEndian() ? v : (int64_t)ByteSwap((uint64_t)v);
			return *this;
		}

		ISwitchableStream& operator>>(float& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			uint32_t tmp = isLittleEndian() ? (*(uint32_t*)(&v)) : ByteSwap(*(uint32_t*)(&v));
			std::memcpy(&v, &tmp, sizeof(tmp));
			return *this;
		}

		ISwitchableStream& operator>>(double& v)
		{
			m_stream->read((char*)&v, sizeof(v));
			uint64_t tmp = isLittleEndian() ? (*(uint64_t*)(&v)) : ByteSwap(*(uint64_t*)(&v));
			std::memcpy(&v, &tmp, sizeof(tmp));
			return *this;
		}

	private:
		bool m_isLittleEndian;
	};


	/// Stream position marker with rewinding support.
	class IStreamMarker
	{
	public:
		IStreamMarker(IStream& stream) : m_stream(stream)
		{
			m_pos = m_stream.position();
		}

		void rewind()
		{
			m_stream.seek(m_pos);
		}

	private:
		std::streampos m_pos;
		IStream& m_stream;
		IStreamMarker(const IStreamMarker&);
		IStreamMarker& operator=(const IStreamMarker&); // not implemented
	};
}
