#include <QCoreApplication>
#include <QDebug>
#include <QtTest/QtTest>

#include "BigInteger.hpp"

using namespace apmath;

class TestBigInteger: public QObject
{
    Q_OBJECT

    static constexpr const char* OneToZero = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
    static constexpr const char* DoubledOneToZero = "24691357802469135780246913578024691357802469135780246913578024691357802469135780";
    static constexpr const char* ManyNines = "999999999999999999999999999999999999999999999999999999999999999999999999999999999";
    static constexpr const char* ManyNinesAndEight = "999999999999999999999999999999999999999999999999999999999999999999999999999999998";
    static constexpr const char* SomePowerOfTen = "1000000000000000000000000000000000000000000000000000000000000000000000000000000000";

private slots:

    void sameNumberSum() {
        BigInteger bn(OneToZero);
        QCOMPARE(bn + bn, BigInteger(DoubledOneToZero));

        BigInteger manyNines(ManyNines);
        BigInteger somePowerOfTen(SomePowerOfTen);
        BigInteger expected("1999999999999999999999999999999999999999999999999999999999999999999999999999999999");
        QCOMPARE(manyNines + somePowerOfTen, expected);
        QCOMPARE(somePowerOfTen + manyNines, expected);
    }

    void constructorParser() {
        BigInteger bnWithPlus(std::string("+") + OneToZero);
        QCOMPARE(bnWithPlus + bnWithPlus, BigInteger(DoubledOneToZero));

        BigInteger bnWithLeadingZeros(std::string("000") + OneToZero);
        QCOMPARE(bnWithLeadingZeros + bnWithLeadingZeros, BigInteger(DoubledOneToZero));

        BigInteger bnWithPlusAndLeadingZeros(std::string("+000") + OneToZero);
        QCOMPARE(bnWithPlusAndLeadingZeros + bnWithPlusAndLeadingZeros, BigInteger(DoubledOneToZero));

        QCOMPARE(BigInteger("+00000") + BigInteger("0"), BigInteger("0"));
    }

    void differentLength() {
        BigInteger bn(OneToZero);
        BigInteger shortBN("1234567890");
        BigInteger result1 {bn + shortBN};
        BigInteger result2 {shortBN + bn};
        BigInteger expected("12345678901234567890123456789012345678901234567890123456789012345678902469135780");
        QCOMPARE(result1, expected);
        QCOMPARE(result2, expected);
    }

    void longCarry() {
        BigInteger manyNines(ManyNines);
        BigInteger one("1");
        BigInteger expected(SomePowerOfTen);
        QCOMPARE(manyNines + one, expected);
        QCOMPARE(one + manyNines, expected);

        {
            BigInteger bn("100000000000000000000000000000000000000000000000000000000000000000000000000000000");
            QCOMPARE(manyNines + bn, BigInteger("1099999999999999999999999999999999999999999999999999999999999999999999999999999999"));
        }

        {
            BigInteger bn("100000000000000000000000000000000000000000000000000000000000000000000000000000001");
            QCOMPARE(manyNines + bn, BigInteger("1100000000000000000000000000000000000000000000000000000000000000000000000000000000"));
        }
    }

    void increment() {
        {
            BigInteger manyNines(ManyNines);
            BigInteger expected(SomePowerOfTen);
            QCOMPARE(++manyNines, expected);
            QCOMPARE(manyNines, expected);
        }

        {
            BigInteger manyNines(ManyNines);
            BigInteger expected(SomePowerOfTen);
            QCOMPARE(manyNines++, BigInteger(ManyNines));
            QCOMPARE(manyNines, expected);
        }

        {
            BigInteger manyZeros(std::string("-") + SomePowerOfTen);
            BigInteger expected(std::string("-") + ManyNines);
            QCOMPARE(++manyZeros, expected);
            QCOMPARE(manyZeros, expected);
        }

        {
            BigInteger manyZeros(std::string("-") + SomePowerOfTen);
            BigInteger expected(std::string("-") + ManyNines);
            QCOMPARE(manyZeros++, BigInteger(std::string("-") + SomePowerOfTen));
            QCOMPARE(manyZeros, expected);
        }
    }

    void unaryPlusMinus() {
        BigInteger oneToZero(OneToZero);
        QCOMPARE(oneToZero, +oneToZero);

        BigInteger minusOneToZero(std::string("-") + OneToZero);
        QCOMPARE(minusOneToZero, +minusOneToZero);

        QCOMPARE(-oneToZero, minusOneToZero);
        QCOMPARE(oneToZero, -minusOneToZero);

        BigInteger zero("0");
        QCOMPARE(zero, +zero);
        QCOMPARE(zero, -zero);
    }

