#include "Include/VVVFlip.hpp"

bool writeToFile(
	std::string const& path,
	std::vector<uint8_t> const& data)
{
	std::ofstream h(path, std::ios::binary);

	if (h.is_open())
	{
		h.write(
			reinterpret_cast<char const*>(data.data()),
			data.size());

		return true;
	}

	return false;
}

int main(
	int const argc,
	char const* const* argv)
{
	if (argc > 2)
	{
		auto blob = vvvflip::fromPath(argv[1]);

		if (blob)
		{
			for (auto i = 0u; i < blob.getSize(); ++i)
			{
				if (!writeToFile(
					std::to_string(i) + ".ogg",
					blob[i].getData()))
				{
					std::cout << "WARNING: Could not dump song n. " << i << '\n';
				}
			}
		}
		else
		{
			std::cout << "ERROR: Could not open binary file.\n";
			return 1;
		}
	}
	else
		std::cout << "Usage: " << argv[0] << " [PATH TO BINARY FILE]\n";

	return 0;
}