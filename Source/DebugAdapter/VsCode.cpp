#include "VsCode.h"
#include <iostream>

void VsCode::Send(const JSON::ValueRef &data)
{
    auto serialized = data.Serialize();
    std::cout << "Content-Length: " << serialized.length() << "\r\n\r\n";
    std::cout.write(serialized.data(), serialized.size());
    std::cout.flush();
}

JSON::Value VsCode::Recv()
{
    std::string content_len_str;
    std::getline(std::cin, content_len_str);
    auto pos = content_len_str.find_last_of(' ');
    auto content_len = std::stol(content_len_str.substr(pos + 1));
    std::getline(std::cin, content_len_str);

    std::string buffer;
    buffer.resize(content_len, ' ');
    std::cin.read(&buffer[0], content_len);

    return JSON::Value::Parse(buffer);
}

