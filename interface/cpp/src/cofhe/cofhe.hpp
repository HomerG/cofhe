#include <map>
#include <memory>
#include <string>
#include <vector>
#include <variant>

#include "cofhe_const.hpp"
#include "communication.hpp"
#include "spi.hpp"
#include "uart.hpp"

namespace e3
{

namespace cofhe
{

using DataType = std::variant< int, std::string, std::vector<int>, std::vector<std::string> >;

enum ComType { UART, SPI };
enum Register { R0, R1, R2,  R3,  R4,  R5,  R6,   // n = 8192 can use Regs [0, 6]
                R7, R8, R9, R10, R11, R12, R13 }; // n = 4096 can use Regs [0,13]

const std::vector<bool> isDualPort { true, true, true, false, false, false, false,
                                     true, true, true, false, false, false, false };

class Cofhe
{
    private:
        std::shared_ptr<Communication> com;
        std::shared_ptr<Uart> interrupt;
        uint32_t logqBytes;
        uint32_t logn;
        uint32_t n;
        uint32_t polyBytes;
        uint32_t frequency;
        bool dual = false;

        uint32_t concatenate(const std::vector<Register> & regs, bool allowDual, size_t idx=0);
        void init();
        uint32_t merge(uint32_t command, const std::vector<Register> & regs, bool allowDual, size_t idx=0);
        void setFrequency(const std::map<std::string,DataType> & params);
        void setInterrupt(const std::map<std::string,DataType> & params);
        void setPolyBytes();
        void setPads();
        void setUart(const std::map<std::string,DataType> & params);

    public:
        Cofhe(){}
        Cofhe(ComType comtype, const std::map<std::string,DataType> & params);

        void add(const Register & regOut, const Register & regIn1, const Register regIn2, bool waitInterrupt=true);
        void cmul(const Register & regOut, const Register & regIn, bool waitInterrupt=true);
        void configure(uint32_t logn, uint32_t logq, const uint8_t * modulus, const uint8_t * nInv, uint32_t barrett_2k, const uint8_t * barrett_md, const uint8_t * twiddle, const Register & regTwiddle, bool dual);
        uint32_t getLogN() const;
        uint8_t * getRegister(const Register & reg);
        bool isDual(const Register & reg);
        Register intt(const Register & regIn, const Register & regTmp, bool waitInterrupt=true);
        uint8_t * intt(const uint8_t * poly, const Register & regIn, const Register & regTmp, bool waitInterrupt=true);
        Register move(const Register & regFrom, const Register & regTo, bool waitInterrupt=true);
        void mul(const Register & regOut, const Register & regIn1, const Register regIn2, bool waitInterrupt=true);
        Register ntt(const Register & regIn, const Register regTmp, bool waitInterrupt=true);
        uint8_t * ntt(const uint8_t * poly, const Register & regIn, const Register & regTmp, bool waitInterrupt=true);
        Register polymul(const Register & regIn1, const Register & regIn2, const Register & regTmp);
        // void polysquare
        uint8_t * polymul(const uint8_t * poly1, const uint8_t * poly2, const Register & regIn1, const Register & regIn2, const Register & regTmp);
        uint8_t * polymul(uint8_t * in1, uint8_t *in2);
        uint8_t * readCommand(uint32_t address, uint32_t size);
        void setBarrett(uint32_t barrett_2k, const uint8_t * barrett_md);
        void setDualPort(bool dual);
        void setInverseN(const uint8_t * invN);
        void setLogQ(uint32_t logq);
        void setModulus(const uint8_t * modulus);
        void setN(uint32_t logn);
        void setRegister(const uint8_t * poly, const Register & reg);
        void setTwiddle(const uint8_t * twiddle, const Register & reg);
        bool validate();
        void writeCommand(const uint8_t * data, uint32_t address, uint32_t size);
    	// void test_cofhe(){
    	// 	std::cout << "Hello from Cofhee!" << std::endl;
    	// }
        void test_cofhe();
};

} // cofhe

} // e3
