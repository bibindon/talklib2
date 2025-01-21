#include <string>
#include <fstream>

class CaesarCipher
{
public:
    static std::string Encrypt(const std::string& text)
    {
        std::string result;
        char* work = nullptr;
        std::size_t len = text.length();
        work = new char[len + 1];
        strcpy_s(work, len + 1, text.c_str());
        for (std::size_t i = 0; i < len + 1; ++i)
        {
            work[i] = work[i] + 10;
        }
        // work[i]��+10�����Ƃ���0�ɂȂ��Ă��܂����Ƃ�����B
        // ���̂���work�̒��ɂ�0x00���܂܂�邱�Ƃ��l������
        result = std::string(&work[0], &work[len + 1]);
        delete[] work;
        return result;
    }

    static void EncryptToFile(const std::string& text, const std::string& filename)
    {
        std::string buff = Encrypt(text);

        std::ofstream ofs(filename);

        ofs.write(buff.data(), buff.size()); // ignore NULL
    }

    static std::string Decrypt(const std::string& text)
    {
        std::string result;
        char* work = nullptr;
        std::size_t len = text.length();
        work = new char[len]; // +1���Ȃ��B�Í������Ƀk��������+10�����L���������ɂ��邩��B

        // �r����null�����������Ă��w��o�C�g���܂ŃR�s�[����
        memcpy(work, text.c_str(), len);
        for (std::size_t i = 0; i < len; ++i)
        {
            work[i] = work[i] - 10;
        }
        result = std::string(work);
        delete[] work;
        return result;
    }

    static std::string DecryptFromFile(const std::string& text)
    {
        std::ifstream ifs(text);
        if (!ifs)
        {
            return std::string();
        }
        std::istreambuf_iterator<char> itBegin(ifs);
        std::istreambuf_iterator<char> itEnd;
        std::string work(itBegin, itEnd);

        work = Decrypt(work);
        return work;
    }
};