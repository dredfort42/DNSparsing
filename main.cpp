#include <iostream>
#include <string>
#include <sstream>

std::string getDNSClass(const int DNSClassCode)
{
    switch (DNSClassCode)
    {
    case 1:
        return "IN"; // Internet
    case 3:
        return "CH"; // Chaos
    case 4:
        return "HS"; // Hesiod
    default:
        return "ERROR";
    }
}

std::string getDNSType(const int DNSTypeCode)
{
    switch (DNSTypeCode)
    {
    case 1:
        return "A"; // IPv4 Address
    case 5:
        return "CNAME"; // Canonical name for an alias
    case 28:
        return "AAAA"; // IPv6 Address
    default:
        return "ERROR";
    }
}

std::string getURL(const std::string &bites)
{
    std::string url;
    bool prefix = false;
    unsigned i = 0;

    if (bites.substr(0, 8) == "00000111")
        i = 8;

    if (bites.substr(i, 8) == "00000011")
    {
        i += 8;
        prefix = true;
    }

    while (prefix)
    {
        for (; i != bites.length() && prefix; i += 8)
        {
            std::string temp = bites.substr(i, 8);

            if (temp == "00001010" || temp == "00000111")
            {
                prefix = false;
                url += ".";
            }
            else
                url += (char)std::stoi(temp, nullptr, 2);
        }
    }

    for (; i != bites.length(); i += 8)
    {
        std::string temp = bites.substr(i, 8);

        if (temp == "00000011")
            url += ".";
        else
            url += (char)std::stoi(temp, nullptr, 2);
    }

    return url + ".";
}

std::string hexToBit(const char c)
{
    switch (toupper(c))
    {
    case '0':
        return "0000";
    case '1':
        return "0001";
    case '2':
        return "0010";
    case '3':
        return "0011";
    case '4':
        return "0100";
    case '5':
        return "0101";
    case '6':
        return "0110";
    case '7':
        return "0111";
    case '8':
        return "1000";
    case '9':
        return "1001";
    case 'A':
        return "1010";
    case 'B':
        return "1011";
    case 'C':
        return "1100";
    case 'D':
        return "1101";
    case 'E':
        return "1110";
    case 'F':
        return "1111";
    default:
        return "ERROR";
    }
}

std::string hexToBites(const std::string &hex)
{
    std::string bites;
    for (unsigned i = 0; i != hex.length(); i++)
    {
        std::string temp = hexToBit(hex[i]);
        if (temp == "ERROR")
            return "ERROR";
        else
            bites += temp;
    }
    return bites;
}

std::string getHexFromBites(const std::string &bites)
{
    std::stringstream stream;
    stream << std::hex << std::stoi(bites, nullptr, 2);
    if (stream.str().length() == 1)
        return "0" + stream.str();
    else
        return stream.str();
}

void octetsPrint(const std::string &bites)
{
    for (size_t i = 0; i < bites.length(); i += 8)
        std::cout << i + 8 << "\t| "
                  << bites.substr(i, 8)
                  << "\t| " << (char)std::stoi(bites.substr(i, 8), nullptr, 2)
                  << "\t| " << std::stoi(bites.substr(i, 8), nullptr, 2)
                  << "\t| " << getHexFromBites(bites.substr(i, 8))
                  << std::endl;
}

class DNSHeader
{
private:
    void _printDNSHeader()
    {
        std::cout << ";; ->>HEADER<<- opcode: " << getOPCODE()
                  << ", status: " << getStatus()
                  << ", id: " << std::to_string(ID)
                  << std::endl;

        std::cout << ";; flags: " << (QR ? "qr " : "") << (RD ? "rd " : "") << (RA ? "ra" : "") << (AA ? "aa" : "") << (TC ? "tc" : "")
                  << "; QUERY: " << std::to_string(QDCOUNT)
                  << ", ANSWER: " << std::to_string(ANCOUNT)
                  << ", AUTHORITY: " << std::to_string(NSCOUNT)
                  << ", ADDITIONAL: " << std::to_string(ARCOUNT)
                  << std::endl;
        std::cout << std::endl;
    }

public:
    DNSHeader(std::string &bites)
    {
        ID = std::stoi(bites.substr(0, 16), nullptr, 2);
        QR = std::stoi(bites.substr(16, 1), nullptr, 2);
        OPCODE = std::stoi(bites.substr(17, 4), nullptr, 2);
        AA = std::stoi(bites.substr(21, 1), nullptr, 2);
        TC = std::stoi(bites.substr(22, 1), nullptr, 2);
        RD = std::stoi(bites.substr(23, 1), nullptr, 2);
        RA = std::stoi(bites.substr(24, 1), nullptr, 2);
        Z = std::stoi(bites.substr(25, 3), nullptr, 2);
        RCODE = std::stoi(bites.substr(28, 4), nullptr, 2);
        QDCOUNT = std::stoi(bites.substr(32, 16), nullptr, 2);
        ANCOUNT = std::stoi(bites.substr(48, 16), nullptr, 2);
        NSCOUNT = std::stoi(bites.substr(64, 16), nullptr, 2);
        ARCOUNT = std::stoi(bites.substr(80, 16), nullptr, 2);

        bites = bites.substr(96);
        _printDNSHeader();
    }

    std::string getOPCODE()
    {
        switch (OPCODE)
        {
        case 0:
            return "QUERY";
        case 1:
            return "IQUERY";
        case 2:
            return "STATUS";
        case 3:
            return "RESERVED";
        case 4:
            return "NOTIFY";
        case 5:
            return "UPDATE";
        default:
            return "ERROR";
        }
    }

