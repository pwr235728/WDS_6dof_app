#ifndef IMUPARSER_H
#define IMUPARSER_H

#include   <QSerialPort>

/**
 * @brief Struktrua Vector3
 *
 * Struktura przechowuje dane z jednego sensora.
 * Każde pole zawiera wskazania jednej osi.
 */
struct Vector3
{
    int16_t x;
    int16_t y;
    int16_t z;
};


/**
 * @brief Struktura ImuData
 *
 * Struktura zawierająca dane ze wszystkich sensorów.
 */
struct ImuData
{
    Vector3 acc;    //!< Dane z akcelerometru
    Vector3 gyro;   //!< Dane z żyroskopu
    float tmp;      //!< Odczyt temperatury
};


/**
 * @brief Struktura ImuFrame
 *
 * Struktura opisująca ramke danych odbiernaych przez program.
 * Jedna ramka opisuje jedno polecenie.
 * Sumy kontrolne nagłówka i dnaych są oddzielone celowo.
 * W przypadku gdy ramka jest niepoprawna(błąd w przesyłaniu),
 * można już po samym nagłówku odżucić pakiet. I nasłuchiwać na kolejny.
 *
 */
struct ImuFrame
{
    uchar id;           //!< Identyfikator polecenia.
    uchar length;       //!< Ilość przesyłanych danych.
    uchar data[256];    //!< Tablica zawierajaca odebrane dane.

    //! Funkcja licząca sumę kontrolną nagłówka.
    uchar GetHeaderSum();

    //! Funckaj licząca sume kontrolną danych.
    uchar GetDataSum();

    //! Domyślny konstruktor.
    ImuFrame();
};

/**
 * @brief Enumerator ParserState
 *
 * Wartości zwracane przez funkcje parsujące przychodzące dane.
 */
enum ParserState
{
    PARSER_ERROR = 0,
    PARSER_COMPLETE,
};

/**
 * @brief Klasa ImuParser
 *
 * Klasa służaca do przetwarzania odbieranych danych.
 * Jej funkcje umożliwiają odczyt odbieranych dnaych.
 */
class ImuParser
{
private:
    //! Stany w których może znajdowac się parser.
    enum State{
        STATE_WAIT,
        STATE_GOT_MARKER,
        STATE_GOT_ID,
        STATE_GOT_LEN,
        STATE_GOT_HEADER,
        STATE_GOT_DATA
    };

    ImuParser();

public:

    //! Identyfikator początku ramki.
    static const char STATRT_BYTE = '>';

    //! Identyfikator informujących że ramka zawiera dane z sensorów
    static const char ID_IMU_DATA = 'i';

    //! Odczyt ramki danych, z tablicy odebranych bajtów.
    /*!
      \param bytes tablica odebranych bajtów.
      \param out zwracana ramka danych.
      \return Stan parsera.
    */
    static ParserState ReadFrame(QByteArray &bytes, ImuFrame &out);

    //! Odczyt danych z sensorów zawartych w ramce.
    /*!
     * \param frame ramka danych do przetworzenia
     * \param out zwracana ramka danych z sensorów
     * \return Stan parsera.
     */
    static ParserState GetImuData(ImuFrame &frame, ImuData &out);
};

#endif // IMUPARSER_H