    void equals() {
        BigInteger bn(OneToZero);
        QCOMPARE(bn, BigInteger(OneToZero));
        QCOMPARE(bn, BigInteger(std::string("+") + OneToZero));
        QCOMPARE(bn, BigInteger(std::string("+000") + OneToZero));

        BigInteger minusOneToZero(std::string("-") + OneToZero);
        BigInteger anotherMinusOneToZero(std::string("-00000") + OneToZero);
        QVERIFY(bn != minusOneToZero);
        QVERIFY(bn != anotherMinusOneToZero);

        QVERIFY(minusOneToZero == anotherMinusOneToZero);
    }

    void compare() {
        BigInteger oneToZero(OneToZero);
        BigInteger doubledOneToZero(DoubledOneToZero);
        QVERIFY(oneToZero < doubledOneToZero);
        QVERIFY(doubledOneToZero > oneToZero);
        QVERIFY(oneToZero <= doubledOneToZero);
        QVERIFY(doubledOneToZero >= oneToZero);
        QVERIFY(oneToZero >= oneToZero);
        QVERIFY(oneToZero <= oneToZero);

        BigInteger minusOneToZero(std::string("-") + OneToZero);
        BigInteger minusDoubleOneToZero(std::string("-") + DoubledOneToZero);
        QVERIFY(minusDoubleOneToZero < minusOneToZero);
        QVERIFY(minusOneToZero > minusDoubleOneToZero);
        QVERIFY(minusDoubleOneToZero <= minusOneToZero);
        QVERIFY(minusOneToZero >= minusDoubleOneToZero);
        QVERIFY(minusDoubleOneToZero >= minusDoubleOneToZero);
        QVERIFY(minusDoubleOneToZero <= minusDoubleOneToZero);

        QVERIFY(minusOneToZero < oneToZero);
        QVERIFY(oneToZero > minusOneToZero);
        QVERIFY(minusOneToZero <= oneToZero);
        QVERIFY(oneToZero >= minusOneToZero);

        BigInteger manyNines(ManyNines);
        BigInteger manyZeros(SomePowerOfTen);
        QVERIFY(manyNines < manyZeros);
        QVERIFY(manyZeros > manyNines);
        QVERIFY(manyNines <= manyZeros);
        QVERIFY(manyZeros >= manyNines);
        QVERIFY(manyNines >= manyNines);
        QVERIFY(manyNines <= manyNines);

        BigInteger minusManyNines(std::string("-") + ManyNines);
        BigInteger minusManyZeros(std::string("-") + SomePowerOfTen);
        QVERIFY(minusManyZeros < minusManyNines);
        QVERIFY(minusManyNines > minusManyZeros);
        QVERIFY(minusManyZeros <= minusManyNines);
        QVERIFY(minusManyNines >= minusManyZeros);
        QVERIFY(minusManyZeros >= minusManyZeros);
        QVERIFY(minusManyZeros <= minusManyZeros);
    }

    void zeros() {
        QCOMPARE(BigInteger("0"), BigInteger("+0"));
        QCOMPARE(BigInteger("0"), BigInteger("-0"));
        QCOMPARE(BigInteger("-0"), BigInteger("+0"));

        QCOMPARE(BigInteger("000"), BigInteger("+00000"));
        QCOMPARE(BigInteger("00000"), BigInteger("-00"));

        QCOMPARE(BigInteger("0") + BigInteger("-0"), BigInteger("0"));
        QCOMPARE(BigInteger("0") + BigInteger("+0"), BigInteger("0"));
        QCOMPARE(BigInteger("+0") + BigInteger("-0"), BigInteger("0"));
    }

