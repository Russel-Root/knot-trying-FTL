#include "ConfigReader.h"

ConfigReader::ConfigReader(){}
ConfigReader::~ConfigReader(){}

const int ConfigReader::SPACE = ' ';
const int ConfigReader::TAB = '\t';
const int ConfigReader::EQUAL = '=';
const int ConfigReader::COLON = ':';
const int ConfigReader::LINE_END = '\n';
const int ConfigReader::LINE_BEGIN = -10000;
const int ConfigReader::CARRIAGE = '\r'; // »Ø³µ
const int ConfigReader::KEY_MAX_LENGTH = 100;
const int ConfigReader::VALUE_MAX_LENGTH = 100;
