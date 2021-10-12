// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Битовое поле

#include <limits>
#include <iostream>
#include "tbitfield.h"

TBitField::TBitField(size_t len)
{
    if (len <= 0)
    {
        throw len;
    }
    bitLen = len;
    memLen = len / 32 + 1;
    pMem = new uint[memLen];
    for (int i = 0; i < memLen; i++)
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    bitLen = bf.bitLen;
    memLen = bf.memLen;
    pMem = new uint[memLen];
    for (int i = 0; i < memLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}
TBitField::~TBitField()
{
    delete[] pMem;
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    if (n < 0 || n >= bitLen)
    {
        throw n;
    }
    return (n / 32);

}

uint TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    if (n < 0 || n >= bitLen)
    {
        throw n;
    }
    return (1 << (n % 32));
}

// доступ к битам битового поля
uint TBitField::getLength() const // получить длину (к-во битов)
{
    return bitLen;
}

void TBitField::setBit(const size_t n) // установить бит
{
    if (n < 0 || n >= bitLen)
    {
        throw n;
    }
    int i = getIndex(n);
    uint m = getMask(n);
    pMem[i] |= m;
}

void TBitField::clrBit(const size_t n) // очистить бит
{
    if (n < 0 || n >= bitLen)
    {
        throw n;
    }
    int i = getIndex(n);
    uint m = getMask(n);
    pMem[i] = pMem[i] & (~m);
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
    if (n < 0 || n >= bitLen)
    {
        throw n;
    }
    int i = getIndex(n);
    uint m = getMask(n);
    return (pMem[i] & m);
    return false;
}

// битовые операции
TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf)
    {
        if (bitLen != bf.bitLen)
        {
            delete[] pMem;
            memLen = bf.memLen;
            bitLen = bf.bitLen;
            pMem = new uint[memLen];
        }
        for (int i = 0; i < memLen - 1; i++)
        {
            pMem[i] = bf.pMem[i];
        }
        for (int i = (memLen - 1) * 32; i < bitLen; i++)
        {
            if (bf.getBit(i))
                setBit(i);
        }
    }
    return *this;
}

bool TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (this != &bf)
    {
        if (bitLen != bf.bitLen)
            return false;
        for (int i = 0; i < memLen - 1; i++)
        {
            if (pMem[i] != bf.pMem[i])
            {
                return false;
            }
        }
        for (int i = (memLen - 1) * 32; i < bitLen; i++)
        {
            if (getBit(i) != bf.getBit(i))
            {
                return false;
            }
        }
    }
    return true;
}

bool TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    if (this == &bf)
    {
        TBitField res(*this);
        return res;
    }
    int max, flag = 0;
    if (bitLen < bf.bitLen)
    {
        max = bf.bitLen;
    }
    else
    {
        flag = 1;
        max = bitLen;
    }
    TBitField res(max);
    if (flag == 0)
    {
        for (int i = 0; i < memLen - 1; i++)
        {
            res.pMem[i] = pMem[i];
        }
        for (int i = (memLen - 1) * 32; i < bitLen; i++)
        {
            if (getBit(i))
            {
                res.setBit(i);
            }
        }
        for (int i = 0; i < bf.memLen; i++)
        {
            res.pMem[i] |= bf.pMem[i];
        }
    }
    else
    {
        for (int i = 0; i < bf.memLen - 1; i++)
        {
            res.pMem[i] = bf.pMem[i];
        }
        for (int i = (bf.memLen - 1) * 32; i < bf.bitLen; i++)
        {
            if (bf.getBit(i))
            {
                res.setBit(i);
            }
        }
        for (int i = 0; i < memLen; i++)
        {
            res.pMem[i] |= pMem[i];
        }
    }
    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    if (this == &bf)
    {
        TBitField res(*this);
        return res;
    }
    int max, flag = 0;
    if (bitLen < bf.bitLen)
    {
        max = bf.bitLen;
    }
    else
    {
        flag = 1;
        max = bitLen;
    }
    TBitField res(max);
    if (flag == 0)
    {
        for (int i = 0; i < memLen - 1; i++)
        {
            res.pMem[i] = pMem[i];
        }
        for (int i = (memLen - 1) * 32; i < bitLen; i++)
        {
            if (getBit(i))
            {
                res.setBit(i);
            }
        }
        for (int i = 0; i < memLen; i++)
        {
            res.pMem[i] &= bf.pMem[i];
        }
    }
    else
    {
        for (int i = 0; i < bf.memLen - 1; i++)
        {
            res.pMem[i] = bf.pMem[i];
        }
        for (int i = (bf.memLen - 1) * 32; i < bf.bitLen; i++)
        {
            if (bf.getBit(i))
            {
                res.setBit(i);
            }
        }
        for (int i = 0; i < bf.memLen; i++)
        {
            res.pMem[i] &= pMem[i];
        }
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField mask(bitLen);
    for (int i = 0; i < mask.getLength(); i++)
    {
        mask.setBit(i);
    }
    for (int i = 0; i < (*this).getLength(); i++)
    {
        (*this).pMem[i] = ~pMem[i];
        (*this).pMem[i] = (*this).pMem[i] & mask.pMem[i];
    }

    return (*this);
    return TBitField(1);
}

TBitField::~TBitField()
{

}

// ввод/вывод
std::istream& operator>>(std::istream& istr, TBitField& bf) // ввод
{
    char x;
    for (int i = 0; i < bf.bitLen; i++)
    {
        istr >> x;
        if (x == '0')
        {
            bf.clrBit(i);
        }
        else
        {
            if (x == '1')
            {
                bf.setBit(i);
            }
            else
                return istr;
        }
    }
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.bitLen; i++)
    {
        if (bf.getBit(i))
        {
            ostr << '1';
        }
        else
        {
            ostr << '0';
        }
    }
    return ostr;
    return ostr;
}