    void signs() {
        BigInteger minusOneToZero(std::string("-") + OneToZero);
        QCOMPARE(minusOneToZero + minusOneToZero, BigInteger(std::string("-") + DoubledOneToZero));

        QCOMPARE(minusOneToZero + BigInteger(OneToZero), BigInteger("0"));
        QCOMPARE(BigInteger(OneToZero) + minusOneToZero, BigInteger("0"));

        QCOMPARE(BigInteger(ManyNines) +
                 BigInteger(std::string("-") + ManyNinesAndEight),
                 BigInteger("1"));

        QCOMPARE(BigInteger(ManyNinesAndEight) +
                 BigInteger(std::string("-") + ManyNines),
                 BigInteger("-1"));

        QCOMPARE(BigInteger(std::string("-") + ManyNines) +
                 BigInteger(ManyNinesAndEight),
                 BigInteger("-1"));

        QCOMPARE(BigInteger(std::string("-") + ManyNinesAndEight) +
                 BigInteger(ManyNines),
                 BigInteger("1"));

        BigInteger manyZeros(SomePowerOfTen);
        BigInteger minusOne("-1");
        QCOMPARE(manyZeros + minusOne,
                 BigInteger(ManyNines));

        QCOMPARE(BigInteger(std::string("-") + SomePowerOfTen) + BigInteger("1"),
                 BigInteger(std::string("-") + ManyNines));
    }

    void subtract() {
        BigInteger minusOneToZero(std::string("-") + OneToZero);
        QCOMPARE(minusOneToZero - minusOneToZero, BigInteger("0"));

        QCOMPARE(BigInteger(ManyNines) -
                 BigInteger(ManyNinesAndEight),
                 BigInteger("1"));

        QCOMPARE(BigInteger(ManyNinesAndEight) -
                 BigInteger(ManyNines),
                 BigInteger("-1"));

        QCOMPARE(BigInteger(std::string("-") + ManyNinesAndEight) -
                 BigInteger(std::string("-") + ManyNines),
                 BigInteger("1"));

        BigInteger manyZeros(SomePowerOfTen);
        BigInteger manyNines(ManyNines);
        BigInteger one("1");
        QCOMPARE(manyZeros - one, manyNines);
        QCOMPARE(manyZeros - manyNines, one);

        QCOMPARE(BigInteger("1") - BigInteger(std::string("-") + ManyNines),
                 BigInteger(SomePowerOfTen));
        QCOMPARE(BigInteger("-1") - BigInteger(std::string("-") + ManyNines),
                 BigInteger(ManyNinesAndEight));
    }

    void decrement() {
        {
            BigInteger manyZeros(SomePowerOfTen);
            BigInteger expected(ManyNines);
            QCOMPARE(--manyZeros, expected);
            QCOMPARE(manyZeros, expected);
        }

        {
            BigInteger manyZeros(SomePowerOfTen);
            BigInteger expected(ManyNines);
            QCOMPARE(manyZeros--, BigInteger(SomePowerOfTen));
            QCOMPARE(manyZeros, expected);
        }

        {
            BigInteger manyNines(std::string("-") + ManyNines);
            BigInteger expected(std::string("-") + SomePowerOfTen);
            QCOMPARE(--manyNines, expected);
            QCOMPARE(manyNines, expected);
        }

        {
            BigInteger manyNines(std::string("-") + ManyNines);
            BigInteger expected(std::string("-") + SomePowerOfTen);
            QCOMPARE(manyNines--, BigInteger(std::string("-") + ManyNines));
            QCOMPARE(manyNines, expected);
        }
    }

    void toString() {
        QCOMPARE(BigInteger(OneToZero).toString(), std::string(OneToZero));
        std::string minusOneToZero{std::string("-") + OneToZero};
        QCOMPARE(BigInteger(minusOneToZero).toString(), minusOneToZero);

        QCOMPARE(BigInteger("+0").toString(), std::string("0"));
        QCOMPARE(BigInteger("00").toString(), std::string("0"));
        QCOMPARE(BigInteger("-000").toString(), std::string("0"));
    }

    void implicitConversion() {
        QCOMPARE(BigInteger(ManyNines) +
                 BigInteger(std::string("-") + ManyNinesAndEight),
                 BigInteger("1"));

        QCOMPARE(BigInteger(ManyNines) +
                 BigInteger(std::string("-") + ManyNinesAndEight),
                 BigInteger("1"));

        QCOMPARE(BigInteger(ManyNines) +
                 BigInteger(std::string("-") + ManyNinesAndEight),
                 BigInteger("1"));

        QVERIFY(BigInteger(ManyNines) +
                 BigInteger(std::string("-") + ManyNinesAndEight) ==
                 BigInteger("1"));

        QCOMPARE(BigInteger{BigInteger(ManyNines) +
                 BigInteger(std::string("-") + ManyNinesAndEight)},
                 BigInteger("1"));
    }
};

QTEST_MAIN(TestBigInteger)
#include "main.moc"
