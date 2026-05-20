#include "IOFd.hpp"
#include "status.hpp"
#include <cctype>
#include <map>
#include <string>
#include <vector>
#include <cctype>

static int	check_header(const string_map& headers);
static void	no_version(const IOFd& iofd, int& status);
static int	check_last_line(const std::string& last_line);

// here depending on the version, it should exepct no header 
// maybe header should be in key-value pairs ? like:
// std::map<std::string, std::string>, but a map is annoying because it
// does not allow duplicate keys, and keys can be duplicate
// so maybe more an std::vector of std::pairs of std::string, std::string
// maybe this is better this way ? i think we will go this way
// but its so much verbose ...
// or a std::map<std::string, std::vector<std::string>>
// this ones gives flexibility, but also very verbose
// so i do not really know ??
// so getting here, if version is empty, then it is a 'simple-request'
// if it is a simple request, then the method should be 'GET'
// and if it is the case, the request is complete, and should 
// be processed using only method and uri, then closed and all other
// ressources send should be ignored

void	IOFd::process_header(std::string& buf, size_t& start)
{
	// std::cout << "in process header\n";
	std::vector<std::string> delims;
	size_t	delimPosition;
	size_t	until;

	delims.reserve(2);
	delims.push_back("\n");
	delims.push_back("\r\n");

	if (this->version == "")
	{
		return (no_version(*this, this->status));
	}

	// goal, fill in the map
	static std::string last_line;
	while (start < buf.size())
	{
		if (check_last_line(last_line) != SUCCESS)
		{
			return (send_bad_request(this->fd, this->status));
		}

		if (last_line.size() > 0 && last_line[last_line.size() - 1] == '\n')
		{
			add_line_headers(last_line, this->header);
			last_line.clear();
		}

		delimPosition = getDelimPosition(buf, start, delims);
		if (delimPosition == std::string::npos) // no \r\n
		{
			until = buf.size();
			last_line.append(buf, start, until - start);
		}
		else 
		{
			if (buf[delimPosition] == '\r')
				until = delimPosition + 2;
			else
				until = delimPosition + 1;

			if (delimPosition == start && last_line.size() == 0)
			{
				if (check_header(this->header) != SUCCESS)
					return (send_bad_request(this->fd, this->status));
				this->state++;
				start = until;
				return;
			}
			else
			{
				last_line.append(buf, start, until - start);
			}
			start = until;
		}
	}
}

static void	no_version(const IOFd& iofd, int& status)
{
		if (iofd.getMethod() == "GET")
		{
			// do stuff to stop parsing incoming data,
			// and process the request using only information
			// in 'method' and in 'uri'
			// return ...
		}
		else
		{
			return (send_bad_request(iofd.getFd(), status));
		}
		return ;
}

static int	check_header(const std::map<std::string, std::vector<std::string> > headers)
{
	(void) headers;
	return (SUCCESS);
}

#include "abnf.hpp"

# define ALLOWED_CHAR_KEY   ABNF_ALPHA ABNF_DIGIT "_;.,\\/\"'?!(){}[]@<>=-+*#$&`|~^%"
# define ALLOWED_CHAR_VALUE ABNF_ALPHA ABNF_DIGIT "_;.,\\/\"'?!(){}[]@<>=-+*#$&`|~^%" " "

// "coucou\r\n" -> valid
// coucou\r:\r\n -> invalid
// coucou:yo\ryo\r\n -> invalid
static int check_last_line(const std::string& last_line)
{
	size_t	colonPosition = last_line.find(":");
	size_t	key_size;
	size_t	value_start;
	size_t	value_end;
	
	if (colonPosition == std::string::npos)
	{
		key_size = last_line.size();
		value_start = 0;
		value_end = 0;
	}
	else
	{
		key_size = colonPosition;
		value_start = colonPosition + 1;
		value_end = last_line.find_last_not_of("\r\n");
	}

	std::string key(last_line, 0, key_size);
	std::string value(last_line, value_start, value_end - value_start);

	if (key.find_first_not_of(ALLOWED_CHAR_KEY) != std::string::npos)
		return (FAILURE);
	if (value.find_first_not_of(ALLOWED_CHAR_VALUE) != std::string::npos)
		return (FAILURE);
	return (SUCCESS);
}

void	lowering(std::string& line);

static void add_line_headers(std::string& line,  string_map& headers)
{
	size_t		colonPosition = line.find(":");
	std::string	key;

	if (colonPosition == std::string::npos)
	{
		key.append(line, 0, line.size());
		lowering(key);
		headers[key].push_back("");
	}
	else
	{
		size_t	value_begin = line.find_first_not_of(" \r\n", colonPosition + 1);
		size_t	value_end = line.find_last_not_of(" \r\n");
		key.append(line, 0, colonPosition);
		lowering(key);
		if (value_end == std::string::npos || value_begin == std::string::npos)
			headers[key].push_back("");
		else
			headers[key].push_back(std::string(line, value_begin, value_end - value_begin + 1));
	}
}

void	lowering(std::string& line)
{
	for (size_t i = 0; i < line.size(); i++)
	{
		line[i] = std::tolower(line[i]);
	}
}