    std::string getStatus()
    {
        switch (RCODE)
        {
        case 0:
            return "NOERROR";
        case 1:
            return "FORMERR";
        case 2:
            return "SERVFAIL";
        case 3:
            return "NXDOMAIN";
        case 4:
            return "NOTIMP";
        case 5:
            return "REFUSED";
        case 6:
            return "YXDOMAIN";
        case 7:
            return "XRRSET";
        case 8:
            return "NOTAUTH";
        case 9:
            return "NOTZONE";
        default:
            return "ERROR";
        }
    }

    int ID;      // 16 bits
    int QR;      // 1 bit
    int OPCODE;  // 4 bits
    int AA;      // 1 bit
    int TC;      // 1 bit
    int RD;      // 1 bit
    int RA;      // 1 bit
    int Z;       // 3 bits
    int RCODE;   // 4 bits
    int QDCOUNT; // 16 bits
    int ANCOUNT; // 16 bits
    int NSCOUNT; // 16 bits
    int ARCOUNT; // 16 bits
};

class DNSQuestion
{
private:
    void _printDNSQuestion()
    {
        std::cout << ";; QUESTION SECTION:" << std::endl;
        std::cout << ";" << QNAME << "\t\t"
                  << getDNSClass(QCLASS) << "\t"
                  << getDNSType(QTYPE)
                  << std::endl;
        std::cout << std::endl;
    }

public:
    DNSQuestion(std::string &bites)
    {
        // octetsPrint(bites);
        size_t pos = bites.find("00000000");

        QNAME = getURL(bites.substr(0, pos));
        QTYPE = std::stoi(bites.substr(pos + 8, 16), nullptr, 2);
        QCLASS = std::stoi(bites.substr(pos + 24, 16), nullptr, 2);

        _printDNSQuestion();
        bites = bites.substr(pos + 40);
    }

    std::string QNAME;
    int QCLASS;
    int QTYPE;
};

class DNSAnswer
{
private:
    std::string _getIP4Address(const std::string &rdata)
    {
        std::string ip;

        for (size_t i = 0; i < rdata.length(); i += 8)
        {
            ip += std::to_string(std::stoi(rdata.substr(i, 8), nullptr, 2));
            if (i + 8 != rdata.length())
                ip += ".";
        }

        return ip;
    }

    std::string _getIP6Address(const std::string &rdata)
    {
        std::string ip;

        for (size_t i = 0; i < rdata.length(); i += 16)
        {
            std::string hex;
            hex += getHexFromBites(rdata.substr(i, 8));
            hex += getHexFromBites(rdata.substr(i + 8, 8));
            if (hex != "0000")
            {
                while (hex[0] == '0')
                    hex = hex.substr(1);
                ip += hex;
            }
            if (ip.find("::") != ip.length() - 2 && i + 16 != rdata.length())
                ip += ":";
        }

        return ip;
    }

    std::string _getCanonicalName(std::string &rdata, std::string *name)
    {
        std::string canonicalName;

        for (size_t i = 8; i + 16 < rdata.length(); i += 8)
        {
            std::string temp = rdata.substr(i, 8);
            if (temp == "00000000")
                break;
            else if (temp == "00000011" || temp == "00001001" || temp == "00001010")
                canonicalName += ".";
            else
                canonicalName += (char)std::stoi(temp, nullptr, 2);
        }

        if (name->substr(0, 4) == "www.")
            canonicalName += name->substr(3);
        else
            canonicalName += "." + *name;

        return canonicalName;
    }

    void _printDNSAnswer()
    {

        std::cout << *NAME << "\t\t"
                  << RDLENGTH << "\t"
                  << getDNSClass(CLASS) << "\t"
                  << getDNSType(TYPE) << "\t";

        switch (TYPE)
        {
        case 1:
            std::cout << _getIP4Address(RDATA);
            break;
        case 5:
            *NAME = _getCanonicalName(RDATA, NAME);
            std::cout << *NAME;
            break;
        case 28:
            std::cout << _getIP6Address(RDATA);
            break;
        default:
            break;
        }

        std::cout << std::endl;
    }

    int _length;

public:
    DNSAnswer(std::string &name, std::string &bites)
    {
        NAME = &name;
        TYPE = std::stoi(bites.substr(16, 16), nullptr, 2);
        CLASS = std::stoi(bites.substr(32, 16), nullptr, 2);
        TTL = std::stoi(bites.substr(48, 16), nullptr, 2);
        RDLENGTH = std::stoi(bites.substr(64, 16), nullptr, 2);
        _length = std::stoi(bites.substr(80, 16), nullptr, 2);
        RDATA = bites.substr(96, _length * 8);
        bites = bites.substr(96 + _length * 8);

        _printDNSAnswer();
    }

    std::string *NAME;
    int TYPE;
    int CLASS;
    int TTL;
    int RDLENGTH;
    std::string RDATA;
};

int main()
{
    // std::string hex = "a01d81800001000100000000076578616d706c6503636f6d0000010001c00c0001000100001bbc00045db8d822";
    // std::string hex = "9b4c84000001000200000000037777770a636c6f7564666c61726503636f6d0000010001c00c000100010000012c000468107c60c00c000100010000012c000468107b60";
    // std::string hex = "762081800001000200000000037777770773706f7469667903636f6d0000010001c00c0005000100000102001f12656467652d7765622d73706c69742d67656f096475616c2d67736c62c010c02d000100010000006c000423bae019";

    std::string hex;

    std::getline(std::cin, hex);
    std::string bites = hexToBites(hex);

    DNSHeader header(bites);
    DNSQuestion question(bites);

    std::cout << ";; ANSWER SECTION:" << std::endl;
    for (int i = 0; i < header.ANCOUNT; i++)
        DNSAnswer answer(question.QNAME, bites);

    return 0;
}
