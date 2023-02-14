#include <iostream> // remove
#include <unistd.h> // posix - for sleep

#include "cofhe.hpp"
// #include "cofhe_const.hpp"

using namespace std;

namespace e3
{

namespace cofhe
{

constexpr bool isOdd(uint32_t n) { return bool(n&1); }

// Constructors

Cofhe::Cofhe(ComType comtype, const map<string,DataType> & params)
{
    setFrequency(params);
    switch (comtype)
    {
        case UART : setUart(params); break;
        case SPI  : com = shared_ptr<Spi>( new Spi ( std::get<string>( params.at("portname") ) ) ); break;
        default   : throw "Communication not defined";
    }
    setInterrupt(params);
    init(); // add parameters optional on init
}

// Private functions

uint32_t Cofhe::concatenate(const vector<Register> & regs, bool allowDual, size_t idx)
{
    uint32_t i, out = 0;
    for ( i=0; i<regs.size(); i++ )
        out |= ( i==idx && allowDual && isDual(regs[i]) ? FHEMEM_DP_BASE[regs[i]] : FHEMEM_BASE[regs[i]] ) >> byte2bit(i);
    out >>= byte2bit( WORDSIZE_BYTES - i );
    return out;
}

void Cofhe::init()
{
    if ( !validate() ) throw "Cofhe signature invalid";
    setPads();
}

uint32_t Cofhe::merge(uint32_t command, const vector<Register> & regs, bool allowDual, size_t idx)
{
    return command | concatenate(regs, allowDual, idx);
}

void Cofhe::setFrequency(const std::map<std::string,DataType> & params)
{
    try { frequency = uint32_t( std::get<int>( params.at("frequency") ) ); }
    catch (...) { throw "Frequency not defined."; }
}

void Cofhe::setInterrupt(const std::map<std::string,DataType> & params)
{
    uint32_t data = frequency / std::get<int>( params.at("baudrate") );
    writeCommand( (uint8_t *) &data, GPCFG_UARTS_BAUD_CTL, WORDSIZE_BYTES );
    sleep(2);
    interrupt = shared_ptr<Uart>(new Uart( std::get<string>( params.at("portinterrupt") ), std::get<int>( params.at("baudrate") ) ) );
}

void Cofhe::setPolyBytes()
{
    polyBytes = n * logqBytes;
}

void Cofhe::setPads()
{
    // Interrupt 'done'
    uint32_t data = GPCFG_HOSTIRQ_PAD_CTL_CFG;
    writeCommand( (uint8_t *) &data, GPCFG_HOSTIRQ_PAD_CTL, WORDSIZE_BYTES );

    // Uart communication
    data = GPCFG_UARTSTX_PAD_CTL_CFG;
    writeCommand( (uint8_t *) &data, GPCFG_UARTSTX_PAD_CTL, WORDSIZE_BYTES );

    // SPI communication
    // ...
    // ...
}

void Cofhe::setUart(const std::map<std::string,DataType> & params)
{
    // cout << "Uart in\n";
    com = shared_ptr<Uart>( new Uart( std::get<string>( params.at("portname") ), frequency / DEFAULT_FREQ2UART_RATIO ) );
    uint32_t data = frequency / std::get<int>( params.at("baudrate") );
    // cout << data << '\n';
    writeCommand( (uint8_t *) &data, GPCFG_UARTM_BAUD_CTL, WORDSIZE_BYTES );
    sleep(2);
    com = shared_ptr<Uart>(new Uart( std::get<string>( params.at("portname") ), std::get<int>( params.at("baudrate") ) ) );
    // cout << "Uart out\n";
}

// Public functions

void Cofhe::add(const Register & regOut, const Register & regIn1, const Register regIn2, bool waitInterrupt)
{
    auto cmd = CMD_ADD;
    auto flag1 = regOut == regIn1;
    auto flag2 = regOut == regIn2;
    auto flag3 = regIn1 == regIn2;
    auto flag = flag1 || flag2 || flag3;
    size_t idx = 0;
    if ( flag )
    {
        if ( ( flag3 && !isDual(regIn1) ) || !isDual(regOut) ) throw "Invalid operands on ADD";
        if ( flag3 ) idx = 1;
        cmd |= DUAL_PORT_MASK;
    }
    if ( !waitInterrupt ) cmd |= BYPASS_INTERRUPT_MASK;
    auto data = merge(cmd, {regOut, regIn1, regIn2}, flag, idx);
    std::cout << std::hex << "\nADD: " << data << std::dec << '\n';
    writeCommand( (uint8_t *) &data, GPCFG_COMMNDFIFO, WORDSIZE_BYTES );
    if ( waitInterrupt )
    {
        uint8_t data;
        do
        {
            interrupt->read( &data, BYTE_BYTES );
            cout << "ADD reply: " << hex << uint32_t(data) << dec << '\n';
        } while (data == 0xff);
    }
}

void Cofhe::cmul(const Register & regOut, const Register & regIn, bool waitInterrupt)
{
    auto cmd = CMD_CMUL;
    auto flag = regOut == regIn;
    if ( flag )
    {
        if ( !isDual(regIn) ) throw "Invalid operands on CMUL";
        cmd |= DUAL_PORT_MASK;
    }
    if ( !waitInterrupt ) cmd |= BYPASS_INTERRUPT_MASK;
    auto data = merge(cmd, {regOut, regIn, regIn}, flag, 0);
    std::cout << std::hex << "\nCMUL: " << data << std::dec << '\n';
    writeCommand( (uint8_t *) &data, GPCFG_COMMNDFIFO, WORDSIZE_BYTES );
    if ( waitInterrupt )
    {
        uint8_t data;
        do
        {
            interrupt->read( &data, BYTE_BYTES );
            cout << "CMUL reply: " << hex << uint32_t(data) << dec << '\n';
        } while (data == 0xff);
    }
}

void Cofhe::configure(uint32_t logn, uint32_t logq, const uint8_t * modulus, const uint8_t * nInv, uint32_t barrett_2k, const uint8_t * barrett_md, const uint8_t * twiddle, const Register & regTwiddle, bool dual)
{
    setN(logn);
    setLogQ(logq);
    setModulus(modulus);
    setInverseN(nInv);
    setBarrett(barrett_2k, barrett_md);
    setTwiddle(twiddle, regTwiddle);
    setDualPort(dual);
}

uint32_t Cofhe::getLogN() const
{
    return logn;
}

uint8_t * Cofhe::getRegister(const Register & reg)
{
    auto address = FHEMEM_BASE[reg];
    return readCommand(address, polyBytes);
}

bool Cofhe::isDual(const Register & reg)
{
    return dual && isDualPort[reg] && (n <= 4096 || (n == 8192 && reg <= R6));
}

Register Cofhe::intt(const Register & regIn, const Register & regTmp, bool waitInterrupt)
{
    auto cmd = CMD_INTT;
    if ( isDual(regIn) && isDual(regTmp) ) cmd |= DUAL_PORT_MASK;
    if ( !waitInterrupt ) cmd |= BYPASS_INTERRUPT_MASK;
    auto data = merge(cmd, {regTmp, regIn, regIn}, true, 1);
    std::cout << std::hex << "\nINTT: " << data << std::dec << '\n';
    writeCommand( (uint8_t *) &data, GPCFG_COMMNDFIFO, WORDSIZE_BYTES );
    if ( waitInterrupt )
    {
        uint8_t data;
        do
        {
            interrupt->read( &data, BYTE_BYTES );
            cout << "iNTT reply: " << hex << uint32_t(data) << dec << '\n';
        } while (data == 0xff);
    }
    cout << "iNTT out " << (isOdd(logn) ? regTmp : regIn) << "\n";
    return ( isOdd(logn) ? regTmp : regIn );
}

uint8_t * Cofhe::intt(const uint8_t * poly, const Register & regIn, const Register & regTmp, bool waitInterrupt)
{
    setRegister(poly, regIn);
    auto regOut = intt(regIn, regTmp, waitInterrupt);
    return getRegister(regOut);
}

Register Cofhe::move(const Register & regFrom, const Register & regTo, bool waitInterrupt)
{
    // workaround -- buggy DMA or Command FIFO
    writeCommand( (uint8_t *) &FHEMEM_BASE[regFrom], GPCFG73_ADDR, WORDSIZE_BYTES );
    writeCommand( (uint8_t *) &FHEMEM_BASE[regTo  ], GPCFG74_ADDR, WORDSIZE_BYTES );
    writeCommand( (uint8_t *) &CMD_DMA_WORKAROUND, GPCFG_FHECTLP_ADDR, WORDSIZE_BYTES );
    waitInterrupt = true;

    // sad DMA noises
    // auto cmd = CMD_DMA;
    // if ( !waitInterrupt ) cmd |= BYPASS_INTERRUPT_MASK;
    // auto data = merge(cmd, {R0, regTo, regFrom}, false);
    // std::cout << std::hex << "\nDMA: " << data << std::dec << '\n';
    // writeCommand( (uint8_t *) &data, GPCFG_COMMNDFIFO, WORDSIZE_BYTES );
    if ( waitInterrupt )
    {
        uint8_t data;
        do
        {
            interrupt->read( &data, BYTE_BYTES );
            cout << "DMA reply: " << hex << uint32_t(data) << dec << '\n';
        } while (data == 0xff);
    }
    return regTo;
}

void Cofhe::mul(const Register & regOut, const Register & regIn1, const Register regIn2, bool waitInterrupt)
{
    auto cmd = CMD_MUL;
    auto flag1 = regOut == regIn1;
    auto flag2 = regOut == regIn2;
    auto flag3 = regIn1 == regIn2;
    auto flag = flag1 || flag2 || flag3;
    size_t idx = 0;
    if ( flag )
    {
        if ( ( flag3 && !isDual(regIn1) ) || !isDual(regOut) ) throw "Invalid operands on ADD";
        if ( flag3 ) idx = 1;
        cmd |= DUAL_PORT_MASK;
    }
    if ( !waitInterrupt ) cmd |= BYPASS_INTERRUPT_MASK;
    auto data = merge(cmd, {regOut, regIn1, regIn2}, flag, idx);
    std::cout << std::hex << "\nMUL: " << data << std::dec << '\n';
    writeCommand( (uint8_t *) &data, GPCFG_COMMNDFIFO, WORDSIZE_BYTES );
    if ( waitInterrupt )
    {
        uint8_t data;
        do
        {
            interrupt->read( &data, BYTE_BYTES );
            cout << "MUL reply: " << hex << uint32_t(data) << dec << '\n';
        } while (data == 0xff);
    }
}

Register Cofhe::ntt(const Register & regIn, const Register regTmp, bool waitInterrupt)
{
    auto cmd = CMD_NTT;
    if ( isDual(regIn) && isDual(regTmp) ) cmd |= DUAL_PORT_MASK;
    if ( !waitInterrupt ) cmd |= BYPASS_INTERRUPT_MASK;
    auto data = merge(cmd, {regTmp, regIn, regIn}, true, 1);
    std::cout << std::hex << "\nNTT: " << data << std::dec << '\n';
    writeCommand( (uint8_t *) &data, GPCFG_COMMNDFIFO, WORDSIZE_BYTES );
    if ( waitInterrupt )
    {
        uint8_t data;
        do
        {
            interrupt->read( &data, BYTE_BYTES );
            cout << "NTT reply: " << hex << uint32_t(data) << dec << '\n';
        } while (data == 0xff); // something is wrong with termios configuration
    }
    cout << "NTT out " << (isOdd(logn) ? regTmp : regIn) << "\n";
    return ( isOdd(logn) ? regTmp : regIn );
}

uint8_t * Cofhe::ntt(const uint8_t * poly, const Register & regIn, const Register & regTmp, bool waitInterrupt)
{
    setRegister(poly, regIn);
    auto regOut = ntt(regIn, regTmp, waitInterrupt);
    return getRegister(regOut);
}

Register Cofhe::polymul(const Register & regIn1, const Register & regIn2, const Register & regTmp)
{
    auto regA = ntt(regIn1, regTmp, false); // NTT A
    auto regOut = ( regA == regIn1 ? regTmp : regIn1 );
    cout << regA << ", " << regOut << '\n';
    auto regB = ntt(regIn2, regOut, false); // NTT B
    regOut = ( regB == regIn2 ? regTmp : regIn2 );
    cout << regB << ", " << regOut << '\n';
    mul(regOut, regA, regB, false); // A * B
    cout << regOut << ", " << regA << ", " << regB << '\n';
    cmul(regA, regOut, false); // CMUL
    cout << regA << ", " << regOut << '\n';
    regOut = intt(regA, regOut, true); // INTT
    cout << regOut << ", " << regA << '\n';
    return regOut;
}

uint8_t * Cofhe::polymul(const uint8_t * poly1, const uint8_t * poly2, const Register & regIn1, const Register & regIn2, const Register & regTmp)
{
    setRegister(poly1, regIn1);
    setRegister(poly2, regIn2);
    auto regOut = polymul(regIn1, regIn2, regTmp);
    return getRegister(regOut);
}

uint8_t * Cofhe::polymul(uint8_t * in1, uint8_t *in2)
{
    setRegister(in1, R0);
    setRegister(in2, R1);
    ntt(R0, R2);
    ntt(R1, R2); sleep(1);
    mul(R2, R1, R0); sleep(1);
    cmul(R0, R2); sleep(1);
    intt(R0, R1);
    return getRegister(R0);
}

uint8_t * Cofhe::readCommand(uint32_t address, uint32_t size)
{
    uint8_t * data = (uint8_t *) malloc( sizeof(uint8_t) * size );
    for ( uint32_t i=0; i<size; i+=WORDSIZE_BYTES, address+=WORDSIZE_BYTES )
    {
        com->write( (uint8_t *) &CMD_READ, WORDSIZE_BYTES);
        com->write( (uint8_t *) &address, WORDSIZE_BYTES );
        com->read( &data[i], WORDSIZE_BYTES );
    }
    return data;
}

void Cofhe::setBarrett(uint32_t barrett_2k, const uint8_t * barrett_md)
{
    // set barrett_md
    writeCommand(barrett_md, GPCFG_NSQ_ADDR[0], logqBytes<<1);

    // set barrett_2k
    writeCommand( (uint8_t *) &barrett_2k, GPCFG_NSQ_ADDR[5], WORDSIZE_BYTES );
}

void Cofhe::setDualPort(bool dual)
{
    this->dual = dual;
}

void Cofhe::setInverseN(const uint8_t * invN)
{
    writeCommand(invN, GPCFG_NINV_ADDR[0], logqBytes);
}

void Cofhe::setLogQ(uint32_t logq)
{
    logqBytes = bit2byte(logq);
    setPolyBytes();
}

void Cofhe::setModulus(const uint8_t * modulus)
{
    writeCommand(modulus, GPCFG_N_ADDR[0], logqBytes);
}

void Cofhe::setN(uint32_t logn)
{
    this->logn = logn;
    this->n = 1<<logn;
    auto data = CMD_SET_N | n;
    writeCommand( (uint8_t *) &data, GPCFG_FHECTL_ADDR, WORDSIZE_BYTES );
    setPolyBytes();
}

void Cofhe::setRegister(const uint8_t * poly, const Register & reg)
{
    auto address = FHEMEM_BASE[reg];
    writeCommand(poly, address, polyBytes);
    // sleep(300);
}

void Cofhe::setTwiddle(const uint8_t * twiddle, const Register & reg)
{
    setRegister(twiddle, reg);
    auto data = concatenate( {R0, reg, R0, R0}, false); // make this nicer
    // auto data = concatenate( {R3, reg, R0, R0} ); // make this nicer
    writeCommand( (uint8_t *) &data, GPCFG_FHECTL2, WORDSIZE_BYTES );
}

bool Cofhe::validate()
{
    auto signature = readCommand(SIGNATURE_ADDRESS, WORDSIZE_BYTES);
    for (int i=0; i<WORDSIZE_BYTES; i++)
        if (signature[i] != SIGNATURE[i]) return false;
    return true;
}

void Cofhe::writeCommand(const uint8_t * data, uint32_t address, uint32_t size)
{
    for ( uint32_t i=0; i<size; i+=WORDSIZE_BYTES, address+=WORDSIZE_BYTES )
    {
        com->write( (uint8_t *) &CMD_WRITE, WORDSIZE_BYTES );
        com->write( (uint8_t *) &address, WORDSIZE_BYTES );
        com->write( &data[i], WORDSIZE_BYTES );
    }
}

} // cofhe

} // e3
