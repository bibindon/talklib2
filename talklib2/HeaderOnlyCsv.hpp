#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include "Shlwapi.h"
#pragma comment( lib, "Shlwapi.lib" ) 

class csv
{
public:

    static std::vector<std::vector<std::string> > Read(const std::string& filepath)
    {
        std::vector<std::vector<std::string> > csvData;
        int result = PathFileExists(filepath.c_str());
        if (result == 0)
        {
            return csvData;
        }

        // �u"�v�L���ň͂܂�Ă���ƃZ�������s���ł��邱�Ƃɒ���
        std::ifstream ifs(filepath);
        std::string buffComma;
        bool doubleQuoteMode = false;
        std::vector<std::string> work;
        std::istreambuf_iterator<char> itBegin(ifs);
        std::istreambuf_iterator<char> itEnd;

        for (; itBegin != itEnd; itBegin++)
        {
            if (*itBegin != ',' && *itBegin != '\n')
            {
                buffComma += *itBegin;
                if (*itBegin == '"')
                {
                    if (doubleQuoteMode == false)
                    {
                        doubleQuoteMode = true;
                    }
                    else
                    {
                        doubleQuoteMode = false;
                    }
                }
            }
            else if (*itBegin == ',')
            {
                work.push_back(buffComma);
                buffComma.clear();
            }
            else if (*itBegin == '\n')
            {
                if (doubleQuoteMode == false)
                {
                    work.push_back(buffComma);
                    buffComma.clear();
                    csvData.push_back(work);
                    work.clear();
                }
                else
                {
                    buffComma += *itBegin;
                }
            }
        }

        return csvData;
    }

    static std::vector<std::vector<std::string> > ReadFromString(const std::string& text)
    {
        std::vector<std::vector<std::string> > csvData;

        // �u"�v�L���ň͂܂�Ă���ƃZ�������s���ł��邱�Ƃɒ���
        std::string buffComma;
        bool doubleQuoteMode = false;
        std::vector<std::string> work;
        std::string::const_iterator itBegin(text.cbegin());
        std::string::const_iterator itEnd(text.cend());;

        for (; itBegin != itEnd; itBegin++)
        {
            if (*itBegin != ',' && *itBegin != '\n')
            {
                buffComma += *itBegin;
                if (*itBegin == '"')
                {
                    if (doubleQuoteMode == false)
                    {
                        doubleQuoteMode = true;
                    }
                    else
                    {
                        doubleQuoteMode = false;
                    }
                }
            }
            else if (*itBegin == ',')
            {
                work.push_back(buffComma);
                buffComma.clear();
            }
            else if (*itBegin == '\n')
            {
                if (doubleQuoteMode == false)
                {
                    work.push_back(buffComma);
                    buffComma.clear();
                    csvData.push_back(work);
                    work.clear();
                }
                else
                {
                    buffComma += *itBegin;
                }
            }
        }

        return csvData;
    }

    static void Write(const std::string& filepath, const std::vector<std::vector<std::string> >& csvData)
    {
        std::ofstream ofs(filepath);
        for (std::size_t i = 0; i < csvData.size(); ++i)
        {
            for (std::size_t j = 0; j < csvData.at(i).size(); ++j)
            {
                ofs << csvData.at(i).at(j);
                if (j != csvData.at(i).size() - 1)
                {
                    ofs << ",";
                }
            }
            ofs << "\n";
        }
    }

private:
    csv();

    static void ltrim(std::string& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            [](unsigned char ch)
            {
                return !std::isspace(ch);
            }
        ));
    }

    static void rtrim(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(),
            [](unsigned char ch)
            {
                return !std::isspace(ch);
            }
        ).base(), s.end());
    }

    static void trim(std::string& s)
    {
        rtrim(s);
        ltrim(s);
    }
};

