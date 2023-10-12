// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <iostream>

// Fake variables used as placeholders in tests
//static const int FAKE_INT = -1;
//static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) throw "Length<0";
    BitLen = len;
    if (len % sizeof(TELEM) == 0) {
        MemLen = len / sizeof(TELEM);
    }
    else {
        MemLen = len / sizeof(TELEM) + 1;
    }
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    if (bf.BitLen < 0) throw "Length<0";
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    if (pMem != nullptr) {
        delete[]pMem;
        pMem = nullptr;
        BitLen = 0;
    }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n<0 || n>=BitLen) throw"Invalid index";
    return n/(sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n<0 || n>=BitLen) throw"Invalid index";
    return 1 << (n - 1) % (sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n<0 || n>=BitLen) throw"Invalid index";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n<0 || n>=BitLen) throw"Invalid index";
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n<0 || n>=BitLen) throw"Invalid index";
    return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        delete[]pMem;
        BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++) {
			pMem[i] = bf.pMem[i];
		}
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen && MemLen == bf.MemLen) {
        for (int i = 0; i < MemLen; i++) {
            if (pMem[i] != bf.pMem[i]) {
                return 0;
            }
        }
    }
    else {
        return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen && MemLen == bf.MemLen) {
        for (int i = 0; i < MemLen; i++) {
            if (pMem[i] != bf.pMem[i]) {
                return 1;
            }
        }
    }
    else {
        return 0;
    }
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int b = std::max(BitLen, bf.BitLen);
    TBitField tbf(b);
    for (int i = 0; i < MemLen; i++) {
        tbf.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        tbf.pMem[i] |= bf.pMem[i];
    }
    return tbf;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int b = std::max(BitLen, bf.BitLen);
    TBitField tbf(b);
    for (int i = 0; i < MemLen; i++) {
        tbf.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        tbf.pMem[i] &= bf.pMem[i];
    }
    return tbf;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tbf(*this);
    for (int i = 0; i < BitLen; i++) {
        if (tbf.GetBit(i)) {
            tbf.ClrBit(i);
        }
        else {
            tbf.SetBit(i);
        }
    }
    return tbf;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string tmp;
    istr >> tmp;
    if (tmp.size() != bf.GetLength()) {
        throw "Warning Length";
    }
    else {
        for (int i = 0; i < bf.GetLength(); i++) {
            if (tmp[i] == '0') {
                bf.ClrBit(i);
            }
            else if (tmp[i] == '1') {
                bf.SetBit(i);
            }
            else throw "Incorrect string";
        }
    }
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.MemLen; i++) {
        if (bf.GetBit(i)) {
            ostr << 1;
        }
        else {
            ostr << 0;
        }
    }
    return ostr;
}
